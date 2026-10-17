#include <iostream>
#include <string>
#include <vector>
#include "FileSystem.h"
#include "HierarchySetup.h"
#include "Stack.h"
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
    Folder* currentFolder = Campus;

    Stack backStack(20);
    Stack forwardStack(20);
    Queue printQueue;

    int printerClock = 0;
    int nextJobID = 101;

    string command;
    while (true) {
        cout << "\nEnter command: ";
        if (!(cin >> command)) {
            break;
        }

        if (command == "EXIT" || command == "exit") {
            break;
        }
        else if (command == "WHERE") {
            cout << getFullPath(currentFolder) << endl;
        }
        else if (command == "OPEN") {
            string path;
            cout << "Enter path: ";
            cin >> path;

            Folder* dest = resolvePath(path, currentFolder, Campus);
            if (dest == nullptr) {
                cout << "Error; stay at " << getFullPath(currentFolder) << endl;
            } else if (dest == currentFolder) {
                cout << getFullPath(currentFolder) << endl;
            } else {
                backStack.push(getFullPath(currentFolder));
                while (!forwardStack.isEmpty()) {
                    forwardStack.pop();
                }
                currentFolder = dest;
                cout << getFullPath(currentFolder) << endl;
            }
        }
        else if (command == "BACK") {
            int k;
            cout << "Enter steps (k): ";
            cin >> k;

            if (k <= 0) {
                cout << "Error; stay at " << getFullPath(currentFolder) << endl;
                continue;
            }

            int moved = 0;
            while (moved < k && !backStack.isEmpty()) {
                forwardStack.push(getFullPath(currentFolder));
                string prevPath = backStack.top();
                backStack.pop();
                currentFolder = resolvePath(prevPath, Campus, Campus);
                moved++;
            }
            cout << getFullPath(currentFolder) << endl;
        }
        else if (command == "FORWARD") {
            int k;
            cout << "Enter steps (k): ";
            cin >> k;

            if (k <= 0) {
                cout << "Error; stay at " << getFullPath(currentFolder) << endl;
                continue;
            }

            int moved = 0;
            while (moved < k && !forwardStack.isEmpty()) {
                backStack.push(getFullPath(currentFolder));
                string nextPath = forwardStack.top();
                forwardStack.pop();
                currentFolder = resolvePath(nextPath, Campus, Campus);
                moved++;
            }
            cout << getFullPath(currentFolder) << endl;
        }
        else if (command == "SHOW_LEVELS") {
            int folderID;
            cout << "Enter folder ID: ";
            cin >> folderID;

            Item* it = findItemById(folderID, Campus);
            if (it == nullptr) {
                cout << "Error: " << folderID << " does not exist." << endl;
            } else if (!it->isFolder()) {
                cout << "Error: " << folderID << " is a file, not a folder." << endl;
            } else {
                showLevels(static_cast<Folder*>(it));
            }
        }
        else if (command == "SUMMARY") {
            int folderID;
            cout << "Enter folder ID: ";
            cin >> folderID;

            Item* it = findItemById(folderID, Campus);
            if (it == nullptr) {
                cout << "Error: " << folderID << " does not exist." << endl;
            } else if (!it->isFolder()) {
                cout << "Error: " << folderID << " is a file, not a folder." << endl;
            } else {
                printSummary(static_cast<Folder*>(it));
            }
        }
        else if (command == "COMMON_FOLDER") {
            int id1, id2;
            cout << "Enter file ID 1: ";
            cin >> id1;
            cout << "Enter file ID 2: ";
            cin >> id2;

            Item* it1 = findItemById(id1, Campus);
            Item* it2 = findItemById(id2, Campus);

            if (it1 == nullptr) {
                cout << "Error: " << id1 << " does not exist." << endl;
            } else if (it2 == nullptr) {
                cout << "Error: " << id2 << " does not exist." << endl;
            } else if (!it1->isFile()) {
                cout << "Error: " << id1 << " is a folder, not a file." << endl;
            } else if (!it2->isFile()) {
                cout << "Error: " << id2 << " is a folder, not a file." << endl;
            } else {
                Folder* common = findCommonFolder(static_cast<File*>(it1), static_cast<File*>(it2));
                if (common != nullptr) {
                    cout << getFullPath(common) << endl;
                }
            }
        }
        else if (command == "PRINT_FOLDER") {
            int copies;
            string order;
            cout << "Enter copies: ";
            cin >> copies;
            cout << "Enter order (DEPTH or LEVEL): ";
            cin >> order;

            if (copies <= 0) {
                cout << "Invalid copies count: must be positive." << endl;
                continue;
            }
            if (order != "DEPTH" && order != "LEVEL") {
                cout << "Invalid order: must be DEPTH or LEVEL." << endl;
                continue;
            }

            vector<File*> files;
            if (order == "DEPTH") {
                files = getFilesDepth(currentFolder);
            } else {
                files = getFilesLevel(currentFolder);
            }

            for (File* f : files) {
                int totalPages = f->pages * copies;
                PrintJob newJob = { nextJobID, f->id, totalPages, totalPages };
                printQueue.enqueue(newJob);
                cout << "Job " << nextJobID << ": file " << f->id 
                     << ", total " << totalPages << ", remaining " << totalPages << endl;
                nextJobID++;
            }
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