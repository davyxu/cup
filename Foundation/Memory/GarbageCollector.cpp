//
// Created by davyxu on 2025/1/28.
//

#include "GarbageCollector.h"
#include "spdlog/spdlog.h"
#include <list>

class GarbageCollector : public IGarbageCollector {
public:
    void Add(IGCObject *obj) override {
        _objectList.push_back(obj);
    }

    void Collect() override {
        for (auto kv: _objectList) {
            spdlog::debug("GarbageCollector.Collect {}", (void *) kv);
            kv->Release();
        }

        _objectList.clear();
    }


    typedef std::list<IGCObject *> ObjectList;
    ObjectList _objectList;

};

IGarbageCollector *GetGarbageCollector() {
    static GarbageCollector ins;
    return &ins;
}
