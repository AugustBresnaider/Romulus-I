/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

//
//  main.cpp
//  antlr4-cpp-demo
//
//  Created by Mike Lischke on 13.03.16.
//

#include <iostream>
#include <fstream>
#include <string>

#include "antlr4-runtime.h"
#include "romASMLexer.h"
#include "romASMParser.h"
#include "Listener.h"
#include "utils.h"

#include <Windows.h>

#define VERSION_NUMBER 1.1

#pragma execution_character_set("utf-8")

using namespace romASM;
using namespace antlr4;

uint8_t assemble_flags = 0;

int main(int argc, const char * argv[]) {
  std::string outfname = "a.txt";

  std::ifstream file;

  int fileIdx = 1; // argv index to find filename

  for (int i = 1; i < argc; i++)
  {
	  std::string s{ argv[i] };
	  if (s.contains("."))
	  {
		  fileIdx = i;
	  }

	  if (s[0] == '-')
	  {
		  if (s.contains("h")) // todo: needs a more robust solution
		  {
			  std::cout << "romASM Assembler v" << VERSION_NUMBER 
				  << "\nRun romASM --help or romASM - h to see this menu.\n";
			  std::cout << "Options:\n\t-c: Allow built in annotations\n\t-t: Assemble with terminators\n\t-v: See parse tree\n";
			  return 0; // immediate quit out
		  }
		  if (s.contains("t"))
		  {
			  assemble_flags |= TERMINATORS;
			  std::cout << "Assembling with terminators...\n";
		  }
		  if (s.contains("v"))
		  {
			  assemble_flags |= VERBOSE;
			  std::cout << "Showing verbose output...\n";
		  }
		  if (s.contains("c"))
		  {
			  assemble_flags |= COMMENTS;
			  std::cout << "Enabling output comments...\n";
		  }

	  }
  }

  if(argc >= 2)
  {
	file.open(argv[fileIdx], std::ios::in);
	if(!file.good())
	{
		std::cout << "ERROR OPENING FILE\n";
		return 0;
	}
	ANTLRInputStream input(file);
	romASMLexer lexer(&input);
	CommonTokenStream tokens(&lexer);

	romASMParser parser(&tokens);
	tree::ParseTree *tree = parser.program();

	auto s = tree->toStringTree(&parser);
	
	if (lexer.getNumberOfSyntaxErrors() > 0) {
        std::cerr << "Lexer failed with " << lexer.getNumberOfSyntaxErrors() << " error(s)." << std::endl;
        return -1; // Stop the program
    }
	
	if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cerr << "Parsing failed with " << parser.getNumberOfSyntaxErrors() << " error(s)." << std::endl;
        return -1; // Stop the program
    }

	if (assemble_flags & VERBOSE)
	{
		std::cout << "Parse Tree: " << s << std::endl;
	}
	
	
	Listener listener{outfname};
	
	antlr4::tree::ParseTreeWalker walker;
	
	walker.walk(&listener, tree);
	
    walker.walk(&listener, tree);
  }
  

  return 0;
}
