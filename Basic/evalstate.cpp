/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */


#include "evalstate.hpp"

EvalState::EvalState() = default;

EvalState::~EvalState() = default;

void EvalState::setValue(std::string var, int value) {
    symbolTable[var] = value;
}

int EvalState::getValue(std::string var) {
    if (isDefined(var)) return symbolTable[var];
    return 0;
}

bool EvalState::isDefined(std::string var) {
    return symbolTable.find(var) != symbolTable.end();
}

void EvalState::Clear() {
    symbolTable.clear();
}

void EvalState::setNextLine(int line) {
    hasJump = true;
    nextLine = line;
}

bool EvalState::hasNextLine() const { return hasJump; }

int EvalState::getNextLine() const { return nextLine; }

void EvalState::clearControl() {
    hasJump = false;
    nextLine = -1;
    ended = false;
}

void EvalState::setEnd() { ended = true; }

bool EvalState::isEnd() const { return ended; }
