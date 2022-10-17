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

add_library(cpu STATIC
    bpf_disasm.c
    i386_disasm.c
    i386_gendis.c
    i386_lex.c
    i386_parse.c
    riscv_disasm.c
    x86_64_disasm.c
)
target_include_directories(
    cpu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../libasm
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdw
    ${CMAKE_CURRENT_SOURCE_DIR}/../libebl
)
set_property(TARGET cpu PROPERTY C_STANDARD 11)
target_link_libraries(cpu PUBLIC eu)
