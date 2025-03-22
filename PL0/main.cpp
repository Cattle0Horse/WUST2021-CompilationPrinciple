/**
 * @Time: 2023/4/18 11:48
 * @Author: Cattle_Horse
 * @Encoding: GBK
 * @Describe: To Test PL0 Analysis
 */
#include "PL0.h"
#include <iostream>

int main() {
    std::string input_file(R"(D:\Clion\project\PL0\example.txt)");
    std::string output_file(R"(D:\Clion\project\PL0\ans.txt)");
    auto ans = PL0::analysisFile(input_file, output_file);
    for (const auto &w: ans) {
        std::cout << w << std::endl;
    }
}