
// Generated from ../romASMParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "romASMParserListener.h"


namespace romASM {

/**
 * This class provides an empty implementation of romASMParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  romASMParserBaseListener : public romASMParserListener {
public:

  virtual void enterProgram(romASMParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(romASMParser::ProgramContext * /*ctx*/) override { }

  virtual void enterExpr(romASMParser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(romASMParser::ExprContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace romASM
