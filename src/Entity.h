#ifndef JSONOBJECTPROCESSOR_ENTITY_H
#define JSONOBJECTPROCESSOR_ENTITY_H

namespace Entity {
    struct JsonObject {
        long id;
        double cost;
        std::vector<double> costComponents;
    };
}

#endif //JSONOBJECTPROCESSOR_ENTITY_H
