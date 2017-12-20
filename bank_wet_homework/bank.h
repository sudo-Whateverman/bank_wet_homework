/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bank.h
 * Author: nick
 *
 * Created on November 27, 2017, 8:06 PM
 */

#ifndef BANK_H
#define BANK_H
#include <fstream>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstdlib>


//static pthread_mutex_t atmcntmutex;  // atm counter's write mutex
//static pthread_mutex_t atmcntreadmutex;  // atm counter's read mutex

class bank {
public:
    bank();
    bank(const bank& orig);
    static void* bank_run(void* arg);
    static void* bank_status_printer(void* arg);
    static int finishedatms;
    static int totalatms;
    static int bankmoney;
    static pthread_mutex_t atmcntmutex;
    virtual ~bank();
private:

};

#endif /* BANK_H */

