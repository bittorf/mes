/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2018,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

#include <linux/syscall.h>
#include <syscall.h>
#include <sys/mman.h>

#if SYS_mmap

void *
mmap (void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
  return (void *)_sys_call6 (SYS_mmap, (long) addr, (long) len, (int) prot, (int) flags, (int) fd, (long) offset);
}

#elif SYS_mmap2

#include <assert.h>
#include <mes/lib.h>

void *
mmap (void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
  eputs ("TODO: mmap/mmap2\n");
  assert(0);
  return (void *)_sys_call6 (SYS_mmap2, (long) addr, (long) len, (int) prot, (int) flags, (int) fd, (long) offset);
}

#endif
