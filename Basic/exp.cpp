/*
 * File: exp.cpp
 * -------------
 * This file implements the Expression class and its subclasses.
 */

#include "exp.hpp"
#include <unordered_set>

// Registry of live expressions to clean up on exceptions
static std::unordered_set<Expression*> g_exprRegistry;

void cleanupExpressionLeaks() {
    // delete any remaining expressions not yet freed
    while (!g_exprRegistry.empty()) {
        auto it = g_exprRegistry.begin();
        Expression* p = *it;
        g_exprRegistry.erase(it);
        delete p; // destructor will remove from registry if not yet
    }
}

Expression::Expression() {
    g_exprRegistry.insert(this);
}

Expression::~Expression() {
    g_exprRegistry.erase(this);
}

ConstantExp::ConstantExp(int value) { this->value = value; }

int ConstantExp::eval(EvalState &state) { return value; }

std::string ConstantExp::toString() { return integerToString(value); }

ExpressionType ConstantExp::getType() { return CONSTANT; }

int ConstantExp::getValue() { return value; }

IdentifierExp::IdentifierExp(std::string name) { this->name = name; }

int IdentifierExp::eval(EvalState &state) {
    if (!state.isDefined(name)) error("VARIABLE NOT DEFINED");
    return state.getValue(name);
}

std::string IdentifierExp::toString() { return name; }

ExpressionType IdentifierExp::getType() { return IDENTIFIER; }

std::string IdentifierExp::getName() { return name; }

CompoundExp::CompoundExp(std::string op, Expression *lhs, Expression *rhs) {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

int CompoundExp::eval(EvalState &state) {
    if (op == "=") {
        if (lhs->getType() != IDENTIFIER) {
            error("Illegal variable in assignment");
        }
        if (lhs->getType() == IDENTIFIER && lhs->toString() == "LET")
            error("SYNTAX ERROR");
        int val = rhs->eval(state);
        state.setValue(((IdentifierExp *) lhs)->getName(), val);
        return val;
    }
    int left = lhs->eval(state);
    int right = rhs->eval(state);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) error("DIVIDE BY ZERO");
        return left / right;
    }
    return 0;
}

std::string CompoundExp::toString() {
    return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() { return COMPOUND; }

std::string CompoundExp::getOp() { return op; }

Expression *CompoundExp::getLHS() { return lhs; }

Expression *CompoundExp::getRHS() { return rhs; }
