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

#if (!defined(DOBE_TRAIT_SIMD_4X4F) || defined(DOBE_TRAIT_SIMD_4X4F_SSE)) && (defined(__SSE__) || (_M_IX86_FP > 0))

#include "../../tools.h"
#include "simd4f.sse.h"

// Only supported if something didn't go wrong with 4F_SSE
#if defined(DOBE_TRAIT_SIMD_4F_SSE)

// Export info about how SIMD4X4F works.
#define DOBE_TRAIT_SIMD_4X4F_SSE
#define DOBE_TRAIT_SIMD_4X4F "SSE"

namespace math {
  namespace simd {

    typedef struct {
      simd4f x, y, z, w;
    } simd4x4f;

    cclib_static_inline void simd4x4f_transpose_inplace(simd4x4f *s) {
      _MM_TRANSPOSE4_PS(s->x, s->y, s->z, s->w);
    }

    cclib_static_inline void simd4x4f_transpose(const simd4x4f *s, simd4x4f *out) {
      *out = *s;
      simd4x4f_transpose_inplace(out);
    }
  }
}

#endif
#endif