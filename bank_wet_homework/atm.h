/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   atm.h
 * Author: nick
 *
 * Created on November 27, 2017, 7:07 PM
 */

#ifndef ATM_H
#define ATM_H
#include <fstream>
#include <unistd.h>
using namespace std;
class atm {
public:
    atm();
    atm(const atm& orig);
    int create_account(int id);
    int withdraw();
    void print_line(string str);
    virtual ~atm();
private:

    
};

#endif /* ATM_H */

