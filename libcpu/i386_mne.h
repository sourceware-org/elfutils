/* Compute hash value for given string according to ELF standard.
   Copyright (C) 1995-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _I386_MNE_H
#define _I386_MNE_H	1

#ifndef MNEFILE
# define MNEFILE "i386.mnemonics"
#endif

/* The index can be stored in the instrtab.  */
enum
  {
#define MNE(name) MNE_##name,
#include MNEFILE
#undef MNE
    MNE_INVALID,
    MNE_COUNT = MNE_INVALID,
  };

#endif /* i386_mne.h */
