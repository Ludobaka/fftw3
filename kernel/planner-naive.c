/*
 * Copyright (c) 2002 Matteo Frigo
 * Copyright (c) 2002 Steven G. Johnson
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
 */

/* $Id: planner-naive.c,v 1.5 2002-06-10 13:04:21 athena Exp $ */
#include "ifftw.h"

/* naive planner with no memoization */

static plan *mkplan(planner *ego, problem *p)
{
     plan *best = 0;

     FORALL_SOLVERS(ego, s, {
	  plan *pln = s->adt->mkplan(s, p, ego);

	  if (pln) {
	       X(plan_use)(pln);
	       ego->ntry++;
	       ego->hook(pln, p);
	       pln->cost = X(measure_execution_time)(pln, p);
	       if (best) {
		    if (pln->cost < best->cost) {
			 X(plan_destroy)(best);
			 best = pln;
		    } else {
			 X(plan_destroy)(pln);
		    }
	       } else {
		    best = pln;
	       }
	  }
     });

     return best;
}

/* constructor */
planner *X(mkplanner_naive)(void)
{
     return X(mkplanner)(sizeof(planner), mkplan, 0);
}
