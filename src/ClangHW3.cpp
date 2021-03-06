#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Lex/Lexer.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "HW3Util.h"
#include <iostream>

#include <stack>
#include <string>

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory FindFunctionCategory("");


class FunctionVisitor : public RecursiveASTVisitor<FunctionVisitor> {
public:
  explicit FunctionVisitor(ASTContext *_context, CompilerInstance &_compiler)
      : context(_context), compiler(_compiler) {}

  int getLineNumber(Stmt *stmt) {
    FullSourceLoc fullSourceLoc = context->getFullLoc(stmt->getBeginLoc());
    int ln;
    if(!fullSourceLoc.isValid())
      ln = -1;
    else
      ln = fullSourceLoc.getSpellingLineNumber();
    return ln;
  }

  std::string getSource(Stmt *node) {
    bool invalid;
    CharSourceRange range =
        CharSourceRange::getTokenRange(node->getBeginLoc(), node->getEndLoc());
    std::string tokens(Lexer::getSourceText(range, compiler.getSourceManager(),
                                            compiler.getLangOpts(), &invalid));
    if(!invalid)
      return tokens;
    return "";
  }

  void analyzeVariableRedefinition(Stmt *functionBody) {
    /* 
     * TODO: analyze whether there is a variable redefinition.
     * If you find a variable "k" being redefined at line n, 
     *    which was initially defined at line m, you have to call
     *    printVariableRedeclarationInformation(k, n, m)
     */


  }

  bool VisitFunctionDecl(FunctionDecl *function) {
    std::string functionName = function->getNameInfo().getName().getAsString();
    if(!function->hasBody()) {
      // If the function does not have any body, we will not analyze it.
      return false;
    }
    Stmt *body = function->getBody();

    // Variable redefinition function (your task)
    analyzeVariableRedefinition(body);

    return true;
  }

private:
  ASTContext *context;
  CompilerInstance &compiler;
};

class FunctionVisitorConsumer : public clang::ASTConsumer {
public:
  explicit FunctionVisitorConsumer(ASTContext *context,
                                   CompilerInstance &compiler)
      : visitor(context, compiler) {}

  virtual void HandleTranslationUnit(clang::ASTContext &context) {
    visitor.TraverseDecl(context.getTranslationUnitDecl());
  }

private:
  FunctionVisitor visitor;
};

class FunctionVisitAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile) {
    return std::unique_ptr<clang::ASTConsumer>(
        new FunctionVisitorConsumer(&compiler.getASTContext(), compiler));
  }
};

int main(int argc, const char **argv) {
  CommonOptionsParser optionsParser(argc, argv, FindFunctionCategory);
  ClangTool tool(optionsParser.getCompilations(),
                 optionsParser.getSourcePathList());
  return tool.run(newFrontendActionFactory<FunctionVisitAction>().get());
}
