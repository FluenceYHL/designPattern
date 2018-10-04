#include <fstream>
#include <assert.h>
#include "scopeguard.h"
#include "logcall.h"
#include "base.h"
#include "factory.h"
#define  SAFE_DELETE(ptr) do { if(ptr) delete ptr, ptr = nullptr; } while(0);

int main() {
    YHL::Base* ptr = YHL::factory::produce ("Derived_1");
    ON_SCOPE_EXIT ([&]{ SAFE_DELETE(ptr)});
    assert(ptr not_eq nullptr);
    ptr->addRecord ();
    return 0;
}
