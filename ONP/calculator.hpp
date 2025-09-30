#pragma once
#include <vector>
#include <string>

class Calculator {
public:
    // Change return type from int to double
    static double evaluateONP(const std::vector<std::string>& tokens);
};