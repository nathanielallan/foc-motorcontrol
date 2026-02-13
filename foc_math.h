#pragma once

/* ----------------------------------------------------------------------------
-------------------------------------------------------------------------------
  Define the structures and functions used by the math components of the FOC.
  The math components are the Clarke transformation, Park transformation and
  Inverse Park transformation.
-------------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

typedef struct { float alpha, beta; } ab_t;
typedef struct { float d, q; } dq_t;

ab_t clarke(float ia, float ib, float ic);
dq_t park(ab_t in, float sin_t, float cos_t);
ab_t inv_park(dq_t in, float sin_t, float cos_t);

void sincos_f(float theta, float* s, float* c);
