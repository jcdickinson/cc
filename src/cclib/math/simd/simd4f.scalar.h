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

#if !defined(DOBE_TRAIT_SIMD_4F)

#include <stdint.h>
#include <math.h>

#include "../../tools.h"

// Export info about how SIMD4F works.
#define DOBE_TRAIT_SIMD_4F_SCALAR
#define DOBE_TRAIT_SIMD_4F "SCALAR"

namespace cclib {
  namespace math {
    namespace simd {

      typedef union {
        float f[4];
        uint32_t ui[4];
      } simd4f;

      // CREATE

      cclib_static_inline simd4f simd4f_create(float x, float y, float z, float w) throw() {
        const simd4f s = { { x, y, z, w } };
        return s;
      }

      cclib_static_inline simd4f simd4f_zero( ) throw() {
        return simd4f_create(0.0f, 0.0f, 0.0f, 0.0f);
      }

      cclib_static_inline simd4f simd4f_uload4(const float *arr) throw() {
        const simd4f s = { { arr[0], arr[1], arr[2], arr[3] } };
        return s;
      }

      cclib_static_inline simd4f simd4f_uload3(const float *arr) throw() {
        const simd4f s = { { arr[0], arr[1], arr[2], 0.0f } };
        return s;
      }

      cclib_static_inline simd4f simd4f_uload2(const float *arr) throw() {
        const simd4f s = { { arr[0], arr[1], 0.0f, 0.0f } };
        return s;
      }

      // STORE

      cclib_static_inline void simd4f_ustore4(const simd4f val, float *arr) throw() {
        cclib_for_unrolled(i, 4, arr[i] = val.f[i]);
      }

      cclib_static_inline void simd4f_ustore3(const simd4f val, float *arr) throw() {
        cclib_for_unrolled(i, 3, arr[i] = val.f[i]);
      }

      cclib_static_inline void simd4f_ustore2(const simd4f val, float *arr) throw() {
        cclib_for_unrolled(i, 2, arr[i] = val.f[i]);
      }
      
      // UTILITIES

      cclib_static_inline simd4f simd4f_splat(float v) throw() {
        const simd4f s = { { v, v, v, v } };
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_x(simd4f v) throw() {
        const float f = v.f[0];
        const simd4f s = { { f, f, f, f } };
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_y(simd4f v) throw() {
        const float f = v.f[1];
        const simd4f s = { { f, f, f, f } };
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_z(simd4f v) throw() {
        const float f = v.f[2];
        const simd4f s = { { f, f, f, f } };
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_w(simd4f v) throw() {
        const float f = v.f[3];
        const simd4f s = { { f, f, f, f } };
        return s;
      }

      cclib_static_inline simd4f simd4f_reciprocal(simd4f v) throw() {
        const simd4f s = { { 1.0f / v.f[0], 1.0f / v.f[1], 1.0f / v.f[2], 1.0f / v.f[3] } };
        return s;
      }

      cclib_static_inline simd4f simd4f_sqrt(simd4f v) throw() {
        const simd4f s = { sqrtf(v.f[0]), sqrtf(v.f[1]), sqrtf(v.f[2]), sqrtf(v.f[3]) };
        return s;
      }

      cclib_static_inline simd4f simd4f_rsqrt(simd4f v) throw() {
        const simd4f s = { 1.0f / sqrtf(v.f[0]), 1.0f / sqrtf(v.f[1]), 1.0f / sqrtf(v.f[2]), 1.0f / sqrtf(v.f[3]) };
        return s;
      }

      // COMPARISON

      cclib_static_inline simd4f simd4f_cmp_eq(simd4f lhs, simd4f rhs) throw() {
        const simd4f s = { { -(lhs.f[0] == rhs.f[0]), -(lhs.f[1] == rhs.f[1]), -(lhs.f[2] == rhs.f[2]), -(lhs.f[3] == rhs.f[3]) } };
        return s;
      }

      cclib_static_inline simd4f simd4f_cmp_gt(simd4f lhs, simd4f rhs) throw() {
        const simd4f s = { { -(lhs.f[0] > rhs.f[0]), -(lhs.f[1] > rhs.f[1]), -(lhs.f[2] > rhs.f[2]), -(lhs.f[3] > rhs.f[3]) } };
        return s;
      }

      cclib_static_inline simd4f simd4f_cmp_lt(simd4f lhs, simd4f rhs) throw() {
        const simd4f s = { { -(lhs.f[0] < rhs.f[0]), -(lhs.f[1] < rhs.f[1]), -(lhs.f[2] < rhs.f[2]), -(lhs.f[3] < rhs.f[3]) } };
        return s;
      }

      cclib_static_inline int simd4f_getsigns(simd4f v) {
        int s = (signbit(v.f[3]) << 3) | (signbit(v.f[2]) << 2) | (signbit(v.f[1]) << 1) | (signbit(v.f[0]) << 0);
        return s;
      }

      // ARITHMETIC

      cclib_static_inline simd4f simd4f_add(simd4f lhs, simd4f rhs) throw() {
        simd4f ret = { lhs.f[0] + rhs.f[0], lhs.f[1] + rhs.f[1], lhs.f[2] + rhs.f[2], lhs.f[3] + rhs.f[3] };
        return ret;
      }

      cclib_static_inline simd4f simd4f_sub(simd4f lhs, simd4f rhs) throw() {
        simd4f ret = { lhs.f[0] - rhs.f[0], lhs.f[1] - rhs.f[1], lhs.f[2] - rhs.f[2], lhs.f[3] - rhs.f[3] };
        return ret;
      }

      cclib_static_inline simd4f simd4f_mul(simd4f lhs, simd4f rhs) throw() {
        simd4f ret = { lhs.f[0] * rhs.f[0], lhs.f[1] * rhs.f[1], lhs.f[2] * rhs.f[2], lhs.f[3] * rhs.f[3] };
        return ret;
      }

      cclib_static_inline simd4f simd4f_div(simd4f lhs, simd4f rhs) throw() {
        simd4f ret = { lhs.f[0] / rhs.f[0], lhs.f[1] / rhs.f[1], lhs.f[2] / rhs.f[2], lhs.f[3] / rhs.f[3] };
        return ret;
      }

      cclib_static_inline simd4f simd4f_madd(simd4f m1, simd4f m2, simd4f a) throw() {
        return simd4f_add(simd4f_mul(m1, m2), a);
      }

      cclib_static_inline simd4f simd4f_cross3(simd4f lhs, simd4f rhs) throw() {
        return simd4f_create(lhs.f[1] * rhs.f[2] - lhs.f[2] * rhs.f[1],
                             lhs.f[2] * rhs.f[0] - lhs.f[0] * rhs.f[2],
                             lhs.f[0] * rhs.f[1] - lhs.f[1] * rhs.f[0], 
                             0);
      }

      cclib_static_inline float simd4f_dot3(simd4f lhs, simd4f rhs) throw() {
        return lhs.f[0] * rhs.f[0] + lhs.f[1] * rhs.f[1] + lhs.f[2] * rhs.f[2];
      }

      // ACCESS

      cclib_static_inline float simd4f_get(simd4f s, int i) throw() {
        return s.f[i];
      }

      cclib_static_inline simd4f simd4f_shuffle_wxyz(simd4f s) throw() {
        return simd4f_create(s.f[3], s.f[0], s.f[1], s.f[2]);
      }

      cclib_static_inline simd4f simd4f_shuffle_zwxy(simd4f s) throw() {
        return simd4f_create(s.f[2], s.f[3], s.f[0], s.f[1]);
      }

      cclib_static_inline simd4f simd4f_shuffle_yzwx(simd4f s) throw() {
        return simd4f_create(s.f[1], s.f[2], s.f[3], s.f[0]);
      }

      cclib_static_inline simd4f simd4f_zero_w(simd4f s) throw() {
        return simd4f_create(s.f[0], s.f[1], s.f[2], 0.0f);
      }

      cclib_static_inline simd4f simd4f_zero_zw(simd4f s) throw() {
        return simd4f_create(s.f[0], s.f[1], 0.0f, 0.0f);
      }

      // BIT TWIDDLING

      cclib_static_inline simd4f simd4f_merge_high(simd4f abcd, simd4f xyzw) throw() {
        return simd4f_create(abcd.f[2], abcd.f[3], xyzw.f[2], xyzw.f[3]);
      }

      cclib_static_inline simd4f simd4f_flip_sign_0101(simd4f s) throw() {
        return simd4f_create(s.f[0], -s.f[1], s.f[2], -s.f[3]);
      }

      cclib_static_inline simd4f simd4f_flip_sign_1010(simd4f s) throw() {
        return simd4f_create(-s.f[0], s.f[1], -s.f[2], s.f[3]);
      }

      cclib_static_inline simd4f simd4f_min(simd4f a, simd4f b) throw() {
        return simd4f_create(a.f[0] < b.f[0] ? a.f[0] : b.f[0],
                             a.f[1] < b.f[1] ? a.f[1] : b.f[1],
                             a.f[2] < b.f[2] ? a.f[2] : b.f[2],
                             a.f[3] < b.f[3] ? a.f[3] : b.f[3]);
      }

      cclib_static_inline simd4f simd4f_max(simd4f a, simd4f b) throw() {
        return simd4f_create(a.f[0] > b.f[0] ? a.f[0] : b.f[0],
                             a.f[1] > b.f[1] ? a.f[1] : b.f[1],
                             a.f[2] > b.f[2] ? a.f[2] : b.f[2],
                             a.f[3] > b.f[3] ? a.f[3] : b.f[3]);
      }
    }
  }
}

#endif