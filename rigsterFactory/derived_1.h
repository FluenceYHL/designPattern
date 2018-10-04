#ifndef DERIVED_1_H
#define DERIVED_1_H
#include "base.h"
#include <iostream>


namespace YHL {

class Derived_1 : public Base {
private:
    virtual void add() override {
        std::cout << "add in Derived_1\n";
    }
};


}

#endif // DERIVED_1_H
