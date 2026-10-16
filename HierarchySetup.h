#pragma once
#include <string>
#include <vector>
#include "FileSystem.h"
using namespace std;

Folder* buildDefaultFileSystem();
void printHierarchy(Item* item, int depth = 0);
void simulate();
Folder* search(string name, Folder* root);
vector<string> parsePath(string path);
string getFullPath(Folder* folder);
Folder* resolvePath(string path, Folder* currentFolder, Folder* root);
File* findFileById(int id, Item* root);