/* 
 * File:   main.cpp
 * Author: ahmet
 *
 * Created on April 30, 2014, 11:09 AM
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <string>

#include "Semaphore.h"
#include "Airport.h"
#include "Plane.h"

using namespace std;

string runway;
int cap;

Airport airP;
vector< vector< string >> test;

vector<string> lines2;
vector<Semaphore>runwSem;
vector<Semaphore>taxSem;
vector<Semaphore>parkSem;

sem_t mutex;
sem_t mutexMain;

int controller = 0;
int reqCounter = 0;
int reqEndThread = 0;

enum DIRECTION {
    LANDING, TAKEOFF
};
pthread_mutex_t mutexWrite; // This mutex is used for atomic write operation

void writeToFile(int mode, int flightCode, DIRECTION direction, int runwayId, int parkId, int taxiNodeId)// This function is used to write the results of simulation to the file
{
    //writeToFile(mode,fcode,L/T,runw,park,tax)
    // Set the appropriate parameters for the corresponding output

    pthread_mutex_lock(&mutexWrite);

    if (mode == 0)//This will be output when the capacity of the airport is available for the new plane
    {
        printf("Plane = %d started %s\n", flightCode, (direction) ? "take-off" : "landing");
    } else if (mode == 1)//This will be output when the corresponding runway is available and the landing starts
    {
        printf("Plane = %d started its landing on runway %d\n", flightCode, runwayId);
    } else if (mode == 2)//This will be output when the corresponding runway is available and the take-off starts
    {
        printf("Plane = %d started its take off from runway %d\n", flightCode, runwayId);
    } else if (mode == 3)//This will be output when the current landing operation on the corresponding runway finished
    {
        printf("Plane = %d finished its landing on runway %d\n", flightCode, runwayId);
    } else if (mode == 4)//This will be output when the current take-off operation on the corresponding runway finished
    {
        printf("Plane = %d finished its take off from runway %d\n", flightCode, runwayId);
    } else if (mode == 5)//This will be output when reaching to the corresponding taxi node finished
    {
        printf("Plane = %d entered to taxi node %d\n", flightCode, taxiNodeId);
    } else if (mode == 6)//This will be output when the taxying to the corresponding park finished
    {
        printf("Plane = %d finished its taxying to the park %d\n", flightCode, parkId);
    } else if (mode == 7)//This will be output when the taxying to the corresponding runway finished
    {
        printf("Plane = %d finished its taxying to the runway %d\n", flightCode, runwayId);
    }

    pthread_mutex_unlock(&mutexWrite);
}

void* landing(void* ptr) {

    Plane* plane = (Plane*) ptr;
    //runwSem[plane.runway - 1].Sem_wait();

    writeToFile(0, stoi(plane->flightNO), LANDING, plane->runway, plane->parking, 1);


    //cout << "Plane = " << plane.flightNO << " started landing" << endl;
    // cap++;
    // runwSem[plane.runway - 1].Sem_post();
    //---------------------------------------------------------------------
    runwSem[plane->runway - 1].Sem_wait();

    writeToFile(1, stoi(plane->flightNO), LANDING, plane->runway, plane->parking, 1);

    //cout << "Plane = " << plane.flightNO << " started its landing on runway 1" << endl;
    usleep(10000);
    writeToFile(3, stoi(plane->flightNO), LANDING, plane->runway, plane->parking, 1);
    //cout << "Plane = " << plane.flightNO << " finished its landing on runway 1" << endl;
    runwSem[plane->runway - 1].Sem_post();
    //---------------------------------------------------------------------
    //!!!!!!taxying here to the parking
    string p = "R" + to_string(plane->runway) + "P" + to_string(plane->parking);
    int x = -1;

    for (int i = 0; i < airP.taxyinPaths.size(); i++) {

        if (p == airP.taxyinPaths[i][0]) {

            x = i;
            break;
        }
    }


    taxSem[stoi(airP.taxyinPaths[x][1]) - 1].Sem_wait();
    writeToFile(5, stoi(plane->flightNO), LANDING, plane->runway, plane->parking, stoi(airP.taxyinPaths[x][1]));
    //cout << "Plane = " << plane.flightNO << " entered to taxi node " << airP.taxyinPaths[x][i] << endl;
    usleep(10000);

    for (int i = 2; i < airP.taxyinPaths[x].size() - 1; i++) {

        taxSem[stoi(airP.taxyinPaths[x][i]) - 1].Sem_wait();


        writeToFile(5, stoi(plane->flightNO), LANDING, plane->runway, plane->parking, stoi(airP.taxyinPaths[x][i]));
        taxSem[stoi(airP.taxyinPaths[x][i - 1]) - 1].Sem_post();

        //cout << "Plane = " << plane.flightNO << " entered to taxi node " << airP.taxyinPaths[x][i] << endl;
        usleep(10000);

        //   if(i==airP.taxyinPaths[x][i].size() - 1)cap--;
    }


    taxSem[stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 1]) - 1].Sem_wait();

    writeToFile(5, stoi(plane->flightNO), LANDING, plane->runway, plane->parking, stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 1]));
    taxSem[stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 2]) - 1].Sem_post();
    //cout << "Plane = " << plane.flightNO << " entered to taxi node " << airP.taxyinPaths[x][i] << endl;
    usleep(10000);

    taxSem[stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 1]) - 1].Sem_post();
    //---------------------------------------------------------------------


    writeToFile(6, stoi(plane->flightNO), LANDING, plane->runway, plane->parking, 1);
    //cout << "Plane = " << plane.flightNO << " finished its taxying to the park " << plane.parking << endl;

    //for the plane capacity mutex
    cap--;

    if (airP.capacity > cap)sem_post(&mutex);

    reqEndThread++;
    if (reqEndThread == reqCounter && controller) {

        sem_post(&mutexMain);

    }
    pthread_exit(0);
}

void* takingOff(void* ptr) {


    Plane* plane = (Plane*) ptr;
    //---------------------------------------------------------------------

    writeToFile(0, stoi(plane->flightNO), TAKEOFF, plane->runway, plane->parking, 1);

    //cout << "Plane = " << plane.flightNO << " started take-off" << endl;

    //---------------------------------------------------------------------
    //!!!!!!taxying here to the parking
    string p = "P" + to_string(plane->parking) + "R" + to_string(plane->runway);
    int x = -1;

    for (int i = 0; i < airP.taxyinPaths.size(); i++) {

        if (p == airP.taxyinPaths[i][0]) {

            x = i;
            break;
        }
    }

    taxSem[stoi(airP.taxyinPaths[x][1]) - 1].Sem_wait();
    writeToFile(5, stoi(plane->flightNO), TAKEOFF, plane->runway, plane->parking, stoi(airP.taxyinPaths[x][1]));
    //cout << "Plane = " << plane.flightNO << " entered to taxi node " << airP.taxyinPaths[x][i] << endl;
    usleep(10000);

    for (int i = 2; i < airP.taxyinPaths[x].size() - 1; i++) {

        taxSem[stoi(airP.taxyinPaths[x][i]) - 1].Sem_wait();


        writeToFile(5, stoi(plane->flightNO), TAKEOFF, plane->runway, plane->parking, stoi(airP.taxyinPaths[x][i]));
        taxSem[stoi(airP.taxyinPaths[x][i - 1]) - 1].Sem_post();
        //cout << "Plane = " << plane.flightNO << " entered to taxi node " << airP.taxyinPaths[x][i] << endl;
        usleep(10000);

        //   if(i==airP.taxyinPaths[x][i].size() - 1)cap--;
    }

    taxSem[stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 1]) - 1].Sem_wait();

    writeToFile(5, stoi(plane->flightNO), TAKEOFF, plane->runway, plane->parking, stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 1]));
    taxSem[stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 2]) - 1].Sem_post();
    //cout << "ahmetPlane = " << plane->flightNO << " entered to taxi node " << airP.taxyinPaths[x][airP.taxyinPaths[x].size()] << endl;
    usleep(10000);

    taxSem[stoi(airP.taxyinPaths[x][airP.taxyinPaths[x].size() - 1]) - 1].Sem_post();
    //---------------------------------------------------------------------

    writeToFile(7, stoi(plane->flightNO), TAKEOFF, plane->runway, plane->parking, 1);

    //---------------------------------------------------------------------
    runwSem[plane->runway - 1].Sem_wait();
    writeToFile(2, stoi(plane->flightNO), TAKEOFF, plane->runway, plane->parking, 1);
    //cout << "Plane = " << plane.flightNO << " started its take off from runway 1" << endl;
    usleep(50000);
    writeToFile(4, stoi(plane->flightNO), TAKEOFF, plane->runway, plane->parking, 1);
    //cout << "Plane = " << plane.flightNO << " finished its take off from runway 1" << endl;

    runwSem[plane->runway - 1].Sem_post();

    cap--;
    if (airP.capacity > cap)sem_post(&mutex);

    //---------------------------------------------------------------------
    reqEndThread++;
    if (reqEndThread == reqCounter && controller) {

        sem_post(&mutexMain);

    }


    pthread_exit(0);
}

int main(int argc, char** argv) {

    airP.readInput(argv[1]);
    airP.taxyingPaths();
    test = airP.taxyinPaths;
    cap = 0;

    sem_init(&mutex, 0, 1);
    sem_init(&mutexMain, 0, 1);

    for (int i = 0; i < airP.runways; i++) {
        Semaphore semRun("runway" + to_string(i), 1);

        runwSem.push_back(semRun);

    }
    for (int i = 0; i < airP.taxiNodes; i++) {

        Semaphore semTax("taxi" + to_string(i), 1);

        taxSem.push_back(semTax);

    }

    string request;


    for (int i = 0; i < airP.runways; i++) {

        runwSem[i].Sem_init();

    }

    for (int i = 0; i < airP.taxyinPaths.size(); i++) {
        for (int j = 1; j < airP.taxyinPaths[i].size(); j++) {

            taxSem[stoi(airP.taxyinPaths[i][j]) - 1].Sem_init();
        }
    }
    sem_wait(&mutexMain);

    while (getline(cin, request)) {

        pthread_attr_t tattr;
        pthread_attr_init(&tattr);
        pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);



        reqCounter++;
        cap++;

        sem_wait(&mutex);

        Plane* plane = new Plane;


        //if(cap==airP.capacity)break;
        if (request == "")break;
        plane->parseReq(request);


        // cout<<"error : "<<plane.flightNO<<endl;

        //---------------------------------------------------------------------------
        if (plane->request == "L")pthread_create(&plane->planeT, &tattr, landing, (void *) plane);

        //---------------------------------------------------------------------------        
        if (plane->request == "T")pthread_create(&plane->planeT, &tattr, takingOff, (void *) plane);
        //---------------------------------------------------------------------------

        //pthread_join(plane.planeT, NULL);
        //---------------------------------------------------------------------------


//        if (airP.capacity > cap)sem_post(&mutex);
        //    cout<<"error 2: "<<plane.flightNO<<endl;

    }
    controller = 1;

    sem_wait(&mutexMain);
    sem_post(&mutexMain);

    for (int i = 0; i < airP.runways; i++) {

        runwSem[i].Sem_dest();
    }

    for (int i = 0; i < airP.taxyinPaths.size(); i++) {
        for (int j = 1; j < airP.taxyinPaths[i].size(); j++) {

            taxSem[stoi(airP.taxyinPaths[i][j]) - 1].Sem_dest();
        }
    }
    return 0;
}
