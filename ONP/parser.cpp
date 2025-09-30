#include "parser.hpp"
#include "node.hpp"
#include <cctype>
#include <stack>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isdigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

std::vector<std::string> Parser::infixToONP(const std::string& expr) {
    std::vector<std::string> output;
    std::stack<char> opStack;

    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];

        if (isdigit(c)) {
            std::string number;
            while (i < expr.size() && isdigit(expr[i]))
                number += expr[i++];
            --i;
            output.push_back(number);
        }
        else if (c == '(') {
            opStack.push(c);
        }
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                output.push_back(std::string(1, opStack.top()));
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop(); //popujemy nawias
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(c)) { //popujemy az dojdziemy do operatora o wyzszym priorytecie (* or /)
                output.push_back(std::string(1, opStack.top()));
                opStack.pop();
            }
            opStack.push(c); //pushujemy operator normalnie
        }
        else if (c == 'X' || c == 'N') { // X to max, N to min
            size_t j = i+2; // j to iterator, musimy sprawdzic ile argumentow przyjmie max lub min
            //i+2 bo w tej chwili expr[i] to X lub N, kolejne expr to '(' i dopiero expr[i+2] jest warty uwagi
            int bracket_counter = 1; // to counter nawiasow, '(' to +, ')' to -, jak dojdzie do 0 to sie konczy wyrazenie max albo min, zaczynamy od 1 bo pominelismy pierwszy nawias otwierający
            int argument_count = 1; //na podstawie liczby przecinkow po zakończeniu wyrazenia okreslamy liczbe argumentow, arg count = comma count + 1 dlatego zaczynamy od 1
            while (j < expr.size() && bracket_counter != 0) {
                char ch = expr[j];
                if (ch == '(') bracket_counter++;
                else if (ch == ')') bracket_counter--;
                else if (ch == ',' && bracket_counter == 1) //1 nawias bo w nawiasie moze byc kolejna funkcja min lub max i nie chcemy liczyc nawiasow z niej
                    argument_count++;
                j++;
            }
            opStack.push(c);
            opStack.push(argument_count);
            
        }
        else if (c == ',') { //popujemy az dojdziemy do nawiasu ktory zaczyna argumenty funkcji
            while (!opStack.empty() && opStack.top() != '(') {
                output.push_back(std::string(1, opStack.top()));
                opStack.pop();
            }
        }

    }
    while (!opStack.empty()) {
        output.push_back(std::string(1, opStack.top()));
        opStack.pop();
    }
    return output;
}
