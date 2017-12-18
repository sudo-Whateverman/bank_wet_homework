#include "account.h"



static std::vector<account> accountlist;

void account::transaction(int src_serialno, string password, int dst_serialno, int ammount, int atmid){
    pthread_mutex_lock(&filewritemutex);
    ofstream logger;
    logger.open("log.txt", std::ios_base::app);
    // Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect 
    logger << "Error " << atmid << ": Transaction not implemented" << endl;
    logger.flush();
    logger.close();
    pthread_mutex_unlock(&filewritemutex);
}


bool account::check_password(int serialno, string password, int atmid) {
    // Just a reader in the readers-writers hierarchy, for both list and account
    bool _pass_correct = false;
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
        if (i->_serialno == serialno &&  i->_password == password) {
            _pass_correct = true;
        }
    }
    if (_pass_correct == false){
        pthread_mutex_lock(&filewritemutex);
        ofstream logger;
        logger.open("log.txt", std::ios_base::app);
        // Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect 
        logger << "Error " << atmid << ": Your transaction failed – password for account id  " << serialno << " is incorrect  " << endl;
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
    return _pass_correct;
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
    if (spot != -1){
        pthread_mutex_lock(&(accountlist[spot]._readmutex));
        accountlist[spot]._rdcount++;
        if (accountlist[spot]._rdcount == 1) {
            pthread_mutex_lock(&(accountlist[spot]._writemutex));
        }
        pthread_mutex_unlock(&(accountlist[spot]._readmutex));
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
    if (spot != -1){
        pthread_mutex_lock(&(accountlist[spot]._writemutex));
        if (accountlist[spot]._balance >= amount) {
            accountlist[spot]._balance -= amount;
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
        pthread_mutex_lock(&(accountlist[spot]._writemutex));
        accountlist[spot]._balance += amount;
        pthread_mutex_lock(&filewritemutex);
        ofstream logger;
        logger.open("log.txt", std::ios_base::app);
        logger << atmid << ": Account " << serialno << " new balance is " << accountlist[spot]._balance << " after " << amount << " was deposited" << endl;
        logger.flush();
        logger.close();
        pthread_mutex_unlock(&filewritemutex);
        pthread_mutex_unlock(&(accountlist[spot]._writemutex));
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
        pthread_mutex_lock(&(accountlist[spot]._writemutex));
        accountlist[spot]._vip = true;
        pthread_mutex_unlock(&(accountlist[spot]._writemutex));
    }
    pthread_mutex_lock(&listreadmutex);
    listrdcount--;
    if (listrdcount == 0) {
        pthread_mutex_unlock(&listwritemutex);
    }
    pthread_mutex_unlock(&listreadmutex);
}

int account::collectfees() {
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
