cmake_minimum_required(VERSION 3.16)
project(InventoryManagement LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt (prefers Qt 6, gracefully falls back to Qt 5)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

# Find SQLite3
find_package(SQLite3 REQUIRED)

set(BACKEND_SOURCES
    src/Database.cpp
    src/Auth.cpp
    src/Inventory.cpp
    src/Billing.cpp
)

set(GUI_SOURCES
    src/gui/main_gui.cpp
    src/gui/LoginDialog.cpp
    src/gui/MainWindow.cpp
    src/gui/InventoryWidget.cpp
    src/gui/BillingWidget.cpp
    src/gui/InvoicesWidget.cpp
    src/gui/ReportsWidget.cpp
    src/gui/UsersWidget.cpp
    src/gui/InvoiceDialog.cpp
    src/gui/ProductDialogs.cpp
)

# Main Qt GUI Target
add_executable(InventoryApp
    ${BACKEND_SOURCES}
    ${GUI_SOURCES}
)

target_include_directories(InventoryApp PRIVATE
    src
    src/gui
)

target_link_libraries(InventoryApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    SQLite::SQLite3
)

# Optional CLI Target for automated testing or headless execution
add_executable(InventoryCLI
    src/main.cpp
    ${BACKEND_SOURCES}
)

target_include_directories(InventoryCLI PRIVATE
    src
)

target_link_libraries(InventoryCLI PRIVATE
    SQLite::SQLite3
)

