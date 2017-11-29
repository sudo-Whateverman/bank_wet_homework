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
using namespace std;


int main(int argc, char** argv) {
    
//     Validate input
    int N;
    try
    {
        N = stoi(argv[1]); // number of ATMs - the first arg, the zeroth is the filename
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
    for (int i=2; i<argc; i++)
    {
        if( access( argv[i], R_OK ) == -1 ) {
            // file doesn't exists
            cout << "The file specified isn't found" << endl;
            exit(-1);
        } 
    }
    
    pthread_t threads[N];
    for(int i=2; i<argc; i++){
        atm atm(argv[i]); // insert filename;
        pthread_create(&threads[i], NULL, atm.run, NULL); // Responsible for parsing txtfile_i as independent ATM
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
