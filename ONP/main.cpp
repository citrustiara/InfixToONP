#include <iostream>
#include <string>
#include <iomanip>
#include "parser.hpp"
#include "calculator.hpp"

static std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\n\r");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\n\r");
    return s.substr(b, e - b + 1);
}

int main() {
    std::cout << "Podaj wyrazenie infix (zakoncz klikajac enter), np. 3.5+4.2*2 :\n";

    std::string expr;
    std::getline(std::cin, expr, '\n');
    expr = trim(expr);

    if (expr.empty()) {
        std::cout << "Brak wyrazenia.\n";
        return 0;
    }

    auto onp = Parser::infixToONP(expr);

    std::cout << "ONP: ";
    for (size_t i = 0; i < onp.size(); ++i) {
        const std::string& token = onp[i];

        bool nextIsFunc = (i + 1 < onp.size()) &&
            onp[i + 1].size() == 1 &&
            (onp[i + 1][0] == 'X' || onp[i + 1][0] == 'N');

        if (nextIsFunc) {
            int encoded = 0;
            for (unsigned char c : token) encoded += c;
            std::cout << encoded << " ";
        }
        else {
            std::cout << token << " ";
        }
    }
    std::cout << "\n\n";

    try {
        double result = Calculator::evaluateONP(onp);
        std::cout << "\nWynik: " << std::fixed << std::setprecision(2)
            << result << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "\nBlad podczas obliczen: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}