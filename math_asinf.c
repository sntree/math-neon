/*
Math-NEON:  Neon Optimised Math Library based on cmath
Contact:    lachlan.ts@gmail.com
Copyright (C) 2009  Lachlan Tychsen - Smith aka Adventus

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "math.h"
#include "math_neon.h"

/*
Test func : acosf(x)
Test Range: -1.0 < x < 1.0
Peak Error:	~0.005%
RMS  Error: ~0.001%
*/


const float __asinf_lut[4] = {
	0.105312459675071, 	//p7
	0.169303418571894,	//p3
	0.051599985887214, 	//p5
	0.999954835104825	//p1
}; 

const float __asinf_pi_2 = M_PI_2;

float asinf_c(float x)
{

	float a, b, c, d, r, ax;
	int m;
	
	union {
		float f;
		int i;
	} xx;

	ax = fabs(x);
	d = 0.5;
	d = d - ax*0.5;
		
	//fast invsqrt approx
	xx.f = d;
	xx.i = 0x5F3759DF - (xx.i >> 1);		//VRSQRTE
	c = d * xx.f;
	b = (3.0f - c * xx.f) * 0.5;		//VRSQRTS
	xx.f = xx.f * b;		
	c = d * xx.f;
	b = (3.0f - c * xx.f) * 0.5;
    xx.f = xx.f * b;	

	//fast inverse approx
	d = xx.f;
	m = 0x3F800000 - (xx.i & 0x7F800000);
	xx.i = xx.i + m;
	xx.f = 1.41176471f - 0.47058824f * xx.f;
	xx.i = xx.i + m;
	b = 2.0 - xx.f * d;
	xx.f = xx.f * b;	
	b = 2.0 - xx.f * d;
	xx.f = xx.f * b;
	
	//if |x|>0.5 -> x = sqrt((1-x)/2)
	xx.f = xx.f - ax;	
	a = (ax > 0.5f);
	d = __asinf_pi_2 * a;
	c = 1.0f - 3.0f * a;
	ax = ax + xx.f * a;
		
	//polynomial evaluation
	xx.f = ax * ax;	
	a = (__asinf_lut[0] * ax) * xx.f + (__asinf_lut[2] * ax);
	b = (__asinf_lut[1] * ax) * xx.f + (__asinf_lut[3] * ax);
	xx.f = xx.f * xx.f;
	r = b + a * xx.f; 
	r = d + c * r;

	if (x < 0.0f) r = -r;
	return r;
}


float asinf_neon(float x)
{
#ifdef __MATH_NEON
	asm volatile (""
	);

#else
	return asinf_c(x);
#endif
}

float acosf_c(float x)
{
	return __asinf_pi_2 - asinf_c(x);
}


float acosf_neon(float x)
{
#ifdef __MATH_NEON
	asm volatile (""
	);

#else
	return acosf_c(x);
#endif
}




