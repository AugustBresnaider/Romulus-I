
// Generated from ../romASMParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"


namespace romASM {


class  romASMParser : public antlr4::Parser {
public:
  enum {
    MOV = 1, STR = 2, LDA = 3, OR = 4, INV = 5, AND = 6, XOR = 7, ADD = 8, 
    SUB = 9, NOT = 10, SHR = 11, SHL = 12, PUSH = 13, CALL = 14, POP = 15, 
    RET = 16, JMP = 17, JEZ = 18, JNZ = 19, JGZ = 20, JLZ = 21, NOP = 22, 
    COMMA = 23, REG = 24, CHAR = 25, IMM = 26, HEX = 27, LABELID = 28, LABEL = 29, 
    COMMENT = 30, WS = 31
  };

  enum {
    RuleProgram = 0, RuleExpr = 1
  };

  explicit romASMParser(antlr4::TokenStream *input);

  romASMParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~romASMParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ProgramContext;
  class ExprContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ProgramContext* program();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MOV();
    std::vector<antlr4::tree::TerminalNode *> REG();
    antlr4::tree::TerminalNode* REG(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *LABELID();
    antlr4::tree::TerminalNode *IMM();
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *HEX();
    antlr4::tree::TerminalNode *STR();
    antlr4::tree::TerminalNode *LDA();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *INV();
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *XOR();
    antlr4::tree::TerminalNode *ADD();
    antlr4::tree::TerminalNode *SUB();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *SHR();
    antlr4::tree::TerminalNode *SHL();
    antlr4::tree::TerminalNode *PUSH();
    antlr4::tree::TerminalNode *CALL();
    antlr4::tree::TerminalNode *POP();
    antlr4::tree::TerminalNode *RET();
    antlr4::tree::TerminalNode *JMP();
    antlr4::tree::TerminalNode *JEZ();
    antlr4::tree::TerminalNode *JNZ();
    antlr4::tree::TerminalNode *JGZ();
    antlr4::tree::TerminalNode *JLZ();
    antlr4::tree::TerminalNode *COMMENT();
    antlr4::tree::TerminalNode *LABEL();
    antlr4::tree::TerminalNode *NOP();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExprContext* expr();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

}  // namespace romASM
