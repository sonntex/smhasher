cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0003 NEW)

project(SMHasher C CXX)

include(CheckCCompilerFlag)
# Check if the same compile family is used for both C and CXX
if (NOT (CMAKE_C_COMPILER_ID STREQUAL CMAKE_CXX_COMPILER_ID))
    message(WARNING "CMAKE_C_COMPILER_ID (${CMAKE_C_COMPILER_ID}) is different "
                    "from CMAKE_CXX_COMPILER_ID (${CMAKE_CXX_COMPILER_ID}). "
                    "The final binary may be unusable.")
endif()

IF (CMAKE_MAJOR_VERSION GREATER 2)
  IF (CMAKE_MAJOR_VERSION GREATER 3 OR CMAKE_MINOR_VERSION GREATER 1)
    cmake_policy(SET CMP0056 OLD) # since 3.2. Honor link flags in try_compile() source-file signature.
  ENDIF()
  IF (CMAKE_MAJOR_VERSION GREATER 3 OR CMAKE_MINOR_VERSION GREATER 3)
    cmake_policy(SET CMP0065 NEW) # since 3.4. ENABLE_EXPORTS.
  ENDIF()
  IF (CMAKE_MAJOR_VERSION GREATER 3 OR CMAKE_MINOR_VERSION GREATER 6)
    cmake_policy(SET CMP0066 OLD) # since 3.7. Honor per-config flags in try_compile() source-file signature.
  ENDIF()

  include (TestBigEndian)
  TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
  if(IS_BIG_ENDIAN)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -DBIG_ENDIAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBIG_ENDIAN")
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
       "Choose the type of build, options are: Debug Release
  RelWithDebInfo MinSizeRel."
  FORCE)
endif(NOT CMAKE_BUILD_TYPE)

include (CheckTypeSize)
check_type_size (__int64 __INT64)
check_type_size (int64_t INT64_T)

# TODO: rather parse `$CC -march=native -dM -E - <<< ''` [gh #10]
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
   EXEC_PROGRAM(cat ARGS "/proc/cpuinfo" OUTPUT_VARIABLE CPUINFO)
   STRING(REGEX REPLACE "^.*(sse2).*$" "\\1" SSE_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "sse2" "${SSE_THERE}" SSE2_TRUE)
   STRING(REGEX REPLACE "^.*(sse4_2).*$" "\\1" SSE_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "sse4_2" "${SSE_THERE}" SSE42_TRUE)
   set(FALKHASH_OBJ      falkhash-elf64.o)
   IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
     set(FHTW_OBJ        fhtw-elf64.o)
   ENDIF()
   STRING(REGEX REPLACE "^.*(aes).*$" "\\1" AES_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "aes" "${AES_THERE}" AES_TRUE)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
   EXEC_PROGRAM("/usr/sbin/sysctl -n machdep.cpu.features" OUTPUT_VARIABLE
      CPUINFO)
   STRING(REGEX REPLACE "^.*[^S](SSE2).*$" "\\1" SSE_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "SSE2" "${SSE_THERE}" SSE2_TRUE)
   STRING(REGEX REPLACE "^.*(SSE4.2).*$" "\\1" SSE_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "SSE4.2" "${SSE_THERE}" SSE42_TRUE)
   set(FALKHASH_OBJ      falkhash-macho64.o)
   IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
     set(FHTW_OBJ        fhtw-macho64.o)
   ENDIF()
   STRING(REGEX REPLACE "^.*(AES).*$" "\\1" AES_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "AES" "${AES_THERE}" AES_TRUE)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   EXEC_PROGRAM("grep Features /var/run/dmesg.boot" OUTPUT_VARIABLE
      CPUINFO)
   STRING(REGEX REPLACE "^.*[^S](SSE2).*$" "\\1" SSE_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "SSE2" "${SSE_THERE}" SSE2_TRUE)
   STRING(REGEX REPLACE "^.*(SSE4.2).*$" "\\1" SSE_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "SSE4.2" "${SSE_THERE}" SSE42_TRUE)
   set(FALKHASH_OBJ      falkhash-elf64.o)
   IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
     set(FHTW_OBJ        fhtw-elf64.o)
   ENDIF()
   STRING(REGEX REPLACE "^.*(AES).*$" "\\1" AES_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "AES" "${AES_THERE}" AES_TRUE)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(_vendor_id)
  set(_cpu_family)
  set(_cpu_model)
  get_filename_component(_vendor_id "[HKEY_LOCAL_MACHINE\\Hardware\\Description\\System\\CentralProcessor\\0;VendorIdentifier]" NAME CACHE)
  get_filename_component(_cpu_id "[HKEY_LOCAL_MACHINE\\Hardware\\Description\\System\\CentralProcessor\\0;Identifier]" NAME CACHE)
  mark_as_advanced(_vendor_id _cpu_id)
  string(REGEX REPLACE ".* Family ([0-9]+) .*" "\\1" _cpu_family "${_cpu_id}")
  string(REGEX REPLACE ".* Model ([0-9]+) .*" "\\1" _cpu_model "${_cpu_id}")
  if(_vendor_id STREQUAL "GenuineIntel")
    if(_cpu_family EQUAL 6)
      set(SSE2_TRUE TRUE)
      set(SSE42_TRUE TRUE)
      set(AES_TRUE TRUE)
    elseif(_cpu_family EQUAL 15) # NetBurst
      set(SSE2_TRUE TRUE)
      set(SSE42_TRUE TRUE)
    endif()
  elseif(_vendor_id STREQUAL "AuthenticAMD")
    if(_cpu_family GREATER 14)
      set(SSE2_TRUE TRUE)
      set(SSE42_TRUE TRUE)
      set(AES_TRUE TRUE)
    endif()
  endif()
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

if (CMAKE_COMPILER_IS_GNUCC
    OR (CMAKE_C_COMPILER_ID STREQUAL AppleClang)
    OR (CMAKE_C_COMPILER_ID STREQUAL Clang)
    OR (CMAKE_C_COMPILER_ID STREQUAL Intel))
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
  set(CMAKE_C_FLAGS_RELEASE "-O3")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  set(CMAKE_C_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  #IF (SSE2_TRUE)
  #  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -msse2")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
  #ENDIF (SSE2_TRUE)
  #IF (SSE42_TRUE)
  #  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -msse4")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4")
  #ENDIF (SSE42_TRUE)
  #IF (AES_TRUE)
  #  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -maes")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes")
  #ENDIF (AES_TRUE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -march=native")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
elseif (MSVC)
  # using Visual Studio C++, already the default with VS17
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} /arch:SSE2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
else()
  message(WARNING "Unknown ${CMAKE_CXX_COMPILER_ID} compiler")
# TODO: BSD and ARM8 crc detection
# TODO: 32bit filter
endif()

IF (SSE2_TRUE)
  set(SSE2_FOUND true CACHE BOOL "SSE2 available")
ELSE (SSE2_TRUE)
  set(SSE2_FOUND false CACHE BOOL "SSE2 not available")
  message(WARNING "SSE2 not available")
ENDIF (SSE2_TRUE)
IF (SSE42_TRUE)
  set(SSE4_2_FOUND true CACHE BOOL "SSE4.2 available")
ELSE (SSE42_TRUE)
  set(SSE4_2_FOUND false CACHE BOOL "SSE4.2 not available")
  message(WARNING "SSE4.2 not available")
ENDIF (SSE42_TRUE)
IF (AES_TRUE)
  set(AES_FOUND true CACHE BOOL "AES-NI available")
ELSE (AES_TRUE)
  set(AES_FOUND false CACHE BOOL "AES-NI not available")
  message(WARNING "AES-NI not available")
ENDIF (AES_TRUE)

IF(HAVE_INT64_T)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_INT64")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_INT64")
ELSEIF(HAVE__INT64)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_INT64")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_INT64")
ENDIF()
IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_BIT32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_BIT32")
ENDIF()

IF(AES_FOUND AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
  set(SSE4_OBJ        ${FALKHASH_OBJ})
ENDIF()

IF(SSE4_2_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SSE42")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_SSE42")
  SET(CMAKE_THREAD_LIBS_INIT "-lpthread")
  set(SSE2_SRC        hasshe2.c)
  set(SSE4_SRC        crc32_hw.c)
  # 64bit only:
  IF((CMAKE_SIZEOF_VOID_P EQUAL 8) AND (NOT MSVC))
    #enable_language(ASM-NASM)
    #set(CMAKE_ASM_FLAGS "-f elf64") # or macho64/win64
    set(X86_64ONLY_SRC farmhash-c.c farmhash-c-test.cc metrohash64crc.cpp
                       metrohash128crc.cpp crc32_hw1.c clhash.c )
  ELSE()
    message(WARNING "32bit only: CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P}")
  ENDIF()
  set(SIPHASH_SRC     siphash_ssse3.c)
ELSEIF(SSE2_FOUND)
  set(SSE2_SRC        hasshe2.c)
  set(SIPHASH_SRC     siphash_sse2.c)
ELSE()
  set(SIPHASH_SRC     siphash.c)
ENDIF(SSE4_2_FOUND)

IF(AES_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_AESNI")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_AESNI") 
  set(T1HA_SRC
      t1ha/t1ha0.c t1ha/t1ha1.c t1ha/t1ha2.c
      t1ha/t1ha0_ia32aes_noavx.c t1ha/t1ha0_ia32aes_avx.c t1ha/t1ha0_ia32aes_avx2.c)
  if(MSVC)
    #ignoring unknown option '/arch:ia32'
    #set_source_files_properties(t1ha/t1ha0_ia32aes_noavx.c PROPERTIES COMPILE_FLAGS "/arch:ia32")
    set_source_files_properties(t1ha/t1ha0_ia32aes_avx.c PROPERTIES COMPILE_FLAGS "/arch:avx")
    set_source_files_properties(t1ha/t1ha0_ia32aes_avx2.c PROPERTIES COMPILE_FLAGS "/arch:avx2")
  else()
    set_source_files_properties(t1ha/t1ha0_ia32aes_noavx.c PROPERTIES COMPILE_FLAGS "-mno-avx2 -mno-avx -maes")
    set_source_files_properties(t1ha/t1ha0_ia32aes_avx.c PROPERTIES COMPILE_FLAGS "-mno-avx2 -mavx -maes")
    set_source_files_properties(t1ha/t1ha0_ia32aes_avx2.c PROPERTIES COMPILE_FLAGS "-mavx -mavx2 -maes")
  endif()
ELSE()
  set(T1HA_SRC t1ha/t1ha0.c t1ha/t1ha1.c t1ha/t1ha2.c)
ENDIF()

if(NOT MSVC)
  set(NOTMSVC_SRC seahash.c)
  set_source_files_properties(HashMapTest.cpp PROPERTIES COMPILE_FLAGS "-std=c++11")
  set_source_files_properties(SpeedTest.cpp PROPERTIES COMPILE_FLAGS "-std=c++0x -I${CMAKE_SOURCE_DIR}")
  set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS "-std=c++0x")
  # https://github.com/dgryski/trifles/tree/master/tsip
  set(TSIP_SRC tsip.c)
else()
  set_source_files_properties(SpeedTest.cpp PROPERTIES COMPILE_FLAGS "-I${CMAKE_SOURCE_DIR}")
endif()
set_source_files_properties(Hashes.cpp PROPERTIES COMPILE_FLAGS "-I${CMAKE_SOURCE_DIR}")

find_library(HIGHWAY highwayhash)
if (HIGHWAY)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(HIGHWAY_SRC HighwayHash.cpp)
    set(HIGHWAY_LIB highwayhash)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_HIGHWAYHASH")
  endif()
endif()
#IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BeagleHashes_test.c")
#    set(BEAGLE_SRC BeagleHashes_test.c)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_BEAGLE_HASHES")
#ENDIF()
#IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/funny_hash.h")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_FUNNY_HASH")
#ENDIF()
#IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/fanom_hash.h")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_FANOM_HASH")
#ENDIF()

add_library(
  SMHasherSupport
  AvalancheTest.cpp
  Bitslice.cpp
  Bitvec.cpp
  CityTest.cpp
  City.cpp
  crc.cpp
  DifferentialTest.cpp
  HashMapTest.cpp
  Hashes.cpp
  ${HIGHWAY_SRC}
  ${SSE2_SRC}
  ${SSE4_SRC}
  KeysetTest.cpp
  lookup3.cpp
  md5.cpp
  MurmurHash1.cpp
  MurmurHash2.cpp
  MurmurHash3.cpp
  Platform.cpp
  Random.cpp
  sha1.cpp
  ${SIPHASH_SRC}
  SpeedTest.cpp
  Spooky.cpp
  SpookyTest.cpp
  Stats.cpp
  SuperFastHash.cpp
  Types.cpp
  PMurHash.c
  fasthash.cpp
  xxhash.c
  metrohash64.cpp
  metrohash128.cpp
  cmetrohash64.c opt_cmetrohash64_1.c
  farmhash.cc FarmTest.cc
  ${SSE4_OBJ}
  # ${FHTW_OBJ}
  ${T1HA_SRC}
  mum.cc
  jody_hash32.c
  jody_hash64.c
  ${TSIP_SRC}
  # ${BEAGLE_SRC}
  ${NOTMSVC_SRC}
  ${X86_64ONLY_SRC}
  blake2b.c
  blake2s.c
  rmd128.c
  rmd160.c
  rmd256.c
  sha2/sha224.c
  sha2/sha256.c
  sha2/sha512_224.c
  sha2/sha512_256.c
  sha3.c
)

add_executable(
  SMHasher
  main.cpp
)

target_link_libraries(
  SMHasher
  SMHasherSupport
  ${HIGHWAY_LIB}
  ${CMAKE_THREAD_LIBS_INIT}
  )

enable_testing()
add_test(VerifyAll SMHasher --test=VerifyAll)
add_test(Sanity    SMHasher --test=Sanity)
add_test(Speed     SMHasher --test=Speed)
add_test(Cyclic    SMHasher --test=Cyclic)
add_test(Zeroes    SMHasher --test=Zeroes)
add_test(Seed      SMHasher --test=Seed)

add_custom_target (
    TAGS
    COMMAND etags --language=c++ *.cpp *.cc *.h
    DEPENDS ${SRCS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
