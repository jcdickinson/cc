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
#include <math.h>

#include "vec.h"
#include "simd.h"
#include "../tools.h"

namespace cclib {
  namespace math {

    // OPERATORS

    template<>
    cclib_static_inline void vec_negate(const vec<4>* v, vec<4>* const r, const size_t& n) throw () {
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_sub(simd::simd4f_zero( ), simd::simd4f_uload4(v[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_add(const vec<4>* const v1, const vec<4>* const v2, vec<4>* const r, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_add(simd::simd4f_uload4(v1[i]( )), simd::simd4f_uload4(v2[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_add(const vec<4>* const v, const float& s, vec<4>* const r, const size_t& n) throw() {
      const auto vs = simd::simd4f_splat(s);
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_add(simd::simd4f_uload4(v[i]( )), vs),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_sub(const vec<4>* const v1, const vec<4>* v2, vec<4>* const r, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_sub(simd::simd4f_uload4(v1[i]( )), simd::simd4f_uload4(v2[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_sub(const vec<4>* const v, const float& s, vec<4>* const r, const size_t& n) throw() {
      const auto vs = simd::simd4f_splat(s);
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_sub(simd::simd4f_uload4(v[i]( )), vs),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_sub(const float& s, const vec<4>* const v, vec<4>* const r, const size_t& n) throw() {
      const auto vs = simd::simd4f_splat(s);
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_sub(vs, simd::simd4f_uload4(v[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_mul(const vec<4>* const v, const float& s, vec<4>* const r, const size_t& n) throw() {
      const auto vs = simd::simd4f_splat(s);
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_mul(simd::simd4f_uload4(v[i]( )), vs),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_div(const vec<4>* const v, const float& s, vec<4>* const r, const size_t& n) throw() {
      const auto vs = simd::simd4f_splat(s);
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_div(simd::simd4f_uload4(v[i]( )), vs),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void vec_div(const float& s, const vec<4>* const v, vec<4>* const r, const size_t& n) throw() {
      const auto vs = simd::simd4f_splat(s);
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_div(vs, simd::simd4f_uload4(v[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline bool vec_eq(const vec<4>* const v1, const vec<4>* const v2, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        if (simd::simd4f_getsigns(simd::simd4f_cmp_eq(
          simd::simd4f_uload4(v1[i]( )),
          simd::simd4f_uload4(v2[i]( ))
          )) != 0xF)
          return false;
      }
      return true;
    }

    template<>
    cclib_static_inline bool vec_uneq(const vec<4>* const v1, const vec<4>* const v2, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        if (simd::simd4f_getsigns(simd::simd4f_cmp_eq(
            simd::simd4f_uload4(v1[i]( )),
            simd::simd4f_uload4(v2[i]( ))
          )) != 0xF)
          return true;
      }
      return false;
    }

    template<>
    cclib_static_inline bool vec_lt(const vec<4>* const v1, const vec<4>* const v2, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        if (simd::simd4f_getsigns(simd::simd4f_cmp_lt(
            simd::simd4f_uload4(v1[i]( )),
            simd::simd4f_uload4(v2[i]( ))
          )) == 0x0)
          return false;
      }
      return true;
    }

    template<>
    cclib_static_inline bool vec_gt(const vec<4>* const v1, const vec<4>* const v2, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        if (simd::simd4f_getsigns(simd::simd4f_cmp_gt(
            simd::simd4f_uload4(v1[i]( )),
            simd::simd4f_uload4(v2[i]( ))
          )) == 0x0)
          return false;
      }
      return true;
    }

    // VECTOR OPERATORS

    template<>
    cclib_static_inline void dot(const vec<4>* const v1, const vec<4>* const v2, float* const f, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        f[i] = simd::simd4f_get(
          simd::simd4f_dot4(simd::simd4f_uload4(v1[i]( )), simd::simd4f_uload4(v2[i]( ))),
          0);
      }
    }

    template<>
    cclib_static_inline void hadamard_product(const vec<4>* const v1, const vec<4>* const v2, vec<4>* const r, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_mul(simd::simd4f_uload4(v1[i]( )), simd::simd4f_uload4(v2[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void hadamard_factor(const vec<4>* const v1, const vec<4>* const v2, vec<4>* const r, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_div(simd::simd4f_uload4(v1[i]( )), simd::simd4f_uload4(v2[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void length(const vec<4>* const v, float* const r, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        r[i] = simd::simd4f_get(simd::simd4f_length4(simd::simd4f_uload4(v[i]( ))), 0);
      }
    }

    template<>
    cclib_static_inline void normalize(const vec<4>* const v, vec<4>* const r, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_normalize4(simd::simd4f_uload4(v[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void abs(const vec<4>* const v, vec<4>* const r, const size_t& n) throw() {
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_abs(simd::simd4f_uload4(v[i]( ))),
          r[i]( ));
      }
    }

    template<>
    cclib_static_inline void lerp(const vec<4>* const v1, const vec<4>* const v2, const float& s, vec<4>* const r, const size_t& n) {
      assert(s >= 0.0f && s <= 1.0f);
      const auto vr = simd::simd4f_splat(1.0f - s);
      const auto v = simd::simd4f_splat(s);
      for (size_t i = 0; i < n; i++) {
        simd::simd4f_ustore4(
          simd::simd4f_add(
          simd::simd4f_mul(simd::simd4f_uload4(v1[i]( )), vr),
          simd::simd4f_mul(simd::simd4f_uload4(v2[i]( )), v)
          ),
          r[i]( ));
      }
    }
  }
}