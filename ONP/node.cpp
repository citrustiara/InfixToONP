#include "node.hpp"
#include <iostream>

Node* Push(Node* head, char val) {
    Node* p = new Node{ val, head };
    return p;
}

Node* PushEnd(Node* head, char val) {
    Node* p = new Node{ val, nullptr };
    if (!head) return p;
    Node* tmp = head;
    while (tmp->next) tmp = tmp->next;
    tmp->next = p;
    return head;
}

Node* Pop(Node* head, char& val) {
    if (!head) return nullptr;
    Node* tmp = head;
    val = tmp->value;
    head = head->next;
    delete tmp;
    return head;
}

void PrintStack(Node* head) {
    while (head) {
        std::cout << head->value << " ";
        head = head->next;
    }
    std::cout << "\n";
}

void PrintStack2(Node* head) { PrintStack(head); }
void PrintStack3(Node* head) { PrintStack(head); }

IntNode* Push_int(IntNode* head, int val) {
    IntNode* p = new IntNode{ val, head };
    return p;
}

IntNode* Pop_int(IntNode* head, int& val) {
    if (!head) return nullptr;
    IntNode* tmp = head;
    val = tmp->value;
    head = head->next;
    delete tmp;
    return head;
}

void PrintIntStack(IntNode* head) {
    while (head) {
        std::cout << head->value << " ";
        head = head->next;
    }
    std::cout << "\n";
}
