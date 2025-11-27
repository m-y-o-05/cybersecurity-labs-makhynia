/*
 * main.cpp
 *
 * Точка входу програми Email-шифратора.
 * Ініціалізує Qt додаток та показує головне вікно.
 */

#include "EmailEncryptorWindow.h"
#include <QApplication>
#include <QIcon>

/**
 * @brief Головна функція програми.
 * @param argc Кількість аргументів командного рядка.
 * @param argv Масив аргументів командного рядка.
 * @return Код завершення програми.
 */
int main(int argc, char *argv[]) {
    // Створюємо об'єкт додатку Qt.
    QApplication app(argc, argv);

    // Створюємо та показуємо головне вікно.
    EmailEncryptorWindow window;
    window.show();

    // Запускаємо цикл обробки подій.
    return app.exec();
}
