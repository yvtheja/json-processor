#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <thread>
#include <atomic>
#include "Entity.h"
#include "JsonHelper.h"
#include "Helper.h"
#include "BlockingQueue.h"
#include "QueryResult.h"

static QueryResult queryResult;
BlockingQueue jsonStringQueue(100000);
std::atomic<double> jsonReadingTime(0);
std::atomic<double> jsonProcessingTime(0);

void readJsonStrings(char *string);
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

    std::thread jsonLoader (readJsonStrings, filePath);

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

/*
 * Reads JSON strings from the queue and passes them to processJsonObject.
 * It waits if the queue is empty
 * */
void processJsonQueue() {
    while(true) {
        std::string jsonString;
        bool isElemPresent = jsonStringQueue.pop(jsonString);
        /* Element is not present if the queue is empty and the shutdown request has been made */
        if(!isElemPresent) break;

        processJsonObject(jsonString);
    }
}

/*
 * Does calculations for all four queries.
 * */
void processJsonObject(std::string jsonString) {
    const double CUT_OFF_COST = 95.0;
    const double CUT_OFF_COST_COMPONENT = 50.0;

    time_t start, end;
    time(&start);
    Entity::JsonObject jsonObject = JsonHelper::getJsonObject(jsonString);

    /* Maintains the sum of all the costs */
    queryResult.addToAvgSum(jsonObject.cost);
    /* Maintains the count of JSON objects to calculate avg of costs at the end */
    queryResult.addToProcessedObjects(1);

    /* Iterates through cost components for 2nd and 4th queries */
    bool isCostCompThresholdAdded = false;
    for (std::vector<double>::iterator it = jsonObject.costComponents.begin() ; it != jsonObject.costComponents.end(); ++it){
        /* Maintains the maximum cost */
        queryResult.compareAndSetMaxCostComponent(*it);

        /* Adds Id to 4th query result if the costComponent is greater that 50.0 */
        if(*it > CUT_OFF_COST_COMPONENT && !isCostCompThresholdAdded) {
            queryResult.pushToCostCompThresholdedIds(jsonObject.id);
            isCostCompThresholdAdded = true;
        }
    }

    /* Push to 3rd query result if the cost is greater than 95.0 */
    if(jsonObject.cost > CUT_OFF_COST) queryResult.pushToCostThresholdedIds(jsonObject.id);

    time(&end);
    jsonProcessingTime = jsonProcessingTime + double(end - start);
}

/*
 * Reads the file character by character and populates the queue with JSON strings.
 * Assumes there are no nested json objects.
 * */
void readJsonStrings(char *string) {
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
            /* Waits if the queue is full */
            jsonStringQueue.push(jsonString);
            jsonString = "";
        }
    }

    /* Notify consumers to shutdown if the queue is emtpy */
    jsonStringQueue.requestShutdown();
    inFile.close();
    time(&end);

    jsonReadingTime = jsonReadingTime + double(end - start);
}