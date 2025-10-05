#include "calculator.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>

static void printStackVec(const std::vector<double>& st,
    const std::vector<bool>& isEncoded) {
    std::cout << "[";
    for (size_t i = 0; i < st.size(); ++i) {
        if (i) std::cout << ", ";
        if (isEncoded[i])
            std::cout << "func arg count:" << static_cast<int>(st[i]);
        else
            std::cout << st[i];
    }
    std::cout << "]";
}

double Calculator::evaluateONP(const std::vector<std::string>& tokens) {
    std::vector<double> st;
    std::vector<bool> isEncoded;
    std::cout << "Kroki obliczen ONP:\n";
    std::cout << std::fixed << std::setprecision(2);

    for (size_t idx = 0; idx < tokens.size(); ++idx) {
        const std::string& token = tokens[idx];

        bool nextIsFunc = (idx + 1 < tokens.size()) &&
            tokens[idx + 1].size() == 1 &&
            (tokens[idx + 1][0] == 'X' || tokens[idx + 1][0] == 'N');

        bool isOperator = (token.size() == 1 &&
            std::string("+-*/XN").find(token) != std::string::npos);

        char* endptr = nullptr;
        std::strtod(token.c_str(), &endptr);
        bool isNumber = (*endptr == '\0');

        if (nextIsFunc) {
            int encoded = 0;
            isEncoded.push_back(true);
            for (unsigned char c : token) encoded += c;
            std::cout << idx + 1 << ". Token: '" << encoded
                << "' -> liczba argumentow funkcji, push "
                << encoded << " ; stack: ";

            st.push_back(static_cast<double>(encoded));
            printStackVec(st, isEncoded);
            std::cout << "\n";
            continue;
        }
        else {
            isEncoded.push_back(false);
        }

        std::cout << idx + 1 << ". Token: '" << token << "' -> ";

        if (token.size() == 1 && std::string("+-*/").find(token) != std::string::npos) {
            if (st.size() < 2) {
                std::cerr << "Error: za malo operandow dla operatora '" << token << "'\n";
                throw std::runtime_error("Niewystarczajaca liczba operandow");
            }
            double b = st.back(); st.pop_back();
            double a = st.back(); st.pop_back();
            std::cout << "operator '" << token << "', pobrano a=" << a << ", b=" << b;

            double res = 0.0;
            switch (token[0]) {
            case '+': res = a + b; break;
            case '-': res = a - b; break;
            case '*': res = a * b; break;
            case '/':
                if (b == 0.0) {
                    std::cout << " -> Error: dzielenie przez zero\n";
                    throw std::runtime_error("Dzielenie przez zero");
                }
                res = a / b;
                break;
            }
            st.push_back(res);
            std::cout << " -> push " << res << " ; stack: ";
            printStackVec(st, isEncoded);
            std::cout << "\n";
        }
        else if (token.size() == 1 && std::string("XN").find(token) != std::string::npos) {
            int arg_count = static_cast<int>(st.back()); st.pop_back();
            double argument = st.back(); st.pop_back();
            std::cout << "pobrano arg = " << argument << ", ";
            double max = argument;
            double min = argument;
            for (int j = 1; j < arg_count; j++) {
                argument = st.back(); st.pop_back();
                std::cout << "pobrano arg = " << argument << ", ";
                if (argument > max) max = argument;
                if (argument < min) min = argument;
            }
            double res = 0.0;
            switch (token[0]) {
            case 'X': res = max; break;
            case 'N': res = min; break;
            }
            st.push_back(res);
            std::cout << " -> push " << res << " ; stack: ";
            printStackVec(st, isEncoded);
            std::cout << "\n";
        }
        else {
            char* endptr = nullptr;
            double val = std::strtod(token.c_str(), &endptr);
            if (*endptr != '\0') {
                std::cout << "nieznany token (pomin): '" << token << "'\n";
                continue;
            }
            st.push_back(val);
            std::cout << "liczba, push " << val << " ; stack: ";
            printStackVec(st, isEncoded);
            std::cout << "\n";
        }
    }

    if (st.empty()) {
        throw std::runtime_error("Pusty stos po obliczeniach");
    }

    double result = st.back();
    if (st.size() > 1) {
        std::cout << "Uwaga: po obliczeniach na stosie pozostalo więcej elementow: ";
        printStackVec(st, isEncoded);
        std::cout << "\n";
    }

    std::cout << "Koniec krokow obliczen.\n";
    return result;
}