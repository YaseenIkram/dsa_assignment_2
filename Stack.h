#pragma once
#include<string>
using namespace std;


class Stack;

class Stack{
    public:
    string* paths;
    int capacity;
    int count;

    Stack(int);
    void push(string);
    void pop();
    void doubleSize();
    bool isEmpty();
    string top();
};