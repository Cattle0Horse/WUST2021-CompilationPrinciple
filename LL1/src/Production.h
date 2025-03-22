/**
 * @Time: 2023/5/7 22:33
 * @Author: Cattle_Horse
 * @Describe: LL(1) 产生式
 */

#ifndef LL1_PRODUCTION_H
#define LL1_PRODUCTION_H

#include <vector>
#include <string>

namespace LL1_compile {
    std::vector<char> toVectorChar(const std::string &);

    class Production {
    private:
        char left{}; // 产生式左部
        std::vector<char> right; // 产生式右部
    public:
        Production() = default;

        Production(char _left, const std::vector<char> &_right);

        char getLeft() const;

        void setLeft(char _left);

        [[nodiscard]] const std::vector<char> &getRight() const;

        void setRight(const std::vector<char> &_right);

        bool setProduction(const std::string &);

        std::string toString();

        friend bool toProduction(const std::string &, Production &);

    };

}


#endif //LL1_PRODUCTION_H
