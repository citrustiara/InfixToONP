#include "node.hpp"
#include <iostream>

// ---- char stack ----
Node* Push(Node* head, char val)
{
    Node* p = new Node{ val, head };
    return p;
}

Node* Pop(Node* head, char& val)
{
    if (!head) return nullptr;
    val = head->value;
    Node* tmp = head->next;
    delete head;
    return tmp;
}

void PrintStack(Node* head)
{
    for (Node* p = head; p; p = p->next)
        std::cout << p->value << " ";
    std::cout << "\n";
}

// ---- int stack ----
IntNode* Push_int(IntNode* head, int val)
{
    IntNode* p = new IntNode{ val, head };
    return p;
}

IntNode* Pop_int(IntNode* head, int& val)
{
    if (!head) return nullptr;
    val = head->value;
    IntNode* tmp = head->next;
    delete head;
    return tmp;
}

void PrintIntStack(IntNode* head)
{
    for (IntNode* p = head; p; p = p->next)
        std::cout << p->value << " ";
    std::cout << "\n";
}
