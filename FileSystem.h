#pragma once
#include<vector>
#include<string>
using namespace std;

class Item;
class File;
class Folder;


class Item{
    public:
    int id;
    string name;
    Folder* parent;

    Item(string n,  Folder* p);
    virtual bool isFolder() = 0;
    virtual bool isFile() = 0;
    void setParent(Folder* p);

    virtual void print();

};

class Folder: public Item {
    public:
    vector<Item*> children;

    Folder(string name, Folder* parent = nullptr);
    bool isFolder() override;
    bool isFile() override;
    void addChild(Item* c);
    void print() override;
    void printChildren();
};

class File: public Item{
    public:
    int pages;

    File(string name, Folder* parent, int p);
    bool isFolder() override;
    bool isFile() override;
    void setPages(int p);
    void print() override;

};