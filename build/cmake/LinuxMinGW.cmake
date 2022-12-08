set(TARGET_NAME "XCSoarAug-MinGW")  # hardcoded yet

message(STATUS "+++ System = WIN32 / MinGW (${TOOLCHAIN})! on ${CMAKE_HOST_SYSTEM_NAME} ")

set(LIB_PREFIX "lib" )  # "lib")
set(LIB_SUFFIX ".a")    # "a")
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    include_directories("D:/Programs/MinGW/${TOOLCHAIN}/include")
    #  later: include_directories("${PROJECTGROUP_SOURCE_DIR}/output/include")
else()
#    include_directories("/usr/include")
#    include_directories("/usr/include/x86_64-linux-gnu")
#    # message(FATAL_ERROR "Stop????")
endif()


add_compile_definitions(BOOST_NO_IOSTREAM)
add_compile_definitions(BOOST_MATH_NO_LEXICAL_CAST)
add_compile_definitions(BOOST_UBLAS_NO_STD_CERR)
add_compile_definitions(BOOST_ERROR_CODE_HEADER_ONLY)
add_compile_definitions(BOOST_SYSTEM_NO_DEPRECATED)
add_compile_definitions(BOOST_NO_STD_LOCALE)
add_compile_definitions(BOOST_LEXICAL_CAST_ASSUME_C_LOCALE)
add_compile_definitions(BOOST_NO_CXX98_FUNCTION_BASE)
add_compile_definitions(HAVE_POSIX)
add_compile_definitions(HAVE_VASPRINTF)
add_compile_definitions(EYE_CANDY)
add_compile_definitions(_GLIBCXX_ASSERTIONS)
add_compile_definitions(ENABLE_ALSA)
add_compile_definitions(USE_FREETYPE)
add_compile_definitions(ENABLE_OPENGL)
add_compile_definitions(HAVE_GLES)
add_compile_definitions(HAVE_GLES2)
add_compile_definitions(GL_GLEXT_PROTOTYPES)
add_compile_definitions(USE_GLX)
add_compile_definitions(USE_X11)
add_compile_definitions(USE_POLL_EVENT)
# add_compile_definitions(ENABLE_OPENGL)
# add_compile_definitions(HAVE_GLES)
# add_compile_definitions(HAVE_GLES2)
# add_compile_definitions(GL_GLEXT_PROTOTYPES)
# add_compile_definitions(USE_GLX)
# add_compile_definitions(USE_X11)
# add_compile_definitions(USE_POLL_EVENT)
# add_compile_definitions(USE_POLL_EVENT)
add_compile_definitions(BOOST_JSON_STANDALONE)
# string(APPEND CMAKE_CXX_FLAGS " -isystem ./output/src/boost_1_80_0")
# string(APPEND CMAKE_CXX_FLAGS " -isystem ./output/WIN64/lib/x86_64-w64-mingw32/include")
# string(APPEND CMAKE_CXX_FLAGS " -isystem /home/august/Projects/XCsoar/output/WIN64/lib/x86_64-w64-mingw32/include")

string(APPEND CMAKE_CXX_FLAGS " -I./src")
string(APPEND CMAKE_CXX_FLAGS " -I./src/Engine")
string(APPEND CMAKE_CXX_FLAGS " -I./_build/test2/UNIX/include")
string(APPEND CMAKE_CXX_FLAGS " -isystem lib/glm")
# string(APPEND CMAKE_CXX_FLAGS " -isystem lib/glm")
string(APPEND CMAKE_CXX_FLAGS " -isystem /usr/include/x86_64-linux-gnu")


string(APPEND CMAKE_CXX_FLAGS " -Og -funit-at-a-time -ffast-math -g -std=c++20 -fno-threadsafe-statics -fmerge-all-constants -fcoroutines -fconserve-space -fno-operator-names -fvisibility=hidden -finput-charset=utf-8 -Wall -Wextra -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare -Wundef -Wmissing-declarations -Wredundant-decls -Wmissing-noreturn -Wvla -Wno-format-truncation -Wno-missing-field-initializers -Wcast-align -Werror -I./src/unix -I./_build/include -isystem /home/august/Projects/link_libs/boost/boost-1.80.0 ")

#######################################################################

#### add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
# disable WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
### add_compile_definitions(BOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON)

#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++2a") # -std=c++20")
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS}")

set(MINGW ON)
add_compile_definitions(__MINGW__)
#********************************************************************************
if(AUGUST_SPECIAL)
    add_compile_definitions(_AUG_MGW=1)
endif()
#********************************************************************************
# add_compile_definitions(_UNICODE)
# add_compile_definitions(UNICODE)  # ???
# add_compile_definitions(STRICT)
# # add_compile_definitions(WIN32)  # XCSoar used this instead of _WIN32
# add_compile_definitions(_USE_MATH_DEFINES)   # necessary under C++17!
# add_compile_definitions(ZZIP_1_H)   # definition of uint32_t and Co.!

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -m64 -lwsock32 -lws2_32 -lgdi32 -lgdiplus -lcrypt32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -v")
# set(FREEGLUT_LIB_DIR "${LINK_LIBS}/freeglut-MinGW-3.0.0-1/freeglut")
    
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(SSL_LIB)
    set(CRYPTO_LIB Crypt32.lib BCrypt.lib) # no (OpenSSL-)crypto lib on windows!
endif()

set(PERCENT_CHAR \%)
set(DOLLAR_CHAR  $$)

# if(EXISTS "D:/Programs")  # on Windows - and on Flaps6 (August2111)
    list(APPEND CMAKE_PROGRAM_PATH "D:/Programs")
# endif()
