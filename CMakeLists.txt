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

cmake_minimum_required(VERSION 3.16)
project(elfutils
    LANGUAGES C
    VERSION 0.187
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libelf
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/config-cmake
)
if (MSVC)
    add_definitions(
        -D_CRT_NONSTDC_NO_DEPRECATE
        -D_CRT_SECURE_NO_WARNINGS
        -D_CRT_SECURE_NO_WARNINGS_GLOBALS
    )
else()
    add_definitions(
        -D_FILE_OFFSET_BITS=64
    )
endif()
add_definitions(-DHAVE_CONFIG_H)
if (WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config-cmake/win32)
    if (MSVC)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config-cmake/win32-msvc)
    else()
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config-cmake/win32-gnu)
    endif()
endif()

if (MSVC)
    # Enable find libz.a with msvc
    set(CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".a")
endif()
find_package(Intl)
find_package(ZLIB REQUIRED)
if (MSVC)
    find_package(PThreads4W REQUIRED)
endif()

if (WIN32 AND MSVC)
    if (TARGET PThreads4W::PThreads4W)
        set(ENABLE_LIBDW ON)
    endif()
else()
    set(ENABLE_LIBDW ON)
endif()

if (NOT WIN32)
    add_compile_options(-fPIC)
endif()

add_subdirectory(lib)
add_subdirectory(libelf)
if (ENABLE_LIBDW)
    add_subdirectory(backends)
    add_subdirectory(libebl)
    add_subdirectory(libdwelf)
    add_subdirectory(libdwfl)
    add_subdirectory(libcpu)
    add_subdirectory(libasm)
    add_subdirectory(libdw)
endif()

include(GNUInstallDirs)

if (LIBELF_INSTALL)
    # Install the targets
    install(TARGETS elf
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libelf
    )
    set(VERSION ${CMAKE_PROJECT_VERSION})
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/lib")
    set(includedir "\${prefix}/include/libelf")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/libelf.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/libelf.pc
        @ONLY
    )
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/libelf.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif()
