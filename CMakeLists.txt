cmake_minimum_required(VERSION 2.6)
PROJECT(leddarUI)
 
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

set(COMPILE_RASPBERRY_PI FALSE)

# SET TO BUILD THE PROGRAM IN RELEASE OR DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
 

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
 
QT4_WRAP_UI(UISrcs UI/mainwindow.ui)
QT4_WRAP_CPP(MOCSrcs UI/mainwindow.h UI/qcustomplot.h UI/ui_mainwindow.h)
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
 
#ADD_LIBRARY(leddarUI UI/main.cpp UI/mainwindow.cpp UI/qcustomplot.cpp ${MOCSrcs} ${UISrcs})
ADD_LIBRARY(leddarUI UI/mainwindow.cpp UI/qcustomplot.cpp ${MOCSrcs} ${UISrcs})
TARGET_LINK_LIBRARIES(leddarUI ${QT_LIBRARIES})


if(COMPILE_RASPBERRY_PI)
	set(LEDDARTECH_INCLUDE_DIRECTORY include_pi)
	set(LEDDARTECH_LIB_DIRECTORY lib_pi)
else()
	set(LEDDARTECH_INCLUDE_DIRECTORY include)
	set(LEDDARTECH_LIB_DIRECTORY lib)	
endif(COMPILE_RASPBERRY_PI)

include_directories(${LEDDARTECH_INCLUDE_DIRECTORY})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/${LEDDARTECH_LIB_DIRECTORY})

add_executable(leddartech_node 
				src/leddartech.cpp 
				${LEDDARTECH_INCLUDE_DIRECTORY}/LeddarC.h 
				${LEDDARTECH_INCLUDE_DIRECTORY}/LeddarCommands.h 
				${LEDDARTECH_INCLUDE_DIRECTORY}/LeddarProperties.h 
				${LEDDARTECH_INCLUDE_DIRECTORY}/LeddarResults.h
)

target_link_libraries(leddartech_node LeddarTech Leddar LeddarC leddarUI)


