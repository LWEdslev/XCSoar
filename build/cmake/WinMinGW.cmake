set(TARGET_NAME "XCSoarAug-MinGW")  # hardcoded yet

message(STATUS "+++ System = WIN32 / MinGW (${TOOLCHAIN})!")

set(LIB_PREFIX "lib" )  # "lib")
set(LIB_SUFFIX ".a")    # "a")
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    include_directories("D:/Programs/MinGW/${TOOLCHAIN}/include")
    #  later: include_directories("${PROJECTGROUP_SOURCE_DIR}/output/include")
endif()

# gibt es nicht mehr: ---      include_directories("${PROJECTGROUP_SOURCE_DIR}/temp/data")  # temporory data!

add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
# disable WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
add_compile_definitions(BOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS}")

add_compile_definitions(__MINGW__)
#********************************************************************************
if(AUGUST_SPECIAL)
    add_compile_definitions(_AUG_MGW=1)
endif()
#********************************************************************************

        # add_compile_definitions(HAVE_MSVCRT)
add_compile_definitions(_UNICODE)
add_compile_definitions(UNICODE)  # ???
add_compile_definitions(STRICT)
# add_compile_definitions(WIN32)  # XCSoar used this instead of _WIN32
add_compile_definitions(_USE_MATH_DEFINES)   # necessary under C++17!
add_compile_definitions(ZZIP_1_H)   # definition of uint32_t and Co.!

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 -lgdi32 -lgdiplus -lcrypt32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -v")
# set(FREEGLUT_LIB_DIR "${LINK_LIBS}/freeglut-MinGW-3.0.0-1/freeglut")
    
# if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(SSL_LIB) # ${LINK_LIBS}/openssl/openssl_1.1.1i/lib/mgw73/libssl.a)
    set(CRYPTO_LIB Crypt32.lib BCrypt.lib) # no (OpenSSL-)crypto lib on windows!
# endif()

set(PERCENT_CHAR \%)
set(DOLLAR_CHAR  $$)

# if(EXISTS "D:/Programs")  # on Windows - and on Flaps6 (August2111)
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    list(APPEND CMAKE_PROGRAM_PATH "D:/Programs")
endif()

# additional for mgw122??, why
## add_compile_definitions(BOOST_HAS_THREADS)