//
// Created by davyxu on 2025/1/28.
//

#pragma once

class IGCObject {
public:
    virtual ~IGCObject() = default;
};


class IGarbageCollector {
public:

    virtual void Collect() = 0;

    virtual void Trace(void *mem, void *parent, const char *comment) = 0;

    virtual void Trace(IGCObject *obj, void *parent, const char *comment) = 0;

    virtual void Remove(void *mem) = 0;
};


IGarbageCollector *GetGarbageCollector();