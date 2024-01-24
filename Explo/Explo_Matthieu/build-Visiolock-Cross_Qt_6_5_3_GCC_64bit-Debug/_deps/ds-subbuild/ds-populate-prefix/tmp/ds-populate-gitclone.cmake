# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitclone-lastrun.txt" AND EXISTS "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitinfo.txt" AND
  "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --config "advice.detachedHead=false" "https://code.qt.io/qt-labs/qtquickdesigner-components.git" "ds-src"
    WORKING_DIRECTORY "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://code.qt.io/qt-labs/qtquickdesigner-components.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "qds-4.3" --
  WORKING_DIRECTORY "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'qds-4.3'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitinfo.txt" "/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/matthieu/Documents/PFE/PFE.code/Explo/Explo_Matthieu/build-Visiolock-Cross_Qt_6_5_3_GCC_64bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/ds-populate-gitclone-lastrun.txt'")
endif()