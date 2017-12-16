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
  string str;
  char * filename;
  filename = reinterpret_cast<char*> (argument);
  ifstream file(filename);
  char action[10];
  char* parsed;
  char delimiters[] = " ";
  char line[255];
  while(getline(file, str)){
	  strcpy(line, str.c_str());
	  parsed = strtok(line, delimiters);
	  strcpy(action,parsed);
	  if (!strcmp(action, "O")) {
		  // Open account
		  int serialno = atoi(strtok(NULL, delimiters));
		  string password = strtok(NULL, delimiters);
		  int initialbalance = atoi(strtok(NULL, delimiters));
		  account::createAccount(serialno, password, initialbalance);
	  }
	  else if (!strcmp(action, "L")) {
		  // Make VIP
		  int serialno = atoi(strtok(NULL, delimiters));
		  char* password = strtok(NULL, delimiters);
		  account::makeVip(serialno, password);
	  }
	  else if (!strcmp(action, "D")) {
		  // Deposit
		  int serialno = atoi(strtok(NULL, delimiters));
		  string password = strtok(NULL, delimiters);
		  int balance = atoi(strtok(NULL, delimiters));
		  account::deposit(serialno, password, balance);
	  }
	  else if (!strcmp(action, "W")) {
		  // Withdrawal
		  int serialno = atoi(strtok(NULL, delimiters));
		  string password = strtok(NULL, delimiters);
		  int balance = atoi(strtok(NULL, delimiters));
		  account::withdraw(serialno, password, balance);
	  }
	  else if (!strcmp(action, "B")) {
		  // Check balance
		  int serialno = atoi(strtok(NULL, delimiters));
		  string password = strtok(NULL, delimiters);
		  account::getBalance(serialno, password);
	  }

  }
  return NULL;
}
 
