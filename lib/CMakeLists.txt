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

add_library(eu STATIC
    color.c
    crc32.c
    crc32_file.c
    error.c
    next_prime.c
    printversion.c
    system.c
    xasprintf.c
    xmalloc.c
    xstrdup.c
    xstrndup.c
)
if (MSVC)
    target_sources(eu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../config-cmake/win32-msvc/dirent.c)
endif()
set_property(TARGET eu PROPERTY C_STANDARD 11)
if (TARGET PThreads4W::PThreads4W)
    target_link_libraries(eu PUBLIC PThreads4W::PThreads4W)
endif()
