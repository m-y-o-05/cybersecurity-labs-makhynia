/*
 * Main.cpp
 *
 * Головний файл програми стеганографії.
 * Точка входу в програму.
 */

#include "SteganographyWindow.h"
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
    SteganographyWindow window;
    window.show();

    // Запускаємо цикл обробки подій.
    return app.exec();
}
