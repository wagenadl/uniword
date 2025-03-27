# CMakeLists.txt - Part of Uniword, (C) Daniel Wagenaar 2021

# NEW BUILD INSTRUCTIONS (as of version 0.2.0)
# Type:
#   cd build
#   cmake ..
# to build UNIWORD.


######################################################################
cmake_minimum_required(VERSION 3.11.0)

######################################################################
# Set the project name and version
project(Uniword VERSION 0.2.0)
set(CMAKE_PROJECT_HOMEPAGE_URL "http://github.com/wagenadl/uniword")

######################################################################
# Specify the C++ standard [must be done _before_ adding executable!]
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # I thought this would do the trick...
if (UNIX) 
  add_compile_options(-fPIC) # ... but in fact, this is required on Linux
  add_compile_options(-W -Wall)
endif()

# Qt stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

######################################################################
# Add the executables
add_executable(uniword WIN32 MACOSX_BUNDLE src/main.cpp)
target_sources(uniword PRIVATE src/MainWindow.cpp)
target_sources(uniword PRIVATE src/CenterWindow.cpp)
target_sources(uniword PRIVATE src/OutputWidget.cpp)
target_sources(uniword PRIVATE src/Universe.cpp)
target_sources(uniword PRIVATE src/uniword.qrc )

# Libraries to link to - Qt must be mentioned
target_link_libraries(uniword Qt5::Widgets)


if (WIN32)
  #target_sources(uniword PRIVATE uniword/uniword.iconset/uniword.rc)
  target_link_libraries(uniword secur32.lib)
endif()
if (APPLE)
  set(MACOSX_BUNDLE_ICON_FILE uniword.icns)
  add_custom_command(TARGET uniword POST_BUILD
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/uniword.app/Contents/Resources
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/uniword/uniword.icns 
      ${CMAKE_CURRENT_BINARY_DIR}/uniword.app/Contents/Resources
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/symbols
      ${CMAKE_CURRENT_BINARY_DIR}/uniword.app/Contents/Resources
  )
endif()

# Produce configuration files

# Include paths - Qt must be mentioned
target_include_directories(uniword PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(uniword PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")

######################################################################
# General packaging / installation
set(CPACK_PACKAGE_VENDOR "Daniel Wagenaar")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Uniword")
set(CPACK_PACKAGE_DESCRIPTION
  "Uniword is a keyword search–based unicode character picker")
set(CPACK_PACKAGE_EXECUTABLES "uniword;Uniword")

if (WIN32)
  # NSIS packaging - to be updated
  include(WinPack.CMakeLists.txt)
elseif (UNIX)
  include(UnixInstall.CMakeLists.txt)
  include(DebPack.CMakeLists.txt)
endif()

# Packaging
include(CPack)

