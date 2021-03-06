// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/task_scheduler/utils.h"

#include <utility>

#include "base/logging.h"
#include "base/memory/ptr_util.h"
#include "base/task_scheduler/delayed_task_manager.h"
#include "base/task_scheduler/priority_queue.h"
#include "base/task_scheduler/scheduler_task_executor.h"
#include "base/task_scheduler/sequence_sort_key.h"
#include "base/task_scheduler/task_tracker.h"
#include "base/time/time.h"

namespace base {
namespace internal {

bool PostTaskToExecutor(std::unique_ptr<Task> task,
                        scoped_refptr<Sequence> sequence,
                        SchedulerTaskExecutor* executor,
                        TaskTracker* task_tracker,
                        DelayedTaskManager* delayed_task_manager) {
  DCHECK(task);
  DCHECK(sequence);
  DCHECK(executor);
  DCHECK(task_tracker);
  DCHECK(delayed_task_manager);

  if (!task_tracker->WillPostTask(task.get()))
    return false;

  if (task->delayed_run_time.is_null()) {
    executor->PostTaskWithSequence(std::move(task), std::move(sequence));
  } else {
    delayed_task_manager->AddDelayedTask(std::move(task), std::move(sequence),
                                         executor);
  }

  return true;
}

bool AddTaskToSequenceAndPriorityQueue(std::unique_ptr<Task> task,
                                       scoped_refptr<Sequence> sequence,
                                       PriorityQueue* priority_queue) {
  DCHECK(task);
  DCHECK(sequence);
  DCHECK(priority_queue);

  // Confirm that |task| is ready to run (its delayed run time is either null or
  // in the past).
  DCHECK_LE(task->delayed_run_time, TimeTicks::Now());

  const bool sequence_was_empty = sequence->PushTask(std::move(task));
  if (sequence_was_empty) {
    // Insert |sequence| in |priority_queue| if it was empty before |task| was
    // inserted into it. Otherwise, one of these must be true:
    // - |sequence| is already in a PriorityQueue (not necessarily
    //   |priority_queue|), or,
    // - A worker thread is running a Task from |sequence|. It will insert
    //   |sequence| in a PriorityQueue once it's done running the Task.
    const auto sequence_sort_key = sequence->GetSortKey();
    priority_queue->BeginTransaction()->Push(
        WrapUnique(new PriorityQueue::SequenceAndSortKey(std::move(sequence),
                                                         sequence_sort_key)));
  }

  return sequence_was_empty;
}

}  // namespace internal
}  // namespace base
