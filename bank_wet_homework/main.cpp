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

#include <pthread.h>
#include "atm.h"
#include "bank.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int N = atoi(argv[0]); // number of ATMs
    pthread_t threads;
    for(int i=0; i<N; i++){
        atm atm(argv[i]); // insert filename;
        pthread_create(&threads, NULL, atm.run, NULL); // Responsible for parsing txtfile_i as independent ATM
    }
    bank bank;
    pthread_t bank_thread;
    pthread_create(&bank_thread, NULL, bank.bank_run, NULL); // Responsible for taking fees every S seconds
    for (int i=0; i<N; i++) {
        pthread_join(threads, NULL);
    }
    pthread_join(bank_thread, NULL);
    return 0;
}
