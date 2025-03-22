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
��������1
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
��������2
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
        std::cout << "��������ս�������Ի��н�β" << std::endl;
        std::getline(std::cin, temp_str); // SABCD
        for (char c: temp_str) {
            std::cout << ' ' << c;
        }
        std::cout << std::endl;
        do {
            std::cout << "������ȷȷ��?(y/n):";
            std::cin >> op;

        } while (op != 'y' && op != 'n');
    } while (op == 'n');
    ll1.initNonTerminals(toVectorChar(temp_str));
}

void inputTerminal() {
    do {
        std::getline(std::cin, temp_str);
        std::cout << "\n�������ս�������Ի��н�β" << std::endl;
        std::getline(std::cin, temp_str); // abc
        for (char c: temp_str) {
            std::cout << ' ' << c;
        }
        do {
            std::cout << "\n������ȷȷ��?(y/n):";
            std::cin >> op;
        } while (op != 'y' && op != 'n');
    } while (op == 'n');
    ll1.initTerminals(toVectorChar(temp_str));
}

void inputProduction() {
    std::cout << "�������ķ�����ʽ�ĸ�����" << std::endl;
    int n;
    std::cin >> n;
    std::getline(std::cin, temp_str);
    std::cout << "�������ķ�����ʽ���Ի��зָ�" << std::endl;
    std::cout << "���磺S->bA\n" << std::endl;
    int cnt = 0;
    while (cnt < n) {
        std::cout << "�������" << cnt + 1 << "������ʽ" << std::endl;
        std::getline(std::cin, temp_str);
        Production production;
        if (!production.setProduction(temp_str)) {
            std::cout << "�ò���ʽ���ڴ���" << std::endl;
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
        std::cout << "\n�Ƿ�������о��ͷ�����(y / n):";
        std::cin >> op;
        if (op == 'y') {
            std::getline(std::cin, temp_str);
            std::cout << "��������Ŵ����Ի��н���" << std::endl;
            do {
                std::getline(std::cin, temp_str);
            } while (temp_str.empty());
            auto sentence = toVectorChar(temp_str);
            if (ll1.judge(sentence)) {
                std::cout << temp_str << " �Ǹ��ķ��ľ���" << std::endl;
            } else {
                std::cout << temp_str << " ���Ǹ��ķ��ľ���" << std::endl;
            }
        }
    } while (op == 'y');
}

int main() {
    inputNonTerminal(); // ������ս����
    inputTerminal(); // �����ս����
    inputProduction(); // �����ķ�����ʽ
    calculateCanLaunchEmpty(); // �������Ƴ� �� �ķ��ս��
    calculateFirst(); // ���� First ����
    calculateFollow(); // ���� Follow ����
    calculateSelect(); // ���� Select ����
    calculateTable(); // ���� Ԥ�������
    judge(); // ������ LL(1) ����
    return 0;
}