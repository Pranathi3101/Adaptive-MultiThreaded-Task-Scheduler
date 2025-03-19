#ifndef TASK_H
#define TASK_H

#include <functional>

class Task {
private:
    int id;
    int priority;
    std::function<void()> execution;
    double cpuRequirement;
    double memoryRequirement;

public:
    Task(int id, int priority, std::function<void()> exec, 
         double cpuReq = 1.0, double memReq = 1.0);

    int getId() const;
    int getPriority() const;
    void setPriority(int newPriority);
    void execute();
    double getCpuRequirement() const;
    double getMemoryRequirement() const;
};

#endif // TASK_H