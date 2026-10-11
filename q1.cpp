#include<iostream>
#include "HierarchySetup.h"
#include<vector>
#include "Stack.h"
using namespace std;

vector <string> parsePath(string path);
bool validDirs(vector<string>dirs, vector<string>defaultDirs);

int main(){
    Folder* Campus = buildDefaultFileSystem();
    vector<string> defaultDirs = {"Campus", "CS", "Admin", "DSA", "AI", "Empty", "Trees"};
    string path;


    cout<<"Enter path: ";
    cin >> path;

    vector<string>parsed = parsePath(path);
    
    cout<<"Parsed: " <<endl;
    for(string dir: parsed){
        cout<<dir <<", ";
    }
    cout<<endl;

    if(validDirs(parsed, defaultDirs)){
        cout<<"valid dirs" <<endl;
    }
    else{
        cout<<"Invalid dirs" <<endl;
    }
}


vector <string> parsePath(string path){
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


bool validDirs(vector<string>dirs, vector<string>defaultDirs){
    if(dirs.empty()){
        return false;
    }

    for(string dir: dirs){
        bool found = false;
        for(string dDir: defaultDirs){
            if(dir == dDir){
                found = true;
                break;
            }
           
        }
        if(!found){
           return false;
            }
        
    }
    return true;
}
