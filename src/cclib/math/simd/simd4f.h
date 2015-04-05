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
#include "../../tools.h"
#include "simd4f.sse.h"
#include "simd4f.scalar.h"

#if !defined(DOBE_TRAIT_SIMD_4F)
# error "No SIMD4F implementation."
#endif

namespace math {
  namespace simd {
    cclib_static_inline simd4f simd4f_sum(simd4f v) throw() {
      const simd4f s1 = simd4f_add(simd4f_splat_x(v), simd4f_splat_y(v));
      const simd4f s2 = simd4f_add(s1, simd4f_splat_z(v));
      const simd4f s3 = simd4f_add(s2, simd4f_splat_w(v));
      return s3;
    }

    cclib_static_inline simd4f simd4f_dot4(simd4f lhs, simd4f rhs) throw() {
      return simd4f_sum(simd4f_mul(lhs, rhs));
    }

    cclib_static_inline simd4f simd4f_dot2(simd4f lhs, simd4f rhs) throw() {
      const simd4f m = simd4f_mul(lhs, rhs);
      const simd4f s1 = simd4f_add(simd4f_splat_x(m), simd4f_splat_y(m));
      return s1;
    }


    cclib_static_inline simd4f simd4f_length4(simd4f v) throw() {
      return simd4f_sqrt(simd4f_dot4(v, v));
    }

    cclib_static_inline simd4f simd4f_length3(simd4f v) throw() {
      return simd4f_sqrt(simd4f_splat(simd4f_dot3(v, v)));
    }

    cclib_static_inline simd4f simd4f_length2(simd4f v) throw() {
      return simd4f_sqrt(simd4f_dot2(v, v));
    }

    cclib_static_inline simd4f simd4f_length4_squared(simd4f v) throw() {
      return simd4f_dot4(v, v);
    }

    cclib_static_inline float simd4f_length3_squared_scalar(simd4f v) throw() {
      return simd4f_dot3(v, v);
    }

    cclib_static_inline simd4f simd4f_length2_squared(simd4f v) throw() {
      return simd4f_dot2(v, v);
    }


    cclib_static_inline simd4f simd4f_normalize4(simd4f a) throw() {
      simd4f invlen = simd4f_rsqrt(simd4f_dot4(a, a));
      return simd4f_mul(a, invlen);
    }

    cclib_static_inline simd4f simd4f_normalize3(simd4f a) throw() {
      simd4f invlen = simd4f_rsqrt(simd4f_splat(simd4f_dot3(a, a)));
      return simd4f_mul(a, invlen);
    }

    cclib_static_inline simd4f simd4f_normalize2(simd4f a) throw() {
      simd4f invlen = simd4f_rsqrt(simd4f_dot2(a, a));
      return simd4f_mul(a, invlen);
    }

    cclib_static_inline simd4f simd4f_abs(simd4f v) throw() {
      return simd4f_max(simd4f_sub(simd4f_splat(0.0f), v), v);
    }
  }
}