#include<iostream>
#include "FileSystem.h"
using namespace std;

int COUNT = 0;

void Item::setParent(Folder* p){ parent = p; }

bool Folder::isFolder() { return true; }
bool Folder::isFile() { return false; }

bool File::isFile() { return true; }
bool File::isFolder() { return false; }

void Folder::addChild(Item* c){ children.push_back(c); }

Item::Item(string n,  Folder* p){
        id = COUNT;
        COUNT++;
        name = n;
        parent = p;
}

void Item::print(){
    cout<<id <<": " <<name;
}

Folder::Folder(string name, Folder* parent) : Item(name, parent) {}

File::File(string name, Folder* parent, int p):  Item(name, parent), pages(p) {}

void File::setPages(int p){ pages = p; }

void Folder::print(){
    Item::print();
    cout<<"/" <<endl;
}

void File::print(){
    Item::print();
    cout<<"  [" <<pages <<" pages]" <<endl;
}

void Folder::printChildren(){
    for(Item* child: children){
        child->print();
    }
}

Folder* search(string name, Folder* root){
    if(root == nullptr){
        return nullptr;
    }

    if(root->name == name){
        return root;
    }

    for(Item* child: root->children){
        if(child->isFolder()){
            Folder* folder = static_cast<Folder*>(child);

            Folder* result = search(name, folder);

            if(result != nullptr){
                return result;
            }
        }
    }
    return nullptr;
}

File* findFileById(int id, Item* root) {
    if (root == nullptr) return nullptr;

    if (root->id == id) {
        if (root->isFile()) {
            return static_cast<File*>(root);
        }
        return nullptr; // id is found but it is a of a folder
    }

    if (root->isFolder()) {
        Folder* folder = static_cast<Folder*>(root);
        for (Item* child : folder->children) {
            File* res = findFileById(id, child);
            if (res != nullptr) {
                return res;
            }
        }
    }
    return nullptr;
}