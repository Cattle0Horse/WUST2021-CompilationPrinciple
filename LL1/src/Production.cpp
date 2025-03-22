/**
 * @Time: 2023/5/7 22:33
 * @Author: Cattle_Horse
 * @Describe: LL(1) 产生式
 */

#include "Production.h"

namespace LL1_compile {
    std::vector<char> toVectorChar(const std::string &str) {
        std::vector<char> ans{};
        for (char c: str)ans.emplace_back(c);
        return ans;
    }

    Production::Production(char _left, const std::vector<char> &_right) : left(_left), right(_right) {}

    char Production::getLeft() const {
        return this->left;
    }

    void Production::setLeft(char _left) {
        left = _left;
    }

    const std::vector<char> &Production::getRight() const {
        return this->right;
    }

    void Production::setRight(const std::vector<char> &_right) {
        right = _right;
    }

    bool Production::setProduction(const std::string &str) {
        if (str.length() < 3) return false;
        if (str[1] != '-' || str[2] != '>') return false;
        setLeft(str[0]);
        setRight(toVectorChar(str.substr(3)));
        return true;
    }

    bool toProduction(const std::string &str, Production &production) {
        if (str.length() < 3) return false;
        if (str[1] != '-' || str[2] != '>') return false;
        production.setLeft(str[0]);
        production.setRight(toVectorChar(str.substr(3)));
        return true;
    }

    std::string Production::toString() {
        std::string ans = left + std::string("->");
        for (char ch: right) {
            ans += ch;
        }
        return ans;
    }


}