# Changelog

All notable changes to this project will be documented in this file. See [commit-and-tag-version](https://github.com/absolute-version/commit-and-tag-version) for commit guidelines.

## 2.0.0 (2023-09-12)


### Features

* Add C++ code coverage support
* Add PUT support in HTTP client
* Add SetContext() in ConsentedDebuggingLogger
* Change secure_invoke output to be easily parsable by automated scripts
* Log plaintext GenerateBidsRawRequest in Bidding
* override metric's default public partition
* PAS support for BFE
* turn on external load balancer logging by default


### Bug Fixes

* Change generate_bid_code_version from int to string in API
* [logging] Check for log fields in the response json from reporting scripts
* [logging] VLOG the logs, errors and warnings exported from Roma for reporting.
* [reporting] Add ad_cost and interest_group_name inputs to the reportWin function input
* Add copyright line in license block
* Add missing adCost to buyer's browser signals in the code wrapper
* Allow ScoreAd to return a number
* Check rapidjson property presence before access
* Fix invalidCode error when buyer's script is not a correct expression
* Fixes a comment in proto
* revert documentation update until the build_and_test_all_in_docker script is updated
* safe metric should not LogUnsafe
* Set the log context in BFE
* Updates the client types comments


### cleanup

* Change generate_bid_code_version from int to string in API

## 1.2.0 (2023-09-01)


### Features

* Add code blob flags for bidding/auction
* Log plaintext GetBidsRawRequest in BFE
* PAS support in SFE
* Propagate consented debug config from BFE to Bidding
* Propagate consented debug config from SFE to BFE & Auction


### Bug Fixes

* [reporting] Change the order of cbor encoding for win reporting urls in the response.
* [reporting] Populate win reporting urls for seller in top_level_seller_reporting_urls instead of component_seller_reporting_urls
* [reporting] Remove unnecessary signalsForWinner in the response from reporting wrapper function
* Disable enableSellerDebugUrlGeneration flag by default

## 1.1.0 (2023-08-25)


### Features

* Add B&A App Install API
* Add ConsentedDebuggingLogger to write logs via OTel
* Add feature flag enabling/disabling PAS
* Add IG origin to AuctionResult
* add max surge to instance groups
* Add owner field for PAS Ad with bid
* Adds docker build option for local testing
* Adds PAS buyer input to GetBids
* Check debug token in ConsentedDebuggingLogger
* Include IG owner in buyer input
* Log decoded buyer inputs for consented debugging in SFE
* make export interval configurable
* make metric list configurable
* OpenTelemetry logging can be disabled via TELEMETRY_CONFIG flag
* Propagate Ad type from Auction => SFE
* update GCP terraforms to apply updates to instances without
* update LB policy to default to RATE instead of UTILIZATION
* Upgrade to functionaltest-system 0.8.0
* use private meter provider not shared with Otel api


### Bug Fixes

* Bump required tf to 1.2.3 to be compatible with replace_triggered_by
* Change componentAds -> components
* Correct GCP dashboards
* Do not set runtime flags with empty strings
* Fixes race condition in SFE reactor due to mutex lock
* Log a message in case of server flag lookup failures
* Removes callback execution from default_async_grpc_client to prevent double invocation
* specify default cpu util before sending requests to other regions
* update OTel demo documentation
* Use bazel config to set instance and platform


### Dependencies

* **deps:** Upgrade build-system to 0.43.0

## 1.0.0 (2023-08-10)


### BREAKING CHANGES

* if your generateBid() returns ad component render urls in a field named "adComponentRender". You will have to update this to return these in a field named "adComponents".
Changed name in the API. Updated in reactors. Added a unit test.

Testing: Unit test
Bug: b/294917906
Change-Id: Ie4344f55b18ef10f7a81b197ec997be393fa7368

### Features

* Adds Readme for running B&A servers locally
* Enable CBOR encode/decode for ConsentedDebugConfig
* implement dp aggregated histogram metric
* include local server start scripts
* periodic bucket fetching using BlobStorageClient


### Bug Fixes

* Correct paths to terraform modules in AWS demo.tf files
* Improve clarity of aws vs gcp cases
* Improve flag error handling
* include required dep for bucket code fetcher
* Remove enable_buyer_code_wrapper flag.
* Remove enable_seller_code_wrapper flag.
* Remove enableSellerCodeWrapper flag from aws and demo configs
* remove unnecessary flags in terraform configs
* Set cbuild flag --seccomp-unconfined


### cleanup

* Change AdWithBid.ad_component_render to .ad_components to align with OnDevice generateBid() spec


### Documentation

* Update terraform comments to communicate requirement for env name <= 3 characters

## 0.10.0 (2023-08-03)


### Features

* [reporting] Add helper function for cbor encoding and decoding of Win Reporting Urls
* [reporting] Execute reportWin script and return the urls in the response.
* [reporting] Fix reportWin function execution.
* add AWS enclave PCR0 in release notes
* Add bazel configs for roma legacy vs sandboxed
* Add OSSF Scorecard badge to top-level README
* Add OSSF Scorecard GitHub workflow
* Add the server flags to support user consented debugging
* change autoscale cpu utilization measure to GCP suggested default (60%)
* clarify default lb policy within an instance group is ROUND_ROBIN
* Enable logging in the OTel collector
* Encrypt GetBidsRequest for benchmarking
* Flag protect the opentelemetry based logging
* move observable system metric to server definition
* Upgrade data-plane-shared-libraries for Opentelemetry logs API


### Bug Fixes

* Encode Ad Component Render URLs when sending request to Seller Key-Value Server
* Fixes few ASAN issues
* Minimal CBOR encoding for uint/float
* Order the keys in the response
* Remove unwanted seller_origin flag from start_auction script
* Rename the opentelemetry endpoint flag
* secure_invoke respects --insecure flag for BFE


### API: Features

* **api:** Add the fields to support adtech-consented debugging


### Dependencies

* **deps:** Upgrade build-system to v0.39.0
* **deps:** Upgrade build-system to v0.41.0
* **deps:** upgrade opentelemetry-cpp to 1.9.1 for B&A servers


### Documentation

* update release README instructions
* Update secure_invoke README with new instructions for running.


## 0.9.0 (2023-07-20)


### Features

* [reporting] Add helper function to build the dispatch request for
* add buyerReportWinJsUrls to terraform files and enable_report_win_url_generation to BuyerCodeFetchConfig
* add cpu memory metric for debugging
* add GCP metric dashboard
* add method to accumulate metric values before logging
* changing PeriodicCodeFetcher to add wasm support and runtime config support
* load and append wasm string to the JS code wrapper
* support different instance types per service in GCP
* Upgrade build-system to release-0.31.0


### Bug Fixes

* Add seller_code_fetch_config and buyer_code_fetch_config to server start scripts
* CPU monitoring is not limited to a specific environment
* Define container image log variables once
* Don't end select ad request prematurely
* patch google dp library to replace logging with absl
* Update and read the buyer_bids_ with lock


### Dependencies

* **deps:** Upgrade build-system to v0.33.0

## 0.8.0 (2023-07-12)


### Features

* add FetchUrls utility to MultiCurlHttpFetcher
* enable counter metrics with dp aggregation
* update multi-region.tf to use prod js urls and test mode true
* use //:non_prod_build to configure build


### Bug Fixes

* Adjust all/small test configs
* Adjust sanitizer configs
* Correct example BFE hostname
* Correct license block
* Ensure --gcp-image flags are specified for gcp
* Ensure --instance flag is specified
* fix missing telemetry flag and readme
* Improve build_and_test_all_in_docker usage text

## 0.7.0 (2023-06-30)


### Features

*  [AWS] add example terraform directory with README
* [GCP] add example terraform directory with README
* Add bazel build flag --announce_rc
* add build_flavor for AWS packaging
* add build_flavor for packaging
* include coordinator and attestation support for GCP
* Upgrade build-system to release-0.30.1


### Bug Fixes

* Adjust SFE DCHECKs
* bidding_service_test
* Change PeriodicCodeFetcher to use std::string instead of absl::string_view in the parameters
* refactor the test to share initialization
* remove unnecessary flags
* TEE<>TEE fix
* temporarily eliminate requirement to have device signals to generate bids

## 0.6.0 (2023-06-23)


### Features

* add VLOG(2) for code loaded into Roma


### Bug Fixes

* Correct xtrace prompt

## 0.5.0 (2023-06-22)


### Features

* Add --instance to build_and_test_all_in_docker
* Add bazel configurations for platform and instance flags
* Add flag to config telemetry
* Add smalltests kokoro config
* changing MultiCurlHttpFetcherAsync to take a raw pointer executor.
* create a header file for PeriodicCodeFetcher object wtih constructor, necessary functions, and necessary variables
* create a source, BUILD, and test files for PeriodicCodeFetcher.
* create CodeFetcherInterface as an interface for different CodeFetcher classes.
* enforce list order in metric definition
* implement dp to aggregte (partitioned)counter metric with noise
* Implement GCP packaging scripts (including SFE envoy
* Implement Metric API used by B&A server
* Implement Metric context map
* Implement Metric router to pass safe metric to OTel
* Limit build_and_test_all_in_docker to run small tests only
* modify auction_main.cc and bidding_main.cc to integrate PeriodicCodeFetcher for code blob fetching.
* move GCP instances behind a NAT
* reactor own metric context
* remove hardcoded scoreAd.js and generateBid.js
* update TrustedServerConfigClient to work with GCP
* use telemetry flag to configure metric and trace
* Use terraform flag to specify if debug or prod confidential compute


### Bug Fixes

* [GCP] add docker redirect flag for prod images
* [GCP] specify port instead of port_name for lb healthchecks
* Add bazel coverage support
* add GCP as a bazel config
* add missing gcp flags
* auction service returns if no dispatch requests found
* BFE Segfault after grpc reactor Finish
* complete removal of sideload IG data
* do not reference ScoreAdsReactor private members after grpc::Finish
* flaky auction and bidding tests
* GCP SFE dependencies were outdated
* gcp terraform should use env variable for buyer address
* mark docker and async grpc client stub tests large
* potential SFE segfault due to logging after Finish
* Remove --without-shared-cache flag
* rename service to operator, component to service
* Replace glog with absl_log in status macro
* skip scoring signal fetch and Auction if no bids are returned
* Specify test size
* **test:** Add size to cc_test targets
* update gcp packaging script to support all repos globally
* update GCP SFE runtime flags with new values
* update init_server_basic_script to use operator
* update managed instance group healthcheck
* update pending bids so SFE does not hang
* Use --config=value in .bazelrc
* Use buf --config flag
* validate that the buyer list is not empty


### Build Tools: Features

* **build:** Emit test timeout warnings from bazel


### Build Tools: Fixes

* **build:** Add scope-based sections in release notes
* **build:** Adjust small-tests configuration
* **build:** Create all-tests configuration

## 0.4.0 (2023-06-01)

## 0.3.0 (2023-06-01)

## 0.2.0 (2023-05-22)

## 0.1.0 (2023-05-16)

### API

* Initial release
