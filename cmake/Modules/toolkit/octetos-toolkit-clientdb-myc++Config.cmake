
IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR)
  # Already in cache, be silent
  SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_QUIETLY TRUE)
ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR)

if(${APIDB_VERSION_STAGE} STREQUAL "developing" )  
    FIND_PATH(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR toolkit/clientdb/clientdb.hpp
        $ENV{HOME}/develop
    )
    MESSAGE(STATUS "OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR: ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR}")
    SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES octetos-toolkit-clientdb-myc++)
    FIND_LIBRARY(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY
        NAMES ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES}
        PATHS $ENV{HOME}/develop/toolkit/clientdb/build-myc++
        PATH_SUFFIXES octetos-toolkit-clientdb-myc++
    )
    MESSAGE(STATUS "OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY: ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY}")
else()
    FIND_PATH(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR toolkit/clientdb/clientdb.hpp
        /usr/local/include/octetos
        /usr/include/octetos
    )
    SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES octetos-toolkit-clientdb-myc++)
    FIND_LIBRARY(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY
        NAMES ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES}
        PATHS /usr/lib/octetos/toolkit/clientdb /usr/local/lib/octetos/toolkit/clientdb /usr/lib/x86_64-linux-gnu/octetos/toolkit/clientdb
        PATH_SUFFIXES octetos-toolkit-clientdb-myc++
    )
endif()



IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY)
  SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND TRUE)
  SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARIES ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY} )
ELSE (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY)
  SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND FALSE)
  SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARIES )
ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY)

IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)
  IF (NOT OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_QUIETLY)
    MESSAGE(STATUS "Found Toolkit ClientDB C++: ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY}: ${OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR}")
  ENDIF (NOT OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_QUIETLY)
ELSE (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)
  IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for Toolkit ClientDB C++ libraries named ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find Toolkit ClientDB C++ library")
  ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_REQUIRED)
ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)

MARK_AS_ADVANCED(
  OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY
  OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR
  )
