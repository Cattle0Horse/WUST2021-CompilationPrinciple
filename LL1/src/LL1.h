/**
 * @Time: 2023/5/7 21:12
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe: LL(1) ���жϡ�Ԥ��������ж�һ�������Ƿ�Ϊ���ķ��ľ���
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
        static constexpr int NODE_MAX_SIZE = 6; // ���ڵ����
        static constexpr int END_INDEX = NODE_MAX_SIZE - 1; // # ��ֹ���±�
        using Bitset = std::bitset<NODE_MAX_SIZE>;
        using Sentence = std::vector<char>; // LL1 ����
        using SymbolVector = std::vector<Symbol>;

        int getIndexInTerminal(char); // ���ս�����еõ��ַ���Ӧ���±�
        int getIndexInNonTerminal(char); // �ڷ��ս�����еõ��ַ���Ӧ���±�
        char getNameInTerminal(int); // �õ���Ӧ�±���ս��
        char getNameInNonTerminal(int); // �õ���Ӧ�±��Ӧ�ķ��ս��
        bool isTerminal(char); // �Ƿ�Ϊ�ս��
        bool isNonTerminal(char); // �Ƿ�Ϊ���ս��
        bool checkProduction(const Production &); // �жϸò���ʽ�Ƿ���ֲ����������������е��ַ�

        bool initNonTerminals(const Sentence &_nonTerminals); // ��ʼ�����ս������
        bool initTerminals(const Sentence &_terminals); // ��ʼ���ս������
        bool initCanLaunchEmpty(const std::vector<Production> &_productions); // ��ʼ�� �ս���Ƿ�����Ƴ��� �ļ���
        void setFirstSetOfSingleNonTerminal(char); // ���㵥�����ս���� first ���ϣ��ݹ���⣩
        void initFirst(); // ��ʼ�� first ����
        void setFollowSetOfSingleNonTerminal(char); // ���㵥�����ս�� follow ���ϣ����ڵݹ������ѭ������˵�����⣩
        void initFollow(); // ��ʼ�� follow ����
        void initSelect(); // ��ʼ�� select ����
        void initTable(); // ��ʼ��Ԥ�������
        std::string collectionToString(const std::vector<Bitset> &collction, int index); // �����Է�����ʽ���First��follow����
        void printFirst(); // ��ӡ First ����

        void printFollow(); // ��ӡ Follow ����

        void printSelect(); // ��ӡ Select ����

        void printTable(); // ��ӡ Ԥ�������

        /*
         * ʹ�ô��޲ι��캯��ʱ����Ҫ���ε������³�ʼ��������
         * 1. initNonTerminals
         * 2. initTerminals
         * 3. initCanLaunchEmpty
         * 4. initFirst
         * 5. initFollow
         * 6. initSelect
         * 7. initTable
         */
        LL1();

        // ���ȷ�� ����ʽ�� û�����⣬�����ʹ�øù��췽��
        LL1(const Sentence &_non_terminals, const Sentence &_terminals, const std::vector<Production> &_productions);

        bool judge(const Sentence &sentence); // �жϾ����Ƿ��Ǹ� LL1 �ķ���

    private:
        SymbolVector non_terminals; // ���ս����
        std::unordered_map<char, int> non_terminal_id; // ���ս����Ӧ id

        SymbolVector terminals; // �ս����
        std::unordered_map<char, int> terminal_id; // �ս����Ӧ id

        std::vector<bool> has_cal_first; // �Ƿ��Ѿ������ first


        std::array<int, NODE_MAX_SIZE> can_launch_empty; // idΪcan_launch_empty�±���ս���Ƿ�����Ƴ��� ��������Ҫδ��״̬�����Բ���ѡ��ʹ�� bitset��
        std::vector<Bitset> first{}, follow{}, select{}; // ����Ҫ���м��ϲ��������ѡ�� bitset

        std::vector<Production> productions;

        std::vector<std::vector<int>> table; // Ԥ����������н��Ϊ ����ʽ ���±ꣻ��Ϊ -1 ��˵˵������
    };
}

#endif //LL1_LL1_H
