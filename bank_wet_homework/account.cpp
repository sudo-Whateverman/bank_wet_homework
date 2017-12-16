#include "account.h"
#include <iostream>


static std::vector<account> accountlist;


void account::createAccount(int serialno, string password, int initialbalance) {
	pthread_mutex_lock(&listwritemutex);
	for (auto & i : accountlist) {
		if (i._serialno == serialno) {
			std::cout << "Account number " << serialno << " already exists." << endl;
			pthread_mutex_unlock(&listwritemutex);
			return;
		}
	}
	account tmpAccount(serialno, password, initialbalance);
	accountlist.push_back(tmpAccount);
	std::cout << "Account number " << serialno << " created." << endl;
	pthread_mutex_unlock(&listwritemutex);
}


int account::getBalance(int serialno, string password) {
	// Just a reader in the readers-writers hierarchy, for both list and account
	pthread_mutex_lock(&listreadmutex);
	listrdcount++;
	if (listrdcount == 1) {
		pthread_mutex_lock(&listwritemutex);
	}
	pthread_mutex_unlock(&listreadmutex);
	// Reading from list
	int spot = -1;
	int iterator = 0;
	int balance;
	for (auto & i : accountlist) {
		if (i._serialno == serialno) {
			spot = iterator;
		}
		iterator++;
	}
	if (spot != -1){
		pthread_mutex_lock(&(accountlist[spot]._readmutex));
		accountlist[spot]._rdcount++;
		if (accountlist[spot]._rdcount == 1) {
			pthread_mutex_lock(&(accountlist[spot]._writemutex));
		}
		pthread_mutex_unlock(&(accountlist[spot]._readmutex));
		balance = accountlist[spot]._balance;
		pthread_mutex_lock(&(accountlist[spot]._readmutex));
		accountlist[spot]._rdcount--;
		if (accountlist[spot]._rdcount == 0) {
			pthread_mutex_unlock(&(accountlist[spot]._writemutex));
		}
		pthread_mutex_unlock(&(accountlist[spot]._readmutex));
	}
	pthread_mutex_lock(&listreadmutex);
	listrdcount--;
	if (listrdcount == 0) {
		pthread_mutex_unlock(&listwritemutex);
	}
	pthread_mutex_unlock(&listreadmutex);
	return balance;
}

void account::withdraw(int serialno, string password, int amount) {
	std::cout << "Got withdraw request " << serialno << " " << password << " " << amount << endl;
}

void account::deposit(int serialno, string password, int amount) {
	std::cout << "Got deposit request " << serialno << " " << password << " " << amount << endl;
}

void account::makeVip(int serialno, string password) {
	// A writer in the readers-writers hierarchy
}

