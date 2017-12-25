/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   parser.cpp
 * Author: nick
 * 
 * Created on November 30, 2017, 12:19 AM
 */

#include "parser.h"
#include "account.h"
#include "bank.h"
#include <stdio.h>
#include <string.h>

void* perform_work(void* argument) {
    // Main parsing thread to parse a line a tokenize inside functions to pass to account manager
    string filename, str;
    int line_num = 0;
    threadargs thargs = *((threadargs*) argument);
    if (thargs.filename.empty()) {
        printf("%d\n" ,thargs.atmid);
        printf("thargs is empty\n");
    }
    filename.assign(thargs.filename);
    int atmid = thargs.atmid;
    ifstream file(filename.c_str());
    if (!file.good()){
//        cout << "File error: " << filename << " The file specified isn't found" << endl;
        exit(-1); 
    }
    
    while(getline(file, str)){
        if (!str.empty()){
            char* args[MAX_ARG];
            cout << atmid << " : "<< line_num << " : " << str << endl;
            char* line = strdup(str.c_str());
            int num_arg;
            char* action;
            char delimiters[] = " ";
            action = strtok(line, delimiters);
            if (action == NULL)
		continue; 
            args[0] = action;
            for (int i=1; i<MAX_ARG; i++)
            {
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
                    num_arg++;   
            }
            if (!strcmp(action, "O")) {
                // Open account
                if (num_arg == 4){
                    int serialno = atoi(args[1]);
                    string password = args[2];
                    int initialbalance = atoi(args[3]);
                    account::createAccount(serialno, password, initialbalance, atmid);
                }
            }
            else if (!strcmp(action, "L")) {
                // Make VIP
                if (num_arg == 3){
                    int serialno = atoi(args[1]);
                    string password = args[2];
                    account::makeVip(serialno, password, atmid);
                }
            }
            else if (!strcmp(action, "D")) {
                // Deposit
                if (num_arg == 4){
                    int serialno = atoi(args[1]);
                    string password = args[2];
                    int balance = atoi(args[3]);
                    account::deposit(serialno, password, balance, atmid);
                }
            }
            else if (!strcmp(action, "W")) {
                // Withdrawal
                if (num_arg == 4){
                    int serialno = atoi(args[1]);
                    string password = args[2];
                    int balance = atoi(args[3]);
                    account::withdraw(serialno, password, balance, atmid);
                }
            }
            else if (!strcmp(action, "B")) {
                // Check balance
                if (num_arg == 3){
                    int serialno = atoi(args[1]);
                    string password = args[2];
                    account::getBalance(serialno, password, atmid);
                }
            }
            else if (!strcmp(action, "T")) {
                // Make Transaction
                if (num_arg == 5){
                    int src_serialno = atoi(args[1]);
                    string password = args[2];
                    int dst_serialno = atoi(args[3]);
                    int amount = atoi(args[4]);
                    account::transaction(src_serialno, password, dst_serialno, amount, atmid);
                }
            }
            else
            {
                // case that function is neither of the above.
                pthread_mutex_lock(&account::filewritemutex);
                ofstream logger;
                logger.open("log.txt", std::ios_base::app); // append, has nothing to do with ios, apps or any STD's
                logger << "Parser: illegal function of \""<< str <<"\" has been encountered. Skipped"  << endl;
                logger.flush();
                logger.close();
                pthread_mutex_unlock(&account::filewritemutex);
            }
        }
        usleep(100000);
        str.clear();
        line_num ++;
    }
    pthread_mutex_lock(&(bank::atmcntmutex));
    (bank::finishedatms)++;
    pthread_mutex_unlock(&(bank::atmcntmutex));
    return NULL;
}

