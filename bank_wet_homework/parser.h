/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   parser.h
 * Author: nick
 *
 * Created on November 30, 2017, 12:19 AM
 */

#ifndef PARSER_H
#define PARSER_H
#include <cstdlib>
#include <iostream>
#include <pthread.h>
using namespace std;

typedef struct _threadargs {
	string filename;
	int atmid;
} threadargs;
void* perform_work(void* argument);
#endif /* PARSER_H */
