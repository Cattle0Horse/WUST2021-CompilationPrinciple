/**
 * @Time: 2023/5/7 21:12
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe: LL(1) 的判断、预测分析表、判断一个句子是否为该文法的句型
 */

#ifndef LL1_LL1_H
#define LL1_LL1_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <array>
#include <iostream>
#include "Symbol.h"
#include "Production.h"

namespace LL1_compile {
    class LL1 {
    public:
        static constexpr int NODE_MAX_SIZE = 6; // 最大节点个数
        static constexpr int END_INDEX = NODE_MAX_SIZE - 1; // # 终止符下标
        using Bitset = std::bitset<NODE_MAX_SIZE>;
        using Sentence = std::vector<char>; // LL1 句型
        using SymbolVector = std::vector<Symbol>;

        int getIndexInTerminal(char); // 在终结符集中得到字符对应的下标
        int getIndexInNonTerminal(char); // 在非终结符集中得到字符对应的下标
        char getNameInTerminal(int); // 得到对应下标的终结符
        char getNameInNonTerminal(int); // 得到对应下标对应的非终结符
        bool isTerminal(char); // 是否为终结符
        bool isNonTerminal(char); // 是否为非终结符
        bool checkProduction(const Production &); // 判断该产生式是否出现不存在与两个集合中的字符

        bool initNonTerminals(const Sentence &_nonTerminals); // 初始化非终结符集合
        bool initTerminals(const Sentence &_terminals); // 初始化终结符集合
        bool initCanLaunchEmpty(const std::vector<Production> &_productions); // 初始化 终结符是否可以推出空 的集合
        void setFirstSetOfSingleNonTerminal(char); // 计算单个非终结符的 first 集合（递归求解）
        void initFirst(); // 初始化 first 集合
        void setFollowSetOfSingleNonTerminal(char); // 计算单个非终结符 follow 集合（由于递归会无限循环，因此迭代求解）
        void initFollow(); // 初始化 follow 集合
        void initSelect(); // 初始化 select 集合
        void initTable(); // 初始化预测分析表
        std::string collectionToString(const std::vector<Bitset> &collction, int index); // 用于以符号形式输出First与follow集合
        void printFirst(); // 打印 First 集合

        void printFollow(); // 打印 Follow 集合

        void printSelect(); // 打印 Select 集合

        void printTable(); // 打印 预测分析表

        /*
         * 使用此无参构造函数时，需要依次调用以下初始化方法：
         * 1. initNonTerminals
         * 2. initTerminals
         * 3. initCanLaunchEmpty
         * 4. initFirst
         * 5. initFollow
         * 6. initSelect
         * 7. initTable
         */
        LL1();

        // 如果确保 产生式集 没有问题，则可以使用该构造方法
        LL1(const Sentence &_non_terminals, const Sentence &_terminals, const std::vector<Production> &_productions);

        bool judge(const Sentence &sentence); // 判断句型是否是该 LL1 文法的

    private:
        SymbolVector non_terminals; // 非终结符集
        std::unordered_map<char, int> non_terminal_id; // 非终结符对应 id

        SymbolVector terminals; // 终结符集
        std::unordered_map<char, int> terminal_id; // 终结符对应 id

        std::vector<bool> has_cal_first; // 是否已经求出了 first


        std::array<int, NODE_MAX_SIZE> can_launch_empty; // id为can_launch_empty下标的终结符是否可以推出空 （由于需要未定状态，所以不能选择使用 bitset）
        std::vector<Bitset> first{}, follow{}, select{}; // 由于要进行集合操作，因此选择 bitset

        std::vector<Production> productions;

        std::vector<std::vector<int>> table; // 预测分析表。其中结果为 产生式 的下标；若为 -1 则说说明出错
    };
}

#endif //LL1_LL1_H
