#include <iostream>
#include <string>
#include "parser.hpp"
#include "calculator.hpp"

// proste trimowanie
static std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\n\r");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\n\r");
    return s.substr(b, e - b + 1);
}

int main() {
    std::cout << "Podaj wyrazenie infix (zakoncz klikajac enter), np. 3+4*2. :\n";

    std::string expr;
    std::getline(std::cin, expr, '\n'); // wczytuje do pierwszej kropki
    expr = trim(expr);

    if (expr.empty()) {
        std::cout << "Brak wyrazenia.\n";
        return 0;
    }

    // Zamiana na ONP
    auto onp = Parser::infixToONP(expr);

    // Wypisz notację ONP
    std::cout << "ONP: ";
    for (size_t i = 0; i < onp.size(); ++i) {
        const std::string& token = onp[i];

        // check if the next token is N or X
        bool nextIsFunc = (i + 1 < onp.size()) &&
            onp[i + 1].size() == 1 &&
            (onp[i + 1][0] == 'X' || onp[i + 1][0] == 'N');

        if (nextIsFunc) {
            // encode the current token
            int encoded = 0;
            for (unsigned char c : token) encoded += c;
            std::cout << encoded << " ";
        }
        else {
            // normal print
            std::cout << token << " ";
        }
    }
    std::cout << "\n\n";


    // Oblicz z wypisywaniem kroków
    try {
        int result = Calculator::evaluateONP(onp);
        std::cout << "\nWynik: " << result << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "\nBlad podczas obliczen: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
