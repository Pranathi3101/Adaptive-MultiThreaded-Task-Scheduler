#include <functional>
#include <string>

class Task {
private:
    int id;
    int priority;
    std::function<void()> execution;
    double cpuRequirement;
    double memoryRequirement;

public:
    Task(int id, int priority, std::function<void()> exec, 
         double cpuReq = 1.0, double memReq = 1.0)
        : id(id), priority(priority), execution(exec),
          cpuRequirement(cpuReq), memoryRequirement(memReq) {}

    int getId() const { return id; }
    int getPriority() const { return priority; }
    void setPriority(int newPriority) { priority = newPriority; }
    void execute() { execution(); }
    double getCpuRequirement() const { return cpuRequirement; }
    double getMemoryRequirement() const { return memoryRequirement; }
};