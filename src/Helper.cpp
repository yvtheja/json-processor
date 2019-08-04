#include <thread>
#include "Helper.h"
#include "JsonHelper.h"

void Helper::showq(std::queue <std::string> gq) {
    if(gq.size() < 1) return;

    std::queue <std::string> g = gq;
    while (!g.empty())
    {
        std::cout << '\n' << g.front();
        g.pop();
    }
    std::cout << '\n';
}

void Helper::keepShowingQueue(std::queue<std::string> queue) {
    while(true) {
        showq(queue);
    }
}

void Helper::showJsonObject(std::string jsonString) {
    const Json::Value value = JsonHelper::getJSONRoot(jsonString);
    long id = JsonHelper::getId(value);
    double cost = JsonHelper::getCost(value);
    std::vector<double> costComponents = JsonHelper::getCostComponents(value);

    std::cout << "Id: " << id << "\n";
    std::cout << "Cost: " << cost << "\n";
    std::cout << "Cost components: ";
    for (std::vector<double>::iterator it = costComponents.begin() ; it != costComponents.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << "\n";
}

void Helper:: showJsonObject(Entity::JsonObject jsonObject) {
    std::cout.precision(17);
    std::cout << "Id: " << jsonObject.id << "\n";
    std::cout << "Cost: " << jsonObject.cost << "\n";
    std::cout << "Cost components: ";
    for (std::vector<double>::iterator it = jsonObject.costComponents.begin() ; it != jsonObject.costComponents.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << "\n";
}

void Helper::printVector(std::vector<long>& vector) {
    int size = vector.size();
    for(int i = 0; i < size; i++){
        std::cout << vector[i];
        if(i + 1 < size) std::cout << ", ";
    }

    std::cout << "\n";
}

void Helper::showQueryResult(QueryResult& queryResult) {
    std::cout.precision(17);
//    std::cout << "Pocessed Objects: " << queryResult.getProcessedObjects() << "\n";
    std::cout << "1. " << queryResult.getAvgSum()/queryResult.getProcessedObjects() << "\n";
    std::cout << "2. " << queryResult.getMaxCostComponent() << "\n";

    std::vector<long> costThresholdedIds;
    queryResult.getCostThresholdedIds(costThresholdedIds);
    std::cout << "3. ";
    Helper::printVector(costThresholdedIds);

    std::vector<long> costCompThresholdedIds;
    queryResult.getCostCompThresholdedIds(costCompThresholdedIds);
    std::cout << "4. ";
    Helper::printVector(costCompThresholdedIds);
}

void Helper::mutexCheckStar() {
    std::unique_lock<std::mutex> lock(mutex);
    for(int i = 0; i < 5; i++) {
        std::this_thread::sleep_for (std::chrono::seconds(1));
        std::cout << "*";
    }
}

void Helper::mutexCheckMinus() {
    std::unique_lock<std::mutex> lock(mutex1);
    for(int i = 0; i < 5; i++) {
        std::this_thread::sleep_for (std::chrono::seconds(2));
        std::cout << "-";
    }
}
