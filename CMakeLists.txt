cmake_minimum_required (VERSION 3.9)
project (log_system VERSION 1.0.0 DESCRIPTION "A simple Log system in C")

#[[ Building Library ]]

include(GNUInstallDirs)

set(CMAKE_BUILD_TYPE Debug)


set(LIBRARY_HEADER_FILES
	src/log_system.h
	src/inih/ini.h
)

set(LIBRARY_SOURCE_FILES
	src/log_system.c
	src/inih/ini.c
)


add_library(log_system STATIC
	${LIBRARY_HEADER_FILES}
	${LIBRARY_SOURCE_FILES}
)

target_link_libraries(log_system
	${ALL_LIBS}
)	

set_target_properties(log_system PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(log_system PROPERTIES PUBLIC_HEADER inc/log_system.h)

target_include_directories(log_system PRIVATE inc)
target_include_directories(log_system PRIVATE src)

install(TARGETS log_system
	  	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})



#[[ Building Server ]]

	#[[ Compiling Main ]]

set(SERVER_SOURCE_FILES
	src/log_system_server.c
)

set(INCLUDE_FILE
	inc/log_system.h
)

add_executable(log_system_server_exe
	${INCLUDE_FILE}
	${SERVER_SOURCE_FILES}
)

target_link_libraries(log_system_server_exe
	log_system
)	


set(EXAMPLE_SOURCE_FILES
	examples/main.c
)

add_executable(example_exe
	${INCLUDE_FILE}
	${EXAMPLE_SOURCE_FILES}
)

target_link_libraries(example_exe
	log_system
	pthread
)	
