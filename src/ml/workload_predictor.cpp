#include "../include/ml/workload_predictor.h"
#include <algorithm>
#include <numeric>
#include <cmath>

WorkloadPredictor::WorkloadPredictor() : cpuTimeCoeff(0), memoryUsageCoeff(0), intercept(0) {}

void WorkloadPredictor::train() {
    if (trainingData.size() < 2) return;

    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (const auto& metrics : trainingData) {
        sumX += metrics.cpuRequirement;
        sumY += metrics.actualExecutionTime;
        sumXY += metrics.cpuRequirement * metrics.actualExecutionTime;
        sumX2 += metrics.cpuRequirement * metrics.cpuRequirement;
    }

    int n = trainingData.size();
    cpuTimeCoeff = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    intercept = (sumY - cpuTimeCoeff * sumX) / n;

    // Reset training data after updating coefficients
    trainingData.clear();
}

float WorkloadPredictor::predictExecutionTime(float cpuRequirement, float memoryRequirement) {
    return cpuTimeCoeff * cpuRequirement + memoryUsageCoeff * memoryRequirement + intercept;
}

void WorkloadPredictor::addTrainingExample(TaskMetrics metrics) {
    trainingData.push_back(metrics);
    if (trainingData.size() >= TRAINING_THRESHOLD) {
        train();
    }
}

std::vector<TaskClass> WorkloadPredictor::clusterTasks(const std::vector<TaskMetrics>& tasks) {
    // Simple k-means clustering implementation
    const int k = 3; // Number of clusters
    std::vector<TaskClass> clusters(k);

    // Initialize cluster centroids randomly
    for (int i = 0; i < k; ++i) {
        clusters[i].avgCpuRequirement = rand() % 100;
        clusters[i].avgMemoryRequirement = rand() % 100;
    }

    bool changed;
    do {
        changed = false;
        std::vector<std::vector<TaskMetrics>> newClusters(k);

        // Assign each task to the nearest centroid
        for (const auto& task : tasks) {
            int nearestCluster = 0;
            float minDistance = std::numeric_limits<float>::max();

            for (int i = 0; i < k; ++i) {
                float distance = std::pow(task.cpuRequirement - clusters[i].avgCpuRequirement, 2) +
                                 std::pow(task.memoryRequirement - clusters[i].avgMemoryRequirement, 2);
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestCluster = i;
                }
            }

            newClusters[nearestCluster].push_back(task);
        }

        // Update centroids
        for (int i = 0; i < k; ++i) {
            if (!newClusters[i].empty()) {
                float sumCpu = 0, sumMem = 0;
                for (const auto& task : newClusters[i]) {
                    sumCpu += task.cpuRequirement;
                    sumMem += task.memoryRequirement;
                }
                float newAvgCpu = sumCpu / newClusters[i].size();
                float newAvgMem = sumMem / newClusters[i].size();

                if (newAvgCpu != clusters[i].avgCpuRequirement || newAvgMem != clusters[i].avgMemoryRequirement) {
                    changed = true;
                    clusters[i].avgCpuRequirement = newAvgCpu;
                    clusters[i].avgMemoryRequirement = newAvgMem;
                }
            }
        }
    } while (changed);

    return clusters;
}