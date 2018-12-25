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

#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int readchar ();
int unreadchar ();

int
peekchar ()
{
  if (__stdin >= 0)
    {
      int c = readchar ();
      unreadchar (c);
      return c;
    }
  SCM port = current_input_port ();
  SCM string = STRING (port);
  size_t length = LENGTH (string);
#if __M2_PLANET__
   if (length == 0)
#else
  if (!length)
#endif
    return -1;
  char const *p = CSTRING (string);
  return p[0];
}

int
readchar ()
{
  if (__stdin >= 0)
    return fdgetc (__stdin);
  SCM port = current_input_port ();
  SCM string = STRING (port);
  size_t length = LENGTH (string);
#if __M2_PLANET__
   if (length == 0)
#else
  if (!length)
#endif
    return -1;
  char const *p = CSTRING (string);
#if __M2_PLANET__
  int c = p[0];
  p = p + 1;
#else
  int c = *p++;
#endif
  STRING (port) = make_string (p, length-1);
  return c;
}

int
unreadchar (int c)
{
  if (__stdin >= 0)
    return fdungetc (c, __stdin);
  SCM port = current_input_port ();
  SCM string = STRING (port);
  size_t length = LENGTH (string);
  char *p = CSTRING (string);
  p = p - 1;
  string = make_string (p, length+1);
  p = CSTRING (string);
  p[0] = c;
  STRING (port) = string;
  return c;
}

SCM
peek_byte ()
{
  return MAKE_NUMBER (peekchar ());
}

SCM
read_byte ()
{
  return MAKE_NUMBER (readchar ());
}

SCM
unread_byte (SCM i)
{
  unreadchar (VALUE (i));
  return i;
}

SCM
peek_char ()
{
  return MAKE_CHAR (peekchar ());
}

SCM
read_char (SCM port) ///((arity . n))
{
  int fd = __stdin;
  if (TYPE (port) == TPAIR && TYPE (car (port)) == TNUMBER)
    __stdin = VALUE (CAR (port));
  SCM c = MAKE_CHAR (readchar ());
  __stdin = fd;
  return c;
}

SCM
unread_char (SCM i)
{
  unreadchar (VALUE (i));
  return i;
}

SCM
write_char (SCM i) ///((arity . n))
{
  write_byte (i);
  return i;
}

SCM
write_byte (SCM x) ///((arity . n))
{
  SCM c = car (x);
  SCM p = cdr (x);
  int fd = __stdout;
  if (TYPE (p) == TPAIR && TYPE (car (p)) == TNUMBER && VALUE (CAR (p)) != 1)
    fd = VALUE (CAR (p));
  if (TYPE (p) == TPAIR && TYPE (car (p)) == TNUMBER && VALUE (CAR (p)) == 2)
    fd = __stderr;
  char cc = VALUE (c);
  write (fd, (char*)&cc, 1);
#if !__MESC__
  assert (TYPE (c) == TNUMBER || TYPE (c) == TCHAR);
#endif
  return c;
}

SCM
getenv_ (SCM s) ///((name . "getenv"))
{
  char *p;
  p = getenv (CSTRING (s));
#if __M2_PLANET__
  if (p != 0)
#else
  if (p)
#endif
    return MAKE_STRING0 (p);
  return cell_f;
}

SCM
setenv_ (SCM s, SCM v) ///((name . "setenv"))
{
#if __M2_PLANET__
  char buf = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
#else
  char buf[1024];
#endif
  strcpy (buf, CSTRING (s));
  setenv (buf, CSTRING (v), 1);
  return cell_unspecified;
}

SCM
access_p (SCM file_name, SCM mode)
{
  if (access (CSTRING (file_name), VALUE (mode)) == 0)
    return cell_t;
  return cell_f;
}

SCM
current_input_port ()
{
  if (__stdin >= 0)
    return MAKE_NUMBER (__stdin);
  SCM x = g_ports;
  while (x && PORT (CAR (x)) != __stdin)
    x = CDR (x);
  return CAR (x);
}

SCM
open_input_file (SCM file_name)
{
  return MAKE_NUMBER (open (CSTRING (file_name), O_RDONLY));
}

SCM
open_input_string (SCM string)
{
  SCM port = MAKE_STRING_PORT (string);
  g_ports = cons (port, g_ports);
  return port;
}

SCM
set_current_input_port (SCM port)
{
  SCM prev = current_input_port ();
  if (TYPE (port) == TNUMBER)
    {
      int p = VALUE (port);
#if __M2_PLANET__
      if (p != 0)
#else
      if (p)
#endif
        __stdin = p;
      else
        __stdin = STDIN;
    }
  else if (TYPE (port) == TPORT)
    __stdin = PORT (port);
  return prev;
}

SCM
current_output_port ()
{
  return MAKE_NUMBER (__stdout);
}

SCM
current_error_port ()
{
  return MAKE_NUMBER (__stderr);
}

SCM
open_output_file (SCM x) ///((arity . n))
{
  SCM file_name = car (x);
  x = cdr (x);
  int mode = S_IRUSR|S_IWUSR;
  if (TYPE (x) == TPAIR && TYPE (car (x)) == TNUMBER)
    mode = VALUE (car (x));
  return MAKE_NUMBER (open (CSTRING (file_name), O_WRONLY|O_CREAT|O_TRUNC,mode));
}

SCM
set_current_output_port (SCM port)
{
#if __M2_PLANET__
  if (VALUE (port) != 0)
#else
  if (VALUE (port))
#endif
    __stdout = VALUE (port);
  else
    __stdout = STDOUT;
  return current_output_port ();
}

SCM
set_current_error_port (SCM port)
{
#if __M2_PLANET__
  if (VALUE (port) != 0)
#else
  if (VALUE (port))
#endif
    __stderr = VALUE (port);
  else
    __stderr = STDERR;
  return current_error_port ();
}

SCM
force_output (SCM p) ///((arity . n))
{
  return cell_unspecified;
}

SCM
chmod_ (SCM file_name, SCM mode) ///((name . "chmod"))
{
  chmod (CSTRING (file_name), VALUE (mode));
  return cell_unspecified;
}

SCM
isatty_p (SCM port)
{
#if __M2_PLANET__
  if (isatty (VALUE (port)) != 0)
#else
  if (isatty (VALUE (port)))
#endif
    return cell_t;
  return cell_f;
}

SCM
primitive_fork ()
{
  return MAKE_NUMBER (fork ());
}

SCM
execl_ (SCM file_name, SCM args) ///((name . "execl"))
{
#if __M2_PLANET__
  char *helper = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
  char **c_argv = helper[0];    // FIXME: how to?  will this work?
#else
  char *c_argv[1000];           // POSIX minimum 4096
#endif
  int i = 0;

  if (length__ (args) > 1000)
    error (cell_symbol_system_error,
           cons (file_name,
                 cons (MAKE_STRING0 ("too many arguments"),
                       cons (file_name, args))));
  c_argv[i] = CSTRING (file_name);
  i = i + 1;
  while (args != cell_nil)
    {
      assert (TYPE (CAR (args)) == TSTRING);
      c_argv[i] = CSTRING (CAR (args));
      i = i + 1;
      args = CDR (args);
      if (g_debug > 2)
        {
          eputs ("arg["); eputs (itoa (i)); eputs ("]: "); eputs (c_argv[i-1]); eputs ("\n");
        }
    }
  c_argv[i] = 0;
  return MAKE_NUMBER (execv (c_argv[0], c_argv));
}

SCM
waitpid_ (SCM pid, SCM options)
{
  int status;
  int child = waitpid (VALUE (pid), &status, VALUE (options));
  return cons (MAKE_NUMBER (child), MAKE_NUMBER (status));
}

#if __x86_64__
/* Nanoseconds on 64-bit systems with POSIX timers.  */
#define TIME_UNITS_PER_SECOND 1000000000
#else
/* Milliseconds for everyone else.  */
#define TIME_UNITS_PER_SECOND 1000
#endif

struct timespec g_start_time;
SCM
init_time (SCM a) ///((internal))
{
  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &g_start_time);
  a = acons (cell_symbol_internal_time_units_per_second, MAKE_NUMBER (TIME_UNITS_PER_SECOND), a);
}

SCM
current_time ()
{
  return MAKE_NUMBER (time (0));
}

SCM
gettimeofday_ () ///((name . "gettimeofday"))
{
  struct timeval time;
  gettimeofday (&time, 0);
  return cons (MAKE_NUMBER (time.tv_sec), MAKE_NUMBER (time.tv_usec));
}

long
seconds_and_nanoseconds_to_long (long s, long ns)
{
  return s * TIME_UNITS_PER_SECOND
    + ns / (1000000000 / TIME_UNITS_PER_SECOND);
}

SCM
get_internal_run_time ()
{
  struct timespec ts;
  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &ts);
  long time = seconds_and_nanoseconds_to_long (ts.tv_sec - g_start_time.tv_sec,
                                               ts.tv_nsec - g_start_time.tv_nsec);
  return MAKE_NUMBER (time);
}

SCM
getcwd_ () ///((name . "getcwd"))
{
#if __M2_PLANET__
  char buf = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
#else
  char buf[PATH_MAX];
#endif
  return MAKE_STRING0 (getcwd (buf, PATH_MAX));
}

SCM
dup_ (SCM port) ///((name . "dup"))
{
  return MAKE_NUMBER (dup (VALUE (port)));
}

SCM
dup2_ (SCM old, SCM new) ///((name . "dup2"))
{
  dup2 (VALUE (old), VALUE (new));
  return cell_unspecified;
}

SCM
delete_file (SCM file_name)
{
  unlink (CSTRING (file_name));
  return cell_unspecified;
}
