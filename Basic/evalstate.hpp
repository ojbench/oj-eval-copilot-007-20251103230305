/*
 * File: evalstate.h
 * -----------------
 * This interface exports a class called EvalState, which keeps track
 * of additional information required by the evaluator, most notably
 * the values of variables.
 */

#ifndef _evalstate_h
#define _evalstate_h

#include <string>
#include <map>

class EvalState {
public:
    EvalState();
    ~EvalState();

    void setValue(std::string var, int value);
    int getValue(std::string var);
    bool isDefined(std::string var);
    void Clear();

    // control flow helpers for RUN
    void setNextLine(int line);    // set jump target
    bool hasNextLine() const;
    int getNextLine() const;
    void clearControl();
    void setEnd();
    bool isEnd() const;

private:
    std::map<std::string, int> symbolTable;
    bool hasJump = false;
    int nextLine = -1;
    bool ended = false;
};

#endif
