//
// Created by davyxu on 2025/1/28.
//

#include <cstring>
#include "ImmutableString.h"
#include "MemoryPool.h"
#include "GarbageCollector.h"
#include "spdlog/spdlog.h"


String::String(const char *str) {
    spdlog::debug("String {}", (void *) this);

    GetGarbageCollector()->Trace(this, nullptr, "String");

    if (str != nullptr) {
        Make(strlen(str));
        memcpy(_data, str, _len);
    }
}


String::String(const String &other) {
    spdlog::debug("String copy {}", (void *) this);
    _data = other._data;
    _len = other._len;
}


String::~String() {
    spdlog::debug("~String {}", (void *) this);

    GetGarbageCollector()->Remove(this);

}

void String::Make(int size) {
    _len = size;
    SourceLocation loc(__FILE__, __LINE__);
    loc.Parent = this;
    _data = (char *) GetMemoryPool()->Alloc(loc, size + 1);
    GetGarbageCollector()->Trace(_data, this, "String.data");
}

//
//bool String::operator==(const String &other) const {
//    if (this == &other)
//        return true;
//
//    if (_len != other._len)
//        return false;
//
//    return strcmp(_data, other._data);
//}


String String::ToUpper() {
    String str;
    str.Make(_len);
    for (int i = 0; i < _len; i++) {
        str._data[i] = toupper(_data[i]);
    }

    return str;
}



