QT += core gui widgets

TARGET = qtass
TEMPLATE = app

HEADERS       = ../../src/qtass/*.h ../../src/uibuilder/*.h
SOURCES       = ../../src/qtass/*.cpp ../../src/uibuilder/*.cpp

LIBS += -llua
