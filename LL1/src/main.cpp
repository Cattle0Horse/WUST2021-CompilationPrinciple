/**
 * @Time: 2023/5/8 15:14
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe: 
 */
#include "LL1.h"
#include <iostream>

using namespace LL1_compile;

/*
测试数据1
SABCD
y
abc
y
10
S->AB
S->bC
A->
A->b
B->
B->aD
C->AD
C->b
D->aS
D->c
y
a
 */



/*
测试数据2
SHMA
adbe
7
S->aH
H->aMd
H->d
M->Ab
M->
A->aM
A->e
 */
LL1 ll1{};
std::vector<Production> p;
std::string temp_str;
char op;

void inputNonTerminal() {
    do {
        std::cout << "请输入非终结符集，以换行结尾" << std::endl;
        std::getline(std::cin, temp_str); // SABCD
        for (char c: temp_str) {
            std::cout << ' ' << c;
        }
        std::cout << std::endl;
        do {
            std::cout << "输入正确确认?(y/n):";
            std::cin >> op;

        } while (op != 'y' && op != 'n');
    } while (op == 'n');
    ll1.initNonTerminals(toVectorChar(temp_str));
}

void inputTerminal() {
    do {
        std::getline(std::cin, temp_str);
        std::cout << "\n请输入终结符集，以换行结尾" << std::endl;
        std::getline(std::cin, temp_str); // abc
        for (char c: temp_str) {
            std::cout << ' ' << c;
        }
        do {
            std::cout << "\n输入正确确认?(y/n):";
            std::cin >> op;
        } while (op != 'y' && op != 'n');
    } while (op == 'n');
    ll1.initTerminals(toVectorChar(temp_str));
}

void inputProduction() {
    std::cout << "请输入文法产生式的个数：" << std::endl;
    int n;
    std::cin >> n;
    std::getline(std::cin, temp_str);
    std::cout << "请输入文法产生式，以换行分隔" << std::endl;
    std::cout << "例如：S->bA\n" << std::endl;
    int cnt = 0;
    while (cnt < n) {
        std::cout << "请输入第" << cnt + 1 << "个产生式" << std::endl;
        std::getline(std::cin, temp_str);
        Production production;
        if (!production.setProduction(temp_str)) {
            std::cout << "该产生式存在错误" << std::endl;
        } else {
            p.emplace_back(production);
            ++cnt;
        }
    }
}

void calculateCanLaunchEmpty() {
    ll1.initCanLaunchEmpty(p);
    std::cout << '\n' << std::endl;
}

void calculateFirst() {
    ll1.initFirst();
    ll1.printFirst();
    std::cout << '\n' << std::endl;
}

void calculateFollow() {
    ll1.initFollow();
    ll1.printFollow();
    std::cout << '\n' << std::endl;
}

void calculateSelect() {
    ll1.initSelect();
    ll1.printSelect();
    std::cout << '\n' << std::endl;
}

void calculateTable() {
    ll1.initTable();
    ll1.printTable();
    std::cout << '\n' << std::endl;
}

void judge() {
    do {
        std::cout << "\n是否继续进行句型分析？(y / n):";
        std::cin >> op;
        if (op == 'y') {
            std::getline(std::cin, temp_str);
            std::cout << "请输入符号串，以换行结束" << std::endl;
            do {
                std::getline(std::cin, temp_str);
            } while (temp_str.empty());
            auto sentence = toVectorChar(temp_str);
            if (ll1.judge(sentence)) {
                std::cout << temp_str << " 是该文法的句型" << std::endl;
            } else {
                std::cout << temp_str << " 不是该文法的句型" << std::endl;
            }
        }
    } while (op == 'y');
}

int main() {
    inputNonTerminal(); // 读入非终结符集
    inputTerminal(); // 读入终结符集
    inputProduction(); // 读入文法产生式
    calculateCanLaunchEmpty(); // 计算能推出 空 的非终结符
    calculateFirst(); // 计算 First 集合
    calculateFollow(); // 计算 Follow 集合
    calculateSelect(); // 计算 Select 集合
    calculateTable(); // 计算 预测分析表
    judge(); // 表驱动 LL(1) 分析
    return 0;
}