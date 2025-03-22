/**
 * @Time: 2023/5/7 21:12
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe: LL(1) ���жϡ�Ԥ��������ж�һ�������Ƿ�Ϊ���ķ��ľ���
 *              �ַ�����ֹ�� '#' �±�Ϊ����ַ��� - 1
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
     * �������Ƴ� �� �ķ��ս������ P73��
     */
    bool LL1::initCanLaunchEmpty(const std::vector<Production> &_productions) {
        productions = _productions;
        int nt_size = static_cast<int>(non_terminals.size());
        int p_size = static_cast<int>(productions.size());
        std::vector<std::vector<std::vector<char>>> adj_production(nt_size); // ��ĳ���ս��Ϊ�󲿵Ĳ���ʽ��һ���ڽӱ�Ĵ洢
        // (1)
        can_launch_empty.fill(-1);
        // (2)
        for (int i = 0; i < p_size; ++i) {
            auto left = productions[i].getLeft();
            if (!isNonTerminal(left)) {
                return false; // ���ս���в������󲿷���
            }
            const auto &right = productions[i].getRight();
            if (right.empty()) { // �ò���ʽ�Ҳ�Ϊ��
                can_launch_empty[getIndexInNonTerminal(left)] = 1; // ���Ϊ�����Ƴ���
                continue;
            }
            bool mark = false; // �Ƿ��� �ս��
            for (auto name: right) {
                if (isTerminal(name)) {
                    mark = true;
                } else {
                    if (!isNonTerminal(name)) {
                        return false; // �÷��Ų���������������������һ��
                    }
                }
            }
            if (!mark) {
                adj_production[getIndexInNonTerminal(left)].emplace_back(right);
            }
        }
        for (int i = 0; i < nt_size; ++i) {
            if (can_launch_empty[i] == -1 && adj_production[i].empty()) {
                can_launch_empty[i] = 0; // ���Ϊ �����Ƴ���
            }
        }

        // (4) ��ʱ adj_production ��ֻ�� �Ҳ���Ϊ���ս���� ����ʽ
        while (true) {
            // (3)
            bool edited = false;
            for (int i = 0; i < adj_production.size(); ++i) { // i Ϊ�󲿷��ս���±�
                for (int j = 0; j < adj_production[i].size(); ++j) {
                    auto &right = adj_production[i][j];
                    for (int k = 0; k < right.size();) {
                        int id = getIndexInNonTerminal(right[k]);
                        if (can_launch_empty[id] == 1) { // ����� ���ܡ�
                            right.erase(right.begin() + k); // ɾȥ�÷��ս��
                            if (right.empty()) {
                                can_launch_empty[i] = 1;
                                adj_production[i].clear();
                                edited = true;
                                break;
                            }
                        } else if (can_launch_empty[id] == 0) { // ����� �����ܡ�
                            edited = true;
                            adj_production[i].erase(adj_production[i].begin() + j); // ɾȥ��ǰ����ʽ
                            if (adj_production[i].empty()) {
                                can_launch_empty[i] = 0;
                                edited = true;
                                break;
                            }
                        } else {
                            ++k;
                        } // ���� ��δ���������ж���һ������
                    }
                }
            }
            if (!edited) {
                break;
            }
        }
        return true;
    }

    // ���㵥�����ս���� first ���ϣ��ݹ���⣩
    void LL1::setFirstSetOfSingleNonTerminal(char target) {
        int target_index = getIndexInNonTerminal(target);
        if (has_cal_first[target_index]) return;
        auto &collection = first[target_index];
        for (const auto &production: productions) {
            // �����Ŀ��ƥ��
            if (production.getLeft() == target) {
                const auto &right = production.getRight();
                // �յ�ֱ������
                if (right.empty()) {
                    continue;
                }
                // �ս��ֱ�Ӽ���
                if (isTerminal(right[0])) {
                    collection.set(getIndexInTerminal(right[0]), true);
                } else {
                    for (auto ch: right) {
                        // �����ս��ֹͣ�ݹ�
                        if (isTerminal(ch)) {
                            collection.set(getIndexInTerminal(ch), true);
                            break;
                        }
                        // ��Ϊ���ս������Ӧ��������ַ��� First ����
                        setFirstSetOfSingleNonTerminal(ch);
                        // �� First(ch) �У��� �� ����Ľ������ First(target) ��
                        first[target_index] |= first[getIndexInNonTerminal(ch)];
                        // �����ǰ���Ų������Ƴ��գ���ֹͣ
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

    // ���㵥�����ս�� follow ���ϣ����ڵݹ������ѭ������˵�����⣩
    void LL1::setFollowSetOfSingleNonTerminal(char) {
        bool change = true;
        while (change) { // Follow���Ϸ����仯��ִ��
            change = false;
            for (const auto &production: productions) {
                const auto &right = production.getRight();
                for (int i = 0, n = static_cast<int>(right.size()); i < n; ++i) {
                    auto current_char = right[i];
                    if (isNonTerminal(right[i])) { // �Ƿ��ս��
                        auto pre_size = follow[getIndexInNonTerminal(current_char)].count();
                        if (i != n - 1) { // �������һ���ַ�
                            auto next_char = right[i + 1];
                            if (isTerminal(next_char)) {
                                follow[getIndexInNonTerminal(current_char)].set(getIndexInTerminal(next_char), true);
                            } else {
                                follow[getIndexInNonTerminal(current_char)] |= first[getIndexInNonTerminal(next_char)];
                                // �����Ƴ���
                                if (can_launch_empty[getIndexInNonTerminal(next_char)]) {
                                    follow[getIndexInNonTerminal(current_char)] |= follow[getIndexInNonTerminal(next_char)];
                                }
                            }
                        } else { // Ϊ���һ���ַ�ʱ���� A->aB
                            // Follow(B)+=Follow(A)
                            follow[getIndexInNonTerminal(current_char)] |= follow[getIndexInNonTerminal(production.getLeft())];
                        }
                        if (pre_size != follow[getIndexInNonTerminal(current_char)].count()) {
                            change = true; // Follow���Ϸ����仯
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
        // ���ò���ʽ�Ƿ�����Ƴ���
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
         * �������ŵ� first ����
         * 1. First(aAB) = {a}
         * 2. First(AB...) = {} �� A��B�Ƿ�����Ƶ�Ϊ���й�
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
                // 1. ����������ʽ�����Ƴ��ţ���ô����SELECT����{FIRST(�ò���ʽ�Ҳ�)-��} �� FOLLOW(�ò���ʽ�󲿵ķ��ս��)��
                select[i] = getFirstOfRight(productions[i].getRight()) | follow[getIndexInNonTerminal(productions[i].getLeft())];
            } else {
                // 2. ����������ʽ�����Ƴ��ţ���ô����SELECT����{FIRST(�ò���ʽ�Ҳ�)}��
                select[i] = getFirstOfRight(productions[i].getRight());
            }
        }
    }

    void LL1::initTable() {
        // �ҵ� ch in select(left->any) �Ĳ���ʽ���±꣬�����ڶ�����򷵻� -2���������ڣ��򷵻�-1
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

        // Ԥ������� ����+1 ���ڴ��#
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

    // ��ȷ�� ����ʽ�� �����Ƿ��ɷ֣������ʽ�󲿴��ڲ��ڷ��ս�����еķ��ţ�
    LL1::LL1(
            const Sentence &_non_terminals,
            const Sentence &_terminals,
            const std::vector<Production> &_productions) {
        if (!initNonTerminals(_non_terminals)) {
            throw "������ս��������";
        }
        if (!initTerminals(_terminals)) {
            throw "�����ս��������";
        }
        if (!initCanLaunchEmpty(_productions)) {
            throw "�������Ƴ��յķ��ս������";
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
        std::cout << "-----------First����-------------\n" << std::endl;
        for (int i = 0, n = static_cast<int>(first.size()); i < n; ++i) {
            auto ans = collectionToString(first, i);
            if (can_launch_empty[i]) {
                if (!ans.empty()) {
                    ans += ", ";
                }
                ans += "��";
            }
            std::cout << "First(" << getNameInNonTerminal(i) << ")\t=\t{" << ans << "}" << std::endl;
        }
    }

    void LL1::printFollow() {
        std::cout << "-----------Follow����-------------\n" << std::endl;
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
        std::cout << "-----------Select����-------------\n" << std::endl;
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

        std::cout << "-----------Ԥ�������-------------\n" << std::endl;
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
