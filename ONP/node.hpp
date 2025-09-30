#pragma once

struct Node {
    char value;
    Node* next;
};

struct IntNode {
    int value;
    IntNode* next;
};

// deklaracje funkcji
Node* Push(Node* head, char val);
Node* Pop(Node* head, char& val);
void  PrintStack(Node* head);

IntNode* Push_int(IntNode* head, int val);
IntNode* Pop_int(IntNode* head, int& val);
void  PrintIntStack(IntNode* head);
