/*********************************************************************************
 Copyright (c) 2017, Geopipe, Inc.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of Geopipe, Inc. nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************/

#include <Cxx-TEDSL/Cxx-TEDSL.hpp>

/*
 * -----------------
 * Stringify (MACH7)
 * -----------------
 */

/*
 * This function uses the solodon/mach7 pattern matching library
 * to print TEDSL trees either symbolically (printing the string name
 * of symbolic vars) or literally (printing the bound literals of
 * symbols).
 */
void TEDSL::stringify(Node& n, size_t indent, bool isSymbolic) {
	// Literal members
	int i; float f; bool b;
	// Operator members
	SPExpr<Number> left, right;
	
	/* Symbol members */
	std::string name;
	std::shared_ptr<Environment> boundFrom;
	/* Let members */
	std::shared_ptr<Environment> bindings;
	SPExpr<AnyVal> result;
	
	Match(n) {
		
		/*
		 * 1337
		 */
		Case(mch::C<Integer>(i))
		printIndent(indent, "%d", i);
		break;
		
		/*
		 * true
		 */
		Case(mch::C<Boolean>(b))
		printIndent(indent, "%b", b);
		break;
		
		/*
		 * 0.000000
		 */
		Case(mch::C<Float>(f))
		printIndent(indent, "%f", f);
		break;
		
		Case(mch::C<Plus<Integer> >(left,right))
		printIndent(indent, "(+");
		stringify(*left,indent +1,isSymbolic);
		stringify(*right,indent +1,isSymbolic);
		fmt::printf(")\n");
		
		Case(mch::C<Minus<Integer> >(left,right))
		printIndent(indent, "(-");
		stringify(*left,indent +1,isSymbolic);
		stringify(*right,indent +1,isSymbolic);
		fmt::printf(")\n");
		
		/*
		 * with context: zero is a variable bound to 0.
		 * {symbolic:}    zero
		 * {~symbolic:}   0
		 */
		Case(mch::C<Symbol<AnyVal> >(name, boundFrom))
		if (isSymbolic) {
			printIndent(indent, name.c_str());
		}
		else {
			if (auto var = boundFrom->getBoundVar<AnyVal>(name)) {
				stringify(**var, indent, isSymbolic);
			}
		}
		break;
		
		/*
		 * (Let
		 *      ([zero 0]
		 *       [one  1]
		 *       [two  2])
		 *    (Plus one (Minus two zero)))
		 */
		Case(mch::C<Let<AnyVal> >(bindings, result))
		/* If stringifying symbolically, print "(Let" and bindings */
		if (isSymbolic) {
			printIndent(indent, "(%s\n", "Let");
			size_t envIndent = indent + 2;
			auto boundVars = bindings->getBoundVars();
			printIndent(envIndent, "(");
			for (auto iter = boundVars.begin(); iter != boundVars.end(); iter++) {
				/* If current binding is not first, indent and add whitespace for alignment  */
				if (iter != boundVars.begin()) {
					printIndent(envIndent, " ");
				}
				fmt::printf("[%s ", iter->first);
				stringify(*iter->second, NO_INDENT, isSymbolic);
				/* If current binding is last, do not print newline*/
				fmt::printf("]%c", std::next(iter) == boundVars.end() ? '\0' : '\n');
			}
			fmt::printf(")\n");
		}
		/* Stringify result */
		stringify(*result, isSymbolic ? indent + 1 : indent, isSymbolic);
		/* If isSymbolic, finish let parentheses */
		if (isSymbolic) {
			fmt::printf(" )");
		}
		break;
		
	} EndMatch
}
