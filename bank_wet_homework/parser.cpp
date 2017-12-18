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
#include <stdio.h>
#include <string.h>

void* perform_work(void* argument) {
    string filename, str;
    threadargs thargs = *((threadargs*) argument);
    filename = thargs.filename;
    int atmid = thargs.atmid;
    ifstream file(filename.c_str());
    if (!file.good()){
        cout << "File error: " << filename << " The file specified isn't found" << endl;
        exit(-1); 
    }
    char action[10];
    char* parsed;
    char delimiters[] = " ";
    char line[255];
    while(getline(file, str)){
        if (!str.empty()){
            strcpy(line, str.c_str());
            parsed = strtok(line, delimiters);
            strcpy(action,parsed);
            if (!strcmp(action, "O")) {
                // Open account
                int serialno = atoi(strtok(NULL, delimiters));
                string password = strtok(NULL, delimiters);
                int initialbalance = atoi(strtok(NULL, delimiters));
                account::createAccount(serialno, password, initialbalance, atmid);
            }
            else if (!strcmp(action, "L")) {
                // Make VIP
                int serialno = atoi(strtok(NULL, delimiters));
                char* password = strtok(NULL, delimiters);
                if (account::check_password(serialno, password, atmid))
                {
                    account::makeVip(serialno, password, atmid);
                }
            }
            else if (!strcmp(action, "D")) {
                // Deposit
                int serialno = atoi(strtok(NULL, delimiters));
                string password = strtok(NULL, delimiters);
                int balance = atoi(strtok(NULL, delimiters));
                if (account::check_password(serialno, password, atmid))
                {
                    account::deposit(serialno, password, balance, atmid);
                }
            }
            else if (!strcmp(action, "W")) {
                // Withdrawal
                int serialno = atoi(strtok(NULL, delimiters));
                string password = strtok(NULL, delimiters);
                int balance = atoi(strtok(NULL, delimiters));
                if (account::check_password(serialno, password, atmid))
                {
                    account::withdraw(serialno, password, balance, atmid);
                    
                }
            }
            else if (!strcmp(action, "B")) {
                // Check balance
                int serialno = atoi(strtok(NULL, delimiters));
                string password = strtok(NULL, delimiters);
                if (account::check_password(serialno, password, atmid))
                {
                    account::getBalance(serialno, password, atmid);
                }
            }
            else if (!strcmp(action, "T")) {
                // Make Transaction
                int src_serialno = atoi(strtok(NULL, delimiters));
                string password = strtok(NULL, delimiters);
                int dst_serialno = atoi(strtok(NULL, delimiters));
                int ammount = atoi(strtok(NULL, delimiters));
                if (account::check_password(src_serialno, password, atmid))
                {
                    account::transaction(src_serialno, password, dst_serialno, ammount, atmid);
                }
            }
            else
            {
                // case that function is neither of the above.
                pthread_mutex_lock(&filewritemutex);
                ofstream logger;
                logger.open("log.txt", std::ios_base::app); // append, has nothing to do with ios, apps or any STD's
                logger << "Parser: illegal function of \""<< str <<"\" has been encountered. Skipped"  << endl;
                logger.flush();
                logger.close();
                pthread_mutex_unlock(&filewritemutex);
            }
        }
        usleep(100000);
    }
    return NULL;
}

