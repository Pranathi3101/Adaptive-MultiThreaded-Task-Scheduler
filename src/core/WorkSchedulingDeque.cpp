#include <deque>
#include <mutex>
#include <optional>

class WorkStealingDeque {
private:
    std::deque<Task> tasks;
    mutable std::mutex mutex;

public:
    void pushFront(Task task) {
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push_front(std::move(task));
    }

    std::optional<Task> popFront() {
        std::lock_guard<std::mutex> lock(mutex);
        if (tasks.empty()) return std::nullopt;
        Task task = std::move(tasks.front());
        tasks.pop_front();
        return task;
    }

    std::optional<Task> popBack() {
        std::lock_guard<std::mutex> lock(mutex);
        if (tasks.empty()) return std::nullopt;
        Task task = std::move(tasks.back());
        tasks.pop_back();
        return task;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return tasks.empty();
    }
};