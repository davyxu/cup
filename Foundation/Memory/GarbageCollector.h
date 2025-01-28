//
// Created by davyxu on 2025/1/28.
//

#pragma once

class IGCObject {
public:
    virtual void Release() = 0;
};


class IGarbageCollector {
public:
    virtual void Add(IGCObject *obj) = 0;

    virtual void Collect() = 0;
};


IGarbageCollector *GetGarbageCollector();