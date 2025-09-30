#include "calculator.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

static void printStackVec(const std::vector<int>& st,
    const std::vector<bool>& isEncoded) { //isEncoded vector shows which values were encoded. since the vaue pushed is already encoded this does not chage how the number is displayed, it just lets us write in the printed stack which values are arg counts, this can help with understanding and visibility
    std::cout << "[";
    for (size_t i = 0; i < st.size(); ++i) {
        if (i) std::cout << ", ";
        if (isEncoded[i])
            std::cout << "func arg count:" << st[i];  // mark encoded values
        else
            std::cout << st[i];
    }
    std::cout << "]";
}


int Calculator::evaluateONP(const std::vector<std::string>& tokens) {
    std::vector<int> st;
    std::vector<bool> isEncoded;
    std::cout << "Kroki obliczen ONP:\n";

    for (size_t idx = 0; idx < tokens.size(); ++idx) {
        const std::string& token = tokens[idx];

        // check the NEXT token, checks if it is X(max) or N(min), possible to add more functions in the future
        bool nextIsFunc = (idx + 1 < tokens.size()) &&
            tokens[idx + 1].size() == 1 &&
            (tokens[idx + 1][0] == 'X' || tokens[idx + 1][0] == 'N');

        // normal classification
        bool isOperator = (token.size() == 1 &&
            std::string("+-*/XN").find(token) != std::string::npos);
        char* endptr = nullptr;
        std::strtol(token.c_str(), &endptr, 10);
        bool isNumber = (*endptr == '\0');

        // encode the CURRENT token if the NEXT token is N or X, the number of arguments is written as a char bc it is easier this way, we have to encode this for displaying purposes, it doesnt change how the program works
        if (nextIsFunc) {
            int encoded = 0;
            isEncoded.push_back(true);
            for (unsigned char c : token) encoded += c;
            std::cout << idx + 1 << ". Token: '" << encoded
                <<"' ->liczba argumentow funkcji, push "
                << encoded << " ; stack: ";

            st.push_back(encoded);
            printStackVec(st, isEncoded);
            std::cout << "\n";
            continue;   // skip normal handling
        }

        // encode unknown tokens - this is not needed 
        else {
            isEncoded.push_back(false);
        }

        // 🔹 normal handling for operators/numbers
        std::cout << idx + 1 << ". Token: '" << token << "' -> ";

        // operator jednoznakowy + - * /
        if (token.size() == 1 && std::string("+-*/").find(token) != std::string::npos) {
            if (st.size() < 2) {
                std::cerr << "Error: za malo operandow dla operatora '" << token << "'\n";
                throw std::runtime_error("Niewystarczajaca liczba operandow");
            }
            int b = st.back(); st.pop_back();
            int a = st.back(); st.pop_back();
            std::cout << "operator '" << token << "', pobrano a=" << a << ", b=" << b;

            int res = 0;
            switch (token[0]) {
            case '+': res = a + b; break;
            case '-': res = a - b; break;
            case '*': res = a * b; break;
            case '/':
                if (b == 0) {
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
            int arg_count = st.back(); st.pop_back();
            int argument = st.back(); st.pop_back();
            std::cout << "pobrano arg = " << argument << ", ";
            int max = argument;
            int min = argument;
            for (int j = 1; j < arg_count; j++) {
                argument = st.back(); st.pop_back();
                std::cout <<"pobrano arg = " << argument <<", ";
                if (argument > max) max = argument;
                if (argument < min) min = argument;
            }
            int res = 0;
            switch (token[0]) {
            case 'X': res = max; break;
            case 'N': res = min; break;
            }
            st.push_back(res);
            std::cout << " -> push " << res << " ; stack: ";
            printStackVec(st, isEncoded);
            std::cout << "\n";
        
        }
        else { // traktujemy token jako liczbę (wielocyfrową) 
        char* endptr = nullptr;
        long val = std::strtol(token.c_str(), &endptr, 10);
        if (*endptr != '\0') { // token nie jest ani liczbą ani rozpoznanym operatorem
            std::cout << "nieznany token (pomin): '" << token << "'\n"; continue; }
        st.push_back(static_cast<int>(val)); std::cout << "liczba, push " << val << " ; stack: ";
        printStackVec(st, isEncoded); std::cout << "\n"; 
        }

    }

    if (st.empty()) {
        throw std::runtime_error("Pusty stos po obliczeniach");
    }

    int result = st.back();
    if (st.size() > 1) {
        std::cout << "Uwaga: po obliczeniach na stosie pozostalo więcej elementow: ";
        printStackVec(st, isEncoded);
        std::cout << "\n";
    }

    std::cout << "Koniec krokow obliczen.\n";
    return result;
}
