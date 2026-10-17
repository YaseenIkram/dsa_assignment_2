#include <iostream>
#include <string>
#include "FileSystem.h"
#include "HierarchySetup.h"
#include "Queue.h"

using namespace std;

bool cancelJob(Queue& q, int jobID) {
    Queue tempQueue;
    bool found = false;

    while (!q.isEmpty()) {
        PrintJob j = q.front();
        q.dequeue();

        if (j.jobID == jobID && !found) {
            found = true; 
        } else {
            tempQueue.enqueue(j);
        }
    }

    while (!tempQueue.isEmpty()) {
        q.enqueue(tempQueue.front());
        tempQueue.dequeue();
    }

    return found;
}

void runTurns(Queue& q, int turns, int& clock) {
    if (turns <= 0) {
        cout << "Invalid turns count: must be positive." << endl;
        return;
    }

    for (int t = 0; t < turns && !q.isEmpty(); t++) {
        PrintJob curr = q.front();
        q.dequeue();

        int pagesToPrint = (curr.remainingPages > 2) ? 2 : curr.remainingPages;
        curr.remainingPages -= pagesToPrint;
        clock += pagesToPrint;

        cout << "Job " << curr.jobID << ": "
             << pagesToPrint << " pages printed, "
             << curr.remainingPages << " pages left, "
             << "Clock: " << clock << endl;

        if (curr.remainingPages > 0) {
            q.enqueue(curr);
        } else {
            cout << "Job " << curr.jobID << " completed at " << clock << " seconds." << endl;
        }
    }
}
int main() {
    Folder* Campus = buildDefaultFileSystem();
    Queue printQueue;

    int printerClock = 0;
    int nextJobID = 101;

    string command;
    while (true) {
        cout << "\nEnter command (PRINT_FILE, RUN, CANCEL, SHOW_QUEUE, EXIT): ";
        if (!(cin >> command)) {
            break;
        }

        if (command == "EXIT" || command == "exit") {
            break;
        }
        else if (command == "PRINT_FILE" || command == "PRINT") {
            if (command == "PRINT") {
                string sub;
                cin >> sub;
            }

            int fileID, copies;
            cout << "Enter file ID: ";
            cin >> fileID;
            cout << "Enter number of copies: ";
            cin >> copies;

            if (copies <= 0) {
                cout << "Invalid copies count: must be positive." << endl;
                continue;
            }

            File* targetFile = findFileById(fileID, Campus);
            if (targetFile == nullptr) {
                cout << "Invalid file ID: " << fileID << " (not found or is a folder)." << endl;
                continue;
            }

            int totalPages = targetFile->pages * copies;
            PrintJob newJob = { nextJobID, fileID, totalPages, totalPages };
            printQueue.enqueue(newJob);

            cout << "Job " << nextJobID << ": file " << fileID 
                 << ", total " << totalPages << ", remaining " << totalPages << endl;

            nextJobID++;
        }
        else if (command == "RUN") {
            int turns;
            cout << "Enter number of turns: ";
            cin >> turns;
            runTurns(printQueue, turns, printerClock);
        }
        else if (command == "CANCEL") {
            int jobID;
            cout << "Enter job ID to cancel: ";
            cin >> jobID;
            if (cancelJob(printQueue, jobID)) {
                cout << "Cancelled job " << jobID << endl;
            } else {
                cout << "Job " << jobID << " not found in waiting queue." << endl;
            }
        }
        else if (command == "SHOW_QUEUE" || command == "SHOW") {
            if (command == "SHOW") {
                string sub;
                cin >> sub;
            }
            printQueue.showQueue();
        }
        else {
            cout << "Unknown command: " << command << endl;
        }
    }

    return 0;
}