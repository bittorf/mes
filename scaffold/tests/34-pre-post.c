/* -*-comment-start: "//";comment-end:""-*-
 * Mes --- Maxwell Equations of Software
 * Copyright © 2017 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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
  int i = 0;

  puts ("\n");
  puts ("t: if (i++)\n");
  if (i++) return 1;

  puts ("t: if (--i)\n");
  if (--i) return 1;

  puts ("t: i += 2\n");
  i += 2;
  if (i != 2) return 1;

  puts ("t: i -= 2\n");
  i -= 2;
  if (i != 0) return 1;

  puts ("t: if (++i)\n");
  if (++i) goto ok0;
  return 1;
 ok0:

  puts ("t: if (i--)\n");
  if (i--) goto ok1;
  return 1;
 ok1:

  return 0;
}
