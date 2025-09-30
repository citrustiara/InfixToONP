#pragma once
#include <string>
#include <vector>

class Parser {
public:
    // zamiana wyrażenia infixowego na ONP
    static std::vector<std::string> infixToONP(const std::string& expr);
};

// dodatkowe funkcje pomocnicze
int precedence(char op);
bool isdigit1(char c);
