/* -*-comment-start: "//";comment-end:""-*-
 * Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017 Jan Nieuwenhuizen <janneke@gnu.org>
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

int g_depth;
#if _POSIX_SOURCE

char const*
itoa (int x)
{
  static char buf[10];
  char *p = buf+9;
  *p-- = 0;

  int sign = x < 0;
  if (sign)
    x = -x;
  
  do
    {
      *p-- = '0' + (x % 10);
      x = x / 10;
    } while (x);

  if (sign)
    *p-- = '-';

  return p+1;
}

// from mlib.c
#define fputs fdputs
int
fdputs (char const* s, int fd)
{
  int i = strlen (s);
  write (fd, s, i);
  return 0;
}

#ifdef putc
#undef putc
#endif
#define putc fdputc
int
fdputc (int c, int fd)
{
  write (fd, (char*)&c, 1);
  return 0;
}
#endif

SCM fdisplay_ (SCM, int);

SCM
display_helper (SCM x, int cont, char* sep, int fd)
{
  fputs (sep, fd);
  if (g_depth == 0) return cell_unspecified;
  g_depth = g_depth - 1;
  
  switch (TYPE (x))
    {
    case TCHAR:
      {
        fputs ("#\\", fd);
        putc (VALUE (x), fd);
        break;
      }
    case TFUNCTION:
      {
        fputs ("#<procedure ", fd);
#if __GNUC__
        char const *p = "?";
#else
        char *p = "?";
#endif
        if (FUNCTION (x).name != 0)
          p = FUNCTION (x).name;
        fputs (p, fd);
        fputs ("[", fd);
        fputs (itoa (CDR (x)), fd);
        fputs (",", fd);
        fputs (itoa (x), fd);
        fputs ("]>", fd);
        break;
      }
    case TMACRO:
      {
        fputs ("#<macro ", fd);
        display_helper (cdr (x), cont, "", fd);
        fputs (">", fd);
        break;
      }
    case TNUMBER:
      {
        fputs (itoa (VALUE (x)), fd);
        break;
      }
    case TPAIR:
      {
        if (!cont) fputs ("(", fd);
        if (x && x != cell_nil) fdisplay_ (CAR (x), fd);
        if (CDR (x) && TYPE (CDR (x)) == TPAIR)
          display_helper (CDR (x), 1, " ", fd);
        else if (CDR (x) && CDR (x) != cell_nil)
          {
            if (TYPE (CDR (x)) != TPAIR)
              fputs (" . ", fd);
            fdisplay_ (CDR (x), fd);
          }
        if (!cont) fputs (")", fd);
        break;
      }
    case TSPECIAL:
#if __MESC__
      // FIXME
      //{}
      {
        SCM t = CAR (x);
        while (t && t != cell_nil)
          {
            putc (VALUE (CAR (t)), fd);
            t = CDR (t);
          }
        break;
      }
#endif
    case TSTRING:
#if __MESC__
      // FIXME
      {
        SCM t = CAR (x);
        while (t && t != cell_nil)
          {
            putc (VALUE (CAR (t)), fd);
            t = CDR (t);
          }
        break;
      }
#endif
    case TSYMBOL:
      {
        SCM t = CAR (x);
        while (t && t != cell_nil)
          {
            putc (VALUE (CAR (t)), fd);
            t = CDR (t);
          }
        break;
      }
    default:
      {
        fputs ("<", fd);
        fputs (itoa (TYPE (x)), fd);
        fputs (":", fd);
        fputs (itoa (x), fd);
        fputs (">", fd);
        break;
      }
    }
  return 0;
}

SCM
display_ (SCM x)
{
  g_depth = 5;
  return display_helper (x, 0, "", STDOUT);
}

SCM
display_error_ (SCM x)
{
  g_depth = 5;
  return display_helper (x, 0, "", STDERR);
}

SCM
fdisplay_ (SCM x, int fd) ///((internal))
{
  g_depth = 5;
  return display_helper (x, 0, "", fd);
}

SCM
exit_ (SCM x) ///((name . "exit"))
{
  assert (TYPE (x) == TNUMBER);
  exit (VALUE (x));
}

SCM
xassq (SCM x, SCM a) ///for speed in core only
{
  while (a != cell_nil && x != CDAR (a)) a = CDR (a);
  return a != cell_nil ? CAR (a) : cell_f;
}

#if _POSIX_SOURCE
#undef fdputs
#undef fdputc
#endif
