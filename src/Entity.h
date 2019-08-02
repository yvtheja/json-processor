#ifndef JSONOBJECTPROCESSOR_ENTITY_H
#define JSONOBJECTPROCESSOR_ENTITY_H

namespace Entity {
    struct JsonObject {
        long id;
        double cost;
        std::vector<double> costComponents;
    };

//    struct QueryResult {
//        long processedObjects = 0;
//        double avgSum = 0;
//        double maxCostComponent = 0;
//        std::vector<long> costThresholdedIds;
//        std::vector<long> costCompThresholdedIds;
//    };
}

#endif //JSONOBJECTPROCESSOR_ENTITY_H
