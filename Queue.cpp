#include "Queue.h"
#include <iostream>
using namespace std;

Queue::Queue() {
    frontNode = nullptr;
    rearNode = nullptr;
    count = 0;
}

Queue::~Queue() {
    while (!isEmpty()) {
        dequeue();
    }
}

bool Queue::isEmpty() {
    return frontNode == nullptr;
}

void Queue::enqueue(PrintJob job) {
    QueueNode* newNode = new QueueNode(job);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    count++;
}

void Queue::dequeue() {
    if (isEmpty()) {
        cout << "Queue is empty." << endl;
        return;
    }

    QueueNode* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }
    delete temp;
    count--;
}

PrintJob Queue::front() {
    if (isEmpty()) {
        return {-1, -1, 0, 0};
    }
    return frontNode->job;
}

void Queue::showQueue() {
    if (isEmpty()) {
        cout << "Queue is empty." << endl;
        return;
    }

    cout << "[";
    QueueNode* curr = frontNode;
    while (curr != nullptr) {
        cout << "(" << curr->job.jobID << ", " << curr->job.remainingPages << ")";
        if (curr->next != nullptr) {
            cout << ", ";
        }
        curr = curr->next;
    }
    cout << "]" << endl;
}