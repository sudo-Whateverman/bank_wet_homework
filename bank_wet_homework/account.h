

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <fstream>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;


class account {
public:
	account(int serialno, string password, int initialbalance) {
		_serialno = serialno;
		_password = password;
		_balance = initialbalance;
		_vip = false;
		pthread_mutex_init(&_readmutex, NULL);
		pthread_mutex_init(&_writemutex, NULL);
		_rdcount = 0;
	}
	int getSerialNo() const { return this->_serialno; }
    static void transaction(int src_serialno, string password, int dst_serialno, int ammount, int atmid);
    static void createAccount(int serialno, string password, int initialbalance, int atmid);
    static void getBalance(int serialno, string password, int atmid);
    static void deposit(int seriano, string password, int amount, int atmid);
    static void withdraw(int serialno, string password, int amount, int atmid);
    static void makeVip(int serialno, string password, int atmid);
    static int collectFees();
    static void printStatus();
    static bool check_password(int spot, string password, int atmid);
    static int listrdcount;
    static pthread_mutex_t listwritemutex;  // whole list write mutex
    static pthread_mutex_t listreadmutex;  // whole list read mutex
    static pthread_mutex_t filewritemutex; // logger file mutex
    pthread_mutex_t _readmutex;  // read mutex on the i-th account.
    pthread_mutex_t _writemutex; // write mutex on the i-th account.
    int _rdcount;
private:
    int _serialno;
    string _password;
    int _balance;
    bool _vip;
};

#endif /* ACCOUNT_H */

