
// Generated from ../romASMParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "romASMParser.h"


namespace romASM {

/**
 * This interface defines an abstract listener for a parse tree produced by romASMParser.
 */
class  romASMParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(romASMParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(romASMParser::ProgramContext *ctx) = 0;

  virtual void enterExpr(romASMParser::ExprContext *ctx) = 0;
  virtual void exitExpr(romASMParser::ExprContext *ctx) = 0;


};

}  // namespace romASM
