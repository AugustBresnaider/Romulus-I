#include "Listener.h"

#include <iostream>
#include <vector>
#include <string>
#include <regex>

	char Listener::toHex(int x)
	{
		x %= 16;
		if(x <= 9)
		{
			return x + '0';
		}
		else{
			return x + 'A' - 10;
		}
	}

	void Listener::enterProgram(romASMParser::ProgramContext * ctx)
	{
	}

	void Listener::exitProgram(romASMParser::ProgramContext * ctx)
	{
		if(firstPass)
		{
			firstPass = false;
		}
		else
		{
			if (assemble_flags & TERMINATORS)
			{
				outfile << "*\n";
			}
		}
	}
	
	std::string Listener::toLower(std::string str)
	{
		std::string fin = "";
		for(int i = 0; i < str.size(); i++)
		{
			if(str[i] >= 'A' && str[i] <= 'Z')
			{
				fin += (str[i] + 'a' - 'A'); // 32? idk
			}
			else
			{
				fin += str[i];
			}
		}
		return fin;
	}

	int Listener::fromHex(std::string str)
	{
		int res = 0;
		str = toLower(str);

		for(int i = 0; i < str.size(); i++)
		{
			res <<= 4;
			if(str[i] <= '9' && str[i] >= '0')
			{
				res |= str[i] - '0';
			}
			else if(str[i] >= 'a' && str[i] <= 'f')
			{
				res |= str[i] - 'a' + 10;
			}
			else
			{
				std::cout << "ERR: Invalid hex literal";
			}	
			
			
		}
		return res;
	}

	void Listener::output(int op, int rA, int rB, int rY, std::string comment)
	{
		outfile << toHex(op) << toHex(rA) << toHex(rB) << toHex(rY);

		if (assemble_flags & TERMINATORS) outfile << ';';

		if(comment.size() > 0 && (assemble_flags & COMMENTS))
		{
			outfile << " // " << comment;
		}
		outfile << '\n';
	}	
	
	void Listener::output(int x, std::string comment)
	{
		outfile << toHex((x >> 12) & 0xf);
		outfile << toHex((x >> 8) & 0xf);
		outfile << toHex((x >> 4) & 0xf);
		outfile << toHex((x >> 0) & 0xf);

		if (assemble_flags & TERMINATORS) outfile << ';';

		if(comment.size() > 0 && (assemble_flags & COMMENTS))
		{
			outfile << " // " << comment;
		}
		outfile << '\n';	
	}

	void Listener::enterExpr(romASMParser::ExprContext * ctx)
	{
		auto regs = ctx->REG();
		int op = 0;
		int rA = 0;
		int rB = 0;
		int rY = 0;
		if(firstPass)
		{
			if(ctx->LABEL())
			{
				std::string label =  ctx->LABEL()->getText();
				std::string labelID = label.substr(0, label.size() - 1);
				if (labelMap.find(labelID) != labelMap.end())
				{
					std::cout << "Err: Label " << labelID << " already used. Ignoring all repeat occurrences.\n";
				}
				else
				{
					labelMap.insert({ labelID, curAddr });
				}
			}
			else if (
				ctx->STR() ||
				ctx->LDA() ||
				ctx->OR() ||
				ctx->INV() ||
				ctx->AND() ||
				ctx->XOR() ||
				ctx->ADD() ||
				ctx->SUB() ||
				ctx->NOT() ||
				ctx->SHR() ||
				ctx->SHL() ||
				ctx->PUSH() ||
				ctx->POP() ||
				ctx->RET() ||
				ctx->JMP() ||
				ctx->JEZ() ||
				ctx->JNZ() ||
				ctx->JGZ() ||
				ctx->JLZ())
			{
				curAddr++;
			}
			else if (ctx->MOV())
			{
				if (regs.size() == 2)
				{
					curAddr++;
				}
				else
				{
					curAddr += 2;
				}
			}
			else if (ctx->CALL())
			{
				curAddr += 2;
			}
		}
		else
		{
			
			if(ctx->MOV())
			{
				std::string s;
				int imm = 0;
				if(regs.size() == 2)
				{
					op = 0;
					rA = parseReg(regs[0]);
					rY = parseReg(regs[1]);
					s = std::format("Move the value stored in r{} to r{}", rA, rY);
				}
				else if(ctx->IMM() || ctx->HEX() || ctx->CHAR())
				{
					op = 1;
					rY = parseReg(regs[0]);
					if(ctx->IMM())
					{
						std::string str = (ctx->IMM())->getText();
						imm = atoi(str.c_str());
					}
					else if(ctx->HEX())
					{
						std::string str = (ctx->HEX())->getText();
						str = str.substr(2, str.size() - 2);
						imm = fromHex(str.c_str());
					}
					else if (ctx->CHAR())
					{
						std::string str = (ctx->CHAR())->getText();
						str = str.substr(1, str.size() - 2);
						if (str.size() > 1)
						{
							handleEscapeSeq(str);
						}
						imm = str[0];
					}
					s = std::format("Move immediate ({}) into r{}", imm, rY);
					
				}
				else if (ctx->LABELID())
				{
					op = 1;
					rY = parseReg(regs[0]);
					std::string labelID = ctx->LABELID()->getText();
					if (labelMap.find(labelID) == labelMap.end())
					{
						std::cout << "Error: Label " << labelID << " not found. Exiting...";
						std::exit(-1);
					}
					else
					{
						imm = labelMap[labelID];
					}
					s = std::format("Move label ({}:{}) into r{}", labelID, imm, rY);
				}
				output(op, rA, rB, rY, s);
				if(op == 1)
				{
					output(imm, std::format("Immediate value ({})", imm));
				}
			}
			else if(ctx->STR())
			{
				op = 2;
				rA = parseReg(regs[1]);
				rB = parseReg(regs[0]);
				output(op, rA, rB, rY, std::format("Store the value in r{} to the memory address stored in r{}", rA, rB));
			}
			else if(ctx->LDA())
			{
				op = 3;
				rA = parseReg(regs[0]);
				rY = parseReg(regs[1]);
				output(op, rA, rB, rY, std::format("Load value from memory address stored in r{} to r{}", rA, rY));
			}
			else if(ctx->OR())
			{
				op = 4;
				rA = parseReg(regs[0]);
				rB = parseReg(regs[1]);
				if(regs.size() > 2)
				{
					rY = parseReg(regs[2]);
				}
				else
				{
					rY = rB;
				}
				output(op, rA, rB, rY, std::format("Perform OR on r{} and r{}, store in r{}", rA, rB, rY));
			}
			else if(ctx->INV())
			{
				op = 5;
				rA = parseReg(regs[0]);
				if(regs.size() > 1)
				{
					rY = parseReg(regs[1]);
				}
				else
				{
					rY = rA; 
				}
				output(op, rA, rB, rY, std::format("Perform bitwise NOT on r{}, store in r{}", rA, rY));
			}
			else if(ctx->AND())
			{
				op = 6;
				rA = parseReg(regs[0]);
				rB = parseReg(regs[1]);
				if(regs.size() > 2)
				{
					rY = parseReg(regs[2]);
				}
				else
				{
					rY = rB;
				}
				output(op, rA, rB, rY, std::format("Perform AND on r{} and r{}, store in r{}", rA,rB,rY));
			}
			else if(ctx->XOR())
			{
				op = 7;
				rA = parseReg(regs[0]);
				rB = parseReg(regs[1]);
				if(regs.size() > 2)
				{
					rY = parseReg(regs[2]);
				}
				else
				{
					rY = rB;
				}
				output(op, rA, rB, rY, std::format("Perform XOR on r{} and r{}, store in r{}", rA, rB, rY));
			}
			else if(ctx->ADD())
			{
				op = 8;
				rA = parseReg(regs[0]);
				rB = parseReg(regs[1]);
				if(regs.size() > 2)
				{
					rY = parseReg(regs[2]);
				}
				else
				{
					rY = rB;
				}
				output(op, rA, rB, rY, std::format("Add r{} and r{}, store in r{}", rA, rB, rY));
			}
			else if(ctx->SUB())
			{
				op = 9;
				rA = parseReg(regs[0]);
				rB = parseReg(regs[1]);
				if(regs.size() > 2)
				{
					rY = parseReg(regs[2]);
				}
				else
				{
					rY = rB;
				}
				output(op, rA, rB, rY, std::format("Subtract r{} from r{}, store in r{}", rA, rB, rY));
			}
			else if(ctx->NOT())
			{
				op = 10;
				rA = parseReg(regs[0]);
				if(regs.size() > 1)
				{
					rY = parseReg(regs[1]);
				}
				else
				{
					rY = rA; 
				}
				output(op, rA, rB, rY, std::format("Perform logical NOT on r{}, store in r{}", rA, rY));
			}
			else if(ctx->SHR())
			{
				op = 11;
				rA = parseReg(regs[0]);
				if(regs.size() > 1)
				{
					rY = parseReg(regs[1]);
				}
				else
				{
					rY = rA; 
				}
				output(op, rA, rB, rY, std::format("Perform right shift on r{}, store in r{}", rA, rY));
			}
			else if(ctx->SHL()) // replicate add instruction
			{
				op = 8;
				rA = parseReg(regs[0]);
				rB = rA;
				if(regs.size() > 1)
				{
					rY = parseReg(regs[1]);
				}
				else
				{
					rY = rA; 
				}
				output(op, rA, rB, rY, std::format("Perform left shift on r{}, store in r{}", rA, rY));
			}
			else if(ctx->PUSH())
			{
				op = 12;
				rA = parseReg(regs[0]);
				output(op, rA, 0x0, 0x0, std::format("Push r{} to the stack", rA));
			}
			else if(ctx->CALL())
			{
				op = 12;
				rA = parseReg(regs[0]);
				output(op, 0x0, 0b1000, 0x0, std::format("(CALL): Push PC to the stack")); // push PC to stack
				output(0xE, rA, 0x0, 0x0, std::format("(CALL): Jump to memory location stored in r{}", rA)); // unconditional jump to M[rA]
			}
			else if(ctx->POP())
			{
				op = 13;
				rY = parseReg(regs[0]);
				output(op, 0x0, 0x0, rY, std::format("Pop a value off of the stack and put it into r{}", rY));
			}
			else if(ctx->RET())
			{
				op = 13;
				output(op, 0x0, 0b1000, 0x0, std::format("(RET): Pop a value off of the stack and put it into PC"));
			}
			else if(ctx->JMP())
			{
				op = 14;
				rA = parseReg(regs[0]);
				output(op, rA, 0x0, 0x0, std::format("Set PC to the value in r{}", rA));
			}
			else if(ctx->JEZ() || ctx->JNZ() || ctx->JGZ() || ctx->JLZ())
			{
				op = 15;
				rA = parseReg(regs[0]);
				rB = parseReg(regs[1]);
				int flags = 0;
				std::string s = std::format("Set PC to the value in r{} if ", rA);
				if(ctx->JEZ())
				{
					flags = 0b0001;
					s += std::format("r{} == 0", rB);
				}
				else if(ctx->JNZ())
				{
					flags = 0b0010;
					s += std::format("r{} != 0", rB);
				}
				else if(ctx->JLZ())
				{
					flags = 0b0100;
					s += std::format("r{} < 0", rB);
				}
				else if(ctx->JGZ())
				{
					flags = 0b1000;
					s += std::format("r{} > 0", rB);
				}
				output(op, rA, rB, flags, s);
			}
			else if (ctx->NOP())
			{
				output(0, 0, 0, 0, std::format("NOP")); // nop instruction
			}
		}
	
	}

	void Listener::handleEscapeSeq(std::string& str)
	{
		if (str[0] != '\\' || str.size() != 2)
		{
			return;
		}
		switch (str[1])
		{
		case 'a':
			str = '\a';
			return;
		case 'b':
			str = '\b';
			return;
		case 'e':
			str = '\e';
			return;
		case 'f':
			str = '\f';
			return;
		case 'n':
			str = '\n';
			return;
		case 't':
			str = '\t';
			return;
		case 'v':
			str = '\v';
			return;
		case '\\':
			str = '\\';
			return;
		case '\'':
			str = '\'';
			return;
		case '\"':
			str = '\"';
			return;
		case '?':
			str = '\?';
			return;
		}
	}

	int Listener::parseReg(antlr4::tree::TerminalNode* node)
	{
	  std::string text = node->getText();
	  text = text.substr(1, text.size() - 1);
	  return std::stoi(text);
	}
	
	