# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.
cmake_minimum_required(VERSION 3.10.2)
project(libelf
    LANGUAGES C
    VERSION 0.187
)

add_subdirectory(libelf)

if (LIBELF_INSTALL)
    # Install the targets
    install(TARGETS elf
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
