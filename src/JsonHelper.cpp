#include <iostream>
#include <jsoncpp/json/json.h>
#include "JsonHelper.h"

Json::Value JsonHelper::getJSONRoot(std::string jsonString) {
    Json::Value root;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(jsonString,root);
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cerr << "Failed to parse configuration\n" << reader.getFormattedErrorMessages() << "\n" << jsonString << "\n";
        exit(1);
    }

    return root;
}

long JsonHelper::getId(Json::Value value) {
    return value.get("id", -1).asInt64();
}

double JsonHelper::getCost(Json::Value value) {
    return value.get("cost", -1).asDouble();
}

std::vector<double> JsonHelper::getCostComponents(Json::Value value){
    std::vector<double> costComponents;
    Json::Value costComponentsValue = value["cost_components"];
    for ( int index = 0; index < costComponentsValue.size(); ++index )  // Iterates over the sequence elements.
        costComponents.push_back(costComponentsValue[index].asDouble());

    return costComponents;
}

Entity::JsonObject JsonHelper::getJsonObject(std::string jsonString) {
    const Json::Value value = JsonHelper::getJSONRoot(jsonString);
    Entity::JsonObject jsonObject;
    jsonObject.id = JsonHelper::getId(value);
    jsonObject.cost = JsonHelper::getCost(value);
    jsonObject.costComponents = JsonHelper::getCostComponents(value);

    return jsonObject;
}