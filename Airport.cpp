/* 
 * File:   Airport.cpp
 * Author: ahmet
 * 
 * Created on May 1, 2014, 2:28 AM
 */

#include "Airport.h"
#include <string>
#include<vector>
#include <iostream> 
#include <fstream>


using namespace std;

vector<string> lines;

Airport::Airport() {
}
Airport::~Airport() {
}

void  Airport::readInput(string fileName){
			
        ifstream myfile;
        myfile.open(fileName);
	string line		;
	getline(myfile,line)	;
        lines.push_back(line);

	while(line != "-1"){

		getline(myfile,line)	;
		lines.push_back(line);
	}
        myfile.close();
        this->runways   = stoi(lines[0]);
        this->parks     = stoi(lines[1]);
        this->taxiNodes = stoi(lines[2]);
        this->capacity  = stoi(lines[3]);
        for(int i=4;i<lines.size()-1;i++)this->paths.push_back(lines[i]);
        myfile.close();
}
void Airport::writeA(){

    cout<<this->runways<<endl;
    cout<<this->parks<<endl;
    cout<<this->taxiNodes<<endl;
    cout<<this->capacity<<endl;
    for(int i=0;i<this->paths.size();i++)cout<<this->paths[i]<<endl;

}

void Airport::taxyingPaths(){

    vector< string > path;
    vector< string > path2=this->paths;
    
    
    
	
    for(int i =0;i<this->paths.size();i++){
    
    
        string delimiter = " "	;
	size_t pos = 0		;
	string s		;
    
        
        while(path2[i]!=""){
            
            pos = path2[i].find(delimiter);
            s = path2[i].substr(0, pos)  ;
            if(s=="-1")break;
            path.push_back(s)      ;
            path2[i].erase(0, pos + delimiter.length())	;
            
            }
        this->taxyinPaths.push_back(path);
        path.clear();
    }

    
/*    for (int j = 0; j < this->taxyinPaths.size(); j++) {
  
        for (int i = 0; i < this->taxyinPaths[j].size(); i++) {
            cout<<j<<"."<<i<<"-"<< taxyinPaths[j][i]<<" ";
        }
        cout<<endl;
    }
*/
}

