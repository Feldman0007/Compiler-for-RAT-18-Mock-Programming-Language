#include "Parser.h"
#include <iostream>
#include <fstream>
using namespace std;


Parser::Parser(TokenTable table) {
     tokens = table;
}


bool Parser::parseCode() {
     int inputCursor;
     string inputString;
    
     while (tokens.getCursor() < tokens.tableSize()) {
          inputCursor = 0;
          inputString = "";
          if (tokens.nextToken().tokenType == "identifier") {
               inputString += 'i';
               tokens.incCursor();
               while (tokens.nextToken().lexeme != ";") {
                    if (tokens.nextToken().tokenType == "identifier")
                         inputString += 'i';
                    else
                         inputString += tokens.nextToken().lexeme;
                    if (tokens.getCursor() >= tokens.tableSize())
                         break;
			     tokens.incCursor();
               }
               if (tokens.nextToken().lexeme != ";")
                    return false;
               else {
                    inputString += tokens.nextToken().lexeme;
                    tokens.incCursor();
               }
          }
          else {
               cout << "We haven't implemented this portion of the compiler yet.\n";
               return false;
          }
     
          int scenario = 0;
          inputString += '$';
          predictiveStack.push('$');
          predictiveStack.push(startingNonTerminal);
	     //printParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
          //writeParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");

          while (true) {
               if (isTerminal(predictiveStack.top())) {
                    if (predictiveStack.top() == inputString[inputCursor]) {
                         if (predictiveStack.top() == '$') {
                              scenario = 5;
                              printParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                              //writeParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                              break;
                         }
                         inputCursor++;
                         //This is only to deal with spaces, will remove when we link lexer to parser
                         while (inputString[inputCursor] == ' ' && inputCursor < inputString.size())
                              inputCursor++;
                         predictiveStack.pop();
                         scenario = 1;
                         printParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                         //writeParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                    }
                    else {
                         scenario = 4;
                         printParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                         //writeParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                         system("PAUSE");
                         return false; //Invalid input
                    }
               }
               else { //Its a nonTerminal or epsilon
                    char poppedSymbol = predictiveStack.top(); // save the nonterminal for expansion
                    predictiveStack.pop(); //Now get remove so it can be replaced by expansion
                    string production = prodRuleTable[getRowIndex(poppedSymbol)][getColIndex(inputString[inputCursor])];
                    if (production == "ε") {
                         scenario = 2;
                         printParserStatus(inputString[inputCursor], predictiveStack.top(), poppedSymbol, scenario, "");
                         //writeParserStatus(inputString[inputCursor], predictiveStack.top(), scenario, "");
                         continue;
                    }
                    else if (production != "") { //If valid production rule used
                         for (int i = production.size() - 1; i >= 0; i--) {
                              predictiveStack.push(production[i]); //Push production rule onto stack in reverse order
                         }
                         scenario = 3;
                         printParserStatus(inputString[inputCursor], predictiveStack.top(), poppedSymbol, scenario, production);
                         //writeParserStatus(inputString[inputCursor], predictiveStack.top(), poppedSymbol, scenario, production);
                    }
                    else {   //Invalid syntax encountered
                         scenario = 4;
                         printParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                         //writeParserStatus(inputString[inputCursor], predictiveStack.top(), ' ', scenario, "");
                         return false; //Input not accepted by grammar
                    }
               }
          }
     }
}

bool Parser::isTerminal(char top) {
     for (int i = 0; i < 10; i++) {
          if (terminals[i] == top) { //belongs to set of terminals?
               return true; //Is a terminal
          }
     }
     return false; //Is not a terminal
}

bool Parser::isNonTerminal(char top) {
     for (int i = 0; i < 6; i++) {
          if (nonTerminals[i] == top) { //belongs to set of non-terminals?
               return true; //Is a non-terminal
          }
     }
     return false; //Is not a non-terminal
}

int Parser::getRowIndex(char nt) {
     for (int i = 0; i < 6; i++) {
          if (nonTerminals[i] == nt) {
               return i;
          }
     }
}

int Parser::getColIndex(char t) {
     for (int i = 0; i < 10; i++) {
          if (terminals[i] == t) {
               return i;
          }
     }
}

void Parser::writeParserStatus(char currentCharacter, char topOfStack, char popped, int scenario, string production) {
     ofstream fileHandler;
     fileHandler.open("parserOutput.txt", ios::app);
     if (fileHandler.fail())
          cout << "Filehandler failed." << endl;

     switch (scenario) {
     case 0:
          fileHandler << "Initializing stack and input cursor.\n";
          break;
     case 1:
          fileHandler << "\nTop of stack matched to current input. Popping stack and incrementing input cursor.\n";
          break;
     case 2:
          fileHandler << "\nEpsilon encountered. Ignore non-terminal \'" << popped << "\' and pop from stack.\n";
          break;
     case 3:
          fileHandler << "\nExpanding non-terminal \'" << popped << "\' to production rule \'" << production << "\'\n";
          break;
     case 5:
          fileHandler << "\nInput has been accepted.\n";
          break;
     default:
          fileHandler << "\nInvalid syntax\n";
     }
     if (scenario < 4) {
          fileHandler << "Current character in input string: \'" << currentCharacter << "\'\t";
          fileHandler << "Current symbol at the top of the stack: \'" << topOfStack << "\'\n";
     }

     fileHandler.close();
}

void Parser::printParserStatus(char currentCharacter, char topOfStack, char popped, int scenario, string production) {
     switch (scenario) {
     case 0:
          cout << "Initializing stack and input cursor.\n";
          break;
     case 1:
          cout << "\nTop of stack matched to current input. Popping stack and incrementing input cursor.\n";
          break;
     case 2:
		cout << "\nEpsilon encountered. Ignore non-terminal \'" << popped << "\' and pop from stack.\n";
          break;
     case 3:
		cout << "\nExpanding non-terminal \'" << popped << "\' to production rule \'" << production << "\'\n";
          break;
     case 5:
          cout << "\nInput has been accepted.\n";
          break;
     default:
          cout << "\nInvalid syntax\n";
     }

     if (scenario < 4) {
		cout << "Current character in input string: \'" << currentCharacter << "\'\t";
		cout << "Current symbol at the top of the stack: \'" << topOfStack << "\'\n";
     }
}
