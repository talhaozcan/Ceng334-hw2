/* 
 * File:   Semaphore.h
 * Author: ahmet
 *
 * Created on April 30, 2014, 11:36 AM
 */


#ifndef SEMAPHORE_H
#define	SEMAPHORE_H


#include <iostream>
#include <string>
#include <semaphore.h>

using namespace std;

class Semaphore {
public:
        Semaphore();
        Semaphore(string name,int value);
        virtual ~Semaphore();
        void Sem_init();
        void Sem_dest();
        void Sem_wait();
        void Sem_post();

        sem_t mutex;
        string name="sem";
        int value = 1;
};

#endif	/* SEMAPHORE_H */

