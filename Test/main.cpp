
#include "ImmutableString.h"

#include "Application.h"

String foo() {
    String str = "hello";
    return str;
}

int main() {

    auto app = GetApplication();
    app->Init();

    auto str = foo();
    auto u = str.ToUpper();


    app->Exit();
    
    return 0;
}
