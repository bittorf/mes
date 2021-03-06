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
#ifndef __MES_ALLOCA_H
#define __MES_ALLOCA_H 1

#if SYSTEM_LIBC
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#undef __MES_ALLOCA_H
#include_next <alloca.h>

#else // ! SYSTEM_LIBC

#include <sys/types.h>

#if _ALLOCA_UNSIGNED
void *alloca (unsigned size);
#elif _ALLOCA_CHAR
char *alloca (int);
#else
void *alloca (size_t size);
#endif

#endif // ! SYSTEM_LIBC

#endif // __MES_ALLOCA_H
