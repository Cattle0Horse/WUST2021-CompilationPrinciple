/**
 * @Time: 2023/5/7 21:54
 * @Author: Cattle_Horse
 * @Describe: LL(1)中符号
 */

#include "Symbol.h"

namespace LL1_compile {
    Symbol::Symbol(char _name, int _number) : name(_name), number(_number) {}

    char Symbol::getName()const {
        return this->name;
    }

    void Symbol::setName(char _name) {
        name = _name;
    }

    int Symbol::getNumber()const {
        return this->number;
    }

    void Symbol::setNumber(int _number) {
        number = _number;
    }
}
