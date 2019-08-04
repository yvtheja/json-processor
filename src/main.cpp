#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <thread>
#include <atomic>
#include "Entity.h"
#include "JsonHelper.h"
#include "Helper.h"
#include "SynchronizedQueue.h"
#include "QueryResult.h"

//static Entity::QueryResult queryResult;
static QueryResult queryResult;
//std::queue<std::string> jsonStringQueue;
SynchronizedQueue jsonStringQueue(100000);
std::atomic<double> jsonReadingTime(0);
std::atomic<double> jsonProcessingTime(0);

void getNextJson(char *string);
void processJsonObject(std::string);
void processJsonQueue();

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Invalid command line arguments\n";
        exit(1);
    }

    char *filePath = argv[1];

    time_t start, end;
    time(&start);
    const int JSON_PROCESSOR_THREAD_COUNT = 3;

    std::thread jsonLoader (getNextJson, filePath);

    std::thread threads[JSON_PROCESSOR_THREAD_COUNT];
    for(int i = 0; i < JSON_PROCESSOR_THREAD_COUNT; i++) {
        threads[i] = std::thread(processJsonQueue);
    }

    jsonLoader.join();
    for(int i = 0; i < JSON_PROCESSOR_THREAD_COUNT; i++) {
        threads[i].join();
    }

    time(&end);
    Helper::showQueryResult(queryResult);
//    double totalProgramTime = double(end - start);
//    std::cout << "TotalProgramTime: " << totalProgramTime << "\n";
    return 0;
}

void processJsonQueue() {
    while(true) {
        std::string jsonString;
        bool isElemPresent = jsonStringQueue.pop(jsonString);
        if(!isElemPresent) break;

        processJsonObject(jsonString);
    }
}

void processJsonObject(std::string jsonString) {
    const double CUT_OFF_COST = 95.0;
    const double CUT_OFF_COST_COMPONENT = 50.0;

    time_t start, end;
    time(&start);
    Entity::JsonObject jsonObject = JsonHelper::getJsonObject(jsonString);

    // Cost average
    queryResult.addToAvgSum(jsonObject.cost);
    queryResult.addToProcessedObjects(1);

    // Max of cost components
    bool isCostCompThresholdAdded = false;
    for (std::vector<double>::iterator it = jsonObject.costComponents.begin() ; it != jsonObject.costComponents.end(); ++it){
        queryResult.compareAndSetMaxCostComponent(*it);
        // Thresholded cost components
        if(*it > CUT_OFF_COST_COMPONENT && !isCostCompThresholdAdded) {
            queryResult.pushToCostCompThresholdedIds(jsonObject.id);
            isCostCompThresholdAdded = true;
        }
    }

    // Thresholded cost
    if(jsonObject.cost > CUT_OFF_COST) queryResult.pushToCostThresholdedIds(jsonObject.id);

    time(&end);
    jsonProcessingTime = jsonProcessingTime + double(end - start);
}

void getNextJson(char *string) {
    time_t start, end;

    time(&start);
    std::ifstream inFile;
    inFile.open(string);
    if(!inFile) {
        fprintf(stderr, "Can't open the file for reading");
        exit(1);
    }

    std::string jsonString;
    char character;

    while(inFile >> std::noskipws >> character) {
        jsonString += character;
        if(character == '}') {
            inFile >> std::noskipws >> character;
            jsonStringQueue.push(jsonString);
            jsonString = "";
        }
    }

    jsonStringQueue.requestShutdown();
    inFile.close();
    time(&end);

    jsonReadingTime = jsonReadingTime + double(end - start);
}