set(TARGET_NAME "XCSoarAug-Linux")  # hardcoded yet

message(STATUS "+++ System = Linux / GCC (${TOOLCHAIN})  on ${CMAKE_HOST_SYSTEM_NAME} !")

set(LIB_PREFIX "lib" )  # "lib")
set(LIB_SUFFIX ".a")    # "a")

add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
# disable WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
add_compile_definitions(BOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20")
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS}")

set(UNIX ON)
add_compile_definitions(__LINGCC__)

    include_directories("/usr/include/x86_64-linux-gnu")
    message(FATAL_ERROR "Stop????")

#********************************************************************************
if(AUGUST_SPECIAL)
    add_compile_definitions(_AUG_GCC=1)
endif()
#********************************************************************************
set(ENABLE_OPENGL ON)  # better outside????
set(ENABLE_SDL OFF)  # better outside????
set(USE_MEMORY_CANVAS OFF)  # das ist hier auch falsch!!!!

add_compile_definitions(USE_POLL_EVENT)
add_compile_definitions(USE_LIBINPUT)
add_compile_definitions(USE_FREETYPE)
add_compile_definitions(HAVE_POSIX)  # bereits in LinuxGCC.cmake (nicht in toolchain..)
add_compile_definitions(HAVE_VASPRINTF)
add_compile_definitions(ENABLE_ALSA)
add_compile_definitions(USE_X11)

# MinGW only! add_compile_definitions(_UNICODE UNICODE)  # ???
add_compile_definitions(STRICT)
add_compile_definitions(_USE_MATH_DEFINES)   # necessary under C++17!
# add_compile_definitions(ZZIP_1_H)   # definition of uint32_t and Co.!
add_compile_definitions(ZZIP_HAVE_STDINT_H)   # definition of uint32_t and Co.!

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libstdc++ -Wl,-Bstatic,--whole-archive -Wl,--no-whole-archive -v")
# set(FREEGLUT_LIB_DIR "${LINK_LIBS}/freeglut-MinGW-3.0.0-1/freeglut")
    
    set(SSL_LIB)
    set(CRYPTO_LIB) # no extra (OpenSSL-)crypto lib on linux??!

set(PERCENT_CHAR \%)
set(DOLLAR_CHAR  $$)

list(APPEND CMAKE_PROGRAM_PATH "/home/august/Projects/XCSoar/_build")

include_directories(${SRC}/unix)
# message(FATAL_ERROR "Include ${SRC}/unix")

