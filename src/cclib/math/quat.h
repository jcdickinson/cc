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
#include "mat.h"

#include "../tools.h"

namespace cclib {
  namespace math {

    class quat {

      public:
      cclib_inline quat( ) throw() {
      }

      cclib_inline quat(const quat& q) throw() {
        v_ = q.v_;
      }

      cclib_inline quat(const float& x, const float& y, const float& z, const float& w) throw() {
        v_ = vec<4>(x, y, z, w);
      }

      cclib_inline quat(const vec<3>& v, const float& w) throw() {
        v_[0] = v[0];
        v_[1] = v[1];
        v_[2] = v[2];
        v_[3] = w;
      }

      cclib_inline float& x( ) throw() {
        return v_[0];
      }

      cclib_inline const float& x( ) const throw() {
        return v_[0];
      }

      cclib_inline float& y( ) throw() {
        return v_[1];
      }

      cclib_inline const float& y( ) const throw() {
        return v_[1];
      }

      cclib_inline float& z( ) throw() {
        return v_[2];
      }

      cclib_inline const float& z( ) const throw() {
        return v_[2];
      }

      cclib_inline float& w( ) throw() {
        return v_[3];
      }

      cclib_inline const float& w( ) const throw() {
        return v_[3];
      }

      cclib_inline vec<4>& v( ) throw() {
        return v_;
      }

      cclib_inline const vec<4>& v( ) const throw() {
        return v_;
      }

      cclib_inline vec<3>& xyz( ) throw() {
        return v_.xyz( );
      }

      cclib_inline const vec<3>& xyz( ) const throw() {
        return v_.xyz( );
      }

      cclib_inline float& operator()(const int i) throw() {
        return v_[i];
      }

      cclib_inline const float& operator()(const int i) const throw() {
        return v_[i];
      }

      cclib_inline float& operator[](const int i) throw() {
        return v_[i];
      }

      cclib_inline const float& operator[](const int i) const throw() {
        return v_[i];
      }

      cclib_inline const vec<4> angle_axis( ) const throw() {
        vec<4> r;
        r.xyz( ) = w( ) > 0 ? xyz( ) : -xyz( );
        auto l = length(r.xyz( ));
        r.w( ) = 2 * atan2(l, w( ) > 0 ? w( ) : -w( ));
        r.xyz( ) /= l;
        return r;
      }

      cclib_inline const mat<3, 3> rotation_matrix( ) const throw() {
        const vec<4> v2 = v_ * v_;
        const vec<4> sv = v_ * v_[3];
        const float xz = v_[0] * v_[2], /**/ yz = v_[1] * v_[2], /**/ xy = v_[0] * v_[1];

        return mat<3, 3>(1 - 2 * (v2[1] + v2[2]), 2 * (xy + sv[2]), 2 * (xz - sv[1]),
                         2 * (xy - sv[2]), 1 - 2 * (v2[0] + v2[2]), 2 * (sv[0] + yz),
                         2 * (sv[1] + xz), 2 * (yz - sv[0]), 1 - 2 * (v2[0] + v2[1]));
      }

      cclib_inline const mat<4, 4> matrix( ) const throw() {
        return mat_rotate(rotation_matrix( ));
      }

      cclib_inline const vec<3> euler_angles( ) const throw() {
        auto m = matrix( );
        float cos2 = m[0] * m[0] + m[1] * m[1];
        if (cos2 < 1e-6f) {
          return vec<3>(
            0.0f,
            m[2] < 0.0f ? M_PI_2 : -M_PI_2,
            -atan2(m[3], m[4]));
        } else {
          return vec<3>(
            atan2(m[5], m[8]),
            atan2(-m[2], sqrt(cos2)),
            atan2(m[1], m[0]));
        }
      }

      private:
      vec<4> v_;

    };

    // OPERATORS

    cclib_static_inline void quat_mul(const quat* const q1, const quat* const q2, quat* const r, const size_t& n = 1) {
      for (size_t e = 0; e < n; e++) {
        r[e] = quat(
          q1[e].w( ) * q2[e].xyz( ) +
          q2[e].w( ) * q1[e].xyz( ) +
          cross(q1[e].xyz( ), q2[e].xyz( )),
          q1[e].w( ) * q2[e].w( ) - dot(q1[e].xyz( ), q2[e].xyz( ))
        );
      }
    }

    cclib_static_inline void quat_mul(const quat* const q, const float& s, quat* const r, const size_t& n = 1) {
      for (size_t e = 0; e < n; e++) {
        auto aa = q[e].angle_axis( );
        aa.w( ) *= s / 2;
        r[e].w( ) = cos(aa.w( ));
        r[e].xyz( ) = normalize(aa.xyz( )) * sin(aa.w( ));
      }
    }

    cclib_static_inline void quat_mul(const quat* const q, const vec<3>* v, vec<3>* const r, const size_t& n = 1) {
      for (size_t e = 0; e < n; e++) {
        const float ss = q[e].w( ) + q[e].w( );
        r[e] = 
          ss * cross(q[e].xyz( ), v[e]) + 
          (ss * q[e].w( ) - 1) * v[e] + 
          2 * dot(q[e].xyz( ), v[e]) * q[e].xyz( );
      }
    }

    // QUATERNION FUNCTION OPERATORS

    cclib_static_inline void invert(const quat* const q, quat* const r, const size_t& n = 1) throw() {
      for (size_t e = 0; e < n; e++) {
        r[e] = quat(-q[e].x( ), -q[e].y( ), -q[e].z( ), q[e].w( ));
      }
    }

    cclib_static_inline void normalize(const quat* const q, quat* const r, const size_t& n = 1) throw() {
      for (size_t e = 0; e < n; e++) {
        normalize(&q[e].v( ), &r[e].v( ));
      }
    }

    cclib_static_inline void slerp(const quat* const q1, const quat* const q2, const float& s, quat* const r, const size_t& n = 1) throw() {
      quat rr;
      for (size_t e = 0; e < n; e++) {
        if (q1[e].w( ) * q2[e].w( ) + dot(q1[e].xyz( ), q2[e].xyz( )) > 0.999999f) {
          rr.w( ) = q1[e].w( ) * (1.0f - s) + q2[e].w( ) * s;
          rr.xyz( ) = q1[e].xyz( ) * (1.0f - s) + q2[e].xyz( ) * s;
        } else {
          invert(&q1[e], &rr);
          quat_mul(&rr, &q2[e], &rr);
          quat_mul(&rr, s, &rr);
          quat_mul(&rr, &q1[e], &rr);
        }
        r[e] = rr;
      }
    }

    // NON-DOP OVERLOADS

    cclib_static_inline const quat invert(const quat& q) throw() {
      quat r;
      invert(&q, &r);
      return r;
    }

    cclib_static_inline const quat normalize(const quat& q) throw() {
      quat r;
      normalize(&q, &r);
      return r;
    }

    cclib_static_inline const quat slerp(const quat& q1, const quat& q2, const float& s) throw() {
      quat r;
      slerp(&q1, &q2, s, &r);
      return r;
    }

    // QUATERNION FACTORIES

    cclib_static_inline const quat quat_angle_axis(vec<4> axisAngle) throw() {
      const float ha = axisAngle.w( ) / 2;
      const vec<3> a = normalize(axisAngle.xyz( ));
      return quat(a * sin(ha), cos(ha));
    }

    cclib_static_inline const quat quat_angle_axis(const float& angle, vec<3> axis) throw() {
      return quat_angle_axis(vec<4>(axis, angle));
    }

    cclib_static_inline const quat quat_euler(const vec<3>& angles) throw() {
      const vec<3> halfAngles = angles / 2;
      const float
        sinx = sin(halfAngles[0]), cosx = cos(halfAngles[0]),
        siny = sin(halfAngles[1]), cosy = cos(halfAngles[1]),
        sinz = sin(halfAngles[2]), cosz = cos(halfAngles[2]);
      return quat(
        sinx * cosy * cosz - cosx * siny * sinz,
        cosx * siny * cosz + sinx * cosy * sinz,
        cosx * cosy * sinz - sinx * siny * cosz,
        cosx * cosy * cosz + sinx * siny * sinz);
    }

    cclib_static_inline const quat quat_mat(const mat<3>& m) throw() {
      const float trace = m(0, 0) + m(1, 1) + m(2, 2);
      if (trace > 0) {
        const float s = sqrt(trace + 1.0f) * 2.0f;
        const float oneOverS = 1.0f / s;
        return quat(
          (m[5] - m[7]) * oneOverS,
          (m[6] - m[2]) * oneOverS,
          (m[1] - m[3]) * oneOverS,
          s / 4.0f);
      } else if (m[0] > m[4] && m[0] > m[8]) {
        const float s = sqrt(m[0] - m[4] - m[8] + 1) * 2.0f;
        const float oneOverS = 1.0f / s;
        return quat(
          s / 4.0f,
          (m[3] + m[1]) * oneOverS,
          (m[6] + m[2]) * oneOverS,
          (m[5] - m[7]) * oneOverS);
      } else if (m[4] > m[8]) {
        const float s = sqrt(m[4] - m[0] - m[8] + 1) * 2.0f;
        const float oneOverS = 1.0f / s;
        return quat(
          (m[3] + m[1]) * oneOverS,
          s / 4.0f,
          (m[5] + m[7]) * oneOverS,
          (m[6] - m[2]) * oneOverS);
      } else {
        const float s = sqrt(m[8] - m[0] - m[4] + 1) * 2.0f;
        const float oneOverS = 1.0f / s;
        return quat(
          (m[6] + m[2]) * oneOverS,
          (m[5] + m[7]) * oneOverS,
          s / 4.0f,
          (m[1] - m[3]) * oneOverS);
      }
    }

    // OPERATORS


    cclib_static_inline const quat operator*(const quat& q1, const quat& q2) {
      quat r;
      quat_mul(&q1, &q2, &r);
      return r;
    }

    cclib_static_inline const quat operator*(const quat& q, const float& s) {
      quat r;
      quat_mul(&q, s, &r);
      return r;
    }

    cclib_static_inline const quat operator*(const float& s, const quat& q) {
      quat r;
      quat_mul(&q, s, &r);
      return r;
    }

    cclib_static_inline const vec<3> operator*(const quat& q, const vec<3>& v) {
      vec<3> r;
      quat_mul(&q, &v, &r);
      return r;
    }

  }
}