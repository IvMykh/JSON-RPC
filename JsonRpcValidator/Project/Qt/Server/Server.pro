QT += core
QT -= gui

CONFIG += c++11

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    ../../Source/Server/json_rpc_object_type.cpp \
    ../../Source/Server/json_rpc_validator.cpp \
    ../../Source/Server/lexer.cpp \
    ../../Source/Server/non_terminal_node.cpp \
    ../../Source/Server/parse_tree_node.cpp \
    ../../Source/Server/parser.cpp \
    ../../Source/Server/server_app.cpp \
    ../../Source/Server/server_main.cpp \
    ../../Source/Server/server_socket.cpp \
    ../../Source/Server/server_socket_exception.cpp \
    ../../Source/Server/token.cpp \
    ../../Source/Server/validation_error_exception.cpp \
    ../../Source/Server/visualize_tree_visitor.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./../../Source/Server/

HEADERS += \
    ../../Source/Server/json_rpc_object_type.h \
    ../../Source/Server/json_rpc_validator.h \
    ../../Source/Server/lexer.h \
    ../../Source/Server/node_type.h \
    ../../Source/Server/non_terminal_node.h \
    ../../Source/Server/parse_tree_node.h \
    ../../Source/Server/parse_tree_visitor.h \
    ../../Source/Server/parser.h \
    #../../Source/Server/server_app.h \
    #../../Source/Server/server_socket.h \
    ../../Source/Server/server_socket_exception.h \
    ../../Source/Server/token.h \
    ../../Source/Server/validation_error_exception.h \
    ../../Source/Server/validation_state.h \
    ../../Source/Server/visualize_tree_visitor.h \
    ../../Source/Server/server_socket.h
