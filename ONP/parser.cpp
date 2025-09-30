#include "parser.hpp"
#include "node.hpp"
#include <cctype>
#include <stack>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    //if (op == 'N' || op == 'X') return 3;
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

        // Modified to handle floating-point numbers
        if (isdigit(c) || c == '.') {
            std::string number;
            bool hasDot = false;

            while (i < expr.size() && (isdigit(expr[i]) || (expr[i] == '.' && !hasDot))) {
                if (expr[i] == '.') hasDot = true;
                number += expr[i++];
            }
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
            if (!opStack.empty()) opStack.pop();

            int temp;
            if (!opStack.empty()) { //we want to check if there is a func in front of the '(', we have to pop the arg count
                temp = opStack.top();
                opStack.pop();
            }
            //if there is a func in front of the '(' then add the func operand and argcount
            if (!opStack.empty() && (opStack.top() == 'X' || opStack.top() == 'N')) {
                opStack.push(temp); //we bring back the argcount now that we have checked the func
                output.push_back(std::string(1, opStack.top()));
                opStack.pop();
                output.push_back(std::string(1, opStack.top()));
                opStack.pop();
            }
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(c)) {
                output.push_back(std::string(1, opStack.top()));
                opStack.pop();
            }
            opStack.push(c);
        }
        else if (c == 'X' || c == 'N') {
            size_t j = i + 2; //skip func operand and '('
            int bracket_counter = 1; //we keep track of '(' level, '('++, ')'--, we can only count ',' when the level is 1, we dont want to count ',' inside another functions in the function
            int argument_count = 1; //we count arguments based on ',', arg count = ',' count + 1
            while (j < expr.size() && bracket_counter != 0) {
                char ch = expr[j];
                if (ch == '(') bracket_counter++;
                else if (ch == ')') bracket_counter--;
                else if (ch == ',' && bracket_counter == 1)
                    argument_count++;
                j++;
            }
            opStack.push(c);
            opStack.push(argument_count);
        }
        else if (c == ',') {
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