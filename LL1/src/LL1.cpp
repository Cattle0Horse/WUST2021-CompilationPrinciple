/**
 * @Time: 2023/5/7 21:12
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe: LL(1) 的判断、预测分析表、判断一个句子是否为该文法的句型
 *              字符串终止符 '#' 下标为最大字符数 - 1
 */
#include "LL1.h"

namespace LL1_compile {
    int LL1::getIndexInTerminal(char c) {
        return terminal_id[c];
    }

    int LL1::getIndexInNonTerminal(char c) {
        return non_terminal_id[c];
    }

    char LL1::getNameInTerminal(int index) {
        return terminals[index].getName();
    }

    char LL1::getNameInNonTerminal(int index) {
        return non_terminals[index].getName();
    }

    bool LL1::isNonTerminal(char c) {
        return non_terminal_id.find(c) != non_terminal_id.end();
    }

    bool LL1::isTerminal(char c) {
        return terminal_id.find(c) != terminal_id.end();
    }

    bool LL1::checkProduction(const Production &production) {
        if (!isTerminal(production.getLeft())) {
            return false;
        }
        for (char ch: production.getRight()) {
            if (!isTerminal(ch) && !isNonTerminal(ch)) {
                return false;
            }
        }
        return true;
    }


    bool LL1::initNonTerminals(const LL1::Sentence &name) {
        for (int i = 0, n = static_cast<int>(name.size()); i < n; ++i) {
            if (name[i] == '#') {
                return false;
            }
            if (!isNonTerminal(name[i])) {
                non_terminal_id[name[i]] = i;
                non_terminals.emplace_back(name[i], i);
            }
        }
        if (non_terminals.size() > NODE_MAX_SIZE - 1) return false;
        return true;
    }

    bool LL1::initTerminals(const LL1::Sentence &name) {
        for (int i = 0, n = static_cast<int>(name.size()); i < n; ++i) {
            if (name[i] == '#') {
                return false;
            }
            if (isNonTerminal(name[i])) {
                return false;
            }
            if (terminal_id.find(name[i]) == terminal_id.end()) {
                terminal_id[name[i]] = i;
                terminals.emplace_back(name[i], i);
            }
        }
        if (terminals.size() > NODE_MAX_SIZE - 2) {
            return false;
        }
        terminal_id['#'] = END_INDEX;
        return true;
    }

    /**
     * 计算能推出 空 的非终结符（书 P73）
     */
    bool LL1::initCanLaunchEmpty(const std::vector<Production> &_productions) {
        productions = _productions;
        int nt_size = static_cast<int>(non_terminals.size());
        int p_size = static_cast<int>(productions.size());
        std::vector<std::vector<std::vector<char>>> adj_production(nt_size); // 以某非终结符为左部的产生式，一种邻接表的存储
        // (1)
        can_launch_empty.fill(-1);
        // (2)
        for (int i = 0; i < p_size; ++i) {
            auto left = productions[i].getLeft();
            if (!isNonTerminal(left)) {
                return false; // 非终结符中不含该左部符号
            }
            const auto &right = productions[i].getRight();
            if (right.empty()) { // 该产生式右部为空
                can_launch_empty[getIndexInNonTerminal(left)] = 1; // 标记为可以推出空
                continue;
            }
            bool mark = false; // 是否含有 终结符
            for (auto name: right) {
                if (isTerminal(name)) {
                    mark = true;
                } else {
                    if (!isNonTerminal(name)) {
                        return false; // 该符号不存在于两个集合中任意一个
                    }
                }
            }
            if (!mark) {
                adj_production[getIndexInNonTerminal(left)].emplace_back(right);
            }
        }
        for (int i = 0; i < nt_size; ++i) {
            if (can_launch_empty[i] == -1 && adj_production[i].empty()) {
                can_launch_empty[i] = 0; // 标记为 不能推出空
            }
        }

        // (4) 此时 adj_production 中只有 右部均为非终结符的 产生式
        while (true) {
            // (3)
            bool edited = false;
            for (int i = 0; i < adj_production.size(); ++i) { // i 为左部非终结符下标
                for (int j = 0; j < adj_production[i].size(); ++j) {
                    auto &right = adj_production[i][j];
                    for (int k = 0; k < right.size();) {
                        int id = getIndexInNonTerminal(right[k]);
                        if (can_launch_empty[id] == 1) { // 如果是 ”能“
                            right.erase(right.begin() + k); // 删去该非终结符
                            if (right.empty()) {
                                can_launch_empty[i] = 1;
                                adj_production[i].clear();
                                edited = true;
                                break;
                            }
                        } else if (can_launch_empty[id] == 0) { // 如果是 ”不能“
                            edited = true;
                            adj_production[i].erase(adj_production[i].begin() + j); // 删去当前产生式
                            if (adj_production[i].empty()) {
                                can_launch_empty[i] = 0;
                                edited = true;
                                break;
                            }
                        } else {
                            ++k;
                        } // 若是 ”未定“，则判断下一个符号
                    }
                }
            }
            if (!edited) {
                break;
            }
        }
        return true;
    }

    // 计算单个非终结符的 first 集合（递归求解）
    void LL1::setFirstSetOfSingleNonTerminal(char target) {
        int target_index = getIndexInNonTerminal(target);
        if (has_cal_first[target_index]) return;
        auto &collection = first[target_index];
        for (const auto &production: productions) {
            // 左端与目标匹配
            if (production.getLeft() == target) {
                const auto &right = production.getRight();
                // 空的直接跳过
                if (right.empty()) {
                    continue;
                }
                // 终结符直接加入
                if (isTerminal(right[0])) {
                    collection.set(getIndexInTerminal(right[0]), true);
                } else {
                    for (auto ch: right) {
                        // 遇到终结符停止递归
                        if (isTerminal(ch)) {
                            collection.set(getIndexInTerminal(ch), true);
                            break;
                        }
                        // 若为非终结符，则应先求出该字符的 First 集合
                        setFirstSetOfSingleNonTerminal(ch);
                        // 将 First(ch) 中，除 空 以外的结果加入 First(target) 中
                        first[target_index] |= first[getIndexInNonTerminal(ch)];
                        // 如果当前符号不可以推出空，则停止
                        if (!can_launch_empty[getIndexInNonTerminal(ch)]) {
                            break;
                        }
                    }
                }
            }
        }
        has_cal_first[target_index] = true;
    }

    void LL1::initFirst() {
        has_cal_first.resize(non_terminals.size(), false);
        first.resize(non_terminals.size());
        for (int i = 0, n = static_cast<int>(first.size()); i < n; ++i) {
            setFirstSetOfSingleNonTerminal(getNameInNonTerminal(i));
        }
    }

    // 计算单个非终结符 follow 集合（由于递归会无限循环，因此迭代求解）
    void LL1::setFollowSetOfSingleNonTerminal(char) {
        bool change = true;
        while (change) { // Follow集合发生变化就执行
            change = false;
            for (const auto &production: productions) {
                const auto &right = production.getRight();
                for (int i = 0, n = static_cast<int>(right.size()); i < n; ++i) {
                    auto current_char = right[i];
                    if (isNonTerminal(right[i])) { // 是非终结符
                        auto pre_size = follow[getIndexInNonTerminal(current_char)].count();
                        if (i != n - 1) { // 不是最后一个字符
                            auto next_char = right[i + 1];
                            if (isTerminal(next_char)) {
                                follow[getIndexInNonTerminal(current_char)].set(getIndexInTerminal(next_char), true);
                            } else {
                                follow[getIndexInNonTerminal(current_char)] |= first[getIndexInNonTerminal(next_char)];
                                // 若能推出空
                                if (can_launch_empty[getIndexInNonTerminal(next_char)]) {
                                    follow[getIndexInNonTerminal(current_char)] |= follow[getIndexInNonTerminal(next_char)];
                                }
                            }
                        } else { // 为最后一个字符时，即 A->aB
                            // Follow(B)+=Follow(A)
                            follow[getIndexInNonTerminal(current_char)] |= follow[getIndexInNonTerminal(production.getLeft())];
                        }
                        if (pre_size != follow[getIndexInNonTerminal(current_char)].count()) {
                            change = true; // Follow集合发生变化
                        }
                    }
                }
            }
        }
    }

    void LL1::initFollow() {
        follow.resize(non_terminals.size());
        for (int i = 0, n = static_cast<int>(follow.size()); i < n; ++i) {
            follow[i].set(END_INDEX, true);
            setFollowSetOfSingleNonTerminal(getNameInNonTerminal(i));
        }
    }

    void LL1::initSelect() {
        // 检查该产生式是否可以推出空
        auto check = [&](int index) -> bool {
            for (char ch: productions[index].getRight()) {
                if (isTerminal(ch)) {
                    return false;
                } else {
                    if (!can_launch_empty[getIndexInNonTerminal(ch)]) {
                        return false;
                    }
                }
            }
            return true;
        };

        /**
         * 求多个符号的 first 集合
         * 1. First(aAB) = {a}
         * 2. First(AB...) = {} 与 A、B是否可以推导为空有关
         */
        auto getFirstOfRight = [&](const std::vector<char> &right) -> Bitset {
            Bitset ans{};
            for (char ch: right) {
                if (isTerminal(ch)) {
                    ans.set(getIndexInTerminal(ch));
                    break;
                } else {
                    ans |= first[getIndexInNonTerminal(ch)];
                    if (!can_launch_empty[getIndexInNonTerminal(ch)]) {
                        break;
                    }
                }
            }
            return ans;
        };

        int production_size = static_cast<int> (productions.size());
        select.resize(production_size);

        for (int i = 0; i < production_size; ++i) {
            if (check(i)) {
                // 1. 如果这个产生式可以推出ε，那么它的SELECT集是{FIRST(该产生式右部)-ε} ∪ FOLLOW(该产生式左部的非终结符)。
                select[i] = getFirstOfRight(productions[i].getRight()) | follow[getIndexInNonTerminal(productions[i].getLeft())];
            } else {
                // 2. 如果这个产生式不能推出ε，那么它的SELECT集是{FIRST(该产生式右部)}。
                select[i] = getFirstOfRight(productions[i].getRight());
            }
        }
    }

    void LL1::initTable() {
        // 找到 ch in select(left->any) 的产生式的下标，若存在多个，则返回 -2，若不存在，则返回-1
        auto getResultProductionIndex = [&](char left, char ch) -> int {
            int ans = -1;
            for (int i = 0, n = static_cast<int>(productions.size()); i < n; ++i) {
                const auto &production = productions[i];
                if (production.getLeft() == left) {
                    if (select[i][getIndexInTerminal(ch)]) {
                        if (ans != -1) {
                            return -2;
                        }
                        ans = i;
                    }
                }
            }
            return ans;
        };

        // 预测分析表 列数+1 用于存放#
        int n = static_cast<int>(non_terminals.size()), m = static_cast<int>(terminals.size());
        table.resize(n, std::vector<int>(m + 1, -1));

        for (int i = 0; i < n; ++i) {
            char left = non_terminals[i].getName();
            for (int j = 0; j < m; ++j) {
                table[i][j] = getResultProductionIndex(left, terminals[j].getName());
            }
            table[i][m] = getResultProductionIndex(left, '#');
        }
    }

    LL1::LL1() {}

    // 请确保 产生式集 不含非法成分（如产生式左部存在不在非终结符集中的符号）
    LL1::LL1(
            const Sentence &_non_terminals,
            const Sentence &_terminals,
            const std::vector<Production> &_productions) {
        if (!initNonTerminals(_non_terminals)) {
            throw "计算非终结符集错误";
        }
        if (!initTerminals(_terminals)) {
            throw "计算终结符集错误";
        }
        if (!initCanLaunchEmpty(_productions)) {
            throw "计算能推出空的非终结符错误";
        }
        initFirst();
        initFollow();
        initSelect();
        initTable();
    }


    std::string LL1::collectionToString(const std::vector<LL1::Bitset> &collection, int index) {
        std::string ans{};
        bool not_begin = false;
        for (int i = 0, n = std::min(static_cast<int>(terminals.size()), END_INDEX); i < n; ++i) {
            if (collection[index][i]) {
                if (not_begin) {
                    ans += ", ";
                }
                not_begin = true;
                ans += getNameInTerminal(i);
            }
        }
        if (collection[index][END_INDEX]) {
            if (not_begin) {
                ans += ", ";
            }
            ans += '#';
        }
        return ans;
    }

    void LL1::printFirst() {
        std::cout << "-----------First集合-------------\n" << std::endl;
        for (int i = 0, n = static_cast<int>(first.size()); i < n; ++i) {
            auto ans = collectionToString(first, i);
            if (can_launch_empty[i]) {
                if (!ans.empty()) {
                    ans += ", ";
                }
                ans += "ε";
            }
            std::cout << "First(" << getNameInNonTerminal(i) << ")\t=\t{" << ans << "}" << std::endl;
        }
    }

    void LL1::printFollow() {
        std::cout << "-----------Follow集合-------------\n" << std::endl;
        for (int i = 0, n = static_cast<int>(follow.size()); i < n; ++i) {
            //auto ans = collectionToString(follow, i);
            //if (!ans.empty()) {
            //    ans += ", ";
            //}
            //ans += '#';
            std::cout << "Follow(" << getNameInNonTerminal(i) << ")\t=\t{" << collectionToString(follow, i) << "}" << std::endl;
        }
    }

    void LL1::printSelect() {
        std::cout << "-----------Select集合-------------\n" << std::endl;
        for (int i = 0, n = static_cast<int>(select.size()); i < n; ++i) {
            std::cout << "Select(" << productions[i].toString() << ")\t=\t{" << collectionToString(select, i) << "}" << std::endl;
        }
    }

    void LL1::printTable() {
        auto getValue = [&](int index) -> std::string {
            if (index < 0) {
                return "error";
            }
            return std::to_string(index);
            //return productions[index].toString();
        };

        std::cout << "-----------预测分析表-------------\n" << std::endl;
        for (const auto &terminal: terminals) {
            std::cout << '\t' << terminal.getName();
        }
        std::cout << "\t#" << std::endl;
        for (int i = 0, n = static_cast<int>(table.size()); i < n; ++i) {
            std::cout << non_terminals[i].getName();
            for (int index: table[i]) {
                std::cout << '\t' << getValue(index);
            }
            std::cout << std::endl;
        }
    }

    bool LL1::judge(const std::vector<char> &sentence) {
        std::vector<char> s{};
        s.emplace_back('#');
        s.emplace_back(terminals[0].getName());
        int now = 0;
        char ch = sentence[now++];
        while (s.size() != 1) {
            char top = s.back();
            s.pop_back();
            if (isTerminal(top)) {
                if (top == ch) {
                    ch = sentence[now++];
                } else {
                    return false;
                }
            } else {
                if (top == '#') {
                    int index = table[getIndexInNonTerminal(top)][getIndexInTerminal(ch)];
                    if (index == -1) {
                        return false;
                    }
                    const auto &right = productions[index].getRight();
                    for (int i = static_cast<int>(right.size()) - 1; i >= 0; --i) {
                        s.push_back(right[i]);
                    }
                }
            }
            if (s.back() == ch) {
                return true;
            }
        }
        return false;
    }
}
