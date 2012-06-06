/* Update library in table at the given index.
   Copyright (C) 2004 Red Hat, Inc.
   This file is part of elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2004.

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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <gelf.h>
#include <stdlib.h>
#include <string.h>

#include "libelfP.h"


int
gelf_update_lib (data, ndx, src)
     Elf_Data *data;
     int ndx;
     GElf_Lib *src;
{
  if (data == NULL)
    return 0;

  if (unlikely (ndx < 0))
    {
      __libelf_seterrno (ELF_E_INVALID_INDEX);
      return 0;
    }

  Elf_Data_Scn *data_scn = (Elf_Data_Scn *) data;
  if (unlikely (data_scn->d.d_type != ELF_T_LIB))
    {
      /* The type of the data better should match.  */
      __libelf_seterrno (ELF_E_DATA_MISMATCH);
      return 0;
    }

  Elf_Scn *scn = data_scn->s;
  rwlock_wrlock (scn->elf->lock);

  /* Check whether we have to resize the data buffer.  */
  int result = 0;
  if (unlikely ((ndx + 1) * sizeof (Elf64_Lib) > data_scn->d.d_size))
    __libelf_seterrno (ELF_E_INVALID_INDEX);
  else
    {
      ((Elf64_Lib *) data_scn->d.d_buf)[ndx] = *src;

      result = 1;

      /* Mark the section as modified.  */
      scn->flags |= ELF_F_DIRTY;
    }

  rwlock_unlock (scn->elf->lock);

  return result;
}
