#include <iostream>
#include <chrono>
#include <thread>
#include "../include/core/Scheduler.h"
#include "../include/ml/workload_predictor.h"

// Simulated task function
void simulatedTask(int taskId, int duration) {
    std::cout << "Task " << taskId << " started." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    std::cout << "Task " << taskId << " completed." << std::endl;
}

int main() {
    std::cout << "Adaptive Multithreaded Task Scheduler" << std::endl;

    // Initialize the scheduler with 4 threads
    Scheduler scheduler(4);
    WorkloadPredictor predictor;

    // Start the scheduler
    scheduler.start();

    // Add some test tasks with varying priorities and resource requirements
    for (int i = 0; i < 20; ++i) {
        int priority = i % 3;  // 0, 1, or 2
        double cpuReq = (i % 5) + 1;  // 1 to 5
        double memReq = (i % 3) + 1;  // 1 to 3
        int duration = (i % 5 + 1) * 100;  // 100 to 500 ms

        Task task(i, priority, [i, duration]() { simulatedTask(i, duration); }, cpuReq, memReq);
        scheduler.addTask(task);

        // Simulate workload prediction
        float predictedTime = predictor.predictExecutionTime(cpuReq, memReq);
        std::cout << "Predicted execution time for Task " << i << ": " << predictedTime << " ms" << std::endl;
    }

    // Let the scheduler run for a while
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop the scheduler
    scheduler.stop();

    std::cout << "Scheduler stopped. Final processor request: " << scheduler.getProcessorRequest() << std::endl;

    return 0;
}