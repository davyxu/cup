//
// Created by davyxu on 2025/1/28.
//

#pragma once

#include "GarbageCollector.h"


class String : public IGCObject {
public:
    String(const char *str = nullptr);

    String(const String &other);

    ~String() override;

    // 禁止拷贝赋值操作符
//    String &operator=(const String &) = delete;
//
//    bool operator==(const String &other) const;

    String ToUpper();

    const char *Str() { return _data; }

private:
    void Make(int size);

private:
    char *_data = nullptr;
    int _len = 0;
};
