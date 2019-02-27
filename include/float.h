/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2017 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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
#ifndef __MES_FLOAT_H
#define __MES_FLOAT_H 1

#if SYSTEM_LIBC
#undef __MES_FLOAT_H
#include_next <float.h>
#else // ! SYSTEM_LIBC

#define MIN_EXP -1021
#define DBL_MIN_EXP -1021
#define LDBL_MIN_EXP -1021

// gcc-4.6
#define DBL_EPSILON 1
#define DBL_MANT_DIG 1
#define DBL_MAX INT_MAX
#define DBL_MIN INT_MIN

#define FLT_RADIX 10

#define LDBL_EPSILON 1
#define LDBL_MANT_DIG 1
#define LDBL_MAX INT_MAX
#define LDBL_MIN INT_MIN

#endif // ! SYSTEM_LIBC

#endif // __MES_FLOAT_H
