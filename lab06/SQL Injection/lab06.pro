QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Визначення для правильної роботи
DEFINES += QT_DEPRECATED_WARNINGS

# Вихідні файли
SOURCES += \
    Main.cpp \
    MainWindow.cpp \
    DatabaseManager.cpp \
    VulnerableQueries.cpp \
    SecureQueries.cpp \
    AttackLogger.cpp

# Заголовочні файли
HEADERS += \
    MainWindow.h \
    DatabaseManager.h \
    VulnerableQueries.h \
    SecureQueries.h \
    AttackLogger.h

# Форми (якщо використовуються)
FORMS +=

# Ресурси
RESOURCES +=

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
