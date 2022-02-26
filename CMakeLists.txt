cmake_minimum_required(VERSION 3.19.2)
# SETTING UP COMPILE FEATURE BY ENVIRONMENT
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
project(UniversalModels)
find_package(Qt5 REQUIRED COMPONENTS Core Sql Concurrent Widgets Gui)



add_library(UniversalModels STATIC
    "include/TreeFilteringCache.cpp"
    "include/TreeFilteringCache.h"
    "include/TreeModel.h"
    "include/TreeItemInterface.h"
    "include/custom_icon_delegate.h"
    "include/l_tree_controller_global.h"
    "include/AdaptingTableModel.h"
    "include/TreeItem.h"
    "include/TableDataInterface.h"
    "include/TableDataListHolder.h"
    "include/ItemController.h"
    "include/treeviewfunctions.h"
    "include/treeviewtemplatefunctions.h"
    "include/treeitemfunctions.h"
    "include/genericeventfilter.h"
    "src/TreeModel.cpp"
    "src/TreeItemInterface.cpp"
    "src/InterfaceAwareTreeData.cpp"
    "src/AddressTemplateData.cpp"
    "src/AddressTemplateTreeData.cpp"
    "src/AdaptingTableModel.cpp"
    "src/TableDataInterface.cpp"
    "src/AdaptingTableModelPrivate.cpp"
    "src/custom_icon_delegate.cpp"
    "src/treeviewfunctions.cpp"
    "src/treeviewtemplatefunctions.cpp"
    "src/genericeventfilter.cpp"
    )

target_link_libraries(UniversalModels PUBLIC  Qt5::Core Qt5::Sql Qt5::Concurrent Qt5::Gui Qt5::Widgets Logger)

target_include_directories(UniversalModels PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../    )

include(GenerateExportHeader)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

generate_export_header(UniversalModels
    EXPORT_FILE_NAME "${CMAKE_CURRENT_LIST_DIR}/include/l_tree_controller_global.h"
    EXPORT_MACRO_NAME "L_TREE_CONTROLLER_EXPORT")

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(UniversalModels PUBLIC UIMODELS_STATIC_DEFINE)
endif()

install(TARGETS UniversalModels DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../debug)
