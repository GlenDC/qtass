QT += core gui widgets

TARGET = uibuilder_test
TEMPLATE = app

HEADERS       = ../../src/uibuilder/*.h
SOURCES       = ../../src/uibuilder/*.cpp ../../src/uibuilder/test/*.cpp 

LIBS += -llua
