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

#if !defined(DOBE_TRAIT_SIMD_4F) && (defined(__SSE__) || (_M_IX86_FP > 0))

#include <stdint.h>

#include "../../tools.h"

// Export info about how SIMD4F works.
#define DOBE_TRAIT_SIMD_4F_SSE
#define DOBE_TRAIT_SIMD_4F "SSE"

#include <xmmintrin.h>
#if (defined(_M_IX86_FP) && (_M_IX86_FP >=2)) || defined(__SSE4_1__)
# include <smmintrin.h>
#endif

namespace cclib {
  namespace math {
    namespace simd {

      typedef __m128 simd4f;

      typedef union {
        uint32_t ui[4];
        float f[4];
      } simd4f_uif;

      // CREATE

      cclib_static_inline simd4f simd4f_create(float x, float y, float z, float w) throw() {
        const simd4f s = { x, y, z, w };
        return s;
      }

      cclib_static_inline simd4f simd4f_zero( ) throw() {
        const simd4f s = _mm_setzero_ps( );
        return s;
      }

      cclib_static_inline simd4f simd4f_uload4(const float* arr) throw() {
        const simd4f s = _mm_loadu_ps(arr);
        return s;
      }

      cclib_static_inline simd4f simd4f_uload3(const float* arr) throw() {
        return simd4f_create(arr[0], arr[1], arr[2], 0.0f);
      }

      cclib_static_inline simd4f simd4f_uload2(const float* arr) throw() {
        return simd4f_create(arr[0], arr[1], 0.0f, 0.0f);
      }

      // STORE

      cclib_static_inline void simd4f_ustore4(const simd4f val, float *arr) throw() {
        _mm_storeu_ps(arr, val);
      }

      cclib_static_inline void simd4f_ustore3(const simd4f val, float *arr) throw() {
        cclib_for_unrolled(i, 3, arr[i] = val.m128_f32[i]);
      }

      cclib_static_inline void simd4f_ustore2(const simd4f val, float *arr) throw() {
        cclib_for_unrolled(i, 2, arr[i] = val.m128_f32[i]);
      }

      // UTILITIES

      cclib_static_inline simd4f simd4f_splat(float v) throw() {
        const simd4f s = _mm_set1_ps(v);
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_x(simd4f v) throw() {
        const simd4f s = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_y(simd4f v) throw() {
        const simd4f s = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_z(simd4f v) throw() {
        const simd4f s = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
        return s;
      }

      cclib_static_inline simd4f simd4f_splat_w(simd4f v) throw() {
        const simd4f s = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
        return s;
      }

      // ARITHMETIC

      cclib_static_inline simd4f simd4f_add(simd4f lhs, simd4f rhs) throw() {
        const simd4f ret = _mm_add_ps(lhs, rhs);
        return ret;
      }

      cclib_static_inline simd4f simd4f_sub(simd4f lhs, simd4f rhs) throw() {
        const simd4f ret = _mm_sub_ps(lhs, rhs);
        return ret;
      }

      cclib_static_inline simd4f simd4f_mul(simd4f lhs, simd4f rhs) throw() {
        const simd4f ret = _mm_mul_ps(lhs, rhs);
        return ret;
      }

      cclib_static_inline simd4f simd4f_div(simd4f lhs, simd4f rhs) throw() {
        const simd4f ret = _mm_div_ps(lhs, rhs);
        return ret;
      }

      cclib_static_inline simd4f simd4f_madd(simd4f m1, simd4f m2, simd4f a) throw() {
        return simd4f_add(simd4f_mul(m1, m2), a);
      }

      cclib_static_inline simd4f simd4f_reciprocal(simd4f v) throw() {
        simd4f s = _mm_rcp_ps(v);
        const simd4f two = simd4f_create(2.0f, 2.0f, 2.0f, 2.0f);
        s = simd4f_mul(s, simd4f_sub(two, simd4f_mul(v, s)));
        return s;
      }

      cclib_static_inline simd4f simd4f_sqrt(simd4f v) throw() {
        const simd4f s = _mm_sqrt_ps(v);
        return s;
      }

      cclib_static_inline simd4f simd4f_rsqrt(simd4f v) throw() {
        simd4f s = _mm_rsqrt_ps(v);
        const simd4f half = simd4f_create(0.5f, 0.5f, 0.5f, 0.5f);
        const simd4f three = simd4f_create(3.0f, 3.0f, 3.0f, 3.0f);
        s = simd4f_mul(simd4f_mul(s, half), simd4f_sub(three, simd4f_mul(s, simd4f_mul(v, s))));
        return s;
      }

      // COMPARISON

      cclib_static_inline simd4f simd4f_min(simd4f a, simd4f b) throw() {
        const simd4f s = _mm_min_ps(a, b);
        return s;
      }

      cclib_static_inline simd4f simd4f_max(simd4f a, simd4f b) throw() {
        const simd4f s = _mm_max_ps(a, b);
        return s;
      }

      cclib_static_inline simd4f simd4f_cmp_eq(simd4f lhs, simd4f rhs) throw() {
        const simd4f s = _mm_cmpeq_ps(lhs, rhs);
        return s;
      }

      cclib_static_inline simd4f simd4f_cmp_gt(simd4f lhs, simd4f rhs) throw() {
        const simd4f s = _mm_cmpgt_ps(lhs, rhs);
        return s;
      }

      cclib_static_inline simd4f simd4f_cmp_lt(simd4f lhs, simd4f rhs) throw() {
        const simd4f s = _mm_cmplt_ps(lhs, rhs);
        return s;
      }

      cclib_static_inline int simd4f_getsigns(simd4f v) throw() {
        const auto i = _mm_movemask_ps(v);
        return i;
      }

      // ACCESS

      cclib_static_inline float simd4f_get(simd4f s, int i) throw() {
        return s.m128_f32[i];
      }

      // LINEAR ALGEBRA

      cclib_static_inline simd4f simd4f_dot3_splat(simd4f lhs, simd4f rhs) throw() {
#if (defined(_M_IX86_FP) && (_M_IX86_FP >=2)) || defined(__SSE4_1__)
        const simd4f s = _mm_dp_ps(lhs, rhs, 0x71);
#else
        const simd4f m = _mm_mul_ps(lhs, rhs);
        const simd4f s1 = _mm_add_ps(m, _mm_movehl_ps(m, m));
        const simd4f s = _mm_add_ss(s1, _mm_shuffle_ps(s1, s1, 1));
#endif
        return s;
      }

      cclib_static_inline float simd4f_dot3(simd4f lhs, simd4f rhs) throw() {
        return simd4f_get(simd4f_dot3_splat(lhs, rhs), 0);
      }

      cclib_static_inline simd4f simd4f_cross3(simd4f lhs, simd4f rhs) throw() {
        const simd4f lyzx = _mm_shuffle_ps(lhs, lhs, _MM_SHUFFLE(3, 0, 2, 1));
        const simd4f lzxy = _mm_shuffle_ps(lhs, lhs, _MM_SHUFFLE(3, 1, 0, 2));

        const simd4f ryzx = _mm_shuffle_ps(rhs, rhs, _MM_SHUFFLE(3, 0, 2, 1));
        const simd4f rzxy = _mm_shuffle_ps(rhs, rhs, _MM_SHUFFLE(3, 1, 0, 2));

        const simd4f r = _mm_sub_ps(_mm_mul_ps(lyzx, rzxy), _mm_mul_ps(lzxy, ryzx));
        return r;
      }

      // BIT TWIDDLING

      cclib_static_inline simd4f simd4f_shuffle_wxyz(simd4f s) throw() {
        const simd4f r = _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 1, 0, 3));
        return r;
      }
      cclib_static_inline simd4f simd4f_shuffle_zwxy(simd4f s) throw() {
        const simd4f r = _mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 0, 3, 2));
        return r;
      }
      cclib_static_inline simd4f simd4f_shuffle_yzwx(simd4f s) throw() {
        const simd4f r = _mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 3, 2, 1));
        return r;
      }

      cclib_static_inline simd4f simd4f_zero_w(simd4f s) throw() {
        simd4f r = _mm_unpackhi_ps(s, _mm_setzero_ps( ));
        r = _mm_movelh_ps(s, r);
        return r;
      }

      cclib_static_inline simd4f simd4f_zero_zw(simd4f s) throw() {
        const simd4f r = _mm_movelh_ps(s, _mm_setzero_ps( ));
        return r;
      }

      cclib_static_inline simd4f simd4f_merge_high(simd4f xyzw, simd4f abcd) throw() {
        const simd4f r = _mm_movehl_ps(abcd, xyzw);
        return r;
      }

      cclib_static_inline simd4f simd4f_flip_sign_0101(simd4f s) throw() {
        static const cclib_aligned(16) simd4f_uif upnpn = { { 0x00000000, 0x80000000, 0x00000000, 0x80000000 } };
        const simd4f r = _mm_xor_ps(s, _mm_load_ps(upnpn.f));
        return r;
      }

      cclib_static_inline simd4f simd4f_flip_sign_1010(simd4f s) throw() {
        static const cclib_aligned(16) simd4f_uif unpnp = { { 0x80000000, 0x00000000, 0x80000000, 0x00000000 } };
        const simd4f r = _mm_xor_ps(s, _mm_load_ps(unpnp.f));
        return r;
      }
    }
  }
}

#endif