
#include "ImmutableString.h"

#include "Application.h"
#include "MemoryPool.h"
#include "spdlog/spdlog.h"

String foo() {
    String str = "hello";
    return str;
}


void TestStringAllocation() {

    String str = foo();
    String u = str.ToUpper();

    // 栈上的, 都在这里析构
}

void TestShareObject() {

    String a = "share";

    String b = a;
}

void TestHeapObject() {
    auto str = FNEW(String, "hello");

    // 堆对象需要单独添加作用域解除, 因此很麻烦, 不推荐
    GetGarbageCollector()->Remove(str);
}

void TestGC() {
    auto r1 = FALLOC(1);
    auto s1 = FALLOC(1);
    auto s2 = FALLOC(1);
    auto s3 = FALLOC(1);

    GetGarbageCollector()->Trace(r1, nullptr, "root");
    GetGarbageCollector()->Trace(s1, r1, "sub");
    GetGarbageCollector()->Trace(s2, s1, "sub2");
    GetGarbageCollector()->Trace(s3, r1, "sub2");
    GetGarbageCollector()->Remove(r1);
}


int main() {

    auto app = GetApplication();
    app->Init();

    TestStringAllocation();
    TestShareObject();
    //TestHeapObject();

    //TestGC();


    app->Exit();

    return 0;
}
