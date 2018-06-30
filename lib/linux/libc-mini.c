/* -*-comment-start: "//";comment-end:""-*-
 * Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

#if __MESC__

#include <linux/mini-mes.c>

#else // !__MESC__

#include <linux/mini-gcc.c>

#endif // !__MESC__

ssize_t
write (int filedes, void const *buffer, size_t size)
{
  int r = _write (filedes, buffer, size);
  if (r < 0)
    {
      errno = -r;
      r = -1;
    }
  else
    errno = 0;
  return r;
}
