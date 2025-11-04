/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/error.hpp"

Program::Program() = default;

Program::~Program() { clear(); }

void Program::clear() {
    for (auto &kv : parsed) {
        delete kv.second;
    }
    parsed.clear();
    sourceLines.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // replace existing
    if (parsed.count(lineNumber)) {
        delete parsed[lineNumber];
        parsed.erase(lineNumber);
    }
    sourceLines[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    if (parsed.count(lineNumber)) {
        delete parsed[lineNumber];
        parsed.erase(lineNumber);
    }
    sourceLines.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = sourceLines.find(lineNumber);
    if (it == sourceLines.end()) return "";
    return it->second;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (!sourceLines.count(lineNumber)) error("SET PARSED ON NONEXISTENT LINE");
    if (parsed.count(lineNumber)) {
        delete parsed[lineNumber];
    }
    parsed[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = parsed.find(lineNumber);
    if (it == parsed.end()) return nullptr;
    return it->second;
}

int Program::getFirstLineNumber() {
    if (sourceLines.empty()) return -1;
    return sourceLines.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = sourceLines.upper_bound(lineNumber);
    if (it == sourceLines.end()) return -1;
    return it->first;
}
