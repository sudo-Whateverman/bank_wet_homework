

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <fstream>
#include <unistd.h>
#include <vector>
using namespace std;

static pthread_mutex_t listwritemutex;
static pthread_mutex_t listreadmutex;
static int listrdcount = 0;

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
    static void createAccount(int serialno, string password, int initialbalance);
    static int getBalance(int serialno, string password);
    static void deposit(int seriano, string password, int amount);
    static void withdraw(int serialno, string password, int amount);
    static void makeVip(int serialno, string password);
    void account_write_lock();
    void account_write_unlock();
    void account_read_lock();
    void account_read_unlock();
private:
    int _serialno;
    string _password;
    int _balance;
    pthread_mutex_t _readmutex;
    pthread_mutex_t _writemutex;
    bool _vip;
    int _rdcount;
};

#endif /* ACCOUNT_H */

