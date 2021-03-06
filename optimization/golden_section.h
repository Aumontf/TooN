//Copyright (C) Edward Rosten 2009

//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND OTHER CONTRIBUTORS ``AS IS''
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR OTHER CONTRIBUTORS BE
//LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.

#ifndef TOON_GOLDEN_SECTION_H
#define TOON_GOLDEN_SECTION_H
#include <TooN/TooN.h>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <iomanip>

namespace TooN
{
	using std::numeric_limits;

	/// golden_section_search performs a golden section search line minimization
	/// on the functor provided. The inputs a, b, c must bracket the minimum, and
	/// must be in order, so  that \f$ a < b < c \f$ and \f$ f(a) > f(b) < f(c) \f$.
	/// @param a The most negative point along the line.
	/// @param b The central point.
	/// @param fb The value of the function at the central point (\f$b\f$).
	/// @param c The most positive point along the line.
	/// @param func The functor to minimize
	/// @param maxiterations  Maximum number of iterations
	/// @param tol Tolerance at which the search should be stopped.
	/// @return The minima position is returned as the first element of the vector,
	///         and the minimal value as the second element.
	/// @ingroup gOptimize
	template<class Functor, class Precision> Vector<2, Precision> golden_section_search(Precision a, Precision b, Precision c, Precision fb, const Functor& func, int maxiterations, Precision tol = sqrt(numeric_limits<Precision>::epsilon()))
	{
		using std::abs;
		//The golden ratio:
		const Precision g = (3.0 - sqrt(5))/2;

		Precision x1, x2, fx1, fx2;

		//Perform an initial iteration, to get a 4 point
		//bracketing. This is rather more convenient than
		//a 3 point bracketing.
		if(abs(b-a) > abs(c-b))
		{
			x1 = b - g*(b-a);
			x2 = b;

			fx1 = func(x1);
			fx2 = fb;
		}
		else
		{
			x2 = b + g * (c-b);
			x1 = b;

			fx1 = fb;
			fx2 = func(x2);
		}

		//We now have an ordered list of points a x1 x2 c

		//Termination condition from NR in C
		int itnum = 1; //We've already done one iteration.
		while(abs(c-a) > tol * (abs(x2)+abs(x1)) && itnum < maxiterations)
		{
			if(fx1 > fx2)
			{
				// Bracketing does:
				// a     x1     x2     c
				//        a     x1  x2 c
				a = x1;
				x1 = x2;
				x2 = x1 + g * (c-x1);
				
				fx1 = fx2;
				fx2 = func(x2);
			}
			else
			{
				// Bracketing does:
				// a     x1     x2     c
				// a  x1 x2     c
				c = x2;
				x2 = x1;
				x1= x2 - g * (x2 - a);
				
				fx2 = fx1;
				fx1 = func(x1);
			}
		}

		
		if(fx1 < fx2)
			return makeVector<Precision>(x1, fx1);
		else
			return makeVector<Precision>(x2, fx2);
	}

	/// golden_section_search performs a golden section search line minimization
	/// on the functor provided. The inputs a, b, c must bracket the minimum, and
	/// must be in order, so  that \f$ a < b < c \f$ and \f$ f(a) > f(b) < f(c) \f$.
	/// @param a The most negative point along the line.
	/// @param b The central point.
	/// @param c The most positive point along the line.
	/// @param func The functor to minimize
	/// @param maxiterations  Maximum number of iterations
	/// @param tol Tolerance at which the search should be stopped.
	/// @return The minima position is returned as the first element of the vector,
	///         and the minimal value as the second element.
	/// @ingroup gOptimize
	template<class Functor, class Precision> Vector<2, Precision> golden_section_search(Precision a, Precision b, Precision c, const Functor& func, int maxiterations, Precision tol = sqrt(numeric_limits<Precision>::epsilon()))
	{
		return golden_section_search(a, b, c, func(b), func, maxiterations, tol);
	}

}
#endif
