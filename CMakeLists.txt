cmake_minimum_required(VERSION 3.15)
project(D2Inject)

# which c++ standard we wanna use? i go with a high number as its a new project
set(CMAKE_CXX_STANDARD 20)

# tell the compiler its a 32 bit process
set(CMAKE_SYSTEM_PROCESSOR "i686")

add_compile_definitions(DEBUG_CODE)
add_compile_definitions(WIN32_LEAN_AND_MEAN) # dont add weird crap
add_definitions(-DINJECTION_TARGET="D2Specifics/Init.h")

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.45.0 COMPONENTS matrix)
add_library(D2Inject SHARED
#        Injection/D2Specifics/Events.h
#        Injection/MemoryManipulation.h
        Injection/DLL.cpp
#        Injection/D2Specifics/StartupShutdown.h
#        Injection/DLL.h
#        Injection/D2Specifics/Init.h
        Injection/Maphack/src.cpp
#        Injection/Maphack/src.h
        Injection/D2Specifics/Events.cpp
        Injection/D2Specifics/D2Common.cpp
#        Injection/D2Specifics/D2Common.h
        Map/Diablo_II/ActMap.cpp
        Map/Diablo_II/LevelMap.cpp
        Tools/StringManipulation.cpp Tools/StringManipulation.h Injection/D2Specifics/D2Constants.h)
#add_library(D2Inject SHARED CommandLine.cpp D2FW.cpp)

# in case you need some libs
#target_link_libraries(D2Inject shlwapi ComCtl32 ComDlg32 kernel32.lib;user32.lib;gdi32.lib;shlwapi.lib;advapi32.lib;dbghelp.lib;Winmm.lib)
