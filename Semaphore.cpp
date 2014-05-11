/* 
 * File:   Semaphore.cpp
 * Author: ahmet
 * 
 * Created on April 30, 2014, 11:36 AM
 */

#include "Semaphore.h"
#include <semaphore.h>
Semaphore::Semaphore() {
}

Semaphore::Semaphore(string name, int value) {
    
    this->name = name ;
    this->value= value;
}

Semaphore::~Semaphore() {
}
void Semaphore::Sem_init(){

    sem_init(&this->mutex,0,this->value);
    
}
void Semaphore::Sem_dest(){
    sem_destroy(&this->mutex);
}
void Semaphore::Sem_wait(){
    sem_wait(&this->mutex);
}
void Semaphore::Sem_post(){
    sem_post(&this->mutex);
}
