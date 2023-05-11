cmake_minimum_required(VERSION 2.8)

#SET (CMAKE_C_COMPILER             	"gcc")
#SET (CMAKE_C_FLAGS                	"-Wall -std=c99")
#SET (CMAKE_C_FLAGS_DEBUG          	"-g -DDEBUG")
#SET (CMAKE_C_FLAGS_MINSIZEREL     	"-Os -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELEASE        	"-O4 -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELWITHDEBINFO 	"-O2 -g")
 
#SET (CMAKE_CXX_COMPILER             "g++")
#SET (CMAKE_CXX_FLAGS                "-Wall -std=gnu++11")
#SET (CMAKE_CXX_FLAGS_DEBUG          "-g -DDEBUG")
#SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

#SET (CMAKE_AR      "ar")
#SET (CMAKE_LINKER  "ld")
#SET (CMAKE_NM      "nm")
#SET (CMAKE_OBJDUMP "objdump")
#SET (CMAKE_RANLIB  "ranlib")

SET (CMAKE_C_FLAGS                	"${CMAKE_C_FLAGS} -std=c99")
#SET (CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
SET (CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -std=gnu++11")
#SET (CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")


#SET(CMAKE_BUILD_TYPE Debug)
if(NOT CMAKE_BUILD_TYPE)
	# default build type Relase
	SET (CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")


set(CMAKE_DEBUG_POSTFIX "d") 

# 
SET (OUTPUT_NAME h2645_util)
project (${OUTPUT_NAME})
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_DEBUG 0)
message(STATUS "VERSION=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_DEBUG}")


# 硬件架构检测
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" SYSPROC)

set(X86_ALIASES x86 i386 i686 x86_64 amd64)
set(POWER_ALIASES PPC64 PPC64le)
set(ARM_ALIASES armv61 armv71 armv7-a aarch64)

list(FIND X86_ALIASES "${SYSPROC}" X86MATCH)
list(FIND POWER_ALIASES "${SYSPROC}" POWERMATCH)
list(FIND ARM_ALIASES "${SYSPROC}" ARMMATCH)

if("${SYSPROC}" STREQUAL "" OR X86MATCH GREATER "-1")
	message(STATUS "Detected x86 target processor")
	set(X86 1)
elseif(POWERMATCH GREATER "-1")
	message(STATUS "Detected POWER target processor")
	set(POWER 1)
elseif(ARMMATCH GREATER "-1")
	message(STATUS "Detected ARM target processor")
	set(ARM 1)
else()
	message(WARNING "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")
	message(WARNING "CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P}")
	message(WARNING "Please add this value near ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")
endif()

# Check for 64 or 32 bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  	set(ARCH_64 TRUE)
  	set(PROCESSOR_ARCH "x64")
	message(STATUS "Detected 64 bit processor")
else()
  	set(ARCH_64 FALSE)
  	set(PROCESSOR_ARCH "x86")
  	message(STATUS "Detected 32 bit processor")
endif()

# Get the platform
if(WIN32)
  if(NOT WINDOWS)
    set(WINDOWS TRUE)
  endif()
  set(PLATFORM_NAME "windows")
elseif(UNIX AND NOT ANDROID AND NOT APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    set(LINUX TRUE)
  	set(PLATFORM_NAME "linux")
  elseif(CMAKE_SYSTEM_NAME MATCHES "kFreeBSD.*")
    set(FREEBSD TRUE)
  	set(PLATFORM_NAME "freebsd")
  elseif(CMAKE_SYSTEM_NAME MATCHES "kNetBSD.*|NetBSD.*")
    set(NETBSD TRUE)
  	set(PLATFORM_NAME "netbsd")
  elseif(CMAKE_SYSTEM_NAME MATCHES "kOpenBSD.*|OpenBSD.*")
    set(OPENBSD TRUE)
  	set(PLATFORM_NAME "openbsd")
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*GNU.*")
    set(GNU TRUE)
  	set(PLATFORM_NAME "gnu")
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*BSDI.*")
    set(BSDI TRUE)
  	set(PLATFORM_NAME "bsdi")
  elseif(CMAKE_SYSTEM_NAME MATCHES "DragonFly.*|FreeBSD")
    set(FREEBSD TRUE)
  	set(PLATFORM_NAME "freebsd")
  elseif(CMAKE_SYSTEM_NAME MATCHES "SYSV5.*")
    set(SYSV5 TRUE)
  	set(PLATFORM_NAME "sysv5")
  elseif(CMAKE_SYSTEM_NAME MATCHES "Solaris.*")
    set(SOLARIS TRUE)
  	set(PLATFORM_NAME "solaris")
  elseif(CMAKE_SYSTEM_NAME MATCHES "HP-UX.*")
    set(HPUX TRUE)
  	set(PLATFORM_NAME "hpux")
  elseif(CMAKE_SYSTEM_NAME MATCHES "AIX.*")
    set(AIX TRUE)
  	set(PLATFORM_NAME "aix")
  elseif(CMAKE_SYSTEM_NAME MATCHES "Minix.*")
    set(MINIX TRUE)
  	set(PLATFORM_NAME "minix")
  else()
  	message(WARNING "Please check ${CMAKE_SYSTEM_NAME} near ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")
  endif()
elseif(ANDROID)
   set(PLATFORM_NAME "android")  
elseif(APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Darwin.*")
    set(DARWIN TRUE)
  	set(PLATFORM_NAME "darwin")
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*MacOS.*")
    set(MACOSX TRUE)
  	set(PLATFORM_NAME "macos")
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*tvOS.*")
    set(TVOS TRUE)
  	set(PLATFORM_NAME "tvos")
  else()
  	message(WARNING "Please check ${CMAKE_SYSTEM_NAME} near ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")
  endif()
  # TODO: iOS?
elseif(CMAKE_SYSTEM_NAME MATCHES "BeOS.*")
  message_error("BeOS support has been removed as of BRVP.")
elseif(CMAKE_SYSTEM_NAME MATCHES "Haiku.*")
  set(HAIKU TRUE)
  set(PLATFORM_NAME "haiku")
else()
  	message(WARNING "Please check ${CMAKE_SYSTEM_NAME} near ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")  
endif()

message(STATUS "Detected ${PLATFORM_NAME} platform")

# 安装路径
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	
	#set(MY_INSTALL_PREFIX ../lib/${PLATFORM_NAME})
	set(MY_INSTALL_PREFIX ./lib/${PLATFORM_NAME})

	get_filename_component(MY_INSTALL_ABSOLUTE_PREFIX ${MY_INSTALL_PREFIX} ABSOLUTE)
		
	set(CMAKE_INSTALL_PREFIX ${MY_INSTALL_ABSOLUTE_PREFIX} CACHE PATH "My default install prefix" FORCE)

endif()


# 安装目录
if(NOT CMAKE_INSTALL_LIBDIR)
	if(ANDROID)
		set(CMAKE_INSTALL_LIBDIR ${ANDROID_ABI})
	elseif(X86)
    if(ARCH_64)
		  set(CMAKE_INSTALL_LIBDIR x86_64)
    else()
      set(CMAKE_INSTALL_LIBDIR x86)
    endif()
  else()
    message(WARNING "Please add this install lib dir near ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")
	endif()
endif()

if(NOT CMAKE_INSTALL_BINDIR)
	if(ANDROID)
		set(CMAKE_INSTALL_BINDIR ${ANDROID_ABI})
	elseif(X86)
    if(ARCH_64)
      set(CMAKE_INSTALL_BINDIR x86_64)
    else()
      set(CMAKE_INSTALL_BINDIR x86)
    endif()
  else()
    message(WARNING "Please add this install bin dir near ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")
  endif()
endif()

message(STATUS "Install bin path: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")

#if(LINUX OR Android)
if(UNIX)
	SET (CMAKE_C_FLAGS                	"${CMAKE_C_FLAGS} -fPIC -D_DEFAULT_SOURCE")
	SET (CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -fPIC -D_DEFAULT_SOURCE")
	SET (CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG} -fPIC -D_DEFAULT_SOURCE")
	SET (CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -fPIC -D_DEFAULT_SOURCE")
endif()

configure_file("${CMAKE_SOURCE_DIR}/h2645_version.h.in" "${CMAKE_SOURCE_DIR}/include/h2645_version.h")



# ============================================BUILD============================================

include_directories(${CMAKE_SOURCE_DIR}
					${CMAKE_SOURCE_DIR}/include
					${CMAKE_SOURCE_DIR}/opt)


# ============================================LIBRARY============================================
aux_source_directory(src SRC_LIST)

add_library(${OUTPUT_NAME} STATIC ${SRC_LIST})

#target_compile_definitions(${OUTPUT_NAME}
#	                            PRIVATE
#								PROXY)

if(WINDOWS)
	target_compile_definitions(${OUTPUT_NAME}
	                            PRIVATE
	                            WIN32)
endif() 



# ============================================EXECUTABLE============================================
SET (EXAMPLE_OUTPUT_NAME example)

if(WINDOWS)
	aux_source_directory(opt OPT_SRC_LIST)
	add_executable(${EXAMPLE_OUTPUT_NAME} example.c ${OPT_SRC_LIST})
else()
	add_executable(${EXAMPLE_OUTPUT_NAME} example.c)
endif()

target_link_libraries(${EXAMPLE_OUTPUT_NAME}
					PRIVATE
					${OUTPUT_NAME})

if(WINDOWS)
	target_compile_definitions(${EXAMPLE_OUTPUT_NAME}
	                            PRIVATE
	                            WIN32)
endif() 


#实际安装路径是  ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
#实际安装路径是  ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}

INSTALL(TARGETS ${OUTPUT_NAME} ${EXAMPLE_OUTPUT_NAME}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)