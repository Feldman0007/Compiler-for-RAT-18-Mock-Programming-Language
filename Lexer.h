#pragma once

#include <string>
#include <vector>
#include <list>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

const short NUM_OPS = 8;
const short NUM_SEPS = 13;
const short NUM_KEYWORDS = 15;

const char OPERATORS[NUM_OPS] = { '*', '+', '-', '=', '/', '>', '<', '%'};

const char SEPARATORS[NUM_SEPS] = {'/', '(', ')', '{', '}', '[', ']', ',', '.', ':', ';', '!',' ' };

const string KEYWORDS[NUM_KEYWORDS] = { "int", "float", "bool", "if", "else", "then", "do", "while", 
                                        "whileend", "do", "doend", "for", "and", "or" , "function" };

struct Token {
     string tokenType;
     string lexeme;
     Token() : tokenType(""), lexeme("") {};
     Token(string t, string l) : tokenType(t), lexeme(l) {};
     Token(const Token& token) {
          tokenType = token.tokenType;
          lexeme = token.lexeme;
     }

};

class TokenTable {
private:
     vector<Token> entries;
     int tableCursor;
public:
     Token nextToken();
     void addEntry(Token);
     void printTable();
     void writeTable();
	int getCursor();
     void incCursor();
	int tableSize();
};

class Lexer {
private:
     vector<string> sourceCode;
public:
     Lexer() : sourceCode() {};
     Token nextToken(); //Parses string to find the next available token
     bool getSourceCode(string); //Opens and reads file contents into a vector of strings
     bool isKeyword(string);
     bool isSeparator(char);
     bool isIdentifier(string, string&);
     bool isOperator(char);
     bool isReal(string, string&);
	 bool isInteger(string, string&);
};
