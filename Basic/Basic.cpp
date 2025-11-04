/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"

static void prepareScanner(TokenScanner &sc, const std::string &input) {
    sc.ignoreWhitespace();
    sc.scanNumbers();
    sc.setInput(input);
    sc.addOperator("<=");
    sc.addOperator(">=");
    sc.addOperator("<>");
}

// forward decl
void processLine(std::string line, Program &program, EvalState &state);

static Statement *parseStatementFrom(TokenScanner &scanner, const std::string &keyword) {
    std::string kw = toUpperCase(keyword);
    if (kw == "REM") return new RemStatement(scanner);
    if (kw == "LET") return new LetStatement(scanner);
    if (kw == "PRINT") return new PrintStatement(scanner);
    if (kw == "INPUT") return new InputStatement(scanner);
    if (kw == "END") return new EndStatement();
    if (kw == "GOTO") return new GotoStatement(scanner);
    if (kw == "IF") return new IfStatement(scanner);
    error("SYNTAX ERROR");
    return nullptr;
}

static void runProgram(Program &program, EvalState &state) {
    int line = program.getFirstLineNumber();
    while (line != -1) {
        Statement *stmt = program.getParsedStatement(line);
        if (!stmt) error("SYNTAX ERROR");
        state.clearControl();
        stmt->execute(state, program);
        if (state.isEnd()) break;
        if (state.hasNextLine()) {
            line = state.getNextLine();
        } else {
            line = program.getNextLineNumber(line);
        }
    }
}

int main() {
    EvalState state;
    Program program;
    while (true) {
        try {
            std::string input;
            if (!std::getline(std::cin, input)) break;
            if (input.empty()) continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    prepareScanner(scanner, line);

    std::string first = scanner.nextToken();
    if (first == "") return;
    TokenType t = scanner.getTokenType(first);

    if (t == NUMBER) {
        int lineNumber = stringToInteger(first);
        std::string rest = line.substr(scanner.getPosition());
        if (!scanner.hasMoreTokens()) {
            program.removeSourceLine(lineNumber);
            return;
        }
        program.addSourceLine(lineNumber, line);
        TokenScanner s2;
        prepareScanner(s2, rest);
        std::string keyword = s2.nextToken();
        Statement *stmt = parseStatementFrom(s2, keyword);
        program.setParsedStatement(lineNumber, stmt);
        return;
    }

    std::string cmd = toUpperCase(first);
    if (cmd == "QUIT") {
        std::exit(0);
    } else if (cmd == "HELP") {
        return;
    } else if (cmd == "LIST") {
        for (int ln = program.getFirstLineNumber(); ln != -1; ln = program.getNextLineNumber(ln)) {
            std::cout << program.getSourceLine(ln) << std::endl;
        }
        return;
    } else if (cmd == "CLEAR") {
        program.clear();
        state.Clear();
        return;
    } else if (cmd == "RUN") {
        runProgram(program, state);
        return;
    } else if (cmd == "REM" || cmd == "LET" || cmd == "PRINT" || cmd == "INPUT" || cmd == "END" || cmd == "GOTO" || cmd == "IF") {
        std::string rest = line.substr(scanner.getPosition());
        TokenScanner s2;
        prepareScanner(s2, rest);
        Statement *stmt = parseStatementFrom(s2, first);
        Program dummy;
        stmt->execute(state, dummy);
        delete stmt;
        return;
    }

    error("SYNTAX ERROR");
}

