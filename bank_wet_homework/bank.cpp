/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bank.cpp
 * Author: nick
 * 
 * Created on November 27, 2017, 8:06 PM
 */

#include "bank.h"
#include <cstddef>
#include "account.h"

static int bankmoney = 0;

bank::bank() {
}

bank::bank(const bank& orig) {
}

void* bank::bank_run(void *){
	// Some while loop here, condition is that at least one atm is still running
	// (I guess that's what we need the sockets for...)
	sleep(3);
	bankmoney += account::collectfees();
}

bank::~bank() {
}

