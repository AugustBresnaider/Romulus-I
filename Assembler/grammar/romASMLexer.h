
// Generated from ../romASMLexer.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"


namespace romASM {


class  romASMLexer : public antlr4::Lexer {
public:
  enum {
    MOV = 1, STR = 2, LDA = 3, OR = 4, INV = 5, AND = 6, XOR = 7, ADD = 8, 
    SUB = 9, NOT = 10, SHR = 11, SHL = 12, PUSH = 13, CALL = 14, POP = 15, 
    RET = 16, JMP = 17, JEZ = 18, JNZ = 19, JGZ = 20, JLZ = 21, NOP = 22, 
    COMMA = 23, REG = 24, CHAR = 25, IMM = 26, HEX = 27, LABELID = 28, LABEL = 29, 
    COMMENT = 30, WS = 31
  };

  explicit romASMLexer(antlr4::CharStream *input);

  ~romASMLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

}  // namespace romASM
