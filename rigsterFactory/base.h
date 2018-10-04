#ifndef BASE_H
#define BASE_H
#include <iostream>

namespace YHL {

class Base{
public:
    virtual ~Base() = default;
    void addRecord() {

        // before

        add();

        // after
    }
private:
    virtual void add() {
        std::cout << "add in Base\n";
    }
};

}


#endif // BASE_H
