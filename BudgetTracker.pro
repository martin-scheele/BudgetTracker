QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/BudgetTracker.cpp \
    src/ForgotLoginDialog.cpp \
    src/LoginDatabaseManager.cpp \
    src/RegistrationDialog.cpp \
    src/User.cpp \
    src/main.cpp \
    src/LoginDialog.cpp \
    src/qcustomplot.cpp

HEADERS += \
    src/BudgetTracker.h \
    src/ForgotLoginDialog.h \
    src/LoginDatabaseManager.h \
    src/LoginDialog.h \
    src/RegistrationDialog.h \
    src/User.h \
    src/qcustomplot.h

FORMS += \
    src/BudgetTracker.ui \
    src/ForgotLoginDialog.ui \
    src/LoginDialog.ui \
    src/RegistrationDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
