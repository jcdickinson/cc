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
#include "../tools.h"
#include "mat.h"
#include "simd.h"

namespace math {

  // OPERATORS

  template<>
  cclib_static_inline void mat_negate(const mat<4, 4>* const m, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, ms;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_zero(&rs);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_sub(&rs, &ms, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_add(const mat<4, 4>* const m1, const mat<4, 4>* const m2, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, m1s, m2s;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_uload(&m1s, m1[e]( ));
      simd::simd4x4f_uload(&m2s, m2[e]( ));
      simd::simd4x4f_add(&m1s, &m2s, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_add(const mat<4, 4>* const m, const float& s, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, ms;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_splat(&rs, s);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_add(&ms, &rs, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_sub(const mat<4, 4>* const m1, const mat<4, 4>* const m2, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, m1s, m2s;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_uload(&m1s, m1[e]( ));
      simd::simd4x4f_uload(&m2s, m2[e]( ));
      simd::simd4x4f_sub(&m1s, &m2s, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_sub(const mat<4, 4>* const m, const float& s, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, ms;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_splat(&rs, s);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_sub(&ms, &rs, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_sub(const float& s, const mat<4, 4>* const m, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, ms;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_splat(&rs, s);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_sub(&rs, &ms, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_mul(const mat<4, 4>* const m, const float& s, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, ms;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_splat(&rs, s);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_mul(&ms, &rs, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_div(const mat<4, 4>* const m, const float& s, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, ms;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_splat(&rs, s);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_div(&ms, &rs, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_div(const float& s, const mat<4, 4>* const m, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs, ms;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_splat(&rs, s);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_div(&rs, &ms, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_mul(const mat<4, 4>* const m1, const mat<4, 4>* const m2, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f m1s, m2s;
    for (size_t e = 0; e < n; e++) {

      simd::simd4x4f_uload(&m1s, m1[e]( ));
      simd::simd4x4f_uload(&m2s, m2[e]( ));
      simd::simd4x4f_matrix_mul(&m1s, &m2s, &m1s);
      simd::simd4x4f_ustore(&m1s, r[e]( ));

    }
  }

  cclib_static_inline void mat_mul(const mat<4, 4>* const m, const vec<3>* const v, vec<3>* const r, const size_t& n) {
    simd::simd4f v4;
    simd::simd4x4f ms;

    for (size_t e = 0; e < n; e++) {
      v4 = simd::simd4f_create(v[e][0], v[e][1], v[e][2], 1.0f);
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_matrix_vector_mul(&ms, &v4, &v4);
      simd::simd4f_ustore3(v4, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void mat_mul(const mat<4, 4>* const m, const vec<4>* const v, vec<4>* const r, const size_t& n) {
    simd::simd4f v4;
    simd::simd4x4f ms;

    for (size_t e = 0; e < n; e++) {
      v4 = simd::simd4f_uload4(v[e]( ));
      simd::simd4x4f_uload(&ms, m[e]( ));
      simd::simd4x4f_matrix_vector_mul(&ms, &v4, &v4);
      simd::simd4f_ustore4(v4, r[e]( ));
    }
  }

  // MATRIX FUNCTION OPERATORS

  template<>
  cclib_static_inline void transpose(const mat<4, 4>* const m, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_uload(&rs, m[e]( ));
      simd::simd4x4f_transpose_inplace(&rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void outer_product(const vec<4>* const v1, const vec<4>* const v2, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4f v1s;
    for (size_t e = 0; e < n; e++) {
      v1s = simd::simd4f_uload4(v1[e]( ));
      simd::simd4f_ustore4(simd::simd4f_mul(v1s, simd::simd4f_splat(v2[e][0])), r[e](0)());
      simd::simd4f_ustore4(simd::simd4f_mul(v1s, simd::simd4f_splat(v2[e][1])), r[e](1)());
      simd::simd4f_ustore4(simd::simd4f_mul(v1s, simd::simd4f_splat(v2[e][2])), r[e](2)());
      simd::simd4f_ustore4(simd::simd4f_mul(v1s, simd::simd4f_splat(v2[e][3])), r[e](3)());
    }
  }

  template<>
  cclib_static_inline void invert(const mat<4, 4>* const m, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f rs;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_uload(&rs, m[e]( ));
      simd::simd4x4f_inverse(&rs, &rs);
      simd::simd4x4f_ustore(&rs, r[e]( ));
    }
  }

  template<>
  cclib_static_inline void hadamard_product(const mat<4, 4>* const m1, const mat<4, 4>* const m2, mat<4, 4>* const r, const size_t& n) throw() {
    simd::simd4x4f m1s, m2s;
    for (size_t e = 0; e < n; e++) {
      simd::simd4x4f_uload(&m1s, m1[e]( ));
      simd::simd4x4f_uload(&m2s, m2[e]( ));
      simd::simd4x4f_mul(&m1s, &m2s, &m1s);
      simd::simd4x4f_ustore(&m1s, r[e]( ));
    }
  }

  // FACTORIES

  template<>
  cclib_static_inline const mat<4, 4> mat_identity( ) throw() {
    return mat<4, 4>(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      );
  }

  template<>
  cclib_static_inline const mat<4, 4> mat_translate(const vec<3>& v) throw() {
    return mat<4, 4>(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      v[0], v[1], v[2], 1.0f
      );
  }

  template<>
  cclib_static_inline const mat<4, 4> mat_scale(const vec<3>& v) throw() {
    return mat<4, 4>(
      v[0], 0.0f, 0.0f, 0.0f,
      0.0f, v[1], 0.0f, 0.0f,
      0.0f, 0.0f, v[2], 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      );
  }

  cclib_static_inline const mat<4, 4> mat_rotate(const mat<3, 3>& m) throw() {
    return mat<4, 4>(
      m[0], m[1], m[2], 0.0f,
      m[3], m[4], m[5], 0.0f,
      m[6], m[7], m[8], 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  }

  cclib_static_inline const mat<3, 3> mat_rotate_x(const vec<2>& v) throw() {
    return mat<3, 3>(
      1.0f, 0.0f, 0.0f,
      0.0f, v[0], v[1],
      0.0f, -v[1], v[0]
      );
  }

  cclib_static_inline const mat<3, 3> mat_rotate_y(const vec<2>& v) throw() {
    return mat<3, 3>(
      v[0], 0.0f, -v[1],
      0.0f, 1.0f, 0.0f,
      v[1], 0.0f, v[0]
      );
  }

  cclib_static_inline const mat<3, 3> mat_rotate_z(const vec<2>& v) throw() {
    return mat<3, 3>(
      v[0], v[1], 0.0f,
      -v[1], v[0], 0.0f,
      0.0f, 0.0f, 1.0f
      );
  }

  cclib_static_inline const mat<3, 3> mat_rotate_x(const float& a) throw() {
    return mat_rotate_x(vec<2>(cosf(a), sinf(a)));
  }

  cclib_static_inline const mat<3, 3> mat_rotate_y(const float& a) throw() {
    return mat_rotate_y(vec<2>(cosf(a), sinf(a)));
  }

  cclib_static_inline const mat<3, 3> mat_rotate_z(const float& a) throw() {
    return mat_rotate_z(vec<2>(cosf(a), sinf(a)));
  }

  cclib_static_inline const mat<4, 4> mat_rotate(const vec<2>& x, const vec<2>& y, const vec<2>& z) throw() {
    return mat_rotate(mat_rotate_x(x) * mat_rotate_y(y) * mat_rotate_z(z));
  }

  cclib_static_inline const mat<4, 4> mat_rotate(const float& x, const float& y, const float& z) throw() {
    return mat_rotate(mat_rotate_x(x) * mat_rotate_y(y) * mat_rotate_z(z));
  }
}
