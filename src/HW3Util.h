#include <string>

#ifndef LLVM_PRINTING_H
#define LLVM_PRINTING_H

void printCallExprReformatOutput(int lineNo, const std::string& originalSource,
                                 const std::string& formattedSource);

#endif
