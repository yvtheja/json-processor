#include <iostream>
#include "QueryResult.h"

void QueryResult::addToAvgSum(double value) {
    std::unique_lock<std::mutex> lock(avgSumMutex);
    avgSum += value;
}

void QueryResult::addToProcessedObjects(long value) {
    std::unique_lock<std::mutex> lock(processedObjectsMutex);
    processedObjects += value;
}

void QueryResult::compareAndSetMaxCostComponent(double value) {
    std::unique_lock<std::mutex> lock(maxCostComponentMutex);
    if(value <= maxCostComponent) return;

    maxCostComponent = value;
}

void QueryResult::pushToCostThresholdedIds(long id) {
    std::unique_lock<std::mutex> lock(costThresholdedIdsMutex);
    costThresholdedIds.push_back(id);
}

void QueryResult::pushToCostCompThresholdedIds(long id) {
    std::unique_lock<std::mutex> lock(costCompThresholdedIdsMutex);
    costCompThresholdedIds.push_back(id);
}

double QueryResult::getAvgSum() {
    return avgSum;
}

long QueryResult::getProcessedObjects() {
    return processedObjects;
}

double QueryResult::getMaxCostComponent() {
    return maxCostComponent;
}

bool QueryResult::getCostThresholdedIds(std::vector<long>& value) {
    value = std::move(costThresholdedIds);
    return true;
}

bool QueryResult::getCostCompThresholdedIds(std::vector<long>& value) {
    value = std::move(costCompThresholdedIds);
    return true;
}