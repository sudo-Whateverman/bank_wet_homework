/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   parser.cpp
 * Author: nick
 * 
 * Created on November 30, 2017, 12:19 AM
 */

#include "parser.h"

void* perform_work(void* argument) {
  string filename, str;
  filename = *reinterpret_cast<string*> (argument);
  ifstream file(filename);
  atm *Atm;
  Atm = new atm();
  while(getline(file, str)){
      Atm->print_line(str); // TODO: refactor;
  }
  return NULL;
}
 