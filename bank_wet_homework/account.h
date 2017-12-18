

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <fstream>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

static pthread_mutex_t listwritemutex;  // whole list write mutex
static pthread_mutex_t listreadmutex;  // whole list read mutex
static int listrdcount = 0;
static pthread_mutex_t filewritemutex; // logger file mutex

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
    static void createAccount(int serialno, string password, int initialbalance, int atmid);
    static void getBalance(int serialno, string password, int atmid);
    static void deposit(int seriano, string password, int amount, int atmid);
    static void withdraw(int serialno, string password, int amount, int atmid);
    static void makeVip(int serialno, string password, int atmid);
    static int collectfees();
    static bool check_password(int serialno, string password, int atmid);
private:
    int _serialno;
    string _password;
    int _balance;
    pthread_mutex_t _readmutex;  // read mutex on the i-th account.
    pthread_mutex_t _writemutex; // write mutex on the i-th account.
    bool _vip;
    int _rdcount;
};

#endif /* ACCOUNT_H */

