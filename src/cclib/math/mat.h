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
#include <initializer_list>

#include "../tools.h"
#include "vec.h"

namespace math {

  template <int rows, int columns = rows>
  class mat {
    public:

    // CONSTRUCTORS

    cclib_inline mat( ) throw() {
    }

    cclib_inline mat(const mat<rows, columns>& m) throw() {
      cclib_for_unrolled(i, columns, data_[i] = m.data_[i]);
    }

    cclib_inline mat(const float& s) throw() {
      cclib_for_unrolled(i, columns, data_[i] = vec<rows>(s));
    }

    cclib_inline mat(const float& m00, const float& m10,
                    const float& m01, const float& m11) throw() {
      static_assert(rows == 2 && columns == 2, "Only supported for 2x2 matrices.");
      data_[0] = vec<rows>(m00, m10);
      data_[1] = vec<rows>(m01, m11);
    }

    cclib_inline mat(const float& m00, const float& m10, const float& m20,
                    const float& m01, const float& m11, const float& m21,
                    const float& m02, const float& m12, const float& m22) throw() {
      static_assert(rows == 3 && columns == 3, "Only supported for 3x3 matrices.");
      data_[0] = vec<rows>(m00, m10, m20);
      data_[1] = vec<rows>(m01, m11, m21);
      data_[2] = vec<rows>(m02, m12, m22);
    }

    cclib_inline mat(const float& m00, const float& m10, const float& m20, const float& m30,
                    const float& m01, const float& m11, const float& m21, const float& m31,
                    const float& m02, const float& m12, const float& m22, const float& m32,
                    const float& m03, const float& m13, const float& m23, const float& m33) throw() {
      static_assert(rows == 4 && columns == 4, "Only supported for 4x4 matrices.");
      data_[0] = vec<rows>(m00, m10, m20, m30);
      data_[1] = vec<rows>(m01, m11, m21, m31);
      data_[2] = vec<rows>(m02, m12, m22, m32);
      data_[3] = vec<rows>(m03, m13, m23, m33);
    }

    cclib_inline mat(const vec<4> c0, const vec<4> c1, const vec<4> c2, const vec<4> c3) throw() {
      static_assert(rows == 4 && columns == 4, "Only supported for 4x4 matrices.");
      data_[0] = c0;
      data_[1] = c1;
      data_[2] = c2;
      data_[3] = c3;
    }

    cclib_inline mat(const float* const a) throw() {
      cclib_for_unrolled(i, columns, data_[i] = vec<rows>(&a[i * rows]));
    }

    cclib_inline mat(const vec<rows>* const v) throw() {
      cclib_for_unrolled(i, columns, data_[i] = v[i]);
    }

    cclib_inline mat(const std::initializer_list<float> a) throw() {
      assert("Values must completely fill the matrix." && a.size( ) == rows * columns);
      cclib_for_unrolled(i, columns, data_[i] = vec<rows>(&a.begin( )[i * rows]));
    }

    // INDEXERS

    cclib_inline float* operator()( ) throw() {
      return reinterpret_cast<float*>(data_);
    }

    cclib_inline const float* operator()( ) const throw() {
      return const_cast<mat<rows, columns>*>(this)->operator()( );
    }

    cclib_inline float& operator()(const int row, const int column) throw() {
      return data_[column][row];
    }

    cclib_inline const float& operator()(const int row, const int column) const throw() {
      return data_[column][row];
    }

    cclib_inline vec<rows>& operator()(const int column) throw() {
      return data_[column];
    }

    cclib_inline const vec<rows>& operator()(const int column) const throw() {
      return data_[column];
    }

    cclib_inline float& operator[](const int i) throw() {
      return reinterpret_cast<float*>(data_)[i];
    }

    cclib_inline const float& operator[](const int i) const throw() {
      return const_cast<mat<rows, columns>*>(this)->operator[](i);
    }

    // SPECIAL GETTERS
    cclib_inline const vec<rows - 1> translation_vector( ) const throw();

    private:
    vec<rows> data_[columns];
  };

  // OPERATORS

  template<int rows, int columns = rows>
  cclib_static_inline void mat_negate(const mat<rows, columns>* const m, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = -m[e][i]);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_add(const mat<rows, columns>* const m1, const mat<rows, columns>* const m2, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = m1[e][i] + m2[e][i]);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_add(const mat<rows, columns>* const m, const float& s, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = m[e][i] + s);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_sub(const mat<rows, columns>* const m1, const mat<rows, columns>* const m2, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = m1[e][i] - m2[e][i]);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_sub(const mat<rows, columns>* const m, const float& s, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = m[e][i] - s);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_sub(const float& s, const mat<rows, columns>* const m, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = s - m[e][i]);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_mul(const mat<rows, columns>* const m, const float& s, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = m[e][i] * s);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_div(const mat<rows, columns>* const m, const float& s, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = m[e][i] / s);
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_div(const float& s, const mat<rows, columns>* const m, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = s / m[e][i]);
    }
  }

  template<int size1, int size2 = size1, int size3 = size2>
  cclib_static_inline void mat_mul(const mat<size1, size2>* const m1, const mat<size2, size3>* const m2, mat<size1, size3>* const r, const size_t& n = 1) throw() {
    vec<size2> row;
    for (size_t e = 0; e < n; e++) {

      cclib_for_unrolled(i, size1, {
        cclib_for_unrolled(j, size3, {
          cclib_for_unrolled(k, size2, row[k] = m1[e](i, k));
          dot(&m2[e](j), &row, &r[e](i, j));
        });
      });

    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void mat_mul(const mat<rows, columns>* const m, const vec<columns>* const v, vec<rows>* const r, const size_t& n = 1) {
    for (size_t e = 0; e < n; e++) {
      r[e] = vec<rows>(0);
      cclib_for_unrolled(i, columns, {
        cclib_for_unrolled(j, rows, r[e][j] += m[e][rows * i + j] * v[e][i]);
      });
    }
  }

  // MATRIX FUNCTION OPERATORS

  template<int rows, int columns = rows>
  cclib_static_inline void transpose(const mat<rows, columns>* const m, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, columns, {
        cclib_for_unrolled(j, rows, r[e](j, i) = m[e](i, j));
      });
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void outer_product(const vec<rows>* const v1, const vec<columns>* const v2, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, columns, {
        cclib_for_unrolled(j, rows, {
          r[e][i + j * rows] = v1[e][i] * v2[e][j];
        });
      });
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void invert(const mat<rows, columns>* const m, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    static_assert(false, "Only supported for 2x2, 3x3 and 4x4.");
  }

  cclib_static_inline void invert(const mat<2, 2>* const m, mat<2, 2>* const r, const size_t& n) throw() {
    for (size_t e = 0; e < n; e++) {
      const float determinant = m[e][0] * m[e][3] - m[e][1] * m[e][2];

      r[e][0] = m[e][3] / determinant;
      r[e][1] = m[e][1] / -determinant;
      r[e][2] = m[e][2] / -determinant;
      r[e][3] = m[e][0] / determinant;
    }
  }

  cclib_static_inline void invert(const mat<3, 3>* const m, mat<3, 3>* const r, const size_t& n) throw() {
    for (size_t e = 0; e < n; e++) {
      const float sub11 = m[e][4] * m[e][8] - m[e][5] * m[e][7],
        sub12 = -m[e][1] * m[e][8] + m[e][2] * m[e][7],
        sub13 = m[e][1] * m[e][5] - m[e][2] * m[e][4];

      const float determinant = m[e][0] * sub11 + m[e][3] * sub12 + m[e][6] * sub13;

      r[e](0) = vec<3>(sub11, sub12, sub13) / determinant;
      r[e](1) = vec<3>(m[e][6] * m[e][5] - m[e][3] * m[e][8], m[e][0] * m[e][8] - m[e][6] * m[e][2], m[e][3] * m[e][2] - m[e][0] * m[e][5]) / determinant;
      r[e](2) = vec<3>(m[e][3] * m[e][7] - m[e][6] * m[e][4], m[e][6] * m[e][1] - m[e][0] * m[e][7], m[e][0] * m[e][4] - m[e][3] * m[e][1]) / determinant;
    }
  }

  template<int rows, int columns = rows>
  cclib_static_inline void hadamard_product(const mat<rows, columns>* const m1, const mat<rows, columns>* const m2, mat<rows, columns>* const r, const size_t& n = 1) throw() {
    for (size_t e = 0; e < n; e++) {
      cclib_for_unrolled(i, rows * columns, r[e][i] = m1[e][i] * m2[e][i]);
    }
  }

  // NON-DOP OVERLOADS

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> transpose(const mat<rows, columns>& m) throw() {
    mat<rows, columns> r;
    transpose(&m, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> outer_product(const vec<rows>& v1, const vec<columns>& v2) throw() {
    mat<rows, columns> r;
    outer_product(&v1, &v2, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> invert(const mat<rows, columns>& m) throw() {
    mat<rows, columns> r;
    invert(&m, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> hadamard_product(const mat<rows, columns>& m1, const mat<rows, columns>& m2) throw() {
    mat<rows, columns> r;
    hadamard_product(&m1, &m2, &r);
    return r;
  }

  // SPECIAL GETTERS

  template<>
  cclib_inline const vec<1> mat<2, 2>::translation_vector( ) const throw() {
    return vec<1>(data_[1][0]);
  }

  template<>
  cclib_inline const vec<2> mat<3, 3>::translation_vector( ) const throw() {
    return vec<2>(data_[2][0], data_[2][1]);
  }

  template<>
  cclib_inline const vec<3> mat<4, 4>::translation_vector( ) const throw() {
    return vec<3>(data_[3][0], data_[3][1], data_[3][2]);
  }

  // OPERATORS

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> operator-(const mat<rows, columns>& m) {
    mat<rows, columns> r;
    mat_negate(&m, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> operator+(const mat<rows, columns>& m1, const mat<rows, columns>& m2) {
    mat<rows, columns> r;
    mat_add(&m1, &m2, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> operator+(const mat<rows, columns>& m, const float& s) {
    mat<rows, columns> r;
    mat_add(&m1, s, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> operator+(const float& s, const mat<rows, columns>& m) {
    mat<rows, columns> r;
    mat_add(&m1, s, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns>& operator+=(mat<rows, columns>& m1, const mat<rows, columns>& m2) {
    mat_add(&m1, &m2, &m1);
    return m1;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns>& operator+=(mat<rows, columns>& m, const float& s) {
    mat_add(&m1, s, &m1);
    return m1;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> operator-(const mat<rows, columns>& m1, const mat<rows, columns>& m2) {
    mat<rows, columns> r;
    mat_sub(&m1, &m2, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> operator-(const mat<rows, columns>& m, const float& s) {
    mat<rows, columns> r;
    mat_sub(&m1, s, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> operator-(const float& s, const mat<rows, columns>& m) {
    mat<rows, columns> r;
    mat_sub(s, &m1, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns>& operator-=(mat<rows, columns>& m1, const mat<rows, columns>& m2) {
    mat_sub(&m1, &m2, &m1);
    return m1;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns>& operator-=(mat<rows, columns>& m, const float& s) {
    mat_sub(&m1, s, &m1);
    return m1;
  }

  template<int size1, int size2 = size1, int size3 = size2>
  cclib_static_inline const mat<size1, size3> operator*(const mat<size1, size2>& m1, const mat<size2, size3>& m2) {
    mat<size1, size3> r;
    mat_mul(&m1, &m2, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const mat<d, d>& operator*=(mat<d, d>& m1, const mat<d, d>& m2) {
    mat<size1, size3> r;
    mat_mul(&m1, &m2, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const vec<columns> operator*(const vec<rows>& v, const mat<rows, columns>& m) {
    vec<rows> r;
    mat_mul(&v, &m, &r);
    return r;
  }

  template<int rows, int columns = rows>
  cclib_static_inline const vec<rows> operator*(const mat<rows, columns>& m, const vec<columns>& v) {
    vec<rows> r;
    mat_mul(&m, &v, &r);
    return r;
  }

  template<int d>
  cclib_static_inline const vec<d>& operator*=(vec<d>& v, const mat<d, d>& m) {
    mat_mul(&m, &v, &v);
    return v;
  }

  // FACTORIES

  template<int rows, int columns = rows>
  cclib_static_inline const mat<rows, columns> mat_identity( ) throw() {
    mat<rows, columns> r;
    cclib_for_unrolled(i, (rows < columns ? rows : columns), r(i, i) = 1);
    return r;
  }

  cclib_static_inline const mat<4, 4> mat_perspective(float fovy, float aspect, float znear, float zfar, float handedness) throw() {
    const float y = tan(fovy / 2.0f);
    const float x = y / aspect;
    const float zdist = (znear - zfar) * handedness;
    const float zfar_per_zdist = zfar / zdist;
    return mat<4, 4>(
      x, 0.0f, 0.0f, 0.0f,
      0.0f, y, 0.0f, 0.0f,
      0.0f, 0.0f, zfar_per_zdist, -handedness,
      0.0f, 0.0f, znear * zfar_per_zdist * handedness, 0.0f
      );
  }

  cclib_static_inline const mat<4, 4> mat_ortho(float left, float right, float top, float bottom, float znear = -1, float zfar = +1) throw() {
    return mat<4, 4>(
      2.0f / (right - left), 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
      0.0f, 0.0f, -2.0f / (zfar - znear), 0.0f,
      -(right + left) / (right - left), -(top + bottom) / (top - bottom),
      -(zfar + znear) / (zfar - znear), 1.0f
      );
  }

  cclib_static_inline const mat<4, 4> mat_lookat(const vec<3>& at, const vec<3>& eye, const vec<3>& up) throw() {
    vec<3> axes[4];
    axes[2] = normalize(at - eye);
    axes[0] = normalize(cross(up, axes[2]));
    axes[1] = cross(axes[2], axes[0]);
    axes[3] = vec<3>(-dot(axes[0], eye), -dot(axes[1], eye), -dot(axes[2], eye));

    return mat<4, 4>(
      axes[0][0], axes[1][0], axes[2][0], 0.0f,
      axes[0][1], axes[1][1], axes[2][1], 0.0f,
      axes[0][2], axes[1][2], axes[2][2], 0.0f,
      axes[3][0], axes[3][1], axes[3][2], 1.0f
      );
  }

  template<int d>
  cclib_static_inline const mat<d, d> mat_translate(const vec<d - 1>& v) throw() {
    auto r = mat_identity<d, d>( );
    cclib_for_unrolled(i, d - 1, r(i, d - 1) = v[i]);
    return r;
  }

  template<int d>
  cclib_static_inline const mat<d, d> mat_scale(const vec<d - 1>& v) throw() {
    auto r = mat_identity<d, d>( );
    cclib_for_unrolled(i, d - 1, r(i, i) = v[i]);
    return r;
  }
}

#include "mat4x4.h"
