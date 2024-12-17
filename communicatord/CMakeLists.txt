#
# This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_NAME ${CMANGOS_BINARY_COMMUNICATORD_NAME})

set(EXECUTABLE_SRCS
    CommunicatorDefines.h
    Database/DatabaseManager.h
    Database/DatabaseManager.cpp
    Json/JsonReader.h
    Json/JsonReader.cpp
    Json/NostCoreJson.cpp
    Json/NostCoreJson.h
    Network/JsonSocket.h
    Network/JsonSocket.cpp
    Network/CoreSender.h
    Network/CoreSender.cpp
    main.cpp
    )

# Create visual studio groups using folder structure
foreach(source ${EXECUTABLE_SRCS})
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

if(WIN32)
  # add resource file to windows build
  set(EXECUTABLE_SRCS ${EXECUTABLE_SRCS} communicatord.rc)
endif()

add_executable(${EXECUTABLE_NAME}
  ${EXECUTABLE_SRCS}
)

target_include_directories(${EXECUTABLE_NAME}
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC ${ROOT_DIR}/dep/json
  PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${EXECUTABLE_NAME}
  shared
  ${Boost_LIBRARIES}
)

if(WIN32)
  target_link_libraries(${EXECUTABLE_NAME}
    optimized ${MYSQL_LIBRARY}
    optimized ${OPENSSL_LIBRARIES}
    debug ${MYSQL_DEBUG_LIBRARY}
    debug ${OPENSSL_DEBUG_LIBRARIES}
    ${Boost_LIBRARIES}
  )
  if(MINGW)
    target_link_libraries(${EXECUTABLE_NAME}
      wsock32
      ws2_32
    )
  endif()

    # Define OutDir to source/bin/(platform)_(configuaration) folder.
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DEV_BIN_DIR}")
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DEV_BIN_DIR}")
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEV_BIN_DIR}")
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${DEV_BIN_DIR}")
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${DEV_BIN_DIR}")
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES PROJECT_LABEL "CommunicationServer")

    # Add conf file to dev folder
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${CMAKE_CURRENT_SOURCE_DIR}/communicatord.conf.dist.in\" \"$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/communicatord.conf.dist\")
endif()

if(UNIX)
  target_link_libraries(${EXECUTABLE_NAME}
    ${OPENSSL_LIBRARIES}
    ${OPENSSL_EXTRA_LIBRARIES}
    ${Boost_LIBRARIES}
  )

  if(POSTGRESQL AND POSTGRESQL_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} ${PostgreSQL_LIBRARIES})
  else()
    target_link_libraries(${EXECUTABLE_NAME} ${MYSQL_LIBRARY})
  endif()
endif()

set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS
  "${EXECUTABLE_LINK_FLAGS}"
)

message(STATUS "Executable will be linked with flags: ${EXECUTABLE_LINK_FLAGS}")

install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${BIN_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/communicatord.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/communicatord.conf.dist)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/communicatord.conf.dist DESTINATION ${CONF_DIR})

if(MSVC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/\${BUILD_TYPE}/${EXECUTABLE_NAME}.pdb DESTINATION ${BIN_DIR} CONFIGURATIONS Debug)
endif()
