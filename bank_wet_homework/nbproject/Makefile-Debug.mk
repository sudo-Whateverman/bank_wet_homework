#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/account.o \
	${OBJECTDIR}/accounts_manager.o \
	${OBJECTDIR}/atm.o \
	${OBJECTDIR}/bank.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/parser.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bank_wet_homework

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bank_wet_homework: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bank_wet_homework ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/account.o: account.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -include atm.h -include bank.h -include parser.h -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/account.o account.cpp

${OBJECTDIR}/accounts_manager.o: accounts_manager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -include atm.h -include bank.h -include parser.h -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/accounts_manager.o accounts_manager.cpp

${OBJECTDIR}/atm.o: atm.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -include atm.h -include bank.h -include parser.h -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/atm.o atm.cpp

${OBJECTDIR}/bank.o: bank.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -include atm.h -include bank.h -include parser.h -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bank.o bank.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -include atm.h -include bank.h -include parser.h -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/parser.o: parser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -include atm.h -include bank.h -include parser.h -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/parser.o parser.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
