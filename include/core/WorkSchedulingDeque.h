#ifndef WORK_STEALING_DEQUE_H
#define WORK_STEALING_DEQUE_H

#include <deque>
#include <mutex>
#include <optional>
#include "task.h"

class WorkStealingDeque {
private:
    std::deque<Task> tasks;
    mutable std::mutex mutex;

public:
    void pushFront(Task task);
    std::optional<Task> popFront();
    std::optional<Task> popBack();
    bool isEmpty() const;
};

#endif // WORK_STEALING_DEQUE_H