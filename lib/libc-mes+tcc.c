/* -*-comment-start: "//";comment-end:""-*-
 * Mes --- Maxwell Equations of Software
 * Copyright © 2017 Jan Nieuwenhuizen <janneke@gnu.org>
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

#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>

#if !__GNUC__ && !__TINYC__
#include <libc-mes.c>
#include <getopt.c>

int errno;

int
close (int fd)
{
  asm ("mov____0x8(%ebp),%ebx !8");

  asm ("mov____$i32,%eax SYS_close");
  asm ("int____$0x80");
}

int
unlink (char const *file_name)
{
  asm ("mov____0x8(%ebp),%ebx !8");

  asm ("mov____$i32,%eax SYS_unlink");
  asm ("int____$0x80");
}

off_t
lseek (int fd, off_t offset, int whence)
{
  asm ("mov____0x8(%ebp),%ebx !8");
  asm ("mov____0x8(%ebp),%ecx !12");
  asm ("mov____0x8(%ebp),%edx !16");

  asm ("mov____$i32,%eax SYS_lseek");
  asm ("int____$0x80");
}

char *
getcwd (char *buf, size_t size)
{
  asm ("mov____0x8(%ebp),%ebx !8");
  asm ("mov____0x8(%ebp),%ecx !12");

  asm ("mov____$i32,%eax SYS_getcwd");
  asm ("int____$0x80");
}
#endif // !__GNUC__


int
dlclose (void *handle)
{
  return 0;
}

void *
dlopen (char const *filename, int flags)
{
  return 0;
}

int
execvp (char const *file, char *const argv[])
{
  eputs ("execvp stub\n");
  return 0;
}

int
fclose (FILE *stream)
{
  int fd = (int)stream;
  return close (fd);
}

FILE *
fdopen (int fd, char const *mode)
{
  return (FILE*)fd;
}

int
ferror (FILE *stream)
{
  int fd = (int)stream;
  if (fd == -1) return -1;
  return 0;
}

int
fflush (FILE *stream)
{
  eputs ("fflush stub\n");
  return 0;
}

FILE *
fopen (char const *pathname, char const *mode)
{
  eputs ("fopen stub\n");
  return 0;
}

int
fprintf (FILE *stream, char const *format, ...)
{
  va_list ap;
  va_start (ap, format);
  int r = vfprintf (stream, format, ap);
  va_end (ap);
  return r;
}

size_t
fread (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  eputs ("fread stub\n");
  return 0;
}

int
fseek (FILE *stream, long offset, int whence)
{
  eputs ("fseek stub\n");
  return 0;
}

long
ftell (FILE *stream)
{
  eputs ("ftell stub\n");
  return 0;
}

size_t
fwrite (void const *ptr, size_t size, size_t nmemb, FILE *stream)
{
  int fd = (int)stream;
  return write (fd, ptr, size * nmemb);
}

int
gettimeofday (struct timeval *tv, struct timezone *tz)
{
  return 0;
}

struct tm *
localtime (time_t const *timep)
{
  eputs ("localtime stub\n");
  return 0;
}

void
longjmp (jmp_buf env, int val)
{
  val = val == 0 ? 1 : val;
#if __MESC__
  asm ("mov____0x8(%ebp),%eax !0x0c"); // val
  asm ("mov____0x8(%ebp),%ebp !0x08"); // env*

  asm ("mov____0x8(%ebp),%ebx !0x4");  // env.__pc
  asm ("mov____0x8(%ebp),%esp !0x8");  // env.__sp
  asm ("mov____0x8(%ebp),%ebp !0x0");  // env.__bp
  asm ("jmp____*%ebx");
#else
  asm ("mov    0xc(%ebp),%eax\n\t"     // val
       "mov    0x8(%ebp),%ebp\n\t"     // env*

       "mov    0x4(%ebp),%ebx\n\t"     // env->__pc
       "mov    0x8(%ebp),%esp\n\t"     // env->__sp
       "mov    0x0(%ebp),%ebp\n\t"     // env->__bp
       "jmp    *%ebx\n\t"              // jmp *PC
       );
#endif
  // not reached
  exit (42);
}

void *
memmove (void *dest, void const *src, size_t n)
{
  if (dest < src)
    return memcpy (dest, src, n);
  char *p = dest + n;
  char const *q = src +n;
  while (n--)
    *--p = *--q;
  return dest;
}

void *
memset (void *s, int c, size_t n)
{
  char *p = s;
  while (n--) *p++ = c;
  return s;
}

int
memcmp (void const *s1, void const *s2, size_t n)
{
  char *a = s1;
  char *b = s2;
  while (*a == *b && --n) {a++;b++;}
  return *a - *b;
}

int
mprotect (void *addr, size_t len, int prot)
{
  return 0;
}

void
qsort (void *base, size_t nmemb, size_t size, int (*compar)(void const *, void const *))
{
  eputs ("qsort stub\n");
}

int
remove (char const *file_name)
{
  eputs ("remove stub\n");
  return 0;
}

#if 0
int
setjmp_debug (jmp_buf env, int val)
{
  int i;
#if 1
  i = env->__bp;
  i = env->__pc;
  i = env->__sp;
#else
  i = env[0].__bp;
  i = env[0].__pc;
  i = env[0].__sp;
#endif
  return val == 0 ? 1 : val;
}
#endif

#if __MESC__
int
setjmp (__jmp_buf *env)
#else
int
setjmp (jmp_buf env)
#endif
{
  int *p = (int*)&env;
  env[0].__bp = p[-2];
  env[0].__pc = p[-1];
  env[0].__sp = (int)&env;
  return 0;
}

int
sigaction (int signum, struct sigaction const *act, struct sigaction *oldact)
{
  return 0;
}

int
sigemptyset (sigset_t *set)
{
  return 0;
}

int
snprintf(char *str,  size_t size,  char const *format, ...)
{
  va_list ap;
  va_start (ap, format);
  int r = vsprintf (str, format, ap);
  va_end (ap);
  return r;
}

int
sscanf (char const *str, const char *format, ...)
{
  eputs ("sscanf stub\n");
  return 0;
}

char *
strcat (char *dest, char const *src)
{
  char *p = strchr (dest, '\0');
  while (*src++) *p++ = *src++;
  *p = 0;
  return dest;
}

char *
strchr (char const *s, int c)
{
  char const *p = s;
  while (*p || !c)
    {
      if (c == *p) return p;
      *p++;
    }
  return 0;
}

char *
strrchr (char const *s, int c)
{
  int n = strlen (s);
  if (!n) return 0;
  char const *p = s + n - 1;
  while (*p || !c)
    {
      if (c == *p) return p;
      *p--;
    }
  return 0;
}

char *
strstr (char const *haystack, char const *needle)
{
  eputs ("strstr stub\n");
  return 0;
}

long
strtol (char const *nptr, char **endptr, int base)
{
  eputs ("strtol stub\n");
  if (!strncmp (nptr, "0x", 2))
    {
      char const *p = nptr + 2;
      return _atoi (&p, 16);
    }
  return _atoi (&nptr, base);
}

long long int
strtoll (char const *nptr, char **endptr, int base)
{
  eputs ("strtoll stub\n");
  return 0;
}

unsigned long
strtoul (char const *nptr, char **endptr, int base)
{
  eputs ("strtoul stub\n");
  return 0;
}

unsigned long long
strtoull (char const *p, char **end, int base)
{
  *end = p;
  return _atoi (end, base);
}

time_t time (time_t *tloc)
{
  return 0;
}

int
vsnprintf (char *str, size_t size, char const *format, va_list ap)
{
  return vsprintf (str, format, ap);
}

void *
calloc (size_t nmemb, size_t size)
{
  size_t count = nmemb * size;
  void *p = malloc (count);
  memset (p, 0, count);
  return p;
}

int
vfprintf (FILE* f, char const* format, va_list ap)
{
  int fd = (int)f;
  char const *p = format;
  while (*p)
    if (*p != '%')
      putchar (*p++);
    else
      {
        p++;
        char c = *p;
        switch (c)
          {
          case '%': {fputc (*p, fd); break;}
          case 'c': {char c; c = va_arg (ap, char); fputc (c, fd); break;}
          case 'd': {int d; d = va_arg (ap, int); fputs (itoa (d), fd); break;}
          case 's': {char *s; s = va_arg (ap, char *); fputs (s, fd); break;}
          default: {fputc (*p, fd); break;}
          }
        p++;
      }
  va_end (ap);
  return 0;
}