set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

IF (NOT DEFINED ENV{QT6_DIR})
    message(WARNING "!(1)! Please Set the environment variable QT6_DIR to the CMake directory of QT6")
    message(WARNING "!(2)! Also think about adding the path to Qt6 dlls to PATH")
ENDIF()

find_package(Qt6 COMPONENTS Core Quick Qml gui quickcontrols2 LinguistTools REQUIRED HINTS $ENV{QT6_DIR})

file(GLOB GUI_SRCS *.cpp)
set(TS_FILES doc_speak-gui_en_US.ts)

qt_add_executable(
    DocSpeak
    ${GUI_SRCS}
)

file(
    GLOB QML_FILES 
    RELATIVE ${DOCSPEACK_QML}
    *.qml
)
file(
    GLOB QML_JS_FILES 
    RELATIVE ${DOCSPEACK_QML}
    *.js
)
qt_add_resources(
    DocSpeak
    qmlfiles
    FILES
    ${QML_FILES}
    ${QML_JS_FILES}
)

file(GLOB ASSETS RELATIVE ${DOCSPEACK_QML} ${DOCSPEACK_ASSETS}/*.*)
qt_add_resources(
    DocSpeak
    assets
    FILES
    ${ASSETS}
)

qt_add_translation(
    DocSpeak 
    TS_FILES 
    ${TS_FILES}
)

target_compile_definitions(DocSpeak
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(
    DocSpeak
    PRIVATE 
    DocSpeak::Common
    Qt6::Core 
    Qt6::Quick
)

