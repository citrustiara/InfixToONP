#ifndef STACK_HPP
#define STACK_HPP

#include "Node.hpp"
#include <stdexcept>

class CharStack {
    Node* top;
public:
    CharStack() : top(nullptr) {}
    ~CharStack();
    void push(char c);
    char pop();
    bool empty() const;
};

class IntStack {
    IntNode* top;
public:
    IntStack() : top(nullptr) {}
    ~IntStack();
    void push(int x);
    int pop();
    bool empty() const;
};

#endif
