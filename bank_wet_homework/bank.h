/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bank.h
 * Author: nick
 *
 * Created on November 27, 2017, 8:06 PM
 */

#ifndef BANK_H
#define BANK_H

class bank {
public:
    bank();
    bank(const bank& orig);
    static void* bank_run(void *);
    virtual ~bank();
private:

};

#endif /* BANK_H */

