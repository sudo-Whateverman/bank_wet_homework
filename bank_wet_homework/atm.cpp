/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   atm.cpp
 * Author: nick
 * 
 * Created on November 27, 2017, 7:07 PM
 */

#include <iostream>

#include "atm.h"

atm::atm(char* filename) {
    ifstream file(filename);
    
}

atm::atm(const atm& orig) {
}

int atm::create_account(int id){
    return 0;
}
int atm::withdraw(){
    //etc.
    return 0;
}
string atm::parse(){
    string str; 
    getline(file, str);
    std::cout << str;
    return str;
}
void* atm::run(void *){
    string line;
//    line = parse();
    sleep(1); // lock this.
    return NULL;
}


atm::~atm() {
}

