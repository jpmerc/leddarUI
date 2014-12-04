cmake_minimum_required(VERSION 2.6)
PROJECT(leddarUI)
 
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
 

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
 
QT4_WRAP_UI(UISrcs UI/mainwindow.ui)
QT4_WRAP_CPP(MOCSrcs UI/mainwindow.h UI/qcustomplot.h UI/ui_mainwindow.h)
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
 
#ADD_LIBRARY(leddarUI UI/main.cpp UI/mainwindow.cpp UI/qcustomplot.cpp ${MOCSrcs} ${UISrcs})
ADD_LIBRARY(leddarUI UI/mainwindow.cpp UI/qcustomplot.cpp ${MOCSrcs} ${UISrcs})
TARGET_LINK_LIBRARIES(leddarUI ${QT_LIBRARIES})



include_directories(include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(leddartech_node 
				src/leddartech.cpp 
				include/LeddarC.h 
				include/LeddarCommands.h 
				include/LeddarProperties.h 
				include/LeddarResults.h
)

target_link_libraries(leddartech_node LeddarTech Leddar LeddarC leddarUI)


