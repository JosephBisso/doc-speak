set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick Qml gui quickcontrols2 LinguistTools REQUIRED HINTS $ENV{QT5_DIR})

file(GLOB GUI_SRCS *.cpp)
set(TS_FILES doc_speak-gui_en_US.ts)

set(
    PROJECT_SOURCES
    ${GUI_SRCS}
    qml.qrc
    ${TS_FILES}
)

add_executable(
    DocSpeak
    ${PROJECT_SOURCES}
)

qt5_add_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_compile_definitions(DocSpeak
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(
    DocSpeak
    PRIVATE 
    DocSpeak::Common
    Qt5::Core 
    Qt5::Quick
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(DocSpeak)
    qt_finalize_executable(DocSpeak)
endif()
