#include<iostream>
#include<string>
#include<vector>
#include "FileSystem.h"
#include "HierarchySetup.h"
#include "Stack.h"
using namespace std;

int main(){
    Folder* Campus = buildDefaultFileSystem();
    Folder* currentFolder = Campus;

    Stack backStack(10);
    Stack forwardStack(10);

    string command;
    cout<<"Enter command: ";
    while(cin >> command){
        if(command == "EXIT" || command == "exit"){
            break;
        }
        else if(command == "WHERE"){
            cout << getFullPath(currentFolder) << endl;
        }
        else if(command == "OPEN"){
            string path;
            cin >> path;
            Folder* dest = resolvePath(path, currentFolder, Campus);
            if(dest == nullptr){
                cout << "Invalid path: " << path << endl;
            }
            else{
                backStack.push(getFullPath(currentFolder));
                while(!forwardStack.isEmpty()){
                    forwardStack.pop();
                }
                currentFolder = dest;
                cout << getFullPath(currentFolder) << endl;
            }
        }
        else if(command == "BACK"){
            if(backStack.isEmpty()){
                cout << "Cannot go back: history is empty." << endl;
            }
            else{
                forwardStack.push(getFullPath(currentFolder));
                string prevPath = backStack.top();
                backStack.pop();
                currentFolder = resolvePath(prevPath, Campus, Campus);
                cout << getFullPath(currentFolder) << endl;
            }
        }
        else if(command == "FORWARD"){
            if(forwardStack.isEmpty()){
                cout << "Cannot go forward: history is empty." << endl;
            }
            else{
                backStack.push(getFullPath(currentFolder));
                string nextPath = forwardStack.top();
                forwardStack.pop();
                currentFolder = resolvePath(nextPath, Campus, Campus);
                cout << getFullPath(currentFolder) << endl;
            }
        }
    }
    return 0;
}