/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: nick
 *
 * Created on November 27, 2017, 6:49 PM
 */
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include "atm.h"
#include "bank.h"
#include "parser.h"
#include <stdlib.h>
#include <list>
#include <string>
#include "account.h"
using namespace std;


int main(int argc, char** argv) {
    pthread_mutex_init(&(listwritemutex), NULL);
    pthread_mutex_init(&(listreadmutex), NULL);
    pthread_mutex_init(&(filewritemutex), NULL);
//     Validate input
    int N;
    pthread_mutex_lock(&filewritemutex);
    ofstream logger;
    logger.open("log.txt");
    logger << ""; // Just to make sure we're starting a clean log, clearing old "log.txt"
    logger.flush();
    logger.close();
    pthread_mutex_unlock(&filewritemutex);
    if (argc<2){
        cout << "Usage:" << endl;
        cout << "./program Number_of_atms [N_txt_files]" << endl;
        exit(-1);
    }
    try
    {
        N = atoi(argv[1]); // number of ATMs - the first arg, the zeroth is the filename
    }
    catch(std::invalid_argument& e){
        // if no conversion could be performed
        cout << "First argument should be a number" << endl;
        exit(-1);
    }
    if (N!=argc-2) // Since the first arg is the N_atm, and last is \0 we deduct 2
    {
        cout << "Please supply the correct number of arguments" << endl;
        exit(-1);
    }
    
    pthread_t threads[N];
    threadargs thargs[N];
    for(int i=2; i<argc; i++){
        thargs[i-2].filename = argv[i]; // Need to make a local variable to be passed; argv is behaving funny.
        thargs[i-2].atmid = i-1;
        pthread_create(&threads[i], NULL, perform_work, &(thargs[i-2])); // Responsible for parsing txtfile_i as independent ATM
    }
    bank bank;
    pthread_t bank_thread;
    pthread_create(&bank_thread, NULL, bank.bank_run, NULL); // Responsible for taking fees every S seconds
    for (int i=2; i<argc; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_join(bank_thread, NULL);
    return 0;
}
