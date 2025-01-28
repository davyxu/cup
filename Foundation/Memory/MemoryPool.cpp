//
// Created by davyxu on 2025/1/28.
//

#include "MemoryPool.h"
#include "spdlog/spdlog.h"
#include <map>
#include <mutex>


struct MemoryContent {
    SourceLocation Location;
};


class SysMemoryPool : public IMemoryPool {
public:
    void *Alloc(SourceLocation loc, int size) override {
        std::lock_guard<std::mutex> lock(_contentByPtrMutex);
        auto mem = std::malloc(size);
        auto &content = _contentByPtr[mem];
        content.Location = loc;

        return mem;
    }

    void Free(SourceLocation loc, void *mem) override {
        std::lock_guard<std::mutex> lock(_contentByPtrMutex);
        auto it = _contentByPtr.find(mem);
        if (it == _contentByPtr.end())
            return;

        _contentByPtr.erase(mem);
        std::free(mem);
    }

    void ReportLeak() override {

        std::lock_guard<std::mutex> lock(_contentByPtrMutex);
        if (_contentByPtr.size() == 0)
            return;

        spdlog::debug("Memory leak: {}", _contentByPtr.size());
        for (auto kv: _contentByPtr) {
            auto &loc = kv.second.Location;
            spdlog::debug(" ptr: {} type: {} parent: {} loc: {}:{}", kv.first, loc.TypeName, loc.Parent, loc.File,
                          loc.Line);

        }
    }

private:
    typedef std::map<void *, MemoryContent> ContentByPtr;
    ContentByPtr _contentByPtr;
    std::mutex _contentByPtrMutex;
};


IMemoryPool *GetMemoryPool() {
    static SysMemoryPool ins;
    return &ins;
}

SourceLocation::SourceLocation() {

}

SourceLocation::SourceLocation(const char *file, int line) {
    File = file;
    Line = line;
}
