/* 
 * File:   Airport.h
 * Author: ahmet
 *
 * Created on May 1, 2014, 2:28 AM
 */

#ifndef AIRPORT_H
#define	AIRPORT_H

#include<vector>
#include<string>
using namespace std;

class Airport {
public:
    Airport();
    void readInput(string fileName);
    void writeA();
    virtual ~Airport();
    void taxyingPaths();
    
    
    int runways  ;
    int parks    ;
    int taxiNodes;
    int capacity ;
    vector<string> paths;//to get line R1P1 1 2 3 4 5
    vector< vector< string> > taxyinPaths;//to parse paths 
};

#endif	/* AIRPORT_H */

