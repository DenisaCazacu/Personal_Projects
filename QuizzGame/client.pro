# Tipul de proiect: aplicație
TEMPLATE = app
TARGET = client

# Standardul C++
CONFIG += c++17

# Biblioteci Qt necesare
QT += widgets

# Fișiere sursă
SOURCES += \
    client.cpp \
    Frontend/startwindow.cpp \
    Frontend/mainwindow.cpp \
    Frontend/readywindow.cpp \
    Frontend/playwindow.cpp \
    Frontend/gameoverwindow.cpp \

# Fișiere header
HEADERS += \
    Frontend/startwindow.h \
    Frontend/readywindow.h \
    Frontend/mainwindow.h \
    Frontend/playwindow.h \
    Frontend/gameoverwindow.h \

RESOURCES += \
    Frontend/images.qrc


# Include path-uri suplimentare, dacă e necesar
INCLUDEPATH += Frontend

