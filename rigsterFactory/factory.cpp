#include "factory.h"
#include "derived_1.h"
#include "derived_2.h"

YHL::mapType YHL::factory::rigsterMap = mapType();

YHL::factory* YHL::factory::getInstance () {
    static factory instance;      // 内部 GC 类析构也可
    return &instance;
}

YHL::Base* YHL::factory::produce (const std::string tar) {
    return rigsterMap.find(tar) == rigsterMap.end() ? nullptr : rigsterMap[tar]();
}

//std::shared_ptr<YHL::Base> YHL::factory::getShared (const std::string& tar) {
//    return std::make_shared<YHL::Base>(produce (tar));
//}

//std::unique_ptr<YHL::Base> YHL::factory::getUnique (const std::string& tar) {
//    return std::unique_ptr<YHL::Base>(produce (tar));
//}


