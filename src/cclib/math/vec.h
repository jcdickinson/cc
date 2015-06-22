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
#include <ostream>

#include "../tools.h"

#pragma warning(push)
#pragma warning(disable:4127)

namespace math {

  template <int d>
  class vec {
    public:

    // CONSTRUCTORS

    cclib_inline vec( ) {
    }

    cclib_inline vec(const vec<d>& v) throw() {
      cclib_for_unrolled(i, d, data_[i] = v.data_[i]);
    }

    explicit cclib_inline vec(const float& s) throw() {
      cclib_for_unrolled(i, d, data_[i] = s);
    }

    explicit cclib_inline vec(const float* a) throw() {
      cclib_for_unrolled(i, d, data_[i] = a[i]);
    }

    cclib_inline vec(const float& s1, const float& s2) throw() {
      static_assert(d == 2, "Only supported for vectors with 2 dimensions.");
      data_[0] = s1;
      data_[1] = s2;
    }

    cclib_inline vec(const float& s1, const float& s2, const float& s3) throw() {
      static_assert(d == 3, "Only supported for vectors with 3 dimensions.");
      data_[0] = s1;
      data_[1] = s2;
      data_[2] = s3;
    }

    cclib_inline vec(const float& s1, const float& s2, const float& s3, const float& s4) throw() {
      static_assert(d == 4, "Only supported for vectors with 4 dimensions.");
      data_[0] = s1;
      data_[1] = s2;
      data_[2] = s3;
      data_[3] = s4;
    }

    explicit cclib_inline vec(const vec<3>& v, const float& s) throw() {
      static_assert(d == 4, "Only supported for vectors with 4 dimensions.");
      cclib_for_unrolled(i, 3, data_[i] = v[i]);
      data_[3] = s;
    }

    // INDEXERS

    cclib_inline float* operator() ( ) throw() {
      return data_;
    }

    cclib_inline const float* operator() ( ) const throw() {
      return data_;
    }

    cclib_inline float& operator() (int i) throw() {
      assert(i >= 0 && i < d);
      return data_[i];
    }

    cclib_inline const float& operator() (int i) const throw() {
      assert(i >= 0 && i < d);
      return data_[i];
    }

    cclib_inline float& operator[] (int i) throw() {
      assert(i >= 0 && i < d);
      return data_[i];
    }

    cclib_inline const float& operator[] (int i) const throw() {
      assert(i >= 0 && i < d);
      return data_[i];
    }

    // MEMBERS

    cclib_inline float& x( ) throw() {
      static_assert(d > 0, "Only supported for vectors with at least 1 dimension.");
      return data_[0];
    }

    cclib_inline const float& x( ) const throw() {
      static_assert(d > 0, "Only supported for vectors with at least 1 dimension.");
      return data_[0];
    }

    cclib_inline float& y( ) throw() {
      static_assert(d > 1, "Only supported for vectors with at least 2 dimensions.");
      return data_[1];
    }

    cclib_inline const float& y( ) const throw() {
      static_assert(d > 1, "Only supported for vectors with at least 2 dimensions.");
      return data_[1];
    }

    cclib_inline float& z( ) throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return data_[2];
    }

    cclib_inline const float& z( ) const throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return data_[2];
    }

    cclib_inline float& w( ) throw() {
      static_assert(d > 3, "Only supported for vectors with at least 4 dimensions.");
      return data_[3];
    }

    cclib_inline const float& w( ) const throw() {
      static_assert(d > 3, "Only supported for vectors with at least 4 dimensions.");
      return data_[3];
    }

    // SWIZZLE

    cclib_inline vec<3>& xyz( ) throw() {
      static_assert(d > 3, "Only supported for vectors with at least 4 dimensions.");
      return *reinterpret_cast<vec<3>*>(&data_[0]);
    }

    cclib_inline const vec<3> xyz( ) const throw() {
      static_assert(d > 3, "Only supported for vectors with at least 4 dimensions.");
      return vec<3>(x( ), y( ), z( ));
    }

    cclib_inline vec<2>& xy( ) throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return *reinterpret_cast<vec<2>*>(&data_[0]);
    }

    cclib_inline const vec<2> xy( ) const throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return vec<2>(x( ), y( ));
    }

    cclib_inline vec<2>& yz( ) throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return *reinterpret_cast<vec<2>*>(&data_[1]);
    }

    cclib_inline const vec<2> yz( ) const throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return vec<3>(y( ), z( ));
    }

    cclib_inline vec<2> xz( ) throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return vec<2>(x( ), z( ));
    }

    cclib_inline const vec<2> xz( ) const throw() {
      static_assert(d > 2, "Only supported for vectors with at least 3 dimensions.");
      return vec<2>(x( ), z( ));
    }

    private:
    float data_[d];
  };

  // OPERATORS

  template<int d>
  cclib_static_inline void vec_negate(const vec<d>* const v, vec<d>* const r, const size_t& n = 1) throw () {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = -v[i][j]);
    }
  }

  template<int d>
  cclib_static_inline void vec_add(const vec<d>* const v1, const vec<d>* const v2, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v1[i][j] + v2[i][j]);
    }
  }

  template<int d>
  cclib_static_inline void vec_add(const vec<d>* const v, const float& s, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v[i][j] + s);
    }
  }

  template<int d>
  cclib_static_inline void vec_sub(const vec<d>* const v1, const vec<d>* const v2, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v1[i][j] - v2[i][j]);
    }
  }

  template<int d>
  cclib_static_inline void vec_sub(const vec<d>* const v, const float& s, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v[i][j] - s);
    }
  }

  template<int d>
  cclib_static_inline void vec_sub(const float& s, const vec<d>* const v, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = s - v[i][j]);
    }
  }

  template<int d>
  cclib_static_inline void vec_mul(const vec<d>* const v, const float& s, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v[i][j] * s);
    }
  }

  template<int d>
  cclib_static_inline void vec_div(const vec<d>* const v, const float& s, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v[i][j] / s);
    }
  }

  template<int d>
  cclib_static_inline void vec_div(const float& s, const vec<d>* const v, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = s / v[i][j]);
    }
  }

  template<int d>
  cclib_static_inline bool vec_eq(const vec<d>* const v1, const vec<d>* const v2, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, {
        if (v1[i][j] != v2[i][j]) return false;
      });
    }
    return true;
  }

  template<int d>
  cclib_static_inline bool vec_uneq(const vec<d>* const v1, const vec<d>* const v2, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, {
        if (v1[i][j] != v2[i][j]) return true;
      });
    }
    return false;
  }

  template<int d>
  cclib_static_inline bool vec_lt(const vec<d>* const v1, const vec<d>* const v2, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, {
        if (v1[i][j] < v2[i][j]) return true;
      });
    }
    return false;
  }

  template<int d>
  cclib_static_inline bool vec_gt(const vec<d>* const v1, const vec<d>* const v2, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, {
        if (v1[i][j] > v2[i][j]) return true;
      });
    }
    return false;
  }

  // VECTOR OPERATORS

  template<int d>
  cclib_static_inline void dot(const vec<d>* const v1, const vec<d>* const v2, float* const f, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      f[i] = 0.0f;
      cclib_for_unrolled(j, d, f[i] += v1[i][j] * v2[i][j]);
    }
  }

  template<int d>
  cclib_static_inline void hadamard_product(const vec<d>* const v1, const vec<d>* const v2, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v1[i][j] * v2[i][j]);
    }
  }

  template<int d>
  cclib_static_inline void hadamard_factor(const vec<d>* const v1, const vec<d>* const v2, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = v1[i][j] / v2[i][j]);
    }
  }

  template<int d>
  cclib_static_inline void length_squared(const vec<d>* const v, float* const r, const size_t& n = 1) throw() {
    dot(v, v, r, n);
  }

  template<int d>
  cclib_static_inline void length(const vec<d>* const v, float* const r, const size_t& n = 1) throw() {
    length_squared(v, r, n);
    for (size_t i = 0; i < n; i++) {
      r[i] = sqrt(r[i]);
    }
  }

  template<int d>
  cclib_static_inline void normalize(const vec<d>* const v, vec<d>* const r, const size_t& n = 1) throw() {
    float l;
    for (size_t i = 0; i < n; i++) {
      length(&v[i], &l);
      vec_div(&v[i], l, r);
    }
  }

  template<int d>
  cclib_static_inline void abs(const vec<d>* const v, vec<d>* const r, const size_t& n = 1) throw() {
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = fabs(v[i][j]));
    }
  }

  template<int d>
  cclib_static_inline void lerp(const vec<d>* const v1, const vec<d>* const v2, const float& s, vec<d>* const r, const size_t& n = 1) {
    assert(s >= 0.0f && s <= 1.0f);
    auto sr = 1.0f - s;
    for (size_t i = 0; i < n; i++) {
      cclib_for_unrolled(j, d, r[i][j] = sr * v1[i][j] + s * v2[i][j]);
    }
  }

  // NON-DOP OVERLOADS

  template<int d>
  cclib_static_inline const vec<d> lerp(const vec<d>& v1, const vec<d>& v2, const float& s) {
    vec<d> r;
    lerp(&v1, &v2, s, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> abs(const vec<d>& v) throw() {
    vec<d> r;
    abs(&v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> normalize(const vec<d>& v) throw() {
    vec<d> r;
    normalize(&v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const float length(const vec<d>& v) throw() {
    float r;
    length(&v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const float length_squared(const vec<d>& v) throw() {
    float r;
    length_squared(&v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const float dot(const vec<d>& v1, const vec<d>& v2) throw() {
    float r;
    dot(&v1, &v2, &r);
    return r;
  }

  // OPERATOR OVERLOADS

  template<int d>
  cclib_static_inline const vec<d> operator-(const vec<d>& v) throw() {
    vec<d> r;
    vec_negate(&v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator+(const vec<d>& v1, const vec<d>& v2) throw() {
    vec<d> r;
    vec_add(&v1, &v2, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator+(const vec<d>& v, const float& s) throw() {
    vec<d> r;
    vec_add(&v, s, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator+(const float& s, const vec<d>& v) throw() {
    vec<d> r;
    vec_add(&v, s, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d>& operator+=(vec<d>& v1, const vec<d>& v2) throw() {
    vec_add(&v1, &v1, &v2);
    return v1;
  }

  template<int d>
  cclib_static_inline const vec<d>& operator+=(vec<d>& v, const float& s) throw() {
    vec_add(&v, s, &v);
    return v;
  }

  template<int d>
  cclib_static_inline const vec<d> operator-(const vec<d>& v1, const vec<d>& v2) throw() {
    vec<d> r;
    vec_sub(&v1, &v2, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator-(const vec<d>& v, const float& s) throw() {
    vec<d> r;
    vec_sub(&v, s, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator-(const float& s, const vec<d>& v) throw() {
    vec<d> r;
    vec_sub(s, &v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d>& operator-=(vec<d>& v1, const vec<d>& v2) throw() {
    vec_sub(&v1, &v1, &v2);
    return v1;
  }

  template<int d>
  cclib_static_inline const vec<d>& operator-=(vec<d>& v, const float& s) throw() {
    vec_sub(&v, s, &v);
    return v;
  }

  template<int d>
  cclib_static_inline const vec<d> operator*(const vec<d>& v1, const vec<d>& v2) throw() {
    vec<d> r;
    hadamard_product(&v1, &v2, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator*(const vec<d>& v, const float& s) throw() {
    vec<d> r;
    vec_mul(&v, s, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator*(const float& s, const vec<d>& v) throw() {
    vec<d> r;
    vec_mul(&v, s, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d>& operator*=(vec<d>& v1, const vec<d>& v2) throw() {
    hadamard_product(&v1, &v1, &v2);
    return v1;
  }

  template<int d>
  cclib_static_inline const vec<d>& operator*=(vec<d>& v, const float& s) throw() {
    vec_mul(&v, s, &v);
    return v;
  }

  template<int d>
  cclib_static_inline const vec<d> operator/(const vec<d>& v1, const vec<d>& v2) throw() {
    vec<d> r;
    hadamard_factor(&r, &v1, &v2);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator/(const vec<d>& v, const float& s) throw() {
    vec<d> r;
    vec_div(&v, s, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator/(const float& s, const vec<d>& v) throw() {
    vec<d> r;
    vec_div(s, &v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d> operator/=(vec<d>& v1, const vec<d>& v2) throw() {
    hadamard_factor(&v1, &v1, &v2);
    return v1;
  }

  template<int d>
  cclib_static_inline const vec<d> operator/=(vec<d>& v, const float& s) throw() {
    vec_div(&v, s, &v);
    return v;
  }

  template<int d>
  cclib_static_inline const bool operator==(const vec<d>& v1, const vec<d>& v2) throw() {
    return vec_eq(&v1, &v2);
  }

  template<int d>
  cclib_static_inline const bool operator!=(const vec<d>& v1, const vec<d>& v2) throw() {
    return vec_uneq(&v1, &v2);
  }

  template<int d>
  cclib_static_inline const bool operator<(const vec<d>& v1, const vec<d>& v2) throw() {
    return vec_lt(&v1, &v2);
  }

  template<int d>
  cclib_static_inline const bool operator<=(const vec<d>& v1, const vec<d>& v2) throw() {
    return vec_lt(&v1, &v2) || vec_eq(&v1, &v2);
  }

  template<int d>
  cclib_static_inline const bool operator>(const vec<d>& v1, const vec<d>& v2) throw() {
    return vec_gt(&v1, &v2);
  }

  template<int d>
  cclib_static_inline const bool operator>=(const vec<d>& v1, const vec<d>& v2) throw() {
    return vec_gt(&v1, &v2) || vec_eq(&v1, &v2);
  }

  template<int d>
  cclib_static_inline std::ostream& operator<<(std::ostream& os, const vec<d>& v) {
    os << "[";
    cclib_for_unrolled(i, d, {
      os << " " << v[i];
    });
    os << " ]";
    return os;
  }
}
#pragma warning(pop)

#include "vec3.h"
#include "vec4.h"
