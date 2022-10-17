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

add_library(ebl_backends STATIC
    aarch64_cfi.c
    aarch64_corenote.c
    aarch64_init.c
    aarch64_initreg.c
    aarch64_regs.c
    aarch64_retval.c
    aarch64_symbol.c
    aarch64_unwind.c
    alpha_auxv.c
    alpha_corenote.c
    alpha_init.c
    alpha_regs.c
    alpha_retval.c
    alpha_symbol.c
    arm_attrs.c
    arm_auxv.c
    arm_cfi.c
    arm_corenote.c
    arm_init.c
    arm_initreg.c
    arm_machineflagname.c
    arm_regs.c
    arm_retval.c
    arm_symbol.c
    bpf_init.c
    bpf_regs.c
    bpf_symbol.c
    csky_attrs.c
    csky_cfi.c
    csky_corenote.c
    csky_init.c
    csky_initreg.c
    csky_regs.c
    csky_symbol.c
    i386_auxv.c
    i386_cfi.c
    i386_corenote.c
    i386_init.c
    i386_initreg.c
    i386_regs.c
    i386_retval.c
    i386_symbol.c
    i386_unwind.c
    ia64_init.c
    ia64_regs.c
    ia64_retval.c
    ia64_symbol.c
    m68k_cfi.c
    m68k_corenote.c
    m68k_init.c
    m68k_initreg.c
    m68k_regs.c
    m68k_retval.c
    m68k_symbol.c
    ppc_attrs.c
    ppc_auxv.c
    ppc_cfi.c
    ppc_corenote.c
    ppc_init.c
    ppc_initreg.c
    ppc_regs.c
    ppc_retval.c
    ppc_symbol.c
    ppc64_corenote.c
    ppc64_init.c
    ppc64_resolve_sym.c
    ppc64_retval.c
    ppc64_symbol.c
    ppc64_unwind.c
    riscv_cfi.c
    riscv_corenote.c
    riscv_init.c
    riscv_initreg.c
    riscv_regs.c
    riscv_retval.c
    riscv_symbol.c
    riscv64_corenote.c
    s390_cfi.c
    s390_corenote.c
    s390_init.c
    s390_initreg.c
    s390_regs.c
    s390_retval.c
    s390_symbol.c
    s390_unwind.c
    s390x_corenote.c
    sh_corenote.c
    sh_init.c
    sh_regs.c
    sh_retval.c
    sh_symbol.c
    sparc_attrs.c
    sparc_auxv.c
    sparc_cfi.c
    sparc_corenote.c
    sparc_init.c
    sparc_initreg.c
    sparc_regs.c
    sparc_retval.c
    sparc_symbol.c
    sparc64_corenote.c
    x32_corenote.c
    x86_64_cfi.c
    x86_64_corenote.c
    x86_64_init.c
    x86_64_initreg.c
    x86_64_regs.c
    x86_64_retval.c
    x86_64_symbol.c
    x86_64_unwind.c
)
target_include_directories(
    ebl_backends PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../libasm
    ${CMAKE_CURRENT_SOURCE_DIR}/../libdw
    ${CMAKE_CURRENT_SOURCE_DIR}/../libebl
)
set_property(TARGET ebl_backends PROPERTY C_STANDARD 11)
target_link_libraries(ebl_backends PUBLIC eu)
