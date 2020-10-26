import qbs 1.0
import qbs.Process
import qbs.Environment
import "../../BuildHelpers.js" as Funcs
import "../../BaseDefines.qbs" as Library
import "../../Precompiled.qbs" as Precompiled

Library{
name: "UniversalModels"
type: "staticlibrary"
Depends { name: "Qt.core"}
Depends { name: "Qt.sql" }
Depends { name: "cpp" }
Depends { name: "SettingsProjectLevel"}
Depends { name: "CoreZeroSql"}

Depends { name: "Qt.sql"}
Depends { name: "Qt.concurrent"}
Depends {
    name: "Qt.widgets"
    condition: project.useGuiLib
}
Depends {
    name: "Qt.gui"
    condition: project.useGuiLib
}
Depends { name: "logger" }
Precompiled{condition:SettingsProjectLevel.usePrecompiledHeader}
//cpp.combineCxxSources: conditionals.useCombinedSources &&
//                       Funcs.inCombinedSourcesSet(conditionals.combinedSourcesSet, name)
Export{
    Depends { name: "cpp" }
    cpp.includePaths: [product.sourceDirectory + "/../"]
}

cpp.defines: base.concat(["L_TREE_CONTROLLER_LIBRARY"])
cpp.includePaths: [
                "../",
                "../../",
                sourceDirectory,
                sourceDirectory + "/include",
                "../../external",
]

files: [
        "include/TreeFilteringCache.cpp",
        "include/TreeFilteringCache.h",
        "include/TreeModel.h",
        "include/TreeItemInterface.h",
        "include/l_tree_controller_global.h",
        "include/AdaptingTableModel.h",
        "include/TreeItem.h",
        "include/TableDataInterface.h",
        "include/TableDataListHolder.h",
        "include/ItemController.h",
        "include/treeviewfunctions.h",
        "include/treeviewtemplatefunctions.h",
        "include/treeitemfunctions.h",
        "include/genericeventfilter.h",
        "src/TreeModel.cpp",
        "src/TreeItemInterface.cpp",
        "src/InterfaceAwareTreeData.cpp",
        "src/AddressTemplateData.cpp",
        "src/AddressTemplateTreeData.cpp",
        "src/AdaptingTableModel.cpp",
        "src/TableDataInterface.cpp",
        "src/AdaptingTableModelPrivate.cpp",
        "src/treeviewfunctions.cpp",
        "src/treeviewtemplatefunctions.cpp",
        "src/genericeventfilter.cpp",
        "src/version/version.cpp",
    ]


}
