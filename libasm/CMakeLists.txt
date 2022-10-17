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

add_library(asm
    asm_abort.c
    asm_addint16.c
    asm_addint32.c
    asm_addint64.c
    asm_addint8.c
    asm_addsleb128.c
    asm_addstrz.c
    asm_adduint16.c
    asm_adduint32.c
    asm_adduint64.c
    asm_adduint8.c
    asm_adduleb128.c
    asm_align.c
    asm_begin.c
    asm_end.c
    asm_error.c
    asm_fill.c
    asm_getelf.c
    asm_newabssym.c
    asm_newcomsym.c
    asm_newscn_ingrp.c
    asm_newscn.c
    asm_newscngrp.c
    asm_newsubscn.c
    asm_newsym.c
    asm_scngrp_newsignature.c
    disasm_begin.c
    disasm_cb.c
    disasm_end.c
    disasm_str.c
    symbolhash.c
)
target_include_directories(
    asm PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../libasm
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdw
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdwelf
    ${CMAKE_CURRENT_SOURCE_DIR}/../libebl
)
set_property(TARGET asm PROPERTY C_STANDARD 11)
target_link_libraries(asm PUBLIC eu)
target_link_libraries(asm PRIVATE dwelf ebl elf)
