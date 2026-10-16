#pragma once
#include <iostream>

struct PrintJob {
    int jobID;
    int fileID;
    int totalPages;
    int remainingPages;
};

struct QueueNode {
    PrintJob job;
    QueueNode* next;

    QueueNode(PrintJob j) {
        job = j;
        next = nullptr;
    }
};

class Queue {
public:
    QueueNode* frontNode;
    QueueNode* rearNode;
    int count;

    Queue();
    ~Queue();

    void enqueue(PrintJob job);
    void dequeue();
    PrintJob front();
    bool isEmpty();
    void showQueue();
};