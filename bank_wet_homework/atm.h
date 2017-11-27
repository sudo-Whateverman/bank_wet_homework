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
using namespace std;
class atm {
public:
    atm(char* );
    atm(const atm& orig);
    int create_account(int id);
    int withdraw();
    char* parse();
    static void* run(void*);
    virtual ~atm();
private:
    FILE* filename;
    
};

#endif /* ATM_H */

