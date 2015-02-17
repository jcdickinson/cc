/*
Copyright(c) 2014 cclib

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include <cassert>

#if defined(__GNUC__) 
#  if defined(__cplusplus)
#    define restrict            __restrict
#  endif
#  define cclib_aligned(n)       __attribute__ ((aligned (n)))
#  define cclib_inline           inline
#  define cclib_static_inline    static cclib_inline
#elif defined(_WIN32)
#  define cclib_aligned(n)       __declspec(align(n))
#  define cclib_inline           __forceinline
#  define cclib_static_inline    static cclib_inline
#else
#  define cclib_aligned(n)   
#  define cclib_inline           inline
#  define cclib_static_inline    static cclib_inline
#endif

#if defined(__GNUC__)
#  define pure __attribute__((pure))
#else
#  define pure
#endif

#define cclib_for_unrolled(iterator, number_of_iterations, operation) \
    { \
    const int iterator = 0;  { operation ; } \
    if ((number_of_iterations) > 1) { \
      const int iterator = 1;  { operation ; } \
      if ((number_of_iterations) > 2) { \
        const int iterator = 2;  { operation ; } \
        if ((number_of_iterations) > 3) { \
          const int iterator = 3;  { operation ; } \
          if ((number_of_iterations) > 4) { \
            const int iterator = 4;  { operation ; } \
            if ((number_of_iterations) > 5) { \
              for (int iterator = 5; iterator < (number_of_iterations); \
                   ++iterator) { \
                operation ; \
                              } \
                        } \
                    } \
                } \
            } \
        } \
    }

#define M_E        2.71828183f
#define M_LOG2E    1.44269504f
#define M_LOG10E   0.43429448f
#define M_LN2      0.69314718f
#define M_LN10     2.30258509f
#define M_4PI      12.5663706f
#define M_3PI      9.42477796f
#define M_2PI      6.28318531f
#define M_PI       3.14159265f
#define M_PI_2     1.57079633f
#define M_PI_3     1.04719755f
#define M_PI_4     0.78539816f
#define M_1_PI     0.31830989f
#define M_2_PI     0.63661977f
#define M_2_SQRTPI 1.12837917f
#define M_SQRT2    1.41421356f
#define M_SQRT1_2  0.70710678f
