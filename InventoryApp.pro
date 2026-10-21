QT       += core gui widgets printsupport

CONFIG += c++17

TARGET = InventoryApp
TEMPLATE = app

INCLUDEPATH += src src/gui

HEADERS += \
    src/Database.h \
    src/Auth.h \
    src/Inventory.h \
    src/Billing.h \
    src/models.h \
    src/picosha2.h \
    src/gui/Theme.h \
    src/gui/LoginDialog.h \
    src/gui/MainWindow.h \
    src/gui/InventoryWidget.h \
    src/gui/BillingWidget.h \
    src/gui/InvoicesWidget.h \
    src/gui/ReportsWidget.h \
    src/gui/UsersWidget.h \
    src/gui/InvoiceDialog.h \
    src/gui/ProductDialogs.h

SOURCES += \
    src/Database.cpp \
    src/Auth.cpp \
    src/Inventory.cpp \
    src/Billing.cpp \
    src/gui/main_gui.cpp \
    src/gui/LoginDialog.cpp \
    src/gui/MainWindow.cpp \
    src/gui/InventoryWidget.cpp \
    src/gui/BillingWidget.cpp \
    src/gui/InvoicesWidget.cpp \
    src/gui/ReportsWidget.cpp \
    src/gui/UsersWidget.cpp \
    src/gui/InvoiceDialog.cpp \
    src/gui/ProductDialogs.cpp

LIBS += -lsqlite3

