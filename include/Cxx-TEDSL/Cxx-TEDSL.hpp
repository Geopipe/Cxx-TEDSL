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
#include <unordered_map>
#include <string>
#include <list>
#include <cassert>
#include <fmt/printf.h>
#include <boost/optional.hpp>
#include <memory>

#include <Cxx-TEDSL/Core.hpp>
#include <Cxx-TEDSL/Cxx-TEDSL-Private.hpp>

using namespace std;
using namespace boost;

namespace TEDSL {
	
	using std::enable_if;
	using std::is_base_of;
	
	template<class T, typename _Super = typename T::Super>
	class Let : virtual public DeriveToBase<T, _Super, Let>, virtual public Expr<T, _Super>, virtual public detail::LetBase {
	protected:
		Let() {};
	public:
		typedef _Super Super;
		template<class A1, class A2> Let (A1&& bindings, A2&& result)
		: detail::LetBase(forward<A1>(bindings), forward<A2>(result)) {};
		
		SPExpr<T> getResult() {
			return dynamic_pointer_cast<_Expr<T> >(result_);
		};
		
	};
	
	template<class T, typename _Super = typename T::Super>
	class Symbol : virtual public DeriveToBase<T, _Super, Symbol>, virtual public Expr<T, _Super>, virtual public detail::SymBase {
	protected:
		Symbol() {};
	public:
		typedef _Super Super;
		
		template<class A1, class A2> Symbol(A1&& name, A2&& boundFrom)
		: detail::SymBase(forward<A1>(name), forward<A2>(boundFrom)) {};
		
		boost::optional<SPExpr<T> > getBoundVar() {
			return boundFrom_->getBoundVar<T>(name_);
		}
	};
	
	template<class T, typename _Super = typename T::Super>
	class Operator : virtual public DeriveToBase<T, _Super, Operator>, virtual public Expr<T, _Super> {
	public:
		typedef _Super Super;
		virtual ~Operator() {};
	};
	
	class Boolean : virtual public Expr<Boolean, AnyVal> {
		friend struct mch::bindings<Boolean>;
		bool b_;
	public:
		typedef AnyVal Super;
		Boolean (bool b) : b_(b) {};
	};
	
	class Number : virtual public Expr<Number, AnyVal> {
	public:
		typedef AnyVal Super;
		virtual ~Number() {};
	};
	
	class Integer : virtual public Number, virtual public Expr<Integer, Number> {
		friend struct mch::bindings<Integer>;
		int i_;
	public:
		typedef Number Super;
		Integer (int i) : i_(i) {};
	};
	
	class Float : virtual public Number, virtual public Expr<Float, Number> {
		friend struct mch::bindings<Float>;
		float f_;
	public:
		typedef Number Super;
		Float (float f) : f_(f) {};
	};
	
	// TODO Plus<Number> is not currently a base of Plus<Integer>
	// or Plus<Float>, because we really want a more sophisticated BinOpBase
	template<class T> class Plus : public virtual Operator<T> {
	public:
		using NumericType = typename enable_if<is_base_of<Number, T>::value, T>::type;
		SPExpr<NumericType> left;
		SPExpr<NumericType> right;
		template<class A1, class A2> Plus(A1&& l, A2&& r)
		: left(l), right(r) {};
	};
	
	
	// TODO Minus<Number> is not currently a base of Minus<Integer>
	// or Minus<Float>, because we really want a more sophisticated BinOpBase
	template<class T> class Minus : public virtual Operator<T> {
	public:
		using NumericType = typename enable_if<is_base_of<Number, T>::value, T>::type;
		SPExpr<NumericType> left;
		SPExpr<NumericType> right;
		template<class A1, class A2> Minus(A1&& l, A2&& r)
		: left(l), right(r) {};
	};
	
	void stringify(Node& n, size_t indent, bool isSymbolic);
	
	
	/**********************************************
	 * Indented printf wrapper
	 **********************************************/
#define NO_INDENT 0
#define INDENT "  " //indent seems to be two whitespaces in racket
	template <typename... Args> void printIndent(size_t numIndents, const char *msg, const Args & ... args) {
		/* Hacky way to print n tabs before a printf */
		for (size_t i = 0; i < numIndents; i++) {
			fmt::printf(INDENT);
		}
		fmt::printf(msg, args...);
	}
};


/*
 * -----
 * Mach7
 * -----
 */

namespace mch
{
	using namespace TEDSL;
	/* Literal bindings */
	template <> struct bindings<Integer> { Members(Integer::i_); };
	template <> struct bindings<Float> { Members(Float::f_); };
	template <> struct bindings<Boolean> { Members(Boolean::b_); };
	
	/* Operator bindings */
	template <> struct bindings<Plus<Number>> { Members(Plus<Number>::left,Plus<Number>::right); };
	template <> struct bindings<Minus<Number>> { Members(Minus<Number>::left,Minus<Number>::right); };
	
	/* Variable bindings */
	template <> struct bindings<Symbol<AnyVal> > { Members(TEDSL::detail::SymBase::name_, TEDSL::detail::SymBase::boundFrom_); };
	template <> struct bindings<Let<AnyVal> > { Members(TEDSL::detail::LetBase::bindings_, TEDSL::detail::LetBase::result_); };
}

