/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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
#include <linux/x86_64/syscall.h>

// *INDENT-OFF*
long
__sys_call (long sys_call)
{
  long r;
  asm (
       "mov     %1,%%rax\n\t"
       "syscall \n\t"
       "mov     %%rax,%0\n\t"
       : "=r" (r)
       : "rm" (sys_call)
       : "rax"
       );
  return r;
}

long
__sys_call1 (long sys_call, long one)
{
  long r;
  asm (
       "mov     %1,%%rax\n\t"
       "mov     %2,%%rdi\n\t"
       "syscall \n\t"
       "mov     %%rax,%0\n\t"
       : "=r" (r)
       : "rm" (sys_call), "rm" (one)
       : "rax", "rdi"
       );
  return r;
}

long
__sys_call2 (long sys_call, long one, long two)
{
  long r;
  asm (
       "mov     %1,%%rax\n\t"
       "mov     %2,%%rdi\n\t"
       "mov     %3,%%rsi\n\t"
       "syscall \n\t"
       "mov     %%rax,%0\n\t"
       : "=r" (r)
       : "rm" (sys_call), "rm" (one), "rm" (two)
       : "rax", "rdi", "rsi"
       );
  return r;
}

long
__sys_call3 (long sys_call, long one, long two, long three)
{
  long r;
  asm (
       "mov     %2,%%rdi\n\t"
       "mov     %3,%%rsi\n\t"
       "mov     %4,%%rdx\n\t"
       "mov     %1,%%rax\n\t"
       "syscall \n\t"
       "mov     %%rax,%0\n\t"
       : "=r" (r)
       : "rm" (sys_call), "rm" (one), "rm" (two), "rm" (three)
       : "rax", "rdi", "rsi", "rdx"
       );
  return r;
}

long
__sys_call4 (long sys_call, long one, long two, long three, long four)
{
  long r;
  asm (
       "mov     %2,%%rdi\n\t"
       "mov     %3,%%rsi\n\t"
       "mov     %4,%%rdx\n\t"
       "mov     %5,%%r10\n\t"
       "mov     %1,%%rax\n\t"
  //      );
  // asm (
       "syscall \n\t"
       "mov     %%rax,%0\n\t"
       : "=r" (r)
       : "rm" (sys_call), "rm" (one), "rm" (two), "rm" (three), "rm" (four)
       : "rax", "rdi", "rsi", "rdx", "r10"
       );
  return r;
}
// *INDENT-ON*

long
_sys_call (long sys_call)
{
  long r = __sys_call (sys_call);
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
  long r = __sys_call1 (sys_call, one);
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
  long r = __sys_call2 (sys_call, one, two);
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
  long r = __sys_call3 (sys_call, one, two, three);
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
  long r = __sys_call4 (sys_call, one, two, three, four);
  if (r < 0)
    {
      errno = -r;
      r = -1;
    }
  else
    errno = 0;
  return r;
}
