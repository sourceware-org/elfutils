# Copyright (C) 2022 Yonggang Luo
# This file is part of elfutils.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# elfutils is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

add_library(dw
    cfi.c
    cie.c
    dwarf_abbrev_hash.c
    dwarf_abbrevhaschildren.c
    dwarf_addrdie.c
    dwarf_aggregate_size.c
    dwarf_arrayorder.c
    dwarf_attr_integrate.c
    dwarf_attr.c
    dwarf_begin_elf.c
    dwarf_begin.c
    dwarf_bitoffset.c
    dwarf_bitsize.c
    dwarf_bytesize.c
    dwarf_cfi_addrframe.c
    dwarf_cfi_end.c
    dwarf_child.c
    dwarf_cu_die.c
    dwarf_cu_getdwarf.c
    dwarf_cu_info.c
    dwarf_cuoffset.c
    dwarf_decl_column.c
    dwarf_decl_file.c
    dwarf_decl_line.c
    dwarf_default_lower_bound.c
    dwarf_die_addr_die.c
    dwarf_diecu.c
    dwarf_diename.c
    dwarf_dieoffset.c
    dwarf_end.c
    dwarf_entry_breakpoints.c
    dwarf_entrypc.c
    dwarf_error.c
    dwarf_filesrc.c
    dwarf_formaddr.c
    dwarf_formblock.c
    dwarf_formflag.c
    dwarf_formref_die.c
    dwarf_formref.c
    dwarf_formsdata.c
    dwarf_formstring.c
    dwarf_formudata.c
    dwarf_frame_cfa.c
    dwarf_frame_info.c
    dwarf_frame_register.c
    dwarf_func_inline.c
    dwarf_get_units.c
    dwarf_getabbrev.c
    dwarf_getabbrevattr.c
    dwarf_getabbrevcode.c
    dwarf_getabbrevtag.c
    dwarf_getalt.c
    dwarf_getarange_addr.c
    dwarf_getarangeinfo.c
    dwarf_getaranges.c
    dwarf_getattrcnt.c
    dwarf_getattrs.c
    dwarf_getcfi_elf.c
    dwarf_getcfi.c
    dwarf_getelf.c
    dwarf_getfuncs.c
    dwarf_getlocation_attr.c
    dwarf_getlocation_die.c
    dwarf_getlocation_implicit_pointer.c
    dwarf_getlocation.c
    dwarf_getmacros.c
    dwarf_getpubnames.c
    dwarf_getscopes_die.c
    dwarf_getscopes.c
    dwarf_getscopevar.c
    dwarf_getsrc_die.c
    dwarf_getsrc_file.c
    dwarf_getsrcdirs.c
    dwarf_getsrcfiles.c
    dwarf_getsrclines.c
    dwarf_getstring.c
    dwarf_hasattr_integrate.c
    dwarf_hasattr.c
    dwarf_haschildren.c
    dwarf_hasform.c
    dwarf_haspc.c
    dwarf_highpc.c
    dwarf_line_file.c
    dwarf_lineaddr.c
    dwarf_linebeginstatement.c
    dwarf_lineblock.c
    dwarf_linecol.c
    dwarf_linecontext.c
    dwarf_linediscriminator.c
    dwarf_lineendsequence.c
    dwarf_lineepiloguebegin.c
    dwarf_linefunctionname.c
    dwarf_lineisa.c
    dwarf_lineno.c
    dwarf_lineop_index.c
    dwarf_lineprologueend.c
    dwarf_linesrc.c
    dwarf_lowpc.c
    dwarf_macro_getparamcnt.c
    dwarf_macro_getsrcfiles.c
    dwarf_macro_opcode.c
    dwarf_macro_param.c
    dwarf_macro_param1.c
    dwarf_macro_param2.c
    dwarf_next_cfi.c
    dwarf_next_lines.c
    dwarf_nextcu.c
    dwarf_offabbrev.c
    dwarf_offdie.c
    dwarf_onearange.c
    dwarf_onesrcline.c
    dwarf_peel_type.c
    dwarf_ranges.c
    dwarf_setalt.c
    dwarf_siblingof.c
    dwarf_sig8_hash.c
    dwarf_srclang.c
    dwarf_tag.c
    dwarf_whatattr.c
    dwarf_whatform.c
    fde.c
    frame-cache.c
    libdw_alloc.c
    libdw_find_split_unit.c
    libdw_findcu.c
    libdw_form.c
    libdw_visit_scopes.c
)
target_include_directories(
    dw PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdwelf
)
set_property(TARGET dw PROPERTY C_STANDARD 11)
target_link_libraries(dw eu ebl ebl_backends cpu dwelf elf)
