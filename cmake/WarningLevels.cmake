#-------------------------------------------------------------------------------
# WarningLevels.cmake
#
# Copyright (c) 2017 RWTH Aachen University, Germany,
# Virtual Reality & Immersive Visualization Group.
#-------------------------------------------------------------------------------
#                                 License
#
# Licensed under the 3-Clause BSD License (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://opensource.org/licenses/BSD-3-Clause
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------

macro(SetWarningLevelRWTH target)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND
      CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${target} PRIVATE
      -Werror
      -Weverything
      -pedantic
      -pedantic-errors
      -Wextra-tokens
      -Wambiguous-member-template
      -Wbind-to-temporary-copy
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      )
  elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC" AND
	  CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	target_compile_options(${target} PRIVATE
	  # Options -W1, -W2, -W3, -W4, -Wall for <- less | more -> warnings. -WX makes the compiler treat warnings like errors
	  -W4
	  -WX
    -wd4127
	  )
  target_compile_definitions(${target} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
  else ()
    message(WARNING "SetWarningLevelRWTH not implemented for your compiler. "
      "Implement it to support our no-warning policy.")
  endif()
endmacro()

macro(SetWarningLevelNone target)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND
      CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${target} PRIVATE
      -w
      )
  elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC" AND
	  CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	target_compile_options(${target} PRIVATE
	  # Options -W1, -W2, -W3, -W4, -Wall for <- less | more -> warnings. -WX makes the compiler treat warnings like errors
	  -W0
	  )
  else ()
    message(WARNING "SetWarningLevelRWTH not implemented for your compiler. "
      "Implement it to support our no-warning policy.")
  endif()
endmacro()
