cmake_minimum_required(VERSION 3.16)

project(GoMonte VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        include/GomokuBoard.h
        src/board/GomokuBoard.cpp
        tests/judge_test.cpp
        include/SearchAgent.h
        src/ai/SearchAgent.cpp
        include/FileOP.h
        "src/file operations/FileOP.cpp"
        include/MainBoardWidget.h
        src/widgets/MainBoardWidget.cpp
        include/MainMenuWidget.h
        src/widgets/MainMenuWidget.cpp
        include/BoardCellWidget.h
        src/widgets/BoardCellWidget.cpp
        include/GameData.h
        src/game/GameData.cpp
        include/HelperFunctions.h
        src/helper/HelperFunctions.cpp
        include/GameController.h
        src/game/GameController.cpp
        include/GameSaver.h
        src/game/GameSaver.cpp
        include/SaveSelectionDialog.h
        src/widgets/SaveSelectionDialog.cpp
        include/MCTSTree.h
        src/ai/MCTSTree.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GoMonte
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GoMonte APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GoMonte SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GoMonte
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(GoMonte PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GoMonte)
endif()
set_target_properties(GoMonte PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GoMonte
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GoMonte)
endif()

include_directories("include")

qt_add_resources(GoMonte "resources.qrc"
    PREFIX "/" FILES
        resources/icons/GoMonte.png
)

if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/app_icon.rc")
    target_sources(GoMonte PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")