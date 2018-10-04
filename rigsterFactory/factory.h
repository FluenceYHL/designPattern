#ifndef FACTORY_H
#define FACTORY_H
#include <mutex>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>


namespace YHL {

class Base;
using mapType = std::unordered_map<std::string, std::function<Base*()> >;

//template <typename Base>
class factory final {
private:
    factory(){}
    factory(const factory&) = delete;
    factory(factory&&) = delete;

    static factory* getInstance();
    static std::unordered_map< std::string, std::function<Base*()> > rigsterMap;
public:
    static Base* produce(const std::string tar);

    //static std::shared_ptr<Base> getShared(const std::string&);

    //static std::unique_ptr<Base> getUnique(const std::string&);

    template<typename T>
    class rigsterHolder {
    public:
        rigsterHolder(const std::string& tar) {
            rigsterMap.emplace(tar, []{ return new T(); });
        }

        template<typename ...Args>
        rigsterHolder(const std::string& tar, Args... args) {
            rigsterMap.emplace(tar, [&]{ return new T(args...); });
        }
    };
};


}
#endif // FACTORY_H
