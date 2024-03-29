cmake_minimum_required(VERSION 3.5)

project(Fetpapi_Server)

set (APPLICATION_NAME fetpapiServer)

# version mechanism
set (FetpapiServer_VERSION_MAJOR 0)
set (FetpapiServer_VERSION_MINOR 1)
set (FetpapiServer_VERSION_PATCH 0)
set (FetpapiServer_VERSION_TWEAK 0)
set (FetpapiServer_VERSION ${Fetpapi_VERSION_MAJOR}.${FetpapiServer_VERSION_MINOR}.${FetpapiServer_VERSION_PATCH}.${FetpapiServer_VERSION_TWEAK})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE PATH "${LIBRARY_NAME} install prefix" FORCE)
ENDIF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# required to define CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR and CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs) 

add_executable (${APPLICATION_NAME} "")

# ============================================================================
# some useful variables
# ============================================================================

set (WITH_ETP_SSL OFF CACHE BOOL "Build Fesapi with ETP SSL support.")

# ============================================================================
# checking for required dependencies
# ============================================================================

#find_package(Fetpapi REQUIRED)

# Define the compile options according to the compiler
target_compile_options(${APPLICATION_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/bigobj>
)

find_package(Boost 1.66.0 REQUIRED log filesystem)

if (WIN32)
	target_link_libraries (${APPLICATION_NAME} PRIVATE bcrypt.lib)
	
	set_target_properties (${APPLICATION_NAME} PROPERTIES
		LINK_FLAGS "/INCREMENTAL:NO"
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})	
endif (WIN32)

if (NOT Boost_USE_STATIC_LIBS)
	# https://www.boost.org/doc/libs/1_75_0/libs/log/doc/html/log/rationale/namespace_mangling.html
	target_compile_definitions(${APPLICATION_NAME} PRIVATE BOOST_LOG_DYN_LINK)
endif ()

target_compile_definitions(${APPLICATION_NAME} PRIVATE BOOST_ALL_NO_LIB)
target_link_libraries (${APPLICATION_NAME} PRIVATE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if (WITH_ETP_SSL)
	target_compile_definitions(${APPLICATION_NAME} PRIVATE WITH_ETP_SSL)
	if (NOT TARGET OpenSSL::SSL OR NOT TARGET OpenSSL::Crypto)
		find_package(OpenSSL REQUIRED)
	endif()
	target_link_libraries(${APPLICATION_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif ()

#FESAPI
set (FESAPI_INCLUDE_DIR FESAPI_INCLUDE_DIR-NOTFOUND CACHE PATH "Path to the directory which contains the fesapi header files")
if (NOT IS_DIRECTORY ${FESAPI_INCLUDE_DIR})
	message(SEND_ERROR  "The Fesapi include dir (FESAPI_INCLUDE_DIR variable) does not look to be a valid directory. Please modify it.")
endif ()
set (FESAPI_LIBRARY_RELEASE FESAPI_LIBRARY_RELEASE-NOTFOUND CACHE FILEPATH "Path to the file which contains the fesapi library release")
if (NOT EXISTS ${FESAPI_LIBRARY_RELEASE})
	message(WARNING "The fesapi library (FESAPI_LIBRARY_RELEASE variable) does not look to be a valid file. Please modify it.")
endif ()
set (FESAPI_LIBRARY_DEBUG FESAPI_LIBRARY_DEBUG-NOTFOUND CACHE FILEPATH "Path to the file which contains the fesapi library DEBUG")
if (NOT EXISTS ${FESAPI_LIBRARY_DEBUG})
	message(WARNING "The fesapi library (FESAPI_LIBRARY_DEBUG variable) does not look to be a valid file. Please modify it.")
endif ()

#FETPAPI
set (FETPAPI_INCLUDE_DIR FETPAPI_INCLUDE_DIR-NOTFOUND CACHE PATH "Path to the directory which contains the FETPAPI header files")
if (NOT IS_DIRECTORY ${FETPAPI_INCLUDE_DIR})
	message(SEND_ERROR  "The FETPAPI include dir (FETPAPI_INCLUDE_DIR variable) does not look to be a valid directory. Please modify it.")
endif ()
set (FETPAPI_LIBRARY_RELEASE FETPAPI_LIBRARY_RELEASE-NOTFOUND CACHE FILEPATH "Path to the file which contains the FETPAPI library release")
if (NOT EXISTS ${FETPAPI_LIBRARY_RELEASE})
	message(WARNING "The FETPAPI library (FETPAPI_LIBRARY_RELEASE variable) does not look to be a valid file. Please modify it.")
endif ()
set (FETPAPI_LIBRARY_DEBUG FETPAPI_LIBRARY_DEBUG-NOTFOUND CACHE FILEPATH "Path to the file which contains the FETPAPI library DEBUG")
if (NOT EXISTS ${FETPAPI_LIBRARY_DEBUG})
	message(WARNING "The FETPAPI library (FETPAPI_LIBRARY_DEBUG variable) does not look to be a valid file. Please modify it.")
endif ()

#AVRO
set (AVRO_INCLUDE_DIR AVRO_INCLUDE_DIR-NOTFOUND CACHE PATH "Path to the directory which contains the avro header files")
if (NOT IS_DIRECTORY ${AVRO_INCLUDE_DIR})
	message(SEND_ERROR "The Avro include dir (AVRO_INCLUDE_DIR variable) does not look to be a valid directory. Please modify it.")
endif ()
set (AVRO_LIBRARY_RELEASE AVRO_LIBRARY_RELEASE-NOTFOUND CACHE FILEPATH "Path to the file which contains the avro library release")
if (NOT EXISTS ${AVRO_LIBRARY_RELEASE})
	message(WARNING "The avro library (AVRO_LIBRARY_RELEASE variable) does not look to be a valid file. Please modify it.")
endif ()
set (AVRO_LIBRARY_DEBUG AVRO_LIBRARY_DEBUG-NOTFOUND CACHE FILEPATH "Path to the file which contains the AVRO library DEBUG")
if (NOT EXISTS ${AVRO_LIBRARY_DEBUG})
	message(WARNING "The AVRO library (AVRO_LIBRARY_DEBUG variable) does not look to be a valid file. Please modify it.")
endif ()

if (WIN32)
	if (EXISTS ${FESAPI_LIBRARY_RELEASE} AND EXISTS ${FESAPI_LIBRARY_DEBUG})
		target_link_libraries (${APPLICATION_NAME} PRIVATE optimized ${FESAPI_LIBRARY_RELEASE} debug ${FESAPI_LIBRARY_DEBUG})
	elseif (EXISTS ${FESAPI_LIBRARY_RELEASE})
		target_link_libraries (${APPLICATION_NAME} PRIVATE ${FESAPI_LIBRARY_RELEASE})
	elseif (EXISTS ${FESAPI_LIBRARY_DEBUG})
		target_link_libraries (${APPLICATION_NAME} PRIVATE ${FESAPI_LIBRARY_DEBUG})
	endif ()
	
	if (EXISTS ${FETPAPI_LIBRARY_RELEASE} AND EXISTS ${FETPAPI_LIBRARY_DEBUG})
		target_link_libraries (${APPLICATION_NAME} PRIVATE optimized ${FETPAPI_LIBRARY_RELEASE} debug ${FETPAPI_LIBRARY_DEBUG})
	elseif (EXISTS ${FETPAPI_LIBRARY_RELEASE})
		target_link_libraries (${APPLICATION_NAME} PRIVATE ${FETPAPI_LIBRARY_RELEASE})
	elseif (EXISTS ${FETPAPI_LIBRARY_DEBUG})
		target_link_libraries (${APPLICATION_NAME} PRIVATE ${FETPAPI_LIBRARY_DEBUG})
	endif ()
	
	if (EXISTS ${AVRO_LIBRARY_RELEASE} AND EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (${APPLICATION_NAME} PRIVATE optimized ${AVRO_LIBRARY_RELEASE} debug ${AVRO_LIBRARY_DEBUG})
	elseif (EXISTS ${AVRO_LIBRARY_RELEASE})
		target_link_libraries (${APPLICATION_NAME} PRIVATE ${AVRO_LIBRARY_RELEASE})
	elseif (EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (${APPLICATION_NAME} PRIVATE ${AVRO_LIBRARY_DEBUG})
	endif ()
else (WIN32)
	target_link_libraries (${APPLICATION_NAME} PRIVATE ${FESAPI_LIBRARY_RELEASE})
	target_link_libraries (${APPLICATION_NAME} PRIVATE ${FETPAPI_LIBRARY_RELEASE})
	target_link_libraries (${APPLICATION_NAME} PRIVATE ${AVRO_LIBRARY_RELEASE})
endif (WIN32)

#target_link_libraries(${APPLICATION_NAME} PRIVATE Fetpapi::Fetpapi)

# ============================================================================
# Getting source files
# ============================================================================

file (GLOB ALL_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file (GLOB ALL_H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

target_sources(${APPLICATION_NAME} PRIVATE ${ALL_CPP_FILES} ${ALL_H_FILES})

target_include_directories(${APPLICATION_NAME} PRIVATE ${FESAPI_INCLUDE_DIR})
target_include_directories(${APPLICATION_NAME} PRIVATE ${FETPAPI_INCLUDE_DIR})
target_include_directories(${APPLICATION_NAME} PRIVATE ${AVRO_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (WITH_ETP_SSL)
	target_include_directories(${APPLICATION_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})
endif ()

# organizing sources and headers in the Visual Studio Project
if (WIN32)
	source_group ("src" FILES ${ALL_CPP_FILES} ${ALL_H_FILES})
endif (WIN32)

# ============================================================================
# Install fetpapiServer
# ============================================================================

install (
	TARGETS ${APPLICATION_NAME}
	DESTINATION ${CMAKE_INSTALL_PREFIX}
)