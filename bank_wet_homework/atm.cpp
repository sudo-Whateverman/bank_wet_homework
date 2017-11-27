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

#include "atm.h"

atm::atm(char* ) {
    FILE* filename = "";
    open(filename, 'r');
}

atm::atm(const atm& orig) {
}

atm::create_account(int id){
    
}
atm::withdraw(){
    //etc.
}
atm::parse(){
    
}
atm::run(){
    do{
    parse_line();
    sleep(1); // lock this.
    }while(line!=EOF)
}


atm::~atm() {
}

