/* 
 * File:   Plane.cpp
 * Author: ahmet
 * 
 * Created on May 1, 2014, 12:53 PM
 */

#include <iostream>
#include "Plane.h"
#include "Semaphore.h"
#include <pthread.h>

Plane::Plane() {

}

Plane::~Plane() {
    

}


void Plane::parseReq(string req){

  
	string delimiter = " "	;
	size_t pos = 0		;
	string s		;

      
        pos = req.find(delimiter);
        s = req.substr(0, pos)  ;
	this->flightNO = s      ;
        req.erase(0, pos + delimiter.length())	;
        
        pos = req.find(delimiter);
        s = req.substr(0, pos)  ;
        this->request = s       ;
        req.erase(0, pos + delimiter.length())	;	

        pos = req.find(delimiter);
        s = req.substr(0, pos)  ;
        this->runway = stoi(s) ;
        req.erase(0, pos + delimiter.length())	;	

        pos = req.find(delimiter);
        s = req.substr(0, pos)  ;
        this->parking = stoi(s) ;
        req.erase(0, pos + delimiter.length())	;	
        

}


