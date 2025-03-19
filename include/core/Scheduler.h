#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <thread>
#include <atomic>
#include "work_stealing_deque.h"
#include "task.h"

class Scheduler {
private:
    std::vector<std::thread> workerThreads;
    std::vector<WorkStealingDeque> workerQueues;
    std::atomic<bool> running;
    int processorRequest;
    int processorAllotment;

    void workerFunction(int workerId);
    bool stealTask(int thiefId, Task& task);
    void updateProcessorRequest();

public:
    Scheduler(int numThreads = std::thread::hardware_concurrency());
    ~Scheduler();

    void start();
    void stop();
    void addTask(Task task);
    int getProcessorRequest() const;
    int getProcessorAllotment() const;
};

#endif // SCHEDULER_H