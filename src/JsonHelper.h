#ifndef JSONOBJECTPROCESSOR_JSONHELPER_H
#define JSONOBJECTPROCESSOR_JSONHELPER_H


#include <jsoncpp/json/value.h>
#include "Entity.h"

class JsonHelper {
public:
    static Json::Value getJSONRoot(std::string jsonString);
    static long getId(Json::Value);
    static double getCost(Json::Value value);
    static std::vector<double> getCostComponents(Json::Value);
    static Entity::JsonObject getJsonObject(std::string jsonString);
};


#endif //JSONOBJECTPROCESSOR_JSONHELPER_H
