//
// Created by davyxu on 2025/1/28.
//

#pragma once

#include <new>
#include <utility>
#include "Types.h"

struct SourceLocation {
    void *Parent = nullptr;
    const char *TypeName = "";
    const char *File = "";
    int Line = 0;

    SourceLocation();

    SourceLocation(const char *file, int line);
};


class IMemoryPool {
public:
    virtual void *Alloc(SourceLocation loc, int size) = 0;

    virtual void Free(void *mem) = 0;

    virtual bool Exists(void *mem) = 0;

    virtual void ReportLeak() = 0;

    template<typename T, typename... Args>
    T *New(SourceLocation loc, Args &&... args) {

        loc.TypeName = GetTypeName<T>();

        // 分配内存
        void *memory = Alloc(loc, sizeof(T));
        if (memory == nullptr) {
            throw;
        }

        // 调用构造函数
        try {
            return new(memory) T(std::forward<Args>(args)...);  // 使用 placement new
        } catch (...) {
            Free(memory);  // 如果构造函数抛出异常，释放内存
            throw;
        }
    }

    // 自定义的 Delete 函数
    template<typename T>
    void Delete(T *ptr) {
        if (ptr) {
            ptr->~T();  // 调用析构函数
            Free(ptr);  // 释放内存
        }
    }

};


IMemoryPool *GetMemoryPool();


#define FNEW(Type, ...) GetMemoryPool()->New<Type>(SourceLocation(__FILE__, __LINE__ ), ##__VA_ARGS__)
#define FALLOC(Size) GetMemoryPool()->Alloc(SourceLocation(__FILE__, __LINE__ ), Size)