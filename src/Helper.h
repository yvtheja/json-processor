#ifndef JSONOBJECTPROCESSOR_HELPER_H
#define JSONOBJECTPROCESSOR_HELPER_H
#include <iostream>
#include <queue>
#include "Entity.h"
#include "QueryResult.h"

class Helper {
private:
    std::mutex mutex;
    std::mutex mutex1;
public:
    void mutexCheckStar();
    void mutexCheckMinus();
    static void showq(std::queue <std::string>);
    static void keepShowingQueue(std::queue <std::string>);
    static void showJsonObject(std::string);
    static void showJsonObject(Entity::JsonObject);
//    static void showQueryResult(Entity::QueryResult queryResult);
    static void showQueryResult(QueryResult& queryResult);
};

#endif //JSONOBJECTPROCESSOR_HELPER_H
