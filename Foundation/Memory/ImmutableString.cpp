//
// Created by davyxu on 2025/1/28.
//

#include <cstring>
#include "ImmutableString.h"
#include "MemoryPool.h"
#include "GarbageCollector.h"


String::String(const char *str) {
    GetGarbageCollector()->Add(this);

    if (str != nullptr) {
        Make(strlen(str));

        memcpy(_data, str, _len);
    }
}

void String::Make(int size) {
    _len = size;
    SourceLocation loc(__FILE__, __LINE__);
    loc.Parent = this;
    _data = (char *) GetMemoryPool()->Alloc(loc, size + 1);
}

String::~String() {

}

bool String::operator==(const String &other) const {
    if (this == &other)
        return true;

    if (_len != other._len)
        return false;

    return strcmp(_data, other._data);
}

void String::Release() {
    _len = 0;
    SourceLocation loc(__FILE__, __LINE__);
    loc.Parent = this;
    GetMemoryPool()->Free(loc, _data);
}

String String::ToUpper() {
    String str;
    str.Make(_len);
    for (int i = 0; i < _len; i++) {
        str._data[i] = toupper(_data[i]);
    }

    return str;
}

