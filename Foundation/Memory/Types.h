//
// Created by davyxu on 2025/1/28.
//

#pragma once


#include <cstdint>  // 包含固定大小整数类型
#include <typeinfo>

// 定义有符号整数类型
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

// 定义无符号整数类型
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

// 定义浮点数类型
using float32 = float;
using float64 = double;


template<typename T>
const char *GetTypeName() {

    auto name = typeid(T).name();
    return name + 1;
}