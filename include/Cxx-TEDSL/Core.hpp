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
#pragma once

#include <type_traits>
#include <memory>
using namespace std;

/**********************************************
 * Node is the base class for all AST elements
 **********************************************/

class Node {
public:
	virtual ~Node() {};
};

/**********************************************
 * AnyVal is the base class for all value types
 * that the EDSL can represent.
 **********************************************/

struct AnyVal {
	typedef AnyVal Super;
};

/**********************************************
 * Inheritance magic:
 *   DeriveToBase is a higher-order function on types.
 *   It allows us to generate templated classes which are
 *   *polymorphically* covariant in their template argument.
 * In other words:
 *   if B is a subtype of A, and C is a templated class,
 *   then DeriveToBase ensures that C<B> is a subtype of C<A>
 *
 * DeriveToBase<typename T, typename Super, template C> 'returns':
 *    Node {if T == AnyVal}
 *    C<Super, Super::Super> {otherwise}
 *
 * Example:
 * Expr<Float, Number> : DeriveToBase<Float, Number, Expr>
 *                        == Expr<Number, AnyVal>
 *
 * Expr<Number, AnyVal> : DeriveToBase<Number, AnyVal, Expr>
 *                        == Expr<AnyVal, AnyVal>
 *
 * Expr<AnyVal, AnyVal> : DeriveToBase<AnyVal, AnyVal, Expr>
 *                        == Node
 */
template<typename T, typename Super, template<typename D, typename B, typename ...O> class C, typename ...O>
using DeriveToBase = typename conditional<is_same<T, AnyVal>::value, Node, C<Super, typename Super::Super, O...> >::type;

/* ---------------------------- */


template<class T, typename _Super>
class Expr : virtual public DeriveToBase<T, _Super, Expr> {
public:
	typedef _Super Super;
	virtual ~Expr() {};
};
template<class T> using _Expr = Expr<T, typename T::Super>;
template<typename L> using SPExpr = shared_ptr<_Expr<L> >;
template<typename L> using SPExprList = list<SPExpr<L> >;
