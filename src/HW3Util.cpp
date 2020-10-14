#include <string>
#include <iostream>

#include "HW3Util.h"

void printCallExprReformatOutput(int lineNo, const std::string& originalSource,
                                 const std::string& formattedSource){
  std::cout << "==================================================\n";
  std::cout << "Function call found at line " << lineNo << "\n";
  std::cout << "Original Source\n";
  std::cout << "---------------------------------------------------\n";
  std::cout << originalSource << "\n";
  std::cout << "---------------------------------------------------\n";
  std::cout << "Formatted Source\n";
  std::cout << "---------------------------------------------------\n";
  std::cout << formattedSource << "\n";
  std::cout << "==================================================\n";
}
