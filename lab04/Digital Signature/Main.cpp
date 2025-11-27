/*
 * main.cpp
 *
 * Головний файл програми цифрових підписів.
 * Точка входу в програму.
 */

#include "DigitalSignatureWindow.h"
#include <QApplication>

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
    DigitalSignatureWindow window;
    window.show();

    // Запускаємо цикл обробки подій.
    return app.exec();
}
