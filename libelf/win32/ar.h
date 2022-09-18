/* Copyright (C) 2022 Yonggang Luo
   This file is part of elfutils.

   This file is free software; you can redistribute it and/or modify
   it under the terms of either

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version

   or both in parallel, as here.

   elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>.  */

#ifndef _AR_H
#define _AR_H

#ifdef __cplusplus
extern "C" {
#endif

#define ARMAG "!<arch>\n"
#define SARMAG 8
#define ARFMAG "`\n"

struct ar_hdr {
	char ar_name[16];
	char ar_date[12];
	char ar_uid[6], ar_gid[6];
	char ar_mode[8];
	char ar_size[10];
	char ar_fmag[2];
};

#ifdef __cplusplus
}
#endif

#endif
