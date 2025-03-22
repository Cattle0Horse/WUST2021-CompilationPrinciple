/**
 * @Time: 2023/4/17 22:18
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe:
 */

#ifndef PL0_PL0_H
#define PL0_PL0_H

#include <cctype>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace PL0 {
    /*
     * 无符号整数
     * 标识符（变量函数等命名）
     * 保留字/基本字            const var procedure begin end odd if then call while do read write
     * 运算符                  + - * / = # < <> <= > >= :=
     * 界符                   ( ) , ; .
     */

    // 单词类型
    typedef enum {
        /* ---      无符号整数       --- */
        _unsigned_integer,

        /* ---      标识符         --- */
        _identifier,

        /* ---      基本字         --- */
        _basic_word,

        /* ---      运算符 start   --- */
        _plus,            // 加+
        _minus,           // 减-
        _multiplication,  // 乘*
        _division,        // 除/
        _equal,           // 等于=
        _pound,           // 井号# (不等)
        _less,            // 小于<
        _less_more,       // 小于大于<> (不等)
        _less_equal,      // 小于等于<=
        _more,            // 大于>
        _more_equal,      // 大于等于>=
        _assignment,      // 赋值:=
        /* ---      运算符 end     --- */

        /* ---      界符 start    --- */
        _parenthesis_left,   // 左小括号
        _parenthesis_right,  // 右小括号
        _comma,              // 逗号
        _semicolon,          // 分号
        _dot,                // 点
        /* ---      界符 end      --- */

        /* ---      非法符         --- */
        _illegal,
        /* ---      未判断         --- */
        _not_judge
    } word_type;

    using std::string, std::endl, std::vector, std::unordered_map, std::unordered_set;

    extern unordered_set<string> basic_words;            // 保留字们
    extern unordered_map<int, string> word_type_string;  // 根据id转化为对应名字，用于输出
    extern size_t word_count;

    class word {
        friend std::ofstream &operator<<(std::ofstream &out, const word &value);

        friend std::ostream &operator<<(std::ostream &out, const word &value);

        friend string to_string(const word &value);

    public:
        size_t m_word_no; // 序号
        word_type m_type; // 单词类型
        string m_word; // 单词符

        word(size_t _no, word_type _type, string _word);
    };

    /**
     *
     * @param ch
     * @return 判断ch是否是字母 或者 汉字
     */
    bool checkChar(char ch);

    /**
     * 对一个不含空白字符的单词进行分析
     * @param ans 输出位置
     * @param _word 不含空格的单词
     * @return 是否存在结束符
     */
    bool getSym(vector<word> &ans, const string &_word);

    /**
     * 文件-词法分析 - 输出到文件
     * @param input_file 待进行词法分析的文件路径
     * @param output_file 词法分析后的输出文件路径
     * @return 分析后的单词组
     */
    vector<word> analysisFile(const string &input_file, const string &output_file);
}  // namespace PL0

namespace PL0 {
    extern unordered_map<string, string> grammar;
    extern unordered_map<string, vector<vector<string>>> predictive_analysis_table;

    bool initGrammar(vector<string> _grammar);

    bool ll1Parser();
}

#endif  // PL0_PL0_H
