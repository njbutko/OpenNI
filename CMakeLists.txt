cmake_minimum_required(VERSION 2.8)
project(OpenNI)

# Find platform
# -------------
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(OPENNI_PLATFORM_LINUX true)
elseif (WIN32)
	set(OPENNI_PLATFORM_WIN32 true)
elseif (APPLE)
	set(OPENNI_PLATFORM_APPLE true)
endif()

# Some global variables
# ----------------------
set(TINYXML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/External/TinyXml/")
set(JPEG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/External/LibJPEG/")
set(OPENNI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include/")
set(OPENNI_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/Modules")

set(TINYXML_LIB_NAME TinyXml)
set(JPEG_LIB_NAME JPEG)
set(OPENNI_LIB_NAME OpenNI)
set(NIMCOMMON_LIB_NAME nimCommon)
set(NIMCODECS_LIB_NAME nimCodecs)
set(NIMMOCKNODES_LIB_NAME nimMockNodes)
set(NIMRECORDER_LIB_NAME nimRecorder)


# Use external libraries.
# -----------------------
add_subdirectory("./Source/External/TinyXml/")
add_subdirectory("./Source/OpenNI/")
add_subdirectory("./Source/External/LibJPEG/")
add_subdirectory("./Source/Modules/")

add_dependencies(${OPENNI_LIB_NAME} ${TINYXML_LIB_NAME})
add_dependencies(${JPEG_LIB_NAME} ${OPENNI_LIB_NAME})
# Add samples
# ---------------------------
add_subdirectory("./Samples/NiAudioSample") #Compiles and runs on Mac
add_subdirectory("./Samples/NiBackRecorder") #Compiles and runs on Mac
add_subdirectory("./Samples/NiConvertXToONI") #Compiles and runs on Mac
#add_subdirectory("./Samples/NiCRead") #Compiles and runs on Mac # gives error in linux
add_subdirectory("./Samples/NiRecordSynthetic") #Compiles and runs on Mac
add_subdirectory("./Samples/NiSampleModule") #Library compiles on Mac
add_subdirectory("./Samples/NiSimpleCreate") #Compiles and runs on Mac
add_subdirectory("./Samples/NiSimpleRead")  #Compiles and runs on Mac
add_subdirectory("./Samples/NiSimpleViewer") #Compiles and runs on Mac
add_subdirectory("./Samples/NiUserTracker") #Compiles and runs on Mac
add_subdirectory("./Samples/NiViewer") #Compiles and runs on Mac

# Utils
# ---------------------------
add_subdirectory("./Source/Utils/")


