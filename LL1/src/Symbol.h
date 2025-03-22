/**
 * @Time: 2023/5/7 21:54
 * @Author: Cattle_Horse
 * @Describe: LL(1)中符号
 */

#ifndef LL1_SYMBOL_H
#define LL1_SYMBOL_H

namespace LL1_compile {

    class Symbol {
    private:
        char name;
        int number;
    public:
        Symbol() = delete;

        Symbol(char, int);

        char getName() const;

        void setName(char);

        int getNumber() const;

        void setNumber(int);

    };
}

#endif //LL1_SYMBOL_H
