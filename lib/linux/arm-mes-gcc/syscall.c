/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 *
 * This file is part of GNU Mes.
 *
 * GNU Mes is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * GNU Mes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <linux/x86/syscall.h>

// *INDENT-OFF*
long
_sys_call (long sys_call)
{
  long r;
  asm (
       "mov    r7, %1\n\t"
       "swi    $0\n\t"
       "mov    %0, r0\n\t"
       : "=r" (r)
       : "r" (sys_call)
       : "r0" // , "r7" // error: r7 cannot be used in asm here
       );
  if (r < 0)
    {
      errno = -r;
      r = -1;
    }
  else
    errno = 0;
  return r;
}

long
_sys_call1 (long sys_call, long one)
{
  long r;
  asm (
       "mov    r7, %1\n\t"
       "mov    r0, %2\n\t"
       "swi    $0\n\t"
       "mov    %0, r0\n\t"
       : "=r" (r)
       : "r" (sys_call), "r" (one)
       : "r0"
       );
  if (r < 0)
    {
      errno = -r;
      r = -1;
    }
  else
    errno = 0;
  return r;
}

long
_sys_call2 (long sys_call, long one, long two)
{
  long r;
  asm (
       "mov    r7, %1\n\t"
       "mov    r0, %2\n\t"
       "mov    r1, %3\n\t"
       "swi    $0\n\t"
       "mov    %0, r0\n\t"
       : "=r" (r)
       : "r" (sys_call), "r" (one), "r" (two)
       : "r0", "r1"
       );
  if (r < 0)
    {
      errno = -r;
      r = -1;
    }
  else
    errno = 0;
  return r;
}

long
_sys_call3 (long sys_call, long one, long two, long three)
{
  long r;
  asm (
       "mov    r7, %1\n\t"
       "mov    r0, %2\n\t"
       "mov    r1, %3\n\t"
       "mov    r2, %4\n\t"
       "swi    $0\n\t"
       "mov    %0, r0\n\t"
       : "=r" (r)
       : "r" (sys_call), "r" (one), "r" (two), "r" (three)
       : "r0", "r1", "r2"
       );
  if (r < 0)
    {
      errno = -r;
      r = -1;
    }
  else
    errno = 0;
  return r;
}

long
_sys_call4 (long sys_call, long one, long two, long three, long four)
{
  long r;
  asm (
       "mov    r7, %1\n\t"
       "mov    r0, %2\n\t"
       "mov    r1, %3\n\t"
       "mov    r2, %4\n\t"
       "mov    r3, %5\n\t"
       "swi    $0\n\t"
       "mov    %0, r0\n\t"
       : "=r" (r)
       : "r" (sys_call), "r" (one), "r" (two), "r" (three), "r" (four)
       : "r0", "r1", "r2", "r3"
       );
  if (r < 0)
    {
      errno = -r;
      r = -1;
    }
  else
    errno = 0;
  return r;
}

long
_sys_call6 (long sys_call, long one, long two, long three, long four, long five, long six)
{
  long r;
  asm (
       "mov    r7, %1\n\t"
       "mov    r0, %2\n\t"
       "mov    r1, %3\n\t"
       "mov    r2, %4\n\t"
       "mov    r3, %5\n\t"
       "mov    r4, %6\n\t"
       "mov    r5, %7\n\t"
       "swi    $0\n\t"
       "mov    %0, r0\n\t"
       : "=r" (r)
       : "r" (sys_call), "r" (one), "r" (two), "r" (three), "r" (four), "r" (five), "r" (six)
       : "r0", "r1", "r2", "r3", "r4", "r5"
       );
  return r;
}
