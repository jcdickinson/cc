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
#include "simd4f.h"
#include "simd4x4f.sse.h"
#include "simd4x4f.scalar.h"

#if !defined(DOBE_TRAIT_SIMD_4X4F)
# error "No SIMD4x4F implementation."
#endif


namespace cclib {
  namespace math {
    namespace simd {

      cclib_static_inline simd4x4f simd4x4f_create(const simd4f& x, const simd4f& y, const simd4f& z, const simd4f& w) {
        simd4x4f s = { x, y, z, w };
        return s;
      }

      cclib_static_inline void simd4x4f_splat(simd4x4f* m, const float& v) {
        simd4f vs = simd4f_splat(v);
        *m = simd4x4f_create(vs, vs, vs, vs);
      }

      cclib_static_inline void simd4x4f_zero(simd4x4f* m) {
        simd4x4f_splat(m, 0.0f);
      }

      cclib_static_inline void simd4x4f_identity(simd4x4f* m) {
        *m = simd4x4f_create(simd4f_create(1.0f, 0.0f, 0.0f, 0.0f),
                             simd4f_create(0.0f, 1.0f, 0.0f, 0.0f),
                             simd4f_create(0.0f, 0.0f, 1.0f, 0.0f),
                             simd4f_create(0.0f, 0.0f, 0.0f, 1.0f));
      }

      cclib_static_inline void simd4x4f_uload(simd4x4f* m, const float *f) {

        m->x = simd4f_uload4(f + (0 * sizeof(float)));
        m->y = simd4f_uload4(f + (4 * sizeof(float)));
        m->z = simd4f_uload4(f + (8 * sizeof(float)));
        m->w = simd4f_uload4(f + (12 * sizeof(float)));

      }

      cclib_static_inline void simd4x4f_ustore(const simd4x4f* m, float *f) {

        simd4f_ustore4(m->x, f + (0 * sizeof(float)));
        simd4f_ustore4(m->y, f + (4 * sizeof(float)));
        simd4f_ustore4(m->z, f + (8 * sizeof(float)));
        simd4f_ustore4(m->w, f + (12 * sizeof(float)));

      }

      cclib_static_inline void simd4x4f_sum(const simd4x4f* a, simd4f* out) {
        simd4f t;
        t = simd4f_add(a->x, a->y);
        t = simd4f_add(t, a->z);
        t = simd4f_add(t, a->w);
        *out = t;
      }

      cclib_static_inline void simd4x4f_matrix_vector_mul(const simd4x4f* a, const simd4f * b, simd4f* out) {

        const simd4f x = a->x;
        const simd4f y = a->y;
        const simd4f z = a->z;
        const simd4f w = a->w;
        const simd4f v = *b;
        const simd4f vx = simd4f_splat_x(v);
        const simd4f vy = simd4f_splat_y(v);
        const simd4f vz = simd4f_splat_z(v);
        const simd4f vw = simd4f_splat_w(v);

        *out = simd4f_add(simd4f_mul(x, vx),
                          simd4f_add(simd4f_mul(y, vy),
                          simd4f_add(simd4f_mul(z, vz),
                          simd4f_mul(w, vw))));
      }

      cclib_static_inline void simd4x4f_matrix_vector3_mul(const simd4x4f* a, const simd4f * b, simd4f* out) {

        *out = simd4f_add(simd4f_mul(a->x, simd4f_splat_x(*b)),
                          simd4f_add(simd4f_mul(a->y, simd4f_splat_y(*b)),
                          simd4f_mul(a->z, simd4f_splat_z(*b))));

      }

      cclib_static_inline void simd4x4f_matrix_point3_mul(const simd4x4f* a, const simd4f * b, simd4f* out) {

        *out = simd4f_add(simd4f_mul(a->x, simd4f_splat_x(*b)),
                          simd4f_add(simd4f_mul(a->y, simd4f_splat_y(*b)),
                          simd4f_add(simd4f_mul(a->z, simd4f_splat_z(*b)),
                          a->w)));

      }

      cclib_static_inline void simd4x4f_inv_ortho_matrix_point3_mul(const simd4x4f* a, const simd4f * b, simd4f* out) {
        simd4f translation = simd4f_sub(*b, a->w);

        simd4x4f transpose = *a;

        transpose.w = simd4f_create(0, 0, 0, 0);
        simd4x4f_transpose_inplace(&transpose);

        simd4x4f_matrix_point3_mul(&transpose, &translation, out);
      }

      cclib_static_inline void simd4x4f_inv_ortho_matrix_vector3_mul(const simd4x4f* a, const simd4f * b, simd4f* out) {
        simd4f translation = *b;

        simd4x4f transpose = *a;

        transpose.w = simd4f_create(0, 0, 0, 0);
        simd4x4f_transpose_inplace(&transpose);

        simd4x4f_matrix_vector3_mul(&transpose, &translation, out);
      }


      cclib_static_inline void simd4x4f_matrix_mul(const simd4x4f* a, const simd4x4f* b, simd4x4f* out) {

        simd4x4f_matrix_vector_mul(a, &b->x, &out->x);
        simd4x4f_matrix_vector_mul(a, &b->y, &out->y);
        simd4x4f_matrix_vector_mul(a, &b->z, &out->z);
        simd4x4f_matrix_vector_mul(a, &b->w, &out->w);

      }

      cclib_static_inline void simd4x4f_perspective(simd4x4f *m, float fovy_radians, float aspect, float znear, float zfar) {

        // #define HALF_PI 1.57079633f

        float deltaz = zfar - znear;
        float cotangent = tanf(1.57079633f - fovy_radians * 0.5f);

        float a = cotangent / aspect;
        float b = cotangent;
        float c = -(zfar + znear) / deltaz;
        float d = -2 * znear * zfar / deltaz;

        m->x = simd4f_create(a, 0, 0, 0);
        m->y = simd4f_create(0, b, 0, 0);
        m->z = simd4f_create(0, 0, c, -1);
        m->w = simd4f_create(0, 0, d, 0);

      }

      cclib_static_inline void simd4x4f_ortho(simd4x4f *m, float left, float right, float bottom, float top, float znear, float zfar) {

        float deltax = right - left;
        float deltay = top - bottom;
        float deltaz = zfar - znear;

        float a = 2.0f / deltax;
        float b = -(right + left) / deltax;
        float c = 2.0f / deltay;
        float d = -(top + bottom) / deltay;
        float e = -2.0f / deltaz;
        float f = -(zfar + znear) / deltaz;

        m->x = simd4f_create(a, 0, 0, 0);
        m->y = simd4f_create(0, c, 0, 0);
        m->z = simd4f_create(0, 0, e, 0);
        m->w = simd4f_create(b, d, f, 1);

      }


      cclib_static_inline void simd4x4f_lookat(simd4x4f *m, simd4f eye, simd4f center, simd4f up) {

        simd4f zaxis = simd4f_normalize3(simd4f_sub(center, eye));
        simd4f xaxis = simd4f_normalize3(simd4f_cross3(zaxis, up));
        simd4f yaxis = simd4f_cross3(xaxis, zaxis);

        zaxis = simd4f_sub(simd4f_zero( ), zaxis);

        float x = -simd4f_dot3(xaxis, eye);
        float y = -simd4f_dot3(yaxis, eye);
        float z = -simd4f_dot3(zaxis, eye);

        m->x = xaxis;
        m->y = yaxis;
        m->z = zaxis;

        m->w = simd4f_create(0, 0, 0, 1);
        simd4x4f_transpose_inplace(m);
        m->w = simd4f_create(x, y, z, 1);

      }

      cclib_static_inline void simd4x4f_translation(simd4x4f* m, float x, float y, float z) {
        *m = simd4x4f_create(simd4f_create(1.0f, 0.0f, 0.0f, 0.0f),
                             simd4f_create(0.0f, 1.0f, 0.0f, 0.0f),
                             simd4f_create(0.0f, 0.0f, 1.0f, 0.0f),
                             simd4f_create(x, y, z, 1.0f));
      }

      cclib_static_inline void simd4x4f_axis_rotation(simd4x4f* m, float radians, simd4f axis) {

        radians = -radians;

        axis = simd4f_normalize3(axis);

        const float sine = sinf(radians);
        const float cosine = cosf(radians);

        const float x = simd4f_get(axis, 0);
        const float y = simd4f_get(axis, 1);
        const float z = simd4f_get(axis, 1);

        const float ab = x * y * (1 - cosine);
        const float bc = y * z * (1 - cosine);
        const float ca = z * x * (1 - cosine);

        const float tx = x * x;
        const float ty = y * y;
        const float tz = z * z;

        const simd4f i = simd4f_create(tx + cosine * (1 - tx), ab - z * sine, ca + y * sine, 0);
        const simd4f j = simd4f_create(ab + z * sine, ty + cosine * (1 - ty), bc - x * sine, 0);
        const simd4f k = simd4f_create(ca - y * sine, bc + x * sine, tz + cosine * (1 - tz), 0);

        *m = simd4x4f_create(i, j, k, simd4f_create(0, 0, 0, 1));

      }

      cclib_static_inline void simd4x4f_add(const simd4x4f* a, const simd4x4f* b, simd4x4f* out) {

        out->x = simd4f_add(a->x, b->x);
        out->y = simd4f_add(a->y, b->y);
        out->z = simd4f_add(a->z, b->z);
        out->w = simd4f_add(a->w, b->w);

      }

      cclib_static_inline void simd4x4f_sub(const simd4x4f* a, const simd4x4f* b, simd4x4f* out) {

        out->x = simd4f_sub(a->x, b->x);
        out->y = simd4f_sub(a->y, b->y);
        out->z = simd4f_sub(a->z, b->z);
        out->w = simd4f_sub(a->w, b->w);

      }

      cclib_static_inline void simd4x4f_mul(const simd4x4f* a, const simd4x4f* b, simd4x4f* out) {

        out->x = simd4f_mul(a->x, b->x);
        out->y = simd4f_mul(a->y, b->y);
        out->z = simd4f_mul(a->z, b->z);
        out->w = simd4f_mul(a->w, b->w);

      }

      cclib_static_inline void simd4x4f_div(simd4x4f* a, simd4x4f* b, simd4x4f* out) {

        out->x = simd4f_div(a->x, b->x);
        out->y = simd4f_div(a->y, b->y);
        out->z = simd4f_div(a->z, b->z);
        out->w = simd4f_div(a->w, b->w);

      }

      cclib_static_inline simd4f simd4x4f_inverse(const simd4x4f* a, simd4x4f* out) {

        const simd4f c0 = a->x;
        const simd4f c1 = a->y;
        const simd4f c2 = a->z;
        const simd4f c3 = a->w;

        const simd4f c0_wxyz = simd4f_shuffle_wxyz(c0);
        const simd4f c0_zwxy = simd4f_shuffle_zwxy(c0);
        const simd4f c0_yzwx = simd4f_shuffle_yzwx(c0);

        const simd4f c1_wxyz = simd4f_shuffle_wxyz(c1);
        const simd4f c1_zwxy = simd4f_shuffle_zwxy(c1);
        const simd4f c1_yzwx = simd4f_shuffle_yzwx(c1);

        const simd4f c2_wxyz = simd4f_shuffle_wxyz(c2);
        const simd4f c2_zwxy = simd4f_shuffle_zwxy(c2);
        const simd4f c2_yzwx = simd4f_shuffle_yzwx(c2);

        const simd4f c3_wxyz = simd4f_shuffle_wxyz(c3);
        const simd4f c3_zwxy = simd4f_shuffle_zwxy(c3);
        const simd4f c3_yzwx = simd4f_shuffle_yzwx(c3);

        const simd4f c0_wxyz_x_c1 = simd4f_mul(c0_wxyz, c1);
        const simd4f c0_wxyz_x_c1_yzwx = simd4f_mul(c0_wxyz, c1_yzwx);
        const simd4f c0_wxyz_x_c1_zwxy = simd4f_mul(c0_wxyz, c1_zwxy);

        const simd4f c2_wxyz_x_c3 = simd4f_mul(c2_wxyz, c3);
        const simd4f c2_wxyz_x_c3_yzwx = simd4f_mul(c2_wxyz, c3_yzwx);
        const simd4f c2_wxyz_x_c3_zwxy = simd4f_mul(c2_wxyz, c3_zwxy);

        const simd4f ar1 = simd4f_sub(simd4f_shuffle_wxyz(c2_wxyz_x_c3_zwxy), simd4f_shuffle_zwxy(c2_wxyz_x_c3));
        const simd4f ar2 = simd4f_sub(simd4f_shuffle_zwxy(c2_wxyz_x_c3_yzwx), c2_wxyz_x_c3_yzwx);
        const simd4f ar3 = simd4f_sub(c2_wxyz_x_c3_zwxy, simd4f_shuffle_wxyz(c2_wxyz_x_c3));

        const simd4f br1 = simd4f_sub(simd4f_shuffle_wxyz(c0_wxyz_x_c1_zwxy), simd4f_shuffle_zwxy(c0_wxyz_x_c1));
        const simd4f br2 = simd4f_sub(simd4f_shuffle_zwxy(c0_wxyz_x_c1_yzwx), c0_wxyz_x_c1_yzwx);
        const simd4f br3 = simd4f_sub(c0_wxyz_x_c1_zwxy, simd4f_shuffle_wxyz(c0_wxyz_x_c1));


        const simd4f c0_sum = simd4f_madd(c0_yzwx, ar3,
                                          simd4f_madd(c0_zwxy, ar2,
                                          simd4f_mul(c0_wxyz, ar1)));

        const simd4f c1_sum = simd4f_madd(c1_wxyz, ar1,
                                          simd4f_madd(c1_zwxy, ar2,
                                          simd4f_mul(c1_yzwx, ar3)));

        const simd4f c2_sum = simd4f_madd(c2_yzwx, br3,
                                          simd4f_madd(c2_zwxy, br2,
                                          simd4f_mul(c2_wxyz, br1)));

        const simd4f c3_sum = simd4f_madd(c3_yzwx, br3,
                                          simd4f_madd(c3_zwxy, br2,
                                          simd4f_mul(c3_wxyz, br1)));


        const simd4f d0 = simd4f_mul(c1_sum, c0);
        const simd4f d1 = simd4f_add(d0, simd4f_merge_high(d0, d0));
        const simd4f det = simd4f_sub(d1, simd4f_splat_y(d1));

        const simd4f invdet = simd4f_splat_x(simd4f_div(simd4f_splat(1.0f), det));

        const simd4f o0 = simd4f_mul(simd4f_flip_sign_0101(c1_sum), invdet);
        const simd4f o1 = simd4f_mul(simd4f_flip_sign_1010(c0_sum), invdet);
        const simd4f o2 = simd4f_mul(simd4f_flip_sign_0101(c3_sum), invdet);
        const simd4f o3 = simd4f_mul(simd4f_flip_sign_1010(c2_sum), invdet);

        const simd4x4f mt = simd4x4f_create(o0, o1, o2, o3);

        simd4x4f_transpose(&mt, out);

        return det;
      }

    }
  }
}