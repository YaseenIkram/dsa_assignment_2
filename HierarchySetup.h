#pragma once
#include <string>
#include <vector>
#include "FileSystem.h"
using namespace std;

struct ItemQueueNode {
    Item* item;
    int level;
    ItemQueueNode* next;
    ItemQueueNode(Item* it, int lvl = 0) {
        item = it;
        level = lvl;
        next = nullptr;
    }
};

class ItemQueue {
public:
    ItemQueueNode* frontNode;
    ItemQueueNode* rearNode;

    ItemQueue();
    ~ItemQueue();
    bool isEmpty();
    void enqueue(Item* it, int lvl = 0);
    ItemQueueNode* dequeue();
};

struct ItemStackNode {
    Item* item;
    ItemStackNode* next;
    ItemStackNode(Item* it) {
        item = it;
        next = nullptr;
    }
};

class ItemStack {
public:
    ItemStackNode* topNode;

    ItemStack();
    ~ItemStack();
    bool isEmpty();
    void push(Item* it);
    Item* pop();
};

Folder* buildDefaultFileSystem();
void printHierarchy(Item* item, int depth = 0);
void simulate();
Folder* search(string name, Folder* root);
vector<string> parsePath(string path);
string getFullPath(Folder* folder);
Folder* resolvePath(string path, Folder* currentFolder, Folder* root);
File* findFileById(int id, Item* root);

Item* findItemById(int id, Item* root);
Folder* findFolderById(int id, Item* root);
void showLevels(Folder* startFolder);
void printSummary(Folder* folder);
Folder* findCommonFolder(File* f1, File* f2);
vector<File*> getFilesDepth(Folder* start);
vector<File*> getFilesLevel(Folder* start);