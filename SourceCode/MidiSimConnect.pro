QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

RC_ICONS = myappico.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    RtMidi.cpp \
    dialogassign.cpp \
    fileHandler.cpp \
    flightSim.cpp \
    main.cpp \
    mainwindow.cpp \
    midiObject.cpp

HEADERS += \
    RtMidi.h \
    dialogassign.h \
    fileHandler.h \
    flightSim.h \
    mainwindow.h \
    SimConnect.h \
    midiObject.h

FORMS += \
    dialogassign.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L'C:/MSFS SDK/SimConnect SDK/lib/static/' -lSimConnect

INCLUDEPATH += 'C:/MSFS SDK/SimConnect SDK/include'
INCLUDEPATH += 'C:/MSFS SDK/SimConnect SDK/lib/static'
DEPENDPATH += 'C:/MSFS SDK/SimConnect SDK/lib/static'

win32:!win32-g++: PRE_TARGETDEPS += 'C:/MSFS SDK/SimConnect SDK/lib/static/SimConnect.lib'
else:win32-g++: PRE_TARGETDEPS += 'C:/MSFS SDK/SimConnect SDK/lib/static/libSimConnect.a'

win32:CONFIG(release, debug|release): LIBS += -L'C:/MSFS SDK/SimConnect SDK/lib/static/' -lSimConnect_debug
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/MSFS SDK/SimConnect SDK/lib/static/' -lSimConnect_debug

INCLUDEPATH += 'C:/MSFS SDK/SimConnect SDK/lib/static'
DEPENDPATH += 'C:/MSFS SDK/SimConnect SDK/lib/static'

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += 'C:/MSFS SDK/SimConnect SDK/lib/static/libSimConnect_debug.a'
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += 'C:/MSFS SDK/SimConnect SDK/lib/static/libSimConnect_debug.a'
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += 'C:/MSFS SDK/SimConnect SDK/lib/static/SimConnect_debug.lib'
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += 'C:/MSFS SDK/SimConnect SDK/lib/static/SimConnect_debug.lib'

win32: LIBS += -lWinMM
win32: LIBS += -lshlwapi
win32: LIBS += -luser32
win32: LIBS += -lWs2_32
win32: LIBS += -lkernel32
win32: LIBS += -lgdi32
win32: LIBS += -lwinspool
win32: LIBS += -lcomdlg32
win32: LIBS += -ladvapi32
win32: LIBS += -lshell32
win32: LIBS += -lole32
win32: LIBS += -loleaut32
win32: LIBS += -luuid
win32: LIBS += -lodbc32
win32: LIBS += -lodbccp32
