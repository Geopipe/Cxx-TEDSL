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

using namespace TEDSL;

/*
 * ------------
 * Test helpers
 * ------------
 */
static std::shared_ptr<Integer> makeInt(int i){
	return std::make_shared<Integer>(i);
}
static std::shared_ptr<Float> makeFloat(float f){
	return std::make_shared<Float>(f);
}
static std::shared_ptr<Boolean> makeBoolean(bool b){
	return std::make_shared<Boolean>(b);
}

static std::shared_ptr<Environment> make0123Environment() {
	std::unordered_map<std::string, std::shared_ptr<_Expr<AnyVal> > > boundVars;
	
	boundVars["zero"] = std::make_shared<Integer>(0);
	boundVars["one"] = std::make_shared<Integer>(1);
	boundVars["two"] = std::make_shared<Integer>(2);
	boundVars["three"] = std::make_shared<Integer>(3);;
	
	return std::make_shared<Environment>(boundVars);
}

static std::shared_ptr<Let<Operator<Integer> > > makeTestLet() {
	auto env = std::shared_ptr<Environment>(make0123Environment());
	auto sym0 = std::make_shared<Symbol<Integer> >("zero",env);
	auto sym1 = std::make_shared<Symbol<Integer> >("one",env);
	auto sym2 = std::make_shared<Symbol<Integer> >("two",env);
	auto sym3 = std::make_shared<Symbol<Integer> >("three",env);
	return std::make_shared<Let<Operator<Integer> > >(env,
							 std::make_shared<Plus<Integer> >(std::make_shared<Minus<Integer> >(sym0, sym1),
															  std::make_shared<Minus<Integer> >(sym2, sym3)));
}

/*
 * ---------------------
 * Mach7 stringify tests
 * ---------------------
 */

static void testtedsl_printIndent() {
	for (int i = 0; i < 5; i++) {
		printIndent(i, "Indent == %d\n", i);
	}
}

static void testtedsl_mach7_stringify_integer() {
	fmt::printf("---\nmach7 stringify -Integer-\n---\n");
	auto zero = std::make_shared<Integer>(0);
	stringify(*zero, 0, false);
	fmt::printf("\n\n");
}

static void testtedsl_mach7_stringify_ops() {
	fmt::printf("---\nmach7 stringify -Plus/Minus-\n---\n");
	auto zero = std::make_shared<Integer>(0);
	auto one = std::make_shared<Integer>(1);
	auto plus = std::make_shared<Plus<Integer> >(zero,one);
	auto minus = std::make_shared<Minus<Integer> >(one, zero);
	stringify(*plus, 0, false);
	stringify(*minus, 0, false);
	fmt::printf("\n\n");
}

static void testtedsl_mach7_stringify_let_symbolic() {
	fmt::printf("---\nmach7 {SYMBOLIC} stringify -Let<Operator<Integer> >-\n---\n");
	auto let = makeTestLet();
	stringify(*let, 0, true);
	fmt::printf("\n\n");
}

static void testtedsl_mach7_stringify_let_literal() {
	fmt::printf("---\nmach7 {LITERAL} stringify -Let<Operator<Integer> >-\n---\n");
	auto let = makeTestLet();
	stringify(*let, 0, false);
	fmt::printf("\n\n");
}

/*
 * ---------------
 * Tree node tests
 * ---------------
 */

static void testtedsl_let_getresult() {
	fmt::printf("---\ntesting SPExpr<T> Let::getResult\n---\n");
	auto let = makeTestLet();
	auto ext = let->getResult();
	stringify(*ext, 0, false);
	fmt::printf("\n\n");
}

/*
 * ----
 * Main
 * ----
 */


int main(int argc, char *argv[]) {
	/* Mach7 */
	testtedsl_mach7_stringify_integer();
	testtedsl_mach7_stringify_ops();
	testtedsl_mach7_stringify_let_symbolic();
	testtedsl_mach7_stringify_let_literal();
	testtedsl_let_getresult();
	return 0;
}
