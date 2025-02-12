//
// Created by davyxu on 2025/1/28.
//

#pragma once

#include <new>
#include <utility>
#include <memory>
#include "Types.h"
#include <vector>

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
            throw std::bad_alloc();;
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

template<typename T>
class CustomAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    CustomAllocator() = default;

    ~CustomAllocator() = default;

    template<typename U>
    CustomAllocator(const CustomAllocator<U> &) {}

    T *allocate(std::size_t n) {
        assert(n == 1 && "DO NOT SUPPORTS NEW ARRAY");
        SourceLocation loc;
        return (T *) GetMemoryPool()->Alloc(loc, n * sizeof(T));
    }

    void deallocate(T *p, std::size_t) {
        GetMemoryPool()->Free(p);
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
};

template<typename T, typename... Args>
std::shared_ptr<T> MakePtr(Args &&... args) {
    return std::allocate_shared<T>(CustomAllocator<T>(), std::forward<Args>(args)...);
}

template<typename T>
using TVECTOR = std::vector<T, CustomAllocator<T>>;