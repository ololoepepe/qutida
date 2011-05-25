
HEADERS += \
    src/gui/mainwindow.h \
    src/core/threadmanager.h \
    src/core/imageboardthread.h \
    src/mv/threadmodel.h \
    src/mv/treeitem.h \
    src/common.h \
    src/core/download.h \
    src/core/networkaccessmanager.h \
    src/core/savetask.h \
    src/core/parcetask.h \
    src/core/rmdirtask.h \
    src/mv/categorymodel.h \
    src/mv/treemodel.h \
    src/tr.h \
    src/mv/threadmodelitem.h \
    src/gui/addthread.h \
    src/gui/parametersdialog.h \
    src/gui/removedialog.h \
    src/gui/infowidget.h \
    src/gui/threadparameters.h \
    src/mv/progressbardelegate.h \
    src/gui/authentication.h \
    src/core/localserver.h \
    src/core/threadinfo.h \
    src/core/savepagetask.h

SOURCES += \
    src/gui/mainwindow.cpp \
    src/core/threadmanager.cpp \
    src/core/imageboardthread.cpp \
    src/mv/threadmodel.cpp \
    src/mv/treeitem.cpp \
    src/core/download.cpp \
    src/core/networkaccessmanager.cpp \
    src/core/savetask.cpp \
    src/core/parcetask.cpp \
    src/core/rmdirtask.cpp \
    src/mv/categorymodel.cpp \
    src/mv/treemodel.cpp \
    src/common.cpp \
    src/mv/threadmodelitem.cpp \
    src/main.cpp \
    src/gui/addthread.cpp \
    src/tr.cpp \
    src/gui/parametersdialog.cpp \
    src/gui/removedialog.cpp \
    src/gui/infowidget.cpp \
    src/gui/threadparameters.cpp \
    src/mv/progressbardelegate.cpp \
    src/gui/authentication.cpp \
    src/core/localserver.cpp \
    src/core/threadinfo.cpp \
    src/core/savepagetask.cpp

QT += \
    network

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    res/loc/qutida_ru.ts
