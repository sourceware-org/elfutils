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

add_library(dwfl STATIC
    argp-std.c
    core-file.c
    cu.c
    debuginfod-client.c
    derelocate.c
    dwfl_addrdie.c
    dwfl_addrdwarf.c
    dwfl_addrmodule.c
    dwfl_begin.c
    dwfl_build_id_find_debuginfo.c
    dwfl_build_id_find_elf.c
    dwfl_cumodule.c
    dwfl_dwarf_line.c
    dwfl_end.c
    dwfl_error.c
    dwfl_frame_pc.c
    dwfl_frame_regs.c
    dwfl_frame.c
    dwfl_getdwarf.c
    dwfl_getmodules.c
    dwfl_getsrc.c
    dwfl_getsrclines.c
    dwfl_line_comp_dir.c
    dwfl_linecu.c
    dwfl_lineinfo.c
    dwfl_linemodule.c
    dwfl_module_addrdie.c
    dwfl_module_addrname.c
    dwfl_module_addrsym.c
    dwfl_module_build_id.c
    dwfl_module_dwarf_cfi.c
    dwfl_module_eh_cfi.c
    dwfl_module_getdwarf.c
    dwfl_module_getelf.c
    dwfl_module_getsrc_file.c
    dwfl_module_getsrc.c
    dwfl_module_getsym.c
    dwfl_module_info.c
    dwfl_module_nextcu.c
    dwfl_module_register_names.c
    dwfl_module_report_build_id.c
    dwfl_module_return_value_location.c
    dwfl_module.c
    dwfl_nextcu.c
    dwfl_onesrcline.c
    dwfl_report_elf.c
    dwfl_segment_report_module.c
    dwfl_validate_address.c
    dwfl_version.c
    elf-from-memory.c
    find-debuginfo.c
    frame_unwind.c
    gzip.c
    image-header.c
    libdwfl_crc32_file.c
    libdwfl_crc32.c
    lines.c
    link_map.c
    linux-core-attach.c
    linux-pid-attach.c
    offline.c
    open.c
    relocate.c
    segment.c
)
target_include_directories(
    dwfl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdw
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdwelf
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdwfl
    ${CMAKE_CURRENT_SOURCE_DIR}/../libebl
)
set_property(TARGET dwfl PROPERTY C_STANDARD 11)
target_link_libraries(dwfl PUBLIC eu ZLIB::ZLIB)
