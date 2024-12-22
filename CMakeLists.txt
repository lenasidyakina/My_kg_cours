cmake_minimum_required(VERSION 3.29)
project(qtest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "D:/Qt/6.8.0/msvc2022_64")


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(qtest main.cpp
        MainWindow.cpp
        MainWindow.h
        SettingsDialog.cpp
        SettingsDialog.h
        SettingsDialog.ui
        FigureSelectorDialog.cpp
        FigureSelectorDialog.h
        FigureSelectorDialog.ui
        CubePropertiesDialog.cpp
        CubePropertiesDialog.h
        CubePropertiesDialog.ui
        CurTypes.h
        Figure.cpp
        Figure.h
        Cube.cpp
        Cube.h
        RenderArea.cpp
        RenderArea.h
        Sphere.cpp
        Sphere.h
        SpherePropertiesDialog.cpp
        SpherePropertiesDialog.h
        cylinderpropertiesdialog.cpp
        cylinderpropertiesdialog.h
        CylinderPropertiesDialog.ui
        Cylinder.cpp
        Cylinder.h
        TetraderPropertiesDialog.cpp
        TetraderPropertiesDialog.h
        TetraderPropertiesDialog.ui
        Tetrader.cpp
        Tetrader.h
        TrajectoryPropertiesDialog.cpp
        TrajectoryPropertiesDialog.h
        TrajectoryPropertiesDialog.ui
        Trajectory.cpp
        Trajectory.h
)
target_link_libraries(qtest
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
