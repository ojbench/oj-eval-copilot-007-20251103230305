/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

class Statement {
public:
    Statement();
    virtual ~Statement();
    virtual void execute(EvalState &state, Program &program) = 0;
};

class RemStatement : public Statement {
public:
    explicit RemStatement(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
};

class LetStatement : public Statement {
public:
    explicit LetStatement(TokenScanner &scanner);
    ~LetStatement() override;
    void execute(EvalState &state, Program &program) override;
private:
    Expression *exp = nullptr; // assignment expression
};

class PrintStatement : public Statement {
public:
    explicit PrintStatement(TokenScanner &scanner);
    ~PrintStatement() override;
    void execute(EvalState &state, Program &program) override;
private:
    Expression *exp = nullptr;
};

class InputStatement : public Statement {
public:
    explicit InputStatement(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
private:
    std::string var;
};

class EndStatement : public Statement {
public:
    EndStatement() = default;
    void execute(EvalState &state, Program &program) override;
};

class GotoStatement : public Statement {
public:
    explicit GotoStatement(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
private:
    int target;
};

class IfStatement : public Statement {
public:
    explicit IfStatement(TokenScanner &scanner);
    ~IfStatement() override;
    void execute(EvalState &state, Program &program) override;
private:
    Expression *lhs = nullptr;
    Expression *rhs = nullptr;
    std::string op;
    int target;
};

#endif
