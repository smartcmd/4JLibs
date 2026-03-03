cmake_minimum_required(VERSION 3.24)

if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
endif()

project(4JLibs LANGUAGES C CXX)

if(NOT WIN32)
    message(FATAL_ERROR "4JLibs currently supports Windows builds only.")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Use a 64-bit generator/toolchain (x64).")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<$<CONFIG:Debug>:Embedded>$<$<CONFIG:RelWithDebInfo>:ProgramDatabase>"
    )
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS UNICODE _UNICODE)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C,CXX>:/W3>
        $<$<COMPILE_LANGUAGE:C,CXX>:/MP>
        $<$<COMPILE_LANGUAGE:C,CXX>:/FS>
        $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
    )
endif()

add_subdirectory(Input)
add_subdirectory(Profile)
add_subdirectory(Render)
add_subdirectory(Storage)

install(
    FILES
        include/4JLibs/4J_Input.h
        include/4JLibs/4J_Profile.h
        include/4JLibs/4J_Render.h
        include/4JLibs/4J_Storage.h
        include/4JLibs/Platform.h
        include/4JLibs/extraX64.h
    DESTINATION include/4JLibs
)

install(TARGETS
    4JLibs.Windows.Input
    4JLibs.Windows.Profile
    4JLibs.Windows.Render
    4JLibs.Windows.Storage
    ARCHIVE DESTINATION lib/$<CONFIG>
)
