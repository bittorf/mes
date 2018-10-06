# GNU Mes --- Maxwell Equations of Software
# Copyright © 2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
#
# This file is part of GNU Mes.
#
# GNU Mes is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# GNU Mes is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.

export PACKAGE
export VERSION

export abs_top_builddir
export abs_top_srcdir
export top_builddir
export config.make
export srcdest
export srcdir

export prefix

export datadir
export docdir

export bindir
export guile_site_ccache_dir
export guile_site_dir
export infodir
export libdir
export mandir
export moduledir
export sysconfdir

ifdef V
export V
endif

ifdef DESTDIR
export DESTDIR
endif

ifdef ARCH
export ARCH
endif

ifdef CC
export CC
endif

ifdef CC32
export CC32
endif

ifdef CC64
export CC64
endif

ifdef BLOOD_ELF
export BLOOD_ELF
endif

ifdef DOT
export DOT
endif

ifdef M1
export M1
endif

ifdef MES
export MES
endif

ifdef HEX2
export HEX2
endif

ifdef HELP2MAN
export HELP2MAN
endif

ifdef GUILE
export GUILE
endif

ifdef GUILE_TOOLS
export GUILE_TOOLS
endif

ifdef GUIX
export GUIX
endif

ifdef PERL
export PERL
endif

ifdef TCC
export TCC
endif

ifdef GUILE_LOAD_PATH
export GUILE_LOAD_PATH
endif

ifdef GUILE_LOAD_COMPILED_PATH
export GUILE_LOAD_COMPILED_PATH
endif

ifdef CFLAGS
export CFLAGS
endif

ifdef CPPFLAGS
export CPPFLAGS
endif

ifdef CC32_CFLAGS
export CC32_CFLAGS
endif

ifdef CC64_CFLAGS
export CC64_CFLAGS
endif

ifdef HEX2FLAGS
export HEX2FLAGS
endif

ifdef M1FLAGS
export M1FLAGS
endif

ifdef MES_CFLAGS
export MES_CFLAGS
endif

ifdef MES_SEED
export MES_SEED
endif

ifdef MESCC_TOOLS_SEED
export MESCC_TOOLS_SEED
endif

ifdef TINYCC_SEED
export TINYCC_SEED
endif

ifdef TINYCC_PREFIX
export TINYCC_PREFIX
endif