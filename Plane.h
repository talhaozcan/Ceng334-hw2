/* 
 * File:   Plane.h
 * Author: ahmet
 *
 * Created on May 1, 2014, 12:53 PM
 */

#ifndef PLANE_H
#define	PLANE_H

#include <pthread.h>
#include <string>

using namespace std;

class Plane {
public:
    Plane();
    virtual ~Plane();
    void parseReq(string req);
    
    pthread_t planeT;
    string flightNO ;
    string request  ;
    int runway      ;
    int parking     ;
};

#endif	/* PLANE_H */

