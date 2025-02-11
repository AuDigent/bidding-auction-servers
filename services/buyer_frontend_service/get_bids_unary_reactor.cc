//  Copyright 2022 Google LLC
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "services/buyer_frontend_service/get_bids_unary_reactor.h"

#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_format.h"
#include "api/bidding_auction_servers.grpc.pb.h"
#include "glog/logging.h"
#include "services/buyer_frontend_service/util/proto_factory.h"
#include "services/common/constants/user_error_strings.h"
#include "services/common/loggers/build_input_process_response_benchmarking_logger.h"
#include "services/common/loggers/no_ops_logger.h"
#include "services/common/util/consented_debugging_logger.h"
#include "services/common/util/request_metadata.h"
#include "services/common/util/request_response_constants.h"

namespace privacy_sandbox::bidding_auction_servers {

namespace {

using ::google::cmrt::sdk::crypto_service::v1::HpkeDecryptResponse;
using GenerateProtectedAppSignalsBidsRawRequest =
    GenerateProtectedAppSignalsBidsRequest::
        GenerateProtectedAppSignalsBidsRawRequest;
using GenerateProtectedAppSignalsBidsRawResponse =
    GenerateProtectedAppSignalsBidsResponse::
        GenerateProtectedAppSignalsBidsRawResponse;
inline constexpr int kNumDefaultOutboundBiddingCalls = 1;
inline constexpr int kNumOutboundBiddingCallsWithProtectedAppSignals = 2;

template <typename T>
void HandleSingleBidCompletion(
    absl::StatusOr<std::unique_ptr<T>> raw_response,
    absl::AnyInvocable<void(const absl::Status&) &&> on_error_response,
    absl::AnyInvocable<void() &&> on_empty_response,
    absl::AnyInvocable<void(std::unique_ptr<T>) &&> on_successful_response) {
  // Handle errors
  if (!raw_response.ok()) {
    std::move(on_error_response)(raw_response.status());
    return;
  }

  auto response = *std::move(raw_response);

  // Handle empty response
  if (!response->IsInitialized() || response->bids_size() == 0) {
    std::move(on_empty_response)();
    return;
  }

  // Handle successful response
  std::move(on_successful_response)(std::move(response));
}

}  // namespace

GetBidsUnaryReactor::GetBidsUnaryReactor(
    grpc::CallbackServerContext& context,
    const GetBidsRequest& get_bids_request, GetBidsResponse& get_bids_response,
    const BiddingSignalsAsyncProvider& bidding_signals_async_provider,
    const BiddingAsyncClient& bidding_async_client, const GetBidsConfig& config,
    const ProtectedAppSignalsBiddingAsyncClient* pas_bidding_async_client,
    server_common::KeyFetcherManagerInterface* key_fetcher_manager,
    CryptoClientWrapperInterface* crypto_client, bool enable_benchmarking)
    : context_(&context),
      request_(&get_bids_request),
      get_bids_response_(&get_bids_response),
      get_bids_raw_response_(
          std::make_unique<GetBidsResponse::GetBidsRawResponse>()),
      // TODO(b/278039901): Add integration test for metadata forwarding.
      kv_metadata_(GrpcMetadataToRequestMetadata(context.client_metadata(),
                                                 kBuyerKVMetadata)),
      bidding_signals_async_provider_(&bidding_signals_async_provider),
      bidding_async_client_(&bidding_async_client),
      config_(config),
      protected_app_signals_bidding_async_client_(pas_bidding_async_client),
      key_fetcher_manager_(key_fetcher_manager),
      crypto_client_(crypto_client),
      // `logger_` doesn't have the log context yet.
      logger_(GetLoggingContext()),
      async_task_tracker_(config_.is_protected_app_signals_enabled
                              ? kNumOutboundBiddingCallsWithProtectedAppSignals
                              : kNumDefaultOutboundBiddingCalls,
                          logger_, [this](bool any_successful_bid) {
                            OnAllBidsDone(any_successful_bid);
                          }) {
  if (enable_benchmarking) {
    std::string request_id = FormatTime(absl::Now());
    benchmarking_logger_ =
        std::make_unique<BuildInputProcessResponseBenchmarkingLogger>(
            request_id);
  } else {
    benchmarking_logger_ = std::make_unique<NoOpsLogger>();
  }
  CHECK_OK([this]() {
    PS_ASSIGN_OR_RETURN(metric_context_,
                        metric::BfeContextMap()->Remove(request_));
    return absl::OkStatus();
  }()) << "BfeContextMap()->Get(request) should have been called";

  DCHECK(!config_.is_protected_app_signals_enabled ||
         protected_app_signals_bidding_async_client_ != nullptr)
      << "PAS is enabled but no PAS bidding async client available";
}

GetBidsUnaryReactor::GetBidsUnaryReactor(
    grpc::CallbackServerContext& context,
    const GetBidsRequest& get_bids_request, GetBidsResponse& get_bids_response,
    const BiddingSignalsAsyncProvider& bidding_signals_async_provider,
    const BiddingAsyncClient& bidding_async_client, const GetBidsConfig& config,
    server_common::KeyFetcherManagerInterface* key_fetcher_manager,
    CryptoClientWrapperInterface* crypto_client, bool enable_benchmarking)
    : GetBidsUnaryReactor(context, get_bids_request, get_bids_response,
                          bidding_signals_async_provider, bidding_async_client,
                          config,
                          /*pas_bidding_client=*/nullptr, key_fetcher_manager,
                          crypto_client, enable_benchmarking) {}

void GetBidsUnaryReactor::OnAllBidsDone(bool any_successful_bids) {
  if (context_->IsCancelled()) {
    benchmarking_logger_->End();
    Finish(grpc::Status(grpc::ABORTED, kRequestCancelled));
    return;
  }

  if (VLOG_IS_ON(2)) {
    logger_.vlog(2, "Accumulated Raw response:\n",
                 get_bids_raw_response_->DebugString());
  }

  if (auto encryption_status = EncryptResponse(); !encryption_status.ok()) {
    logger_.vlog(1, "Failed to encrypt the response");
    benchmarking_logger_->End();
    Finish(
        grpc::Status(grpc::StatusCode::INTERNAL, encryption_status.ToString()));
    return;
  }

  if (VLOG_IS_ON(3)) {
    logger_.vlog(3, "Encrypted GetBidsResponse:\n",
                 get_bids_response_->DebugString());
  }

  if (!any_successful_bids) {
    logger_.vlog(3,
                 "Finishing the GetBids RPC with an error, since there are "
                 "no successful bids returned by the bidding service");
    benchmarking_logger_->End();
    Finish(grpc::Status(grpc::INTERNAL, absl::StrJoin(bid_errors_, "; ")));
    return;
  }

  benchmarking_logger_->End();
  FinishWithOkStatus();
}

bool GetBidsUnaryReactor::DecryptRequest() {
  if (request_->key_id().empty()) {
    VLOG(1) << kEmptyKeyIdError;
    Finish(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kEmptyKeyIdError));
    return false;
  }

  if (request_->request_ciphertext().empty()) {
    VLOG(1) << kEmptyCiphertextError;
    Finish(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        kEmptyCiphertextError));
    return false;
  }

  std::optional<server_common::PrivateKey> private_key =
      key_fetcher_manager_->GetPrivateKey(request_->key_id());
  if (!private_key.has_value()) {
    VLOG(1) << kInvalidKeyIdError;
    Finish(
        grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kInvalidKeyIdError));
    return false;
  }

  absl::StatusOr<HpkeDecryptResponse> decrypt_response =
      crypto_client_->HpkeDecrypt(*private_key, request_->request_ciphertext());
  if (!decrypt_response.ok()) {
    VLOG(1) << kMalformedCiphertext;
    Finish(
        grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kMalformedCiphertext));
    return false;
  }

  hpke_secret_ = std::move(decrypt_response->secret());
  if (!raw_request_.ParseFromString(decrypt_response->payload())) {
    VLOG(1) << "Unable to parse proto from the decrypted request: "
            << kMalformedCiphertext;
    Finish(
        grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kMalformedCiphertext));
    return false;
  }

  return true;
}

void GetBidsUnaryReactor::Execute() {
  benchmarking_logger_->Begin();
  DCHECK(config_.encryption_enabled);
  if (!DecryptRequest()) {
    VLOG(1) << "Decrypting the request failed";
    return;
  }
  VLOG(5) << "Successfully decrypted the request";

  // Set the request context right after the decrypted request is available.
  logger_.Configure(GetLoggingContext());
  // TODO(b/299366050): Refactor ContextLogger & ConsentedDebuggingLogger to
  // move initialization to the constructor if possible.
  if (config_.enable_otel_based_logging) {
    debug_logger_ = ConsentedDebuggingLogger(GetLoggingContext(),
                                             config_.consented_debug_token);
  }

  MayLogRawRequest();
  GetProtectedAudienceBids();
  MayGetProtectedSignalsBids();
}

void GetBidsUnaryReactor::MayGetProtectedSignalsBids() {
  if (!config_.is_protected_app_signals_enabled) {
    logger_.vlog(8, "Protected App Signals feature not enabled");
    return;
  }

  if (!raw_request_.has_protected_app_signals_buyer_input() ||
      !raw_request_.protected_app_signals_buyer_input()
           .has_protected_app_signals()) {
    logger_.vlog(3,
                 "No protected app buyer signals input found, skipping "
                 "fetching bids for protected app signals");
    async_task_tracker_.TaskCompleted(TaskStatus::SKIPPED);
    return;
  }

  auto protected_app_signals_bid_request =
      CreateGenerateProtectedAppSignalsBidsRawRequest(raw_request_);

  absl::Status execute_result =
      protected_app_signals_bidding_async_client_->ExecuteInternal(
          std::move(protected_app_signals_bid_request), kv_metadata_,
          [this](absl::StatusOr<
                 std::unique_ptr<GenerateProtectedAppSignalsBidsRawResponse>>
                     raw_response) {
            HandleSingleBidCompletion<
                GenerateProtectedAppSignalsBidsRawResponse>(
                std::move(raw_response),
                // Error response handler
                [this](const absl::Status& status) {
                  logger_.vlog(
                      1,
                      "Execution of GenerateProtectedAppSignalsBids request "
                      "failed with status: ",
                      status);
                  async_task_tracker_.TaskCompleted(
                      TaskStatus::ERROR, [this, &status]() {
                        bid_errors_.push_back(status.ToString());
                      });
                },
                // Empty response handler
                [this]() {
                  async_task_tracker_.TaskCompleted(
                      TaskStatus::EMPTY_RESPONSE, [this]() {
                        get_bids_raw_response_
                            ->mutable_protected_app_signals_bids();
                      });
                },
                // Successful response handler
                [this](auto response) {
                  async_task_tracker_.TaskCompleted(
                      TaskStatus::SUCCESS,
                      [this, response = std::move(response)]() {
                        get_bids_raw_response_
                            ->mutable_protected_app_signals_bids()
                            ->Swap(response->mutable_bids());
                      });
                });
          },
          absl::Milliseconds(
              config_.protected_app_signals_generate_bid_timeout_ms));
  if (!execute_result.ok()) {
    logger_.error(
        "Failed to make async GenerateProtectedAppInstallBids call: (error: ",
        execute_result.ToString(), ")");
    async_task_tracker_.TaskCompleted(
        TaskStatus::ERROR, [this, &execute_result]() {
          bid_errors_.push_back(execute_result.ToString());
        });
  }
}

void GetBidsUnaryReactor::MayLogRawRequest() {
  // Logger for consented debugging.
  if (debug_logger_.has_value() && debug_logger_->IsConsented()) {
    debug_logger_->vlog(
        0, absl::StrCat("GetBidsRawRequest: ", raw_request_.DebugString()));
  }
}

void GetBidsUnaryReactor::GetProtectedAudienceBids() {
  BiddingSignalsRequest bidding_signals_request(raw_request_, kv_metadata_);
  auto kv_request =
      metric::MakeInitiatedRequest(metric::kKv, metric_context_.get(), 0);

  // Get Bidding Signals.
  bidding_signals_async_provider_->Get(
      bidding_signals_request,
      [this, kv_request = std::move(kv_request)](
          absl::StatusOr<std::unique_ptr<BiddingSignals>> response) mutable {
        {  // destruct kv_request, destructor measures request time
          auto not_used = std::move(kv_request);
        }
        if (!response.ok()) {
          LogIfError(metric_context_->AccumulateMetric<
                     server_common::metric::kInitiatedRequestErrorCount>(1));
          // Return error to client.
          logger_.vlog(1, "GetBiddingSignals request failed with status:",
                       response.status());
          async_task_tracker_.TaskCompleted(
              TaskStatus::ERROR, [this, &response]() {
                bid_errors_.push_back(response.status().ToString());
              });
          return;
        }

        // Sends protected audience bid request to bidding service.
        PrepareAndGenerateProtectedAudienceBid(std::move(response.value()));
      },
      absl::Milliseconds(config_.bidding_signals_load_timeout_ms));
}

// Process Outputs from Actions to prepare bidding request.
// All Preload actions must have completed before this is invoked.
void GetBidsUnaryReactor::PrepareAndGenerateProtectedAudienceBid(
    std::unique_ptr<BiddingSignals> bidding_signals) {
  const auto& log_context = raw_request_.log_context();
  std::unique_ptr<GenerateBidsRequest::GenerateBidsRawRequest>
      raw_bidding_input =
          CreateGenerateBidsRawRequest(raw_request_, raw_request_.buyer_input(),
                                       std::move(bidding_signals), log_context);

  logger_.vlog(2, "GenerateBidsRequest:\n", raw_bidding_input->DebugString());
  auto bidding_request = metric::MakeInitiatedRequest(
      metric::kBs, metric_context_.get(), raw_bidding_input->ByteSizeLong());
  absl::Status execute_result = bidding_async_client_->ExecuteInternal(
      std::move(raw_bidding_input), {},
      [this, bidding_request = std::move(bidding_request)](
          absl::StatusOr<
              std::unique_ptr<GenerateBidsResponse::GenerateBidsRawResponse>>
              raw_response) mutable {
        {  // destruct bidding_request, destructor measures request time
          auto not_used = std::move(bidding_request);
        }

        HandleSingleBidCompletion<
            GenerateBidsResponse::GenerateBidsRawResponse>(
            std::move(raw_response),
            // Error response handler
            [this](const absl::Status& status) {
              LogIfError(
                  metric_context_->AccumulateMetric<
                      server_common::metric::kInitiatedRequestErrorCount>(1));
              logger_.vlog(
                  1, "Execution of GenerateBids request failed with status: ",
                  status);
              async_task_tracker_.TaskCompleted(
                  TaskStatus::ERROR, [this, &status]() {
                    bid_errors_.push_back(status.ToString());
                  });
            },
            // Empty response handler
            [this]() {
              async_task_tracker_.TaskCompleted(
                  TaskStatus::EMPTY_RESPONSE,
                  [this]() { get_bids_raw_response_->mutable_bids(); });
            },
            // Successful response handler
            [this](auto response) {
              async_task_tracker_.TaskCompleted(
                  TaskStatus::SUCCESS,
                  [this, response = std::move(response)]() {
                    get_bids_raw_response_->mutable_bids()->Swap(
                        response->mutable_bids());
                  });
            });
      },
      absl::Milliseconds(config_.generate_bid_timeout_ms));
  if (!execute_result.ok()) {
    logger_.error("Failed to make async GenerateBids call: (error: ",
                  execute_result.ToString(), ")");
    async_task_tracker_.TaskCompleted(
        TaskStatus::ERROR, [this, &execute_result]() {
          bid_errors_.push_back(execute_result.ToString());
        });
  }
}

absl::Status GetBidsUnaryReactor::EncryptResponse() {
  std::string payload = get_bids_raw_response_->SerializeAsString();
  PS_ASSIGN_OR_RETURN(auto aead_encrypt,
                      crypto_client_->AeadEncrypt(payload, hpke_secret_));

  get_bids_response_->set_response_ciphertext(
      aead_encrypt.encrypted_data().ciphertext());
  return absl::OkStatus();
}

ContextLogger::ContextMap GetBidsUnaryReactor::GetLoggingContext() {
  const auto& log_context = raw_request_.log_context();
  ContextLogger::ContextMap context_map = {
      {kGenerationId, log_context.generation_id()},
      {kBuyerDebugId, log_context.adtech_debug_id()}};
  if (raw_request_.has_consented_debug_config()) {
    MaybeAddConsentedDebugConfig(raw_request_.consented_debug_config(),
                                 context_map);
  }
  return context_map;
}

void GetBidsUnaryReactor::FinishWithOkStatus() {
  metric_context_->SetRequestSuccessful();
  Finish(grpc::Status::OK);
}

// Deletes all data related to this object.
void GetBidsUnaryReactor::OnDone() { delete this; }

}  // namespace privacy_sandbox::bidding_auction_servers
