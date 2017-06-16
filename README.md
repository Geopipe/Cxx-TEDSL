Typed, Embedded Domain-Specific Languages for C++
=================================================
This library provides a skeletal implemenation of a typed EDSL AST for C++, with several interesting features.

Expression nodes are templated with the C++-type of the literal value they evaluate to.
Thus there is no need for an EDSL implementer to provide an ad hoc representation of their type hierarchy to validate an AST.

Additionally, expression nodes are *polymorphically* covariant in their templated type.
In other words, if `class A` derives from `class B`, then `Expr<A>` also derives from `Expr<B>`.

Additionally, this power can be leveraged during AST traverals with pattern matching based on the highly efficient [Mach7](https://github.com/solodon4/Mach7) library.

It is based on an implementation by [Zach Hammer](https://github.com/zhammer/) (with contributions from [Thomas Dickerson](https://github.com/elfprince13/)) on behalf of @Geopipe.
