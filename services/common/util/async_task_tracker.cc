// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "services/common/util/async_task_tracker.h"

#include <utility>

#include "absl/strings/str_cat.h"
#include "glog/logging.h"

namespace privacy_sandbox::bidding_auction_servers {

AsyncTaskTracker::AsyncTaskTracker(
    int num_tasks_to_track, const ContextLogger& logger,
    absl::AnyInvocable<void(bool) &&> on_all_tasks_done)
    : num_tasks_to_track_(num_tasks_to_track),
      pending_tasks_count_(num_tasks_to_track),
      successful_tasks_count_(0),
      empty_tasks_count_(0),
      skipped_tasks_count_(0),
      error_tasks_count_(0),
      on_all_tasks_done_(std::move(on_all_tasks_done)),
      logger_(logger) {}

void AsyncTaskTracker::TaskCompleted(TaskStatus task_status) {
  TaskCompleted(task_status, std::nullopt);
}

void AsyncTaskTracker::TaskCompleted(
    TaskStatus task_status,
    std::optional<absl::AnyInvocable<void()>> on_single_task_done) {
  bool is_last_response = false;
  bool any_successful_tasks = false;

  {
    absl::MutexLock lock(&mu_);
    DCHECK(pending_tasks_count_ > 0)
        << "Unexpected call (indicates either a bug in the initialization or "
           "the usage)";

    if (on_single_task_done.has_value()) {
      (*on_single_task_done)();
    }

    --pending_tasks_count_;
    switch (task_status) {
      case TaskStatus::ERROR:
        ++error_tasks_count_;
        break;
      case TaskStatus::EMPTY_RESPONSE:
        ++empty_tasks_count_;
        break;
      case TaskStatus::SKIPPED:
        ++skipped_tasks_count_;
        break;
      case TaskStatus::SUCCESS:
        ++successful_tasks_count_;
        break;
      default:
        logger_.error("Unexpected task status : ", absl::StrCat(task_status));
        break;
    }
    logger_.vlog(5, "Updated pending tasks state: ", ToString());
    if (pending_tasks_count_ == 0) {
      is_last_response = true;
      // Accounts for chaffs.
      any_successful_tasks = AnyTaskSuccessfullyCompleted();
    }
  }

  // Since on_all_tasks_done_ can delete AsyncTaskTracker itself all resources
  // must be released before calling on_all_tasks_done_.
  if (is_last_response) {
    std::move(on_all_tasks_done_)(any_successful_tasks);
  }
}

bool AsyncTaskTracker::AnyTaskSuccessfullyCompleted() {
  const int sum_tasks_count = skipped_tasks_count_ + successful_tasks_count_ +
                              error_tasks_count_ + empty_tasks_count_ +
                              pending_tasks_count_;
  DCHECK_EQ(num_tasks_to_track_, sum_tasks_count);

  const bool possible_chaff =
      empty_tasks_count_ > 0 || skipped_tasks_count_ > 0;
  return successful_tasks_count_ > 0 || possible_chaff ||
         error_tasks_count_ == 0;
}

std::string AsyncTaskTracker::ToString() {
  return absl::StrCat("Async Task Stats: succeeded=", successful_tasks_count_,
                      ", errored=", error_tasks_count_,
                      ", skipped=", skipped_tasks_count_,
                      ", returned empty=", empty_tasks_count_,
                      ", pending=", pending_tasks_count_,
                      ", initial count=", num_tasks_to_track_);
}

}  // namespace privacy_sandbox::bidding_auction_servers
