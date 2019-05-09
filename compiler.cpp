#include "Lexer.h"
#include "Parser.h"
int main() { 
     Lexer lex;
     if (lex.getSourceCode("test1assignmentOperator.txt")){
          TokenTable table;
          Token t;
          while (true) {
               t = lex.nextToken();
               if (t.tokenType == "endOfCode")
                    break;
               table.addEntry(t);
               
          }
          table.printTable();
          system("PAUSE");
          //table.writeTable();
          Parser parser(table);
          if (parser.parseCode()) {
               cout << "\nAccepted\n";
          }
          else
          {
               cout << "\nNot Accepted\n";
          }
          system("PAUSE");
          //table.addEntry(t);
     }
     return 0;    
}
