#include "FileSystem.h"
using namespace std;

Folder* buildDefaultFileSystem(){
    Folder* Campus = new Folder("Campus", nullptr); //0
    Folder* CS = new Folder("CS", Campus);  //1
    Folder* Admin = new Folder("Admin", Campus);  //2
    Folder* DSA = new Folder("DSA", CS);  //3
    Folder* AI = new Folder("AI", CS);  //4
    File* Notices = new File("Notices.pdf", Admin, 2); //5
    Folder* Empty = new Folder("Empty", Campus); //6
    File* Stacks = new File("Stacks.pdf", DSA, 5); //7
    File* Queues = new File("Queues.pdf", DSA, 3); //8
    Folder* Trees = new Folder("Trees", DSA);  //9
    File* BST = new File("BST.pdf", Trees, 6); //10
    File* Search = new File("Search.pdf", CS, 4);  //11


    return Campus;
}


void printHierarchy(Item* item){
    if(item == nullptr)
    return;

    item->print();

    if(item->isFolder()){
        Folder* folder = static_cast<Folder*>(item);
        for(Item* child: folder->children){
            printHierarchy(child);  
        }
    }
}


void simulate(){
    Folder* Campus = buildDefaultFileSystem();
    printHierarchy(Campus);
}
