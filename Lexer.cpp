#include "Lexer.h"
#include "DFSM.h"
#include <iomanip>

bool Lexer::getSourceCode(string fileName) {
     ifstream fileHandler(fileName);
     if (fileHandler.fail())
          return false;
     
     char character;
     string temp;
     while (fileHandler.get(character)) {
          if (character == '!') {
               while (fileHandler.get(character)) {
                    if (character == '!')
                         break;
               }
               //if (character != '!') Missing a closing !
          }
          else if (character == ' ' || character == '\n') {
               if (temp.size() == 0)
                    continue;
               sourceCode.push_back(temp);
               temp = "";
          }
          else
               temp += character;
     }

     //If we've reached the end of the file and temp 
     //still has characters in it add it to sourceCode vector
     if (temp.size() != 0) {
          sourceCode.push_back(temp);
     }
     fileHandler.close();
     return true;
}


//Get the next token in the string leaving off from current index
Token Lexer::nextToken() {
     if (sourceCode.empty()) {
          return Token("endOfCode", "");
     }
     string codeChunk = sourceCode.front(); //Chunk of code to analyze for token(s)

     //Buffer will be a reference string that is passed to the helper functions/FSMs.
     //If the helper functions return true, the contents of the buffer (if any)
     //is a valid token
     string buffer = "";

     //If a possible identifier lies within the chunk of source code, 
     //the identifier DFSM will identify it and mark it and store it in the buffer.
     if (isIdentifier(codeChunk, buffer)) {
          //Get the amount of characters that were used/validated by the FSM
          string processedSubstring;  //Stores what was processed by the FSM
          bool processedWholeChunk = false; //Determines if the identifier/keyword is/is not the length of the whole code chunk

          if (codeChunk.size() != buffer.size()) //If it is not the length of the code chunk
               processedSubstring = codeChunk.substr(0, buffer.size()); //Extract the part that is the identifier/keyword
          else { //Otherwise, the whole code chunk is an identifier/keyword
               processedSubstring = codeChunk;
               processedWholeChunk = true;
          }

          //Before we continue, we must remove the part of the code chunk that has already been processed
          if (!processedWholeChunk) {
               sourceCode[0] = codeChunk.substr(buffer.size(), codeChunk.size());
          }
          else //The entire chunk has been processed, so remove the chunk
               sourceCode.erase(sourceCode.begin());

          //Now we can return the token

          //If it's a valid identifier, it could be a valid keyword as well. So check...
          if (isKeyword(processedSubstring))
               return Token("keyword", processedSubstring);
          else
               return Token("identifier", processedSubstring);

     }
     
     else if (isOperator(codeChunk[0])) {
          //Store the operator lexeme
          string opLexeme = "";
          opLexeme += codeChunk[0];
          
          //Update the code chunk
          if (codeChunk.size() == 1) {
               sourceCode.erase(sourceCode.begin());
          }
          else {
               sourceCode[0] = codeChunk.substr(1, codeChunk.size());
          }
          
          //Return the token
          return Token("operator", opLexeme);
     }

     else if (isSeparator(codeChunk[0])) {
          //Store the separator lexeme
          string sepLexeme;
          sepLexeme += codeChunk[0];
          
          //Update the code chunk
          if (codeChunk.size() == 1) {
               sourceCode.erase(sourceCode.begin());
          }
          else {
               sourceCode[0] = codeChunk.substr(1, codeChunk.size());
          }

          //Return the token
          return Token("separator", sepLexeme);
     }

     else if (isReal(codeChunk, buffer)) {
          //Get the amount of characters that were used/validated by the FSM
          string processedSubstring;  //Stores what was processed by the FSM
          bool processedWholeChunk = false; //Determines if the identifier/keyword is/is not the length of the whole code chunk

          if (codeChunk.size() != buffer.size()) //If it is not the length of the code chunk
               processedSubstring = codeChunk.substr(0, buffer.size()); //Extract the part that is the identifier/keyword
          else { //Otherwise, the whole code chunk is an real
               processedSubstring = codeChunk;
               processedWholeChunk = true;
          }

          //Before we continue, we must remove the part of the code chunk that has already been processed
          if (!processedWholeChunk) {
               sourceCode[0] = codeChunk.substr(buffer.size(), codeChunk.size());
          }
          else //The entire chunk has been processed, so remove the chunk
               sourceCode.erase(sourceCode.begin());

          //Now we can return the token
          return Token("real", processedSubstring);

     }


     cout << "\n\nCompiler error caused in code chunk: \n" << codeChunk;
     system("PAUSE");
     return Token();
}

bool Lexer::isKeyword(string input) {
     for (int i = 0; i < NUM_KEYWORDS; i++) {
          if (input == KEYWORDS[i])
               return true;
     }
     return false;
}

bool Lexer::isIdentifier(string input, string &buffer) {
     IdentifierDFSM fsm;
     return fsm.processInput(input, buffer);
}

bool Lexer::isReal(string input, string &buffer) {
     RealDFSM fsm;
     return fsm.processInput(input, buffer);
}

bool Lexer::isOperator(char input) {
     for (int i = 0; i < NUM_OPS; i++) {
          if (input == OPERATORS[i])
               return true;
     }
     return false;
}

bool Lexer::isSeparator(char input) {
     for (int i = 0; i < NUM_SEPS; i++) {
          if (input == SEPARATORS[i])
               return true;
     }
     return false;
}



//-----------------------------------------------------------------------------------------------------------------------------

void SymbolTable::addEntry(Token t) {
     entries.push_back(t);
}

void SymbolTable::printTable() {
     if (entries.empty()) {
          cout << "\nNo entries\n";
          return;
     }
     cout << "    Token     |     Lexeme     |\n";
     cout << "--------------------------------\n";
     for (list<Token>::iterator p = entries.begin(); p != entries.end(); p++) {
          cout << left << "  " << setw(12) << p->tokenType << "|  " << setw(14) << p->lexeme << "|  " << endl;// << p->startingIndex << endl;
     }
}
