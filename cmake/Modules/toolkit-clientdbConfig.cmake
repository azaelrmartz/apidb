
IF (TOOLKIT_CLIENTDB_INCLUDE_DIR)
  # Already in cache, be silent
  SET(TOOLKIT_CLIENTDB_FIND_QUIETLY TRUE)
ENDIF (TOOLKIT_CLIENTDB_INCLUDE_DIR)

FIND_PATH(TOOLKIT_CLIENTDB_INCLUDE_DIR clientdb.hpp
  /home/arm/development/toolkit/clientdb/src
  $ENV{HOME}/root/include/toolkit
  /usr/local/include/toolkit
  /usr/include/toolkit
)

SET(TOOLKIT_CLIENTDB_NAMES toolkit-clientdb)
FIND_LIBRARY(TOOLKIT_CLIENTDB_LIBRARY
  NAMES ${TOOLKIT_CLIENTDB_NAMES}
  PATHS /home/arm/development/toolkit/clientdb/src $ENV{HOME}/root/usr/lib /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
  PATH_SUFFIXES toolkit-clientdb
)

IF (TOOLKIT_CLIENTDB_INCLUDE_DIR AND TOOLKIT_CLIENTDB_LIBRARY)
  SET(TOOLKIT_CLIENTDB_FOUND TRUE)
  SET( TOOLKIT_CLIENTDB_LIBRARIES ${TOOLKIT_CLIENTDB_LIBRARY} )
ELSE (TOOLKIT_CLIENTDB_INCLUDE_DIR AND TOOLKIT_CLIENTDB_LIBRARY)
  SET(TOOLKIT_CLIENTDB_FOUND FALSE)
  SET( TOOLKIT_CLIENTDB_LIBRARIES )
ENDIF (TOOLKIT_CLIENTDB_INCLUDE_DIR AND TOOLKIT_CLIENTDB_LIBRARY)

IF (TOOLKIT_CLIENTDB_FOUND)
  IF (NOT TOOLKIT_CLIENTDB_FIND_QUIETLY)
    MESSAGE(STATUS "Found toolkit-cliendb: ${TOOLKIT_CLIENTDB_LIBRARY}")
  ENDIF (NOT TOOLKIT_CLIENTDB_FIND_QUIETLY)
ELSE (TOOLKIT_CLIENTDB_FOUND)
  IF (TOOLKIT_CLIENTDB_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for toolkit-client libraries named ${TOOLKIT_CLIENTDB_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find toolkit-client library")
  ENDIF (TOOLKIT_CLIENTDB_FIND_REQUIRED)
ENDIF (TOOLKIT_CLIENTDB_FOUND)

MARK_AS_ADVANCED(
  TOOLKIT_CLIENTDB_LIBRARY
  TOOLKIT_CLIENTDB_INCLUDE_DIR
  )
