cmake_minimum_required(VERSION 2.7)

project(ContGen-IntClk)

set(NI_folder "C:/Program Files (x86)/National Instruments/NI-DAQ/DAQmx ANSI C Dev")
set(NI_LIB_DIRS "${NI_folder}/lib/msvc")

include_directories("${NI_folder}/include")
message(STATUS "${NI_LIB_DIRS}/*.lib")
file(GLOB NI_LIBRARIES "${NI_LIB_DIRS}/*.lib")


message(STATUS "NI_LIBRARIES: " ${NI_LIBRARIES})
message(STATUS "NI_FOLDER: " ${NI_folder})

add_executable(ContGen-IntClk ContGen-IntClk.c)
add_executable(ContGen-IntClk-AnlgStart ContGen-IntClk-AnlgStart.c)
add_executable(MultVoltUpdates-IntClk MultVoltUpdates-IntClk.c)
add_executable(signalOutTest signalOutTest.cpp signalOut.h)
 
target_link_libraries(ContGen-IntClk ${NI_LIBRARIES} )
target_link_libraries(ContGen-IntClk-AnlgStart ${NI_LIBRARIES} )
target_link_libraries(MultVoltUpdates-IntClk ${NI_LIBRARIES} )
target_link_libraries(signalOutTest ${NI_LIBRARIES} )