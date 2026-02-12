# Install script for directory: D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 1 - Render to PNG/cmake_install.cmake")
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 2 - Basic App/cmake_install.cmake")
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 3 - Resizable App/cmake_install.cmake")
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 4 - JavaScript/cmake_install.cmake")
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 5 - File Loading/cmake_install.cmake")
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 6 - Intro to C API/cmake_install.cmake")
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 7 - OpenGL Integration/cmake_install.cmake")
  include("D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/Sample 8 - Web Browser/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/Projects/_Libraries/ultralight-1.2.1-winx64/samples/out/build/x64-Debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
