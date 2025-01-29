//
// Created by davyxu on 2025/1/28.
//

#include "GarbageCollector.h"
#include "spdlog/spdlog.h"
#include "MemoryPool.h"
#include <list>

struct GCMem;
typedef std::shared_ptr<GCMem> GCMemPtr;

typedef std::map<void *, GCMemPtr> GCMemByMem;
typedef std::list<GCMemPtr> GCMemList;


struct GCMem {
    int Ref = 0;
    bool Removed = false;
    GCMemPtr Parent;

    void *Mem = nullptr;

    IGCObject *Obj = nullptr;

    void *GetPtr() {
        if (Mem != nullptr)
            return Mem;

        return (void *) Obj;
    }
};


class GarbageCollector : public IGarbageCollector {
public:
    GarbageCollector() {

    }

    void LinkParent(void *parent, GCMemPtr memPtr) {
        if (parent != nullptr) {
            auto it = Flat.find(parent);
            if (it == Flat.end()) {
                spdlog::error("GarbageCollector::LinkParent parent not found {}", parent);
                return;
            }

            auto parentMem = it->second;
            memPtr->Parent = parentMem;
        }
    }

    void Trace(void *mem, void *parent, const char *comment) override {
        spdlog::debug("GarbageCollector::Trace mem:{} parent:{} comment:{}", mem, parent, comment);
        auto memPtr = std::make_shared<GCMem>();
        memPtr->Mem = mem;

        LinkParent(parent, memPtr);


        Flat[memPtr->GetPtr()] = memPtr;
    }

    void Trace(IGCObject *obj, void *parent, const char *comment) override {
        spdlog::debug("GarbageCollector::Trace obj:{} parent:{} comment:{}", (void *) obj, parent, comment);
        auto memPtr = std::make_shared<GCMem>();
        memPtr->Obj = obj;

        LinkParent(parent, memPtr);

        Flat[memPtr->GetPtr()] = memPtr;
    }

    void Remove(void *mem) override {
        spdlog::debug("GarbageCollector::Remove mem:{}", mem);
        auto it = Flat.find(mem);
        if (it == Flat.end()) {
            //spdlog::error("GarbageCollector::Remove mem not found {}", mem);
            return;
        }

        auto memPtr = it->second;
        memPtr->Removed = true;
        memPtr->Parent = nullptr;
    }


    void MarkReachable(const GCMemByMem &thisList, const GCMemByMem &targetParentList, GCMemByMem &nextParentList,
                       GCMemByMem &nextList) {

        for (auto it = thisList.begin(); it != thisList.end(); ++it) {
            auto memPtr = it->second;

            // 第一次, 父级为空
            if (targetParentList.empty()) {

                // 满足根节点
                if (!memPtr->Removed && memPtr->Parent == nullptr) {
                    memPtr->Ref = 1; // 跟节点可达
                    // 加入到下一次的父级节点中
                    spdlog::debug("node {} ref={}", memPtr->GetPtr(), memPtr->Ref);
                    nextParentList[memPtr->GetPtr()] = memPtr;
                } else {
                    memPtr->Ref = 0; // 节点复位
                    spdlog::debug("node {} ref={}", memPtr->GetPtr(), memPtr->Ref);
                    // 非根节点, 加入到下一次的遍历节点
                    nextList[memPtr->GetPtr()] = memPtr;
                }

            } else {
                // 节点是否在父级节点可达列表中
                auto itParent = targetParentList.find(memPtr->Parent->GetPtr());
                if (!memPtr->Removed && itParent != targetParentList.end()) {
                    memPtr->Ref++;
                    spdlog::debug("node {} ref={}", memPtr->GetPtr(), memPtr->Ref);
                    nextParentList[memPtr->GetPtr()] = memPtr;
                } else {
                    nextList[memPtr->GetPtr()] = memPtr;
                }
            }
        }
    }


    void CollectMem() {
        if (Flat.empty())
            return;

        GCMemByMem thisParentList, nextParentList, thisList, nextList;

        thisList = Flat;

        while (true) {
            spdlog::debug("MarkReachable thisList={} thisParentList={} nextParentList={} nextList={}",
                          thisList.size(),
                          thisParentList.size(),
                          nextParentList.size(),
                          nextList.size()
            );
            MarkReachable(thisList, thisParentList, nextParentList, nextList);
            if (nextParentList.size() == 0 || nextList.size() == 0) {
                break;
            }

            thisParentList = nextParentList;
            thisList = nextList;
            nextParentList.clear();
            nextList.clear();
        }

        for (auto kv: nextList) {
            auto memPtr = kv.second;

            if (memPtr->Mem != nullptr) {
                spdlog::debug("gc free: {}", memPtr->Mem);
                GetMemoryPool()->Free(memPtr->Mem);
            } else if (GetMemoryPool()->Exists(memPtr->Obj)) {
                GetMemoryPool()->Delete(memPtr->Obj);
            }

        }

    }


    void Collect() override {
        _collecting = true;

        CollectMem();

        _collecting = false;
    }


    GCMemPtr Root = std::make_shared<GCMem>();

    GCMemByMem Flat;

    bool _collecting = false;

};

IGarbageCollector *GetGarbageCollector() {
    static GarbageCollector ins;
    return &ins;
}
