# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall -Wextra -Werror -std=c++11
LDFLAGS = -pthread
CCLINK = $(CC)
OBJS = main.o bank.o account.o atm.o parser.o
RM = rm -f
# Creating the  executable
main: $(OBJS)
	$(CCLINK) $(CFLAGS) $(LDFLAGS) -o main $(OBJS)
# Creating the object files
account.o: account.cpp account.h
bank.o: bank.cpp bank.h
parser.o: parser.cpp parser.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o*~ "#"* core.*

