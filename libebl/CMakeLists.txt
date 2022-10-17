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

add_library(ebl STATIC
    ebl_check_special_section.c
    ebl_check_special_symbol.c
    ebl_data_marker_symbol.c
    eblabicfi.c
    eblauxvinfo.c
    eblbackendname.c
    eblbsspltp.c
    eblcheckobjattr.c
    eblcheckreloctargettype.c
    eblclosebackend.c
    eblcopyrelocp.c
    eblcorenote.c
    eblcorenotetypename.c
    ebldebugscnp.c
    ebldwarftoregno.c
    ebldynamictagcheck.c
    ebldynamictagname.c
    eblelfclass.c
    eblelfdata.c
    eblelfmachine.c
    eblgotpcreloccheck.c
    eblinitreg.c
    eblmachineflagcheck.c
    eblmachineflagname.c
    eblmachinesectionflagcheck.c
    eblnonerelocp.c
    eblnormalizepc.c
    eblobjnote.c
    eblobjnotetypename.c
    eblopenbackend.c
    eblosabiname.c
    eblreginfo.c
    eblrelativerelocp.c
    eblrelocsimpletype.c
    eblreloctypecheck.c
    eblreloctypename.c
    eblrelocvaliduse.c
    eblresolvesym.c
    eblretval.c
    eblsectionname.c
    eblsectionstripp.c
    eblsectiontypename.c
    eblsegmenttypename.c
    eblstother.c
    eblsymbolbindingname.c
    eblsymboltypename.c
    eblsysvhashentrysize.c
    eblunwind.c
)
set(EBL_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/libebl.h
)
set_target_properties(
    ebl PROPERTIES PUBLIC_HEADER
    "${EBL_PUBLIC_HEADERS}"
)
target_include_directories(
    ebl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../libasm
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdw
)
set_property(TARGET ebl PROPERTY C_STANDARD 11)
target_link_libraries(ebl PUBLIC eu)
