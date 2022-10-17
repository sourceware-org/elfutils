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

add_library(dwelf STATIC
    dwelf_dwarf_gnu_debugaltlink.c
    dwelf_elf_begin.c
    dwelf_elf_e_machine_string.c
    dwelf_elf_gnu_build_id.c
    dwelf_elf_gnu_debuglink.c
    dwelf_scn_gnu_compressed_size.c
    dwelf_strtab.c
)
target_include_directories(
    dwelf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdw
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdwfl
    ${CMAKE_CURRENT_SOURCE_DIR}/../libebl
)
set_property(TARGET dwelf PROPERTY C_STANDARD 11)
if (TARGET PThreads4W::PThreads4W)
    target_link_libraries(dwelf PUBLIC PThreads4W::PThreads4W)
endif()