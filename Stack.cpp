#include "Stack.h"
#include<iostream>
using namespace std;


Stack::Stack(int c){
    capacity = c > 0 ? c : 1; 
    paths = new string[capacity];
    count = 0;
}

void Stack::push(string s){
    if(count >= capacity){
        doubleSize();
    }
    paths[count++] = s;
}

void Stack::pop(){
    if(isEmpty()){
        cout<<"Nothing to pop" <<endl;
        return;
    }
    count--;
}

void Stack::doubleSize(){
    int newSize = capacity*2;
    string* newArr = new string[newSize];
    for(int i=0; i<count; i++){
        newArr[i] = paths[i];
    }

    delete[] paths;
    paths = newArr;
    capacity = newSize;

}

bool Stack::isEmpty(){ return count == 0; }


string Stack::top(){
    if(isEmpty()){
        return "";
    }
    return paths[count - 1];
}

