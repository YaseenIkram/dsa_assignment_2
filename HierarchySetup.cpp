#include "HierarchySetup.h"
#include <iostream>
using namespace std;

ItemQueue::ItemQueue() {
    frontNode = nullptr;
    rearNode = nullptr;
}

ItemQueue::~ItemQueue() {
    while (!isEmpty()) {
        ItemQueueNode* temp = dequeue();
        delete temp;
    }
}

bool ItemQueue::isEmpty() {
    return frontNode == nullptr;
}

void ItemQueue::enqueue(Item* it, int lvl) {
    ItemQueueNode* n = new ItemQueueNode(it, lvl);
    if (isEmpty()) {
        frontNode = rearNode = n;
    } else {
        rearNode->next = n;
        rearNode = n;
    }
}

ItemQueueNode* ItemQueue::dequeue() {
    if (isEmpty()) return nullptr;
    ItemQueueNode* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }
    return temp;
}

ItemStack::ItemStack() {
    topNode = nullptr;
}

ItemStack::~ItemStack() {
    while (!isEmpty()) {
        pop();
    }
}

bool ItemStack::isEmpty() {
    return topNode == nullptr;
}

void ItemStack::push(Item* it) {
    ItemStackNode* n = new ItemStackNode(it);
    n->next = topNode;
    topNode = n;
}

Item* ItemStack::pop() {
    if (isEmpty()) return nullptr;
    ItemStackNode* temp = topNode;
    Item* it = temp->item;
    topNode = topNode->next;
    delete temp;
    return it;
}

Folder* buildDefaultFileSystem() {
    Folder* Campus = new Folder("Campus", nullptr);       // 0
    Folder* CS = new Folder("CS", Campus);                // 1
    Folder* Admin = new Folder("Admin", Campus);          // 2
    Folder* DSA = new Folder("DSA", CS);                  // 3
    Folder* AI = new Folder("AI", CS);                    // 4
    File* Notices = new File("Notices.pdf", Admin, 2);    // 5
    Folder* Empty = new Folder("Empty", Admin);           // 6
    File* Stacks = new File("Stacks.pdf", DSA, 5);        // 7
    File* Queues = new File("Queues.pdf", nullptr, 3);    // 8
    Folder* Trees = new Folder("Trees", DSA);             // 9
    Queues->setParent(Trees);
    File* BST = new File("BST.pdf", Trees, 6);            // 10
    File* Search = new File("Search.pdf", AI, 4);         // 11

    Campus->addChild(CS);
    Campus->addChild(Admin);

    CS->addChild(DSA);
    CS->addChild(AI);

    Admin->addChild(Notices);
    Admin->addChild(Empty);

    DSA->addChild(Stacks);
    DSA->addChild(Trees);

    Trees->addChild(BST);
    Trees->addChild(Queues);

    AI->addChild(Search);

    return Campus;
}

void printHierarchy(Item* item, int depth){
    if(item == nullptr)
    return;

    cout<<"|";
    for(int i=0; i<depth*3; i++){
        cout<<"-";
    }
    item->print();

    if(item->isFolder()){
        Folder* folder = static_cast<Folder*>(item);
        for(Item* child: folder->children){
            printHierarchy(child, depth + 1);  
        }
    }
}

void simulate(){
    Folder* Campus = buildDefaultFileSystem();
    printHierarchy(Campus);
}

vector<string> parsePath(string path){
    int i=0;
    vector<string> dirs;
    string curr = "";
    while(path[i] != '\0'){
        if(path[i] == '/'){
            if(curr != ""){
                dirs.push_back(curr);
                curr = "";
            }
            i++;
            continue;
        }
        curr += path[i];
        i++;
    }

    if(curr != ""){
        dirs.push_back(curr);
    }
    return dirs;
}

string getFullPath(Folder* folder){
    if(folder == nullptr || folder->parent == nullptr){
        return "/";
    }

    string fullPath = "";
    Folder* curr = folder;
    while(curr != nullptr && curr->parent != nullptr){
        fullPath = "/" + curr->name + fullPath;
        curr = curr->parent;
    }
    return fullPath;
}

Folder* resolvePath(string path, Folder* currentFolder, Folder* root){
    if(path == ""){
        return nullptr;
    }

    Folder* temp = (path[0] == '/') ? root : currentFolder;

    vector<string> dirs = parsePath(path);

    for(string dir: dirs){
        if(dir == "."){
            continue;
        }
        if(dir == ".."){
            if(temp->parent != nullptr){
                temp = temp->parent;
            }
            continue;
        }

        Folder* nextFolder = nullptr;
        for(Item* child: temp->children){
            if(child->name == dir){
                if(child->isFolder()){
                    nextFolder = static_cast<Folder*>(child);
                }
                break;
            }
        }

        if(nextFolder == nullptr){
            return nullptr;
        }

        temp = nextFolder;
    }

    return temp;
}

Item* findItemById(int id, Item* root) {
    if (root == nullptr) return nullptr;
    if (root->id == id) return root;

    if (root->isFolder()) {
        Folder* f = static_cast<Folder*>(root);
        for (Item* child : f->children) {
            Item* res = findItemById(id, child);
            if (res != nullptr) return res;
        }
    }
    return nullptr;
}

Folder* findFolderById(int id, Item* root) {
    Item* it = findItemById(id, root);
    if (it != nullptr && it->isFolder()) {
        return static_cast<Folder*>(it);
    }
    return nullptr;
}

void showLevels(Folder* startFolder) {
    if (startFolder == nullptr) return;

    ItemQueue q;
    q.enqueue(startFolder, 0);
    int currentLevel = -1;

    while (!q.isEmpty()) {
        ItemQueueNode* node = q.dequeue();
        Item* item = node->item;
        int level = node->level;

        if (level != currentLevel) {
            if (currentLevel != -1) cout << endl;
            currentLevel = level;
            cout << "Level " << currentLevel << ": ";
        } else {
            cout << ", ";
        }

        cout << item->name;

        if (item->isFolder()) {
            Folder* f = static_cast<Folder*>(item);
            for (Item* child : f->children) {
                q.enqueue(child, level + 1);
            }
        }

        delete node;
    }
    cout << endl;
}

int countFiles(Item* root) {
    if (root == nullptr) return 0;
    if (root->isFile()) return 1;

    int total = 0;
    Folder* f = static_cast<Folder*>(root);
    for (Item* child : f->children) {
        total += countFiles(child);
    }
    return total;
}

int countPages(Item* root) {
    if (root == nullptr) return 0;
    if (root->isFile()) {
        return static_cast<File*>(root)->pages;
    }

    int total = 0;
    Folder* f = static_cast<Folder*>(root);
    for (Item* child : f->children) {
        total += countPages(child);
    }
    return total;
}

int getSubtreeHeight(Item* root) {
    if (root == nullptr || root->isFile()) return 0;

    Folder* f = static_cast<Folder*>(root);
    if (f->children.empty()) return 0;

    int maxHeight = 0;
    for (Item* child : f->children) {
        int h = getSubtreeHeight(child);
        if (h > maxHeight) {
            maxHeight = h;
        }
    }
    return 1 + maxHeight;
}

void printSummary(Folder* folder) {
    if (folder == nullptr) return;
    int files = countFiles(folder);
    int pages = countPages(folder);
    int height = getSubtreeHeight(folder);
    cout << files << " files, " << pages << " pages, height " << height << endl;
}

Folder* findCommonFolder(File* f1, File* f2) {
    if (f1 == nullptr || f2 == nullptr) return nullptr;

    if (f1->id == f2->id) {
        return f1->parent;
    }

    vector<Folder*> ancestors;
    Folder* curr = f1->parent;
    while (curr != nullptr) {
        ancestors.push_back(curr);
        curr = curr->parent;
    }

    curr = f2->parent;
    while (curr != nullptr) {
        for (Folder* a : ancestors) {
            if (curr == a) {
                return curr;
            }
        }
        curr = curr->parent;
    }

    return nullptr;
}

vector<File*> getFilesDepth(Folder* start) {
    vector<File*> files;
    if (start == nullptr) return files;

    ItemStack s;
    s.push(start);

    while (!s.isEmpty()) {
        Item* it = s.pop();
        if (it->isFile()) {
            files.push_back(static_cast<File*>(it));
        } else {
            Folder* f = static_cast<Folder*>(it);
            for (int i = (int)f->children.size() - 1; i >= 0; i--) {
                s.push(f->children[i]);
            }
        }
    }
    return files;
}

vector<File*> getFilesLevel(Folder* start) {
    vector<File*> files;
    if (start == nullptr) return files;

    ItemQueue q;
    q.enqueue(start);

    while (!q.isEmpty()) {
        ItemQueueNode* n = q.dequeue();
        Item* it = n->item;
        delete n;

        if (it->isFile()) {
            files.push_back(static_cast<File*>(it));
        } else {
            Folder* f = static_cast<Folder*>(it);
            for (Item* child : f->children) {
                q.enqueue(child);
            }
        }
    }
    return files;
}