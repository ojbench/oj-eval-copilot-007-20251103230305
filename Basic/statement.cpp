/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include <iostream>

Statement::Statement() = default;

Statement::~Statement() = default;

RemStatement::RemStatement(TokenScanner &scanner) {
    // consume rest; nothing to store
    while (scanner.hasMoreTokens()) scanner.nextToken();
}

void RemStatement::execute(EvalState &, Program &) {
    // no-op
}

LetStatement::LetStatement(TokenScanner &scanner) {
    // The remaining should be an assignment expression like x = expr
    exp = parseExp(scanner);
}

LetStatement::~LetStatement() { delete exp; }

void LetStatement::execute(EvalState &state, Program &) {
    (void) exp->eval(state); // assignment performed in eval for '='
}

PrintStatement::PrintStatement(TokenScanner &scanner) {
    exp = parseExp(scanner);
}

PrintStatement::~PrintStatement() { delete exp; }

void PrintStatement::execute(EvalState &state, Program &) {
    int val = exp->eval(state);
    std::cout << val << std::endl;
}

InputStatement::InputStatement(TokenScanner &scanner) {
    std::string token = scanner.nextToken();
    if (token.empty() || scanner.getTokenType(token) != WORD) error("SYNTAX ERROR");
    var = token;
}

void InputStatement::execute(EvalState &state, Program &) {
    while (true) {
        std::cout << " ? ";
        std::cout.flush();
        std::string s;
        if (!std::getline(std::cin, s)) { s = "0"; }
        s = trim(s);
        try {
            int v = stringToInteger(s);
            state.setValue(var, v);
            break;
        } catch (ErrorException &) {
            std::cout << "INVALID NUMBER" << std::endl;
        }
    }
}

void EndStatement::execute(EvalState &state, Program &) {
    state.setEnd();
}

GotoStatement::GotoStatement(TokenScanner &scanner) {
    std::string token = scanner.nextToken();
    if (token.empty() || scanner.getTokenType(token) != NUMBER) error("SYNTAX ERROR");
    target = stringToInteger(token);
}

void GotoStatement::execute(EvalState &state, Program &) {
    state.setNextLine(target);
}

IfStatement::IfStatement(TokenScanner &scanner) {
    lhs = readE(scanner);
    op = scanner.nextToken();
    if (op != "<" && op != ">" && op != "=" && op != "<=" && op != ">=" && op != "<>") error("SYNTAX ERROR");
    rhs = readE(scanner);
    std::string thenTok = scanner.nextToken();
    if (toUpperCase(thenTok) != "THEN") error("SYNTAX ERROR");
    std::string lineTok = scanner.nextToken();
    if (lineTok.empty() || scanner.getTokenType(lineTok) != NUMBER) error("SYNTAX ERROR");
    target = stringToInteger(lineTok);
}

IfStatement::~IfStatement() {
    delete lhs;
    delete rhs;
}

void IfStatement::execute(EvalState &state, Program &) {
    int a = lhs->eval(state);
    int b = rhs->eval(state);
    bool cond = false;
    if (op == "=") cond = (a == b);
    else if (op == "<") cond = (a < b);
    else if (op == ">") cond = (a > b);
    else if (op == "<=") cond = (a <= b);
    else if (op == ">=") cond = (a >= b);
    else if (op == "<>") cond = (a != b);
    if (cond) state.setNextLine(target);
}
