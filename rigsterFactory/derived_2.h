#ifndef DERIVED_2_H
#define DERIVED_2_H
#include "base.h"
#include <iostream>


namespace YHL {

class Derived_2 : public Base {
private:
    virtual void add() override {
        std::cout << "add in Derived_2\n";
    }
};

}

#endif // DERIVED_1_H
