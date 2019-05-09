#pragma once
#include <string>
#include <stack>
#include "LexerV.h"
using namespace std;

/*
G = (N, T, S, R) :
     N = { E, A, T, B, F, X, Y }
     T = { +, -, *, / , (,), i }
     S = E
     R = {
     	E->TX
          X->A | ε
          A -> + TA | -TA | ε
          T->FY
     	Y->B | ε
     	B -> *FB | /FB | ε
     	F -> (E) | i
     }
*/

class Parser {
private:
     //Set of terminals and non-terminals
     const char nonTerminals[6] = { 'S', 'E', 'Q', 'T', 'R', 'F' };
     const char terminals[10] = { 'i', '=', '+', '-', '*', '/', '(', ')', ';', '$' };

     const char startingNonTerminal = 'S';
     TokenTable tokens;
     //Contains production rules (left factorized and w/o left recursion)
     const string prodRuleTable[6][10] = { /*i        =        +        -        *        /        (        )        ;        $*/
                                   /*S*/  { "i=E;",   "",      "",      "",      "",     "",      "",      "",       "",      ""},
                                   /*E*/  { "TQ",     "",      "",      "",      "",     "",      "TQ",    "",       "",      ""},
                                   /*Q*/  { "",       "",      "+TQ",   "-TQ",   "",     "",      "",      "ε",      "ε",     ""},
                                   /*T*/  { "FR",     "",      "",      "",      "",     "",      "FR",    "",       "",      ""},
                                   /*R*/  { "",       "",      "ε",     "ε",     "*FR",  "/FR",   "",      "ε",      "ε",     ""},
                                   /*F*/  { "i",      "",      "",       "",     "",     "",      "(E)",   "",       "",      ""}
                                        };
                                         
     stack<char> predictiveStack; //Confirm if input is accepted by grammar
    

public:
     Parser(TokenTable);
     bool parseCode();
     bool assignmentStatement();
     bool isTerminal(char);
     bool isNonTerminal(char);
     int  getRowIndex(char);
     int getColIndex(char);
     void writeParserStatus(char, char, char, int, string);
     void printParserStatus(char, char, char, int, string);
};
