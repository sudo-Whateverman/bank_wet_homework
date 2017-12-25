#include "account.h"
#include "bank.h"
#include <algorithm>

static std::vector<account> accountlist;

struct accountCmpr {
    // Compare function for std sort of the vector of accounts.
	bool operator() (const account& lhs, const account& rhs) { return (lhs.getSerialNo() < rhs.getSerialNo()); }
} accountCompare;

void account::transaction(int src_serialno, string password, int dst_serialno, int amount, int atmid){
    pthread_mutex_lock(&listreadmutex);
    listrdcount++;
    if (listrdcount == 1) {
        pthread_mutex_lock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
    int src_spot = -1;
    int dst_spot = -1;
    int iterator = 0;
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
        if (i->_serialno == src_serialno) {
            src_spot = iterator;
        }
        else if(i->_serialno == dst_serialno){
            dst_spot = iterator;
        }
        iterator++;
    }
    if (src_spot != -1 && dst_spot != -1){
        // Should check order of mutex lock.
        pthread_mutex_lock(&(accountlist[src_spot]._writemutex));
        pthread_mutex_lock(&(accountlist[dst_spot]._writemutex));        
        if (accountlist[src_spot]._balance >= amount){
            accountlist[src_spot]._balance -= amount;
            accountlist[dst_spot]._balance += amount;
            sleep(1);
            pthread_mutex_lock(&filewritemutex);
            ofstream logger;
            logger.open("log.txt", std::ios_base::app);
//            <ATM ID>: Transfer <amount> from account <account> to account <target_account> new account 
            logger << atmid << ": Transfer " << amount << " from account " << src_serialno << " to account " << dst_serialno << " new account balance is " << accountlist[src_spot]._balance << " new target account balance is " << accountlist[dst_spot]._balance << endl;
            logger.flush();
            logger.close();
            pthread_mutex_unlock(&filewritemutex);
        }
        else {
            pthread_mutex_lock(&filewritemutex);
            ofstream logger;
            logger.open("log.txt", std::ios_base::app);
            logger << "Error " << atmid << ": Your transaction failed - account id " << src_serialno << " balance is lower than " <<  amount << endl;
            logger.flush();
            logger.close();
            pthread_mutex_unlock(&filewritemutex);
        }
        pthread_mutex_unlock(&(accountlist[dst_spot]._writemutex));
        pthread_mutex_unlock(&(accountlist[src_spot]._writemutex));
        
    }
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
}


bool account::check_password(int spot, string password, int atmid) {
    // Just a reader in the readers-writers hierarchy, for both list and account
    if (accountlist[spot]._password == password) {
    	return true;
    }
    else {
    	pthread_mutex_lock(&filewritemutex);
		ofstream logger;
		logger.open("log.txt", std::ios_base::app);
		logger << "Error " << atmid << ": Your transaction failed - password for account id " << accountlist[spot]._serialno << " is incorrect" << endl;
		logger.flush();
		logger.close();
		pthread_mutex_unlock(&filewritemutex);
		return false;
    }
}

void account::createAccount(int serialno, string password, int initialbalance, int atmid) {
    // A writer in the reader-write hierarchy for a list
    pthread_mutex_lock(&listwritemutex);
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
        if (i->_serialno == serialno) {
            pthread_mutex_lock(&filewritemutex);
            ofstream logger;
            logger.open("log.txt", std::ios_base::app);
            logger << "Error " << atmid << ": Your transaction failed - account with the same id exists" << endl;
            logger.flush();
            logger.close();
            pthread_mutex_unlock(&filewritemutex);
            pthread_mutex_unlock(&listwritemutex);
            return;
        }
    }
    account tmpAccount(serialno, password, initialbalance);
    accountlist.push_back(tmpAccount);
    std::sort(accountlist.begin(), accountlist.end(), accountCompare);
    sleep(1);
    pthread_mutex_lock(&filewritemutex);
    ofstream logger;
    logger.open("log.txt", std::ios_base::app);
    logger << atmid << ": New account id is " << serialno << " with password " << password << " and initial balance " << initialbalance << endl;
    logger.flush();
    logger.close();
    pthread_mutex_unlock(&filewritemutex);
    pthread_mutex_unlock(&listwritemutex);
}


void account::getBalance(int serialno, string password, int atmid) {
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
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
        if (i->_serialno == serialno) {
            spot = iterator;
        }
        iterator++;
    }
    if (spot != -1) {
    	if (check_password(spot, password, atmid)) {
			pthread_mutex_lock(&(accountlist[spot]._readmutex));
			accountlist[spot]._rdcount++;
			if (accountlist[spot]._rdcount == 1) {
				pthread_mutex_lock(&(accountlist[spot]._writemutex));
			}
			pthread_mutex_unlock(&(accountlist[spot]._readmutex));
			sleep(1);
			pthread_mutex_lock(&filewritemutex);
			ofstream logger;
			logger.open("log.txt", std::ios_base::app);
			logger << atmid << ": Account " << serialno << " balance is " << accountlist[spot]._balance << endl;
			logger.flush();
			logger.close();
			pthread_mutex_unlock(&filewritemutex);
			pthread_mutex_lock(&(accountlist[spot]._readmutex));
			accountlist[spot]._rdcount--;
			if (accountlist[spot]._rdcount == 0) {
				pthread_mutex_unlock(&(accountlist[spot]._writemutex));
			}
			pthread_mutex_unlock(&(accountlist[spot]._readmutex));
    	}
    }
    else {
		pthread_mutex_lock(&filewritemutex);
		ofstream logger;
		logger.open("log.txt", std::ios_base::app);
		logger << "Error " << atmid << ": Your transaction failed - account id " << serialno << " does not exist" << endl;
		logger.flush();
		logger.close();
		pthread_mutex_unlock(&filewritemutex);
	}
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
}

void account::withdraw(int serialno, string password, int amount, int atmid) {
    // A reader in the readers-writers hierarchy for a list
    // A writer in the readers-writers hierarchy for an account
    pthread_mutex_lock(&listreadmutex);
    listrdcount++;
    if (listrdcount == 1) {
        pthread_mutex_lock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
    // Reading from list
    int spot = -1;
    int iterator = 0;
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
        if (i->_serialno == serialno) {
            spot = iterator;
        }
        iterator++;
    }
    if (spot != -1) {
    	if (check_password(spot, password, atmid)) {
			pthread_mutex_lock(&(accountlist[spot]._writemutex));
			if (accountlist[spot]._balance >= amount) {
				accountlist[spot]._balance -= amount;
				sleep(1);
				pthread_mutex_lock(&filewritemutex);
				ofstream logger;
				logger.open("log.txt", std::ios_base::app);
				logger << atmid << ": Account " << serialno << " new balance is " << accountlist[spot]._balance << " after " << amount << " was withdrew" << endl;
				logger.flush();
				logger.close();
				pthread_mutex_unlock(&filewritemutex);
			}
			else {
				pthread_mutex_lock(&filewritemutex);
				ofstream logger;
				logger.open("log.txt", std::ios_base::app);
				logger << "Error " << atmid << ": Your transaction failed - account id " << serialno << " balance is lower than " <<  amount << endl;
				logger.flush();
				logger.close();
				pthread_mutex_unlock(&filewritemutex);
			}
			pthread_mutex_unlock(&(accountlist[spot]._writemutex));
    	}
    }
    else {
		pthread_mutex_lock(&filewritemutex);
		ofstream logger;
		logger.open("log.txt", std::ios_base::app);
		logger << "Error " << atmid << ": Your transaction failed - account id " << serialno << " does not exist" << endl;
		logger.flush();
		logger.close();
		pthread_mutex_unlock(&filewritemutex);
	}
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
}

void account::deposit(int serialno, string password, int amount, int atmid) {
    // A reader in the readers-writers hierarchy for a list
    // A writer in the readers-writers hierarchy for an account
    pthread_mutex_lock(&listreadmutex);
    listrdcount++;
    if (listrdcount == 1) {
        pthread_mutex_lock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
    // Reading from list
    int spot = -1;
    int iterator = 0;
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
        if (i->_serialno == serialno) {
            spot = iterator;
        }
        iterator++;
    }
    if (spot != -1){
    	if (check_password(spot, password, atmid)) {
			pthread_mutex_lock(&(accountlist[spot]._writemutex));
			accountlist[spot]._balance += amount;
			sleep(1);
			pthread_mutex_lock(&filewritemutex);
			ofstream logger;
			logger.open("log.txt", std::ios_base::app);
			logger << atmid << ": Account " << serialno << " new balance is " << accountlist[spot]._balance << " after " << amount << " was deposited" << endl;
			logger.flush();
			logger.close();
			pthread_mutex_unlock(&filewritemutex);
			pthread_mutex_unlock(&(accountlist[spot]._writemutex));
    	}
    }
    else {
		pthread_mutex_lock(&filewritemutex);
		ofstream logger;
		logger.open("log.txt", std::ios_base::app);
		logger << "Error " << atmid << ": Your transaction failed - account id " << serialno << " does not exist" << endl;
		logger.flush();
		logger.close();
		pthread_mutex_unlock(&filewritemutex);
	}
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
}

void account::makeVip(int serialno, string password, int atmid) {
    // A reader in the readers-writers hierarchy for a list
    // A writer in the readers-writers hierarchy for an account
    pthread_mutex_lock(&listreadmutex);
    listrdcount++;
    if (listrdcount == 1) {
        pthread_mutex_lock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
    // Reading from list
    int spot = -1;
    int iterator = 0;
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
        if (i->_serialno == serialno) {
            spot = iterator;
        }
        iterator++;
    }
    if (spot != -1){
    	if (check_password(spot, password, atmid)) {
    		sleep(1);
			pthread_mutex_lock(&(accountlist[spot]._writemutex));
			accountlist[spot]._vip = true;
			pthread_mutex_unlock(&(accountlist[spot]._writemutex));
    	}
    }
    else {
    	pthread_mutex_lock(&filewritemutex);
		ofstream logger;
		logger.open("log.txt", std::ios_base::app);
		logger << "Error " << atmid << ": Your transaction failed - account id " << serialno << " does not exist" << endl;
		logger.flush();
		logger.close();
		pthread_mutex_unlock(&filewritemutex);
    }
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
}

int account::collectFees() {
    // A reader in the reader-writer hierarchy for a list
    // A writer in the reader-write hierarchy for an account
    int percent = (rand() % 2) + 2; // Between 2 and 4
    int totalgain = 0;
    int singlegain = 0;
    pthread_mutex_lock(&listreadmutex);
    listrdcount++;
    if (listrdcount == 1) {
        pthread_mutex_lock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
        pthread_mutex_lock(&(i->_writemutex));
        if (!(i->_vip)) {
            singlegain = (i->_balance) * ((float)percent / 100);
            i->_balance -= singlegain;
            totalgain += singlegain;
            pthread_mutex_lock(&filewritemutex);
            ofstream logger;
            logger.open("log.txt", std::ios_base::app);
            logger << "Bank: commissions of " << percent << "% were charged, the bank gained " << singlegain << "$ from account " << i->_serialno << endl;
            logger.flush();
            logger.close();
            pthread_mutex_unlock(&filewritemutex);
        }
        pthread_mutex_unlock(&(i->_writemutex));
    }
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
    return totalgain;
}

void account::printStatus() {
    // Just a reader in the readers-writers hierarchy, for both list and account
	printf("\033[2J"); // Clear screen
	printf("\033[1;1H"); // Place cursor on top-left corner
	printf("Current Bank Status\n");
    pthread_mutex_lock(&listreadmutex);
    listrdcount++;
    if (listrdcount == 1) {
        pthread_mutex_lock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
    // Reading from list
    for (std::vector<account>::iterator i=accountlist.begin(); i!= accountlist.end(); i++) {
    	pthread_mutex_lock(&(i->_readmutex));
		i->_rdcount++;
		if (i->_rdcount == 1) {
			pthread_mutex_lock(&(i->_writemutex));
		}
		pthread_mutex_unlock(&(i->_readmutex));
		printf("Account %d: Balance - %d $ , Account Password - %s\n", i->_serialno, i->_balance, i->_password.c_str());
		pthread_mutex_lock(&(i->_readmutex));
		i->_rdcount--;
		if (i->_rdcount == 0) {
			pthread_mutex_unlock(&(i->_writemutex));
		}
		pthread_mutex_unlock(&(i->_readmutex));
    }
	printf("The bank has %d $\n", bank::bankmoney);
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
}

int account::listrdcount = 0;
pthread_mutex_t account::listreadmutex;
pthread_mutex_t account::listwritemutex;
pthread_mutex_t account::filewritemutex;
