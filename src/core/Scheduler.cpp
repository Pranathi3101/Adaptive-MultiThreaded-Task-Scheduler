#include <vector>
#include <thread>
#include <atomic>
#include <random>

class Scheduler {
private:
    std::vector<std::thread> workerThreads;
    std::vector<WorkStealingDeque> workerQueues;
    std::atomic<bool> running;
    int processorRequest;
    int processorAllotment;

    void workerFunction(int workerId) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, workerQueues.size() - 1);

        while (running) {
            auto taskOpt = workerQueues[workerId].popFront();
            if (taskOpt) {
                taskOpt->execute();
            } else {
                // Try to steal from another queue
                int victimId = dist(gen);
                if (victimId != workerId) {
                    auto stolenTask = workerQueues[victimId].popBack();
                    if (stolenTask) {
                        stolenTask->execute();
                    } else {
                        // No work to do, sleep for a bit
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                }
            }
        }
    }

public:
    Scheduler(int numThreads = std::thread::hardware_concurrency())
        : running(false), processorRequest(numThreads), processorAllotment(numThreads) {
        workerQueues.resize(numThreads);
    }

    void start() {
        if (running) return;
        running = true;
        for (int i = 0; i < processorAllotment; ++i) {
            workerThreads.emplace_back(&Scheduler::workerFunction, this, i);
        }
    }

    void stop() {
        running = false;
        for (auto& thread : workerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        workerThreads.clear();
    }

    void addTask(Task task) {
        static int nextQueue = 0;
        workerQueues[nextQueue].pushFront(std::move(task));
        nextQueue = (nextQueue + 1) % workerQueues.size();
    }

    // Additional methods for adaptive scheduling and ML integration to be implemented
};