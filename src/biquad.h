/*
 * GNUitar
 * Biquad
 * Ciulei Bogdan /Dexterus <dexterus@hackernetwork.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Log$
 * Revision 1.10  2005/09/12 08:26:51  alankila
 * - flip the signs of b1 and b2 (but not b0) because the mathematical
 *   difference equation is usually written that way.
 *
 * Revision 1.9  2005/09/10 10:53:38  alankila
 * - remove the need to reserve biquad's mem in caller's side
 *
 * Revision 1.8  2005/09/09 20:22:17  alankila
 * - phasor reimplemented according to a popular algorithm that simulates
 *   high-impedance isolated varying capacitors
 *
 * Revision 1.7  2005/08/14 23:31:22  alankila
 * revert earlier "fix" that does * sizeof(double). It was a brainfart.
 *
 * Revision 1.6  2005/08/10 11:01:39  alankila
 * - remove separate chebyshev.c, move the code into biquad.c
 * - fix the copy in .h to agree with DoBiquad's implementation
 * - rename functions:
 *   * DoBiquad       -> do_biquad
 *   * SetEqBiquad    -> set_peq_biquad
 *   * CalcChebyshev2 -> set_chebyshev2_biquad
 * - this change is followed by fixups in effects distort2 & eqbank
 *
 * Revision 1.5  2005/08/07 12:42:04  alankila
 * Do not use << 2 because double can be wider than 4.
 * Better say what you mean.
 *
 * Revision 1.4  2005/04/06 19:34:20  fonin
 * Code lickup
 *
 * Revision 1.3  2004/10/21 11:13:45  dexterus
 * Fixed calculus error
 * Added support for any numer of channels
 * iniline support delimited to Visual C
 *
 * Revision 1.2  2003/12/28 10:16:08  fonin
 * Code lickup
 *
 * $Id$
 */


#ifndef _BIQUAD_H_
#define _BIQUAD_H_ 1

#include "pump.h"

struct Biquad {
    double          a0, a1, a2, b1, b2;
    double          mem[MAX_CHANNELS][4];
};
typedef struct Biquad Biquad_t;

/*
 * Sampling rate, Center frequency, Bandwidth, Gain (decibels) 
 */
extern void     set_peq_biquad(double Fs, double Fc, double BW, double G,
			       Biquad_t *f);
extern void     set_bpf_biquad(double Fs, double Fc, double BW,
			       Biquad_t *f);
extern void     set_allpass_biquad(double delay, Biquad_t *f);

/*
 * Sampling rate, Center frequency, Ripple %, Lowpass?
 */
extern void     set_chebyshev1_biquad(double Fs, double Fc, double ripple,
			              int lowpass, Biquad_t *f);

/*
 * computes one sample ; because inline declared, this will be compiled in 
 * place where called works with MS Visual C . Other compilers ? 
 */

#if defined(_MSC_VER)
/* check if the compiler is Visual C or GCC so we can use inline function in C,
 * declared here */
__inline double
do_biquad(double x, Biquad_t *f, int c)
{				
				 
    double          y;
    if(isnan(x))
	x=0;
    y = x * f->a0 + f->mem[c][0] * f->a1 + f->mem[c][1] * f->a2
        - f->mem[c][2] * f->b1 - f->mem[c][3] * f->b2;
    if(isnan(y))
	y=0;
    f->mem[c][1] = f->mem[c][0];
    f->mem[c][0] = x;
    f->mem[c][3] = f->mem[c][2];
    f->mem[c][2] = y;
    return y;
}
#elif defined(__GNUC__)
extern __inline double do_biquad(double x, Biquad_t *f, int c);
#else
/* otherwise declare a standard function */
extern double do_biquad(double x, Biquad_t *f, int c);
#endif


#endif
