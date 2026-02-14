#include "romASMParserBaseListener.h"
#include "romASMParser.h"
#include "antlr4-runtime.h"

#include "utils.h"

#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace romASM;
using namespace antlr4;

class  Listener : public romASMParserBaseListener {
public:
	void enterProgram(romASMParser::ProgramContext * ctx);
	void exitProgram(romASMParser::ProgramContext * ctx);
  
	void enterExpr(romASMParser::ExprContext * ctx);
	void exitExpr(romASMParser::ExprContext * ctx){}


	void enterEveryRule(antlr4::ParserRuleContext * ctx){}
	void exitEveryRule(antlr4::ParserRuleContext * ctx){}
	void visitTerminal(antlr4::tree::TerminalNode * node){}
	void visitErrorNode(antlr4::tree::ErrorNode * node){}
  
	Listener(std::string outfname)
	{
		outfile.open(outfname);
	}
	~Listener() = default;
	
private:

	void handleEscapeSeq(std::string& str);

	int curAddr = 0;
	bool firstPass = true;
	
	std::unordered_map<std::string, int> labelMap;
	
	std::ofstream outfile;
	int parseReg(antlr4::tree::TerminalNode* node);
	
	char toHex(int x);
	
	void output(int op, int rA, int rB, int rY, std::string comment = "");
	void output(int x, std::string comment = "");
	
	int fromHex(std::string str);
	
	std::string toLower(std::string str);
  
};