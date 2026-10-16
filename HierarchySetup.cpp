#include "HierarchySetup.h"
#include <iostream>
using namespace std;

Folder* buildDefaultFileSystem(){
    Folder* Campus = new Folder("Campus", nullptr);
    Folder* CS = new Folder("CS", Campus);
    Folder* Admin = new Folder("Admin", Campus);
    Folder* DSA = new Folder("DSA", CS);
    Folder* AI = new Folder("AI", CS);
    File* Notices = new File("Notices.pdf", Admin, 2);
    Folder* Empty = new Folder("Empty", Campus);
    File* Stacks = new File("Stacks.pdf", DSA, 5);
    File* Queues = new File("Queues.pdf", DSA, 3);
    Folder* Trees = new Folder("Trees", DSA);
    File* BST = new File("BST.pdf", Trees, 6);
    File* Search = new File("Search.pdf", CS, 4);

    Campus->addChild(CS);
    Campus->addChild(Admin);
    Campus->addChild(Empty);
    
    CS->addChild(DSA);
    CS->addChild(AI);
    CS->addChild(Search);

    Admin->addChild(Notices);

    DSA->addChild(Stacks);
    DSA->addChild(Trees);
    DSA->addChild(Queues);

    Trees->addChild(BST);

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