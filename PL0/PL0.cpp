/**
 * @Time: 2023/4/17 22:18
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe:
 */

#include "PL0.h"

namespace PL0 {
    unordered_set<string> basic_words{
            "const", "var", "procedure", "begin", "end", "odd", "if", "then", "call", "while", "do", "read", "write", "Cattle_Horse", "学号", "姓名"
    };  // 保留字们

    unordered_map<int, string> word_type_string{
            {_unsigned_integer,  "数　字"},
            {_identifier,        "标识符"},
            {_basic_word,        "保留字"},
            {_plus,              "运算符"},    // 加+
            {_minus,             "运算符"},    // 减-
            {_multiplication,    "运算符"},    // 乘*
            {_division,          "运算符"},    // 除/
            {_equal,             "运算符"},    // 等于=
            {_pound,             "运算符"},    // 井号# (不等)
            {_less,              "运算符"},    // 小于<
            {_less_more,         "运算符"},    // 小于大于<> (不等)
            {_less_equal,        "运算符"},    // 小于等于<=
            {_more,              "运算符"},    // 大于>
            {_more_equal,        "运算符"},    // 大于等于>=
            {_assignment,        "运算符"},    // 赋值:=
            {_parenthesis_left,  "界　符"},    // 左小括号
            {_parenthesis_right, "界　符"},    // 右小括号
            {_comma,             "界　符"},    // 逗号
            {_semicolon,         "界　符"},    // 分号
            {_dot,               "界　符"},    // 点
            {_illegal,           "非　法"},    // 非法
            {_not_judge,         "未判断"}};
    size_t word_count = 0;

    std::ofstream &operator<<(std::ofstream &out, const word &value) {
        out << to_string(value);
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const word &value) {
        out << to_string(value);
        return out;
    }

    word::word(size_t _no, word_type _type, string _word) : m_word_no(_no), m_type(_type), m_word(std::move(_word)) {}

    string to_string(const word &value) {
        return std::to_string(value.m_word_no) + '\t' + word_type_string[value.m_type] + '\t' + value.m_word;
    }

    bool checkChar(char ch) {
        return std::isalpha(ch) || ch < 0;
    }

    bool get_sym(vector<word> &words, const string &_word) {
        size_t i = 0, len = _word.length();
        bool ended = false;
        while (i < len && !ended) {
            string current_word{};
            word_type current_type = _not_judge;
            if (checkChar(_word[i])) {  // 字母或汉字打头
                current_word += _word[i++];
                // 字符是数字或者字母/汉字 加到当前词内
                while (i < len && (checkChar(_word[i]) || std::isdigit(_word[i]))) {
                    current_word += _word[i++];
                }
                // 保留字组里有当前词，则是保留字，否则是标识符
                if (basic_words.find(current_word) != basic_words.end()) {
                    current_type = _basic_word;
                } else {
                    current_type = _identifier;
                }
            } else if (std::isdigit(_word[i])) {  // 数字打头
                current_word += _word[i++];
                // 字符是数字 加到当前词内
                while (i < len && std::isdigit(_word[i])) {
                    current_word += _word[i++];
                }
                current_type = _unsigned_integer;
            } else if (_word[i] == ':') {  // 冒号打头
                current_word += _word[i++];
                if (i < len && _word[i] == '=') { // 是赋值号则加入词组
                    current_word += _word[i++];
                    current_type = _assignment;
                } else {// 否则定义为非法
                    current_type = _illegal;
                }
            } else if (_word[i] == '<') { // 小于号打头
                current_word += _word[i++];
                if (i < len && _word[i] == '=') { // 是等于号则定义为 小于等于
                    current_word += _word[i++];
                    current_type = _less_equal;
                } else if (i < len && _word[i] == '>') { // 是大于号则定义为 小于大于
                    current_word += _word[i++];
                    current_type = _less_more;
                } else { // 否则定义为小于
                    current_type = _less;
                }
            } else if (_word[i] == '>') { // 大于号打头
                current_word += _word[i++];
                // 是等于号则定义为 大于等于    否则定义为大于
                if (i < len && _word[i] == '=') {
                    current_word += _word[i++];
                    current_type = _more_equal;
                } else {
                    current_type = _more;
                }
            } else { // 否则按照单字符处理
                current_word += _word[i];
                switch (_word[i++]) {
                    case '+':current_type = _plus;
                        break;
                    case '-':current_type = _minus;
                        break;
                    case '*':current_type = _multiplication;
                        break;
                    case '/': current_type = _division;
                        break;
                    case '=':current_type = _pound;
                        break;
                    case '(':current_type = _parenthesis_left;
                        break;
                    case ')':current_type = _parenthesis_right;
                        break;
                    case ',':current_type = _comma;
                        break;
                    case ';':current_type = _semicolon;
                        break;
                    case '.':current_type = _dot;
                        ended = true;
                        break;
                }
            }
            words.emplace_back(++word_count, current_type, current_word);
        }
        return ended;
    }

    vector<word> analysisFile(const string &input_file, const string &output_file) {
        std::ifstream input(input_file);
        // 使用 ifstream 过滤空白字符，并进行分隔
        vector<word> words;
        for (string str; input >> str;) {
            if (get_sym(words, str)) {
                break;
            }
        }
        input.close();

        std::ofstream output(output_file);
        for (const word &w: words) {
            output << w << endl;
        }
        output.close();
        return words;
    }
}  // namespace PL0
