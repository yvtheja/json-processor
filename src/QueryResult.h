#ifndef JSONOBJECTPROCESSOR_QUERYRESULT_H
#define JSONOBJECTPROCESSOR_QUERYRESULT_H

#include <vector>
#include <mutex>

class QueryResult {
private:
    long processedObjects = 0;
    double avgSum = 0;
    double maxCostComponent = 0;
    std::vector<long> costThresholdedIds;
    std::vector<long> costCompThresholdedIds;
    std::mutex processedObjectsMutex;
    std::mutex avgSumMutex;
    std::mutex maxCostComponentMutex;
    std::mutex costThresholdedIdsMutex;
    std::mutex costCompThresholdedIdsMutex;
public:
    void addToProcessedObjects(long value);
    void addToAvgSum(double value);
    void compareAndSetMaxCostComponent(double value);
    void pushToCostThresholdedIds(long id);
    void pushToCostCompThresholdedIds(long id);
    long getProcessedObjects();
    double getAvgSum();
    double getMaxCostComponent();
    bool getCostThresholdedIds(std::vector<long>& value);
    bool getCostCompThresholdedIds(std::vector<long>& value);
};


#endif //JSONOBJECTPROCESSOR_QUERYRESULT_H
