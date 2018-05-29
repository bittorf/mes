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

#include <libmes.h>

int
isdigit (int c)
{
  return c >= '0' && c <= '9';
}

int
isxdigit (int c)
{
  return isdigit (c) || c >= 'a' && c <= 'f';
}

int
isspace (int c)
{
  return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

int
isnumber (int c, int base)
{
  if (base == 2)
    return (c >= '0') && (c <= '1');
  if (base == 8)
    return (c >= '0') && (c <= '7');
  if (base == 10)
    return isdigit (c);
  if (base == 16)
    return isxdigit (c);
}

int
abtoi (char const **p, int base)
{
  char const *s = *p;
  int i = 0;
  int sign = 1;
  if (!base) base = 10;
  if (*s && *s == '-')
    {
      sign = -1;
      s++;
    }
  while (isnumber (*s, base))
    {
      i *= base;
      int m = *s > '9' ? 'a' - 10 : '0';
      i += *s - m;
      s++;
    }
  *p = s;
  return i * sign;
}

int
atoi (char const *s)
{
  char const *p = s;
  return abtoi (&p, 0);
}

char const*
itoa (int x)
{
  static char itoa_buf[12];
  char *p = itoa_buf + 11;
  *p-- = 0;

  int sign = 0;
  unsigned u = x;
  if (x < 0)
    {
      sign = 1;
      u = -x;
    }

  do
     {
       *p-- = '0' + (u % 10);
       u = u / 10;
     } while (u);

  if (sign && *(p + 1) != '0')
    *p-- = '-';

  return p+1;
}

char const*
itoab (int x, int base)
{
  static char itoa_buf[12];
  char *p = itoa_buf + 11;
  *p-- = 0;

  int sign = 0;
  unsigned u = x;
  if (x < 0)
    {
      sign = 1;
      u = -x;
    }

  do
     {
       int i = u % base;
       *p-- = i > 9 ? 'a' + i - 10 : '0' + i;
       x = u / base;
     } while (u);

  if (sign && *(p + 1) != '0')
    *p-- = '-';

  return p+1;
}

int _ungetc_pos = -1;
char _ungetc_buf[10];

int
fdgetc (int fd)
{
  char c;
  int i;
  if (_ungetc_pos == -1)
    {
      int r = read (fd, &c, 1);
      if (r < 1)
        return -1;
      i = c;
   }
  else
    {
      i = _ungetc_buf[_ungetc_pos];
      _ungetc_pos -= 1;
     }
  if (i < 0)
    i += 256;

  return i;
}

int
fdputc (int c, int fd)
{
  write (fd, (char*)&c, 1);
  return 0;
}

int
putchar (int c)
{
  write (STDOUT, (char*)&c, 1);
  return 0;
}

int
fdputs (char const* s, int fd)
{
  int i = strlen (s);
  write (fd, s, i);
  return 0;
}

int
fdungetc (int c, int fd)
{
  _ungetc_pos++;
  _ungetc_buf[_ungetc_pos] = c;
  return c;
}

int
getchar ()
{
  return fdgetc (g_stdin);
}