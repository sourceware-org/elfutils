/* Get return address register for frame.
   Copyright (C) 2009 Red Hat, Inc.
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "cfi.h"
#include "../libebl/libebl.h"

Dwarf_Addr
dwarf_frame_state_pc (Dwarf_Frame_State *state)
{
  Dwarf_CIE abi_info;
  unsigned ra;

  if (ebl_abi_cfi (state->base->ebl, &abi_info) != 0)
    {
      __libdw_seterrno (DWARF_E_UNKNOWN_ERROR);
      return 0;
    }
  ra = abi_info.return_address_register;
  if (ra >= state->base->nregs)
    {
      __libdw_seterrno (DWARF_E_UNKNOWN_ERROR);
      return 0;
    }
  if ((state->regs_set & (1U << ra)) == 0)
    {
      __libdw_seterrno (DWARF_E_RA_UNDEFINED);
      return 0;
    }
  __libdw_seterrno (DWARF_E_NOERROR);
  return state->regs[ra];
}