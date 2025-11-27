/*
 * IconGenerator.h.
 *
 * Заголовочний файл генератора іконок.
 * Містить оголошення класу для програмного створення
 * іконки програми з замком.
 */

#ifndef ICONGENERATOR_H
#define ICONGENERATOR_H

#include <QIcon>
#include <QPixmap>

/**
 * @class IconGenerator.
 * @brief Клас для програмного створення іконок.
 *
 * Цей клас генерує іконку програми без використання
 * зовнішніх файлів зображень. Іконка створюється
 * за допомогою QPainter з використанням градієнтів
 * та геометричних фігур.
 */
class IconGenerator {
public:
    /**
     * @brief Створення іконки програми з замком.
     * @return QIcon з програмно згенерованою іконкою.
     *
     * Головний публічний метод для створення іконки.
     * Викликає приватний метод generateLockIcon() для
     * створення зображення та повертає його як QIcon.
     */
    static QIcon createProgramIcon();

private:
    /**
     * @brief Генерація зображення замка на бірюзовому фоні.
     * @param size Розмір іконки в пікселях.
     * @return QPixmap з намальованим замком.
     *
     * Створює квадратне зображення з фоном та замкомна.
     */
    static QPixmap generateLockIcon(int size = 128);
};

#endif // ICONGENERATOR_H
