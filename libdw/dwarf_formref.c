/* Return reference offset represented by attribute.
   Copyright (C) 2003-2010 Red Hat, Inc.
   This file is part of elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2003.

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

#include <dwarf.h>
#include "libdwP.h"

int
__libdw_formref (attr, return_offset)
     Dwarf_Attribute *attr;
     Dwarf_Off *return_offset;
{
  const unsigned char *datap;

  if (attr->valp == NULL)
    {
      __libdw_seterrno (DWARF_E_INVALID_REFERENCE);
      return -1;
    }

  switch (attr->form)
    {
    case DW_FORM_ref1:
      *return_offset = *attr->valp;
      break;

    case DW_FORM_ref2:
      *return_offset = read_2ubyte_unaligned (attr->cu->dbg, attr->valp);
      break;

    case DW_FORM_ref4:
      *return_offset = read_4ubyte_unaligned (attr->cu->dbg, attr->valp);
      break;

    case DW_FORM_ref8:
      *return_offset = read_8ubyte_unaligned (attr->cu->dbg, attr->valp);
      break;

    case DW_FORM_ref_udata:
      datap = attr->valp;
      get_uleb128 (*return_offset, datap);
      break;

    case DW_FORM_ref_addr:
    case DW_FORM_ref_sig8:
      __libdw_seterrno (DWARF_E_INVALID_REFERENCE);
      return -1;

    default:
      __libdw_seterrno (DWARF_E_NO_REFERENCE);
      return -1;
    }

  return 0;
}

/* This is the old public entry point.
   It is now deprecated in favor of dwarf_formref_die.  */
int
dwarf_formref (attr, return_offset)
     Dwarf_Attribute *attr;
     Dwarf_Off *return_offset;
{
  if (attr == NULL)
    return -1;

  return __libdw_formref (attr, return_offset);
}
