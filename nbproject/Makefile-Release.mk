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
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ASTBinaryNode.o \
	${OBJECTDIR}/ASTCompoundNode.o \
	${OBJECTDIR}/ASTDeclarationNode.o \
	${OBJECTDIR}/ASTExpressionNode.o \
	${OBJECTDIR}/ASTFunctionCallNode.o \
	${OBJECTDIR}/ASTFunctionNode.o \
	${OBJECTDIR}/ASTNode.o \
	${OBJECTDIR}/ASTStatementNode.o \
	${OBJECTDIR}/ASTUnaryNode.o \
	${OBJECTDIR}/ASTVariableNode.o \
	${OBJECTDIR}/Admin.o \
	${OBJECTDIR}/Main.o \
	${OBJECTDIR}/Parser.o \
	${OBJECTDIR}/Scanner.o \
	${OBJECTDIR}/Token.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/parser

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/parser: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/parser ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/ASTBinaryNode.o: ASTBinaryNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTBinaryNode.o ASTBinaryNode.cpp

${OBJECTDIR}/ASTCompoundNode.o: ASTCompoundNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTCompoundNode.o ASTCompoundNode.cpp

${OBJECTDIR}/ASTDeclarationNode.o: ASTDeclarationNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTDeclarationNode.o ASTDeclarationNode.cpp

${OBJECTDIR}/ASTExpressionNode.o: ASTExpressionNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTExpressionNode.o ASTExpressionNode.cpp

${OBJECTDIR}/ASTFunctionCallNode.o: ASTFunctionCallNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTFunctionCallNode.o ASTFunctionCallNode.cpp

${OBJECTDIR}/ASTFunctionNode.o: ASTFunctionNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTFunctionNode.o ASTFunctionNode.cpp

${OBJECTDIR}/ASTNode.o: ASTNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTNode.o ASTNode.cpp

${OBJECTDIR}/ASTStatementNode.o: ASTStatementNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTStatementNode.o ASTStatementNode.cpp

${OBJECTDIR}/ASTUnaryNode.o: ASTUnaryNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTUnaryNode.o ASTUnaryNode.cpp

${OBJECTDIR}/ASTVariableNode.o: ASTVariableNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ASTVariableNode.o ASTVariableNode.cpp

${OBJECTDIR}/Admin.o: Admin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Admin.o Admin.cpp

${OBJECTDIR}/Main.o: Main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Main.o Main.cpp

${OBJECTDIR}/Parser.o: Parser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser.o Parser.cpp

${OBJECTDIR}/Scanner.o: Scanner.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Scanner.o Scanner.cpp

${OBJECTDIR}/Token.o: Token.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Token.o Token.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/parser

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
