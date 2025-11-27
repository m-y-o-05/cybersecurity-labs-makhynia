/*
 * main.cpp
 *
 * Точка входу програми - SQL Injection.
 * Демонструє уразливості та захист від SQL-ін'єкцій.
 */

#include "MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Ініціалізація бази даних.
    DatabaseManager dbManager;
    if (!dbManager.initializeDatabase()) {
        QMessageBox::critical(
            nullptr,
            "Помилка ініціалізації",
            "Не вдалося створити базу даних!\n\n"
            "Програма буде закрита."
            );
        return 1;
    }

    // Створення та показ головного вікна.
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
