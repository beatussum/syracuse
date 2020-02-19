# Copyright (C) 2020 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})

set(SYRACUSE_UI)
set(SYRACUSE_HPP
    core/core.hpp
    core/Sequence.hpp)
set(SYRACUSE_CPP
    main.cpp
    core/Sequence.cpp)

set(DOCS_DIR "${CMAKE_BINARY_DIR}/docs" CACHE PATH
    "The path where the documention is built")
set(DOCS_NAMESPACE "fr.beatussum.syracuse")

find_package(Qt5 REQUIRED
             COMPONENTS Gui Widgets)
find_package(Doxygen REQUIRED)
find_package(Threads REQUIRED)

configure_file(config-syracuse.hpp.in
               "${CMAKE_CURRENT_BINARY_DIR}/config-syracuse.hpp")
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)

add_executable(syracuse ${SYRACUSE_UI} ${SYRACUSE_HPP} ${SYRACUSE_CPP})
target_link_libraries(syracuse Qt5::Gui Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT})

if(DOXYGEN_FOUND AND WITH_DOCS)
    set(DOXYGEN_OUTPUT_DIRECTORY ${DOCS_DIR})
    set(DOXYGEN_ALLOW_UNICODE_NAMES YES)
    set(DOXYGEN_REPEAT_BRIEF NO)
    set(DOXYGEN_INLINE_INHERITED_MEMB YES)
    set(DOXYGEN_HIDE_UNDOC_CLASSES YES)
    set(DOXYGEN_HIDE_IN_BODY_DOCS YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_LOGFILE "${DOCS_DIR}/log")
    set(DOXYGEN_EXCLUDE_PATTERNS "${DOXYGEN_EXCLUDE_PATTERNS} *.in")

    string(CONCAT DOXYGEN_ALIASES_MUSTINIT
           "@warning "
           "To use this method, \\p \\1 must be initialized, or otherwise an "
           "`std::invalid_argument` will be thrown.")
    set(DOXYGEN_ALIASES "mustinit{1} = ${DOXYGEN_ALIASES_MUSTINIT}")

    doxygen_add_docs(docs
                     ${SYRACUSE_HPP}
                     COMMENT "Generating documentation…")
endif()
