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


bank::bank() {
}

bank::bank(const bank& orig) {
}

void* bank::bank_run(void* arg){
	int tmpFinishedAtms;
	int tmpTotalAtms;
	pthread_mutex_lock(&atmcntmutex);
	tmpFinishedAtms = finishedatms;
	tmpTotalAtms = totalatms;
	pthread_mutex_unlock(&atmcntmutex);
	while (tmpFinishedAtms != tmpTotalAtms) {
		bankmoney += account::collectFees();
		sleep(3);
		pthread_mutex_lock(&atmcntmutex);
		tmpFinishedAtms = finishedatms;
		tmpTotalAtms = totalatms;
		pthread_mutex_unlock(&atmcntmutex);
	}
	return NULL;
}

void* bank::bank_status_printer(void* arg){
	int tmpFinishedAtms;
	int tmpTotalAtms;
	pthread_mutex_lock(&atmcntmutex);
	tmpFinishedAtms = finishedatms;
	tmpTotalAtms = totalatms;
	pthread_mutex_unlock(&atmcntmutex);
	while (tmpFinishedAtms != tmpTotalAtms) {
//		usleep(500000);
//		account::printStatus();
		pthread_mutex_lock(&atmcntmutex);
		tmpFinishedAtms = finishedatms;
		tmpTotalAtms = totalatms;
		pthread_mutex_unlock(&atmcntmutex);
	}
	// Just another print to make sure we're not in the buffer zone
//	usleep(500000);
//	account::printStatus();
	return NULL;
}

bank::~bank() {
}

int bank::finishedatms = 0;
int bank::totalatms = -1;
int bank::bankmoney = 0;
pthread_mutex_t bank::atmcntmutex;

