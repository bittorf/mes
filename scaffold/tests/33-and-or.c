/* -*-comment-start: "//";comment-end:""-*-
 * Mes --- Maxwell Equations of Software
 * Copyright © 2017 Jan Nieuwenhuizen <janneke@gnu.org>
 *
 * This file is part of Mes.
 *
 * Mes is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * Mes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mes.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "30-test.i"
#include <stdio.h>

int
test ()
{
  int f = 0;
  int t = 1;
  int one = t;

  puts ("\n");
  puts ("t: if (1 && 0)\n");
  if (1 && 0) return 1;

  puts ("t: if (!t && f)\n");
  if (!t && f) return 1;

  puts ("t: if (t && !one)\n");
  if (t && !one) return 1;

  puts ("t: if (f || !t)\n");
  if (f || !t) return 1;

  puts ("t: if (1 && !0)\n");
  if (1 && !0) goto ok0;
  return 1;
 ok0:

  puts ("t: if (f || t)\n");
  if (f || t) goto ok1;
  return 1;
 ok1:

  return 0;
}
