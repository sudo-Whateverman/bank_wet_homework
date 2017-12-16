# Makefile for the smash program
CC = gcc
CFLAGS = -g -Wall -std=c++11
CCLINK = $(CC)
OBJS = main.o bank.o account.o atm.o parser.o
RM = rm -f
# Creating the  executable
main: $(OBJS)
	$(CCLINK) -o main $(OBJS)
# Creating the object files
account.o: account.cpp account.h
bank.o: bank.cpp bank.h
parser.o: parser.cpp parser.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

