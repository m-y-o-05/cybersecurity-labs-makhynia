/*
 * IconGenerator.cpp.
 *
 * Реалізація класу для програмного створення іконок.
 */

#include "IconGenerator.h"
#include <QPainter>
#include <QRadialGradient>

/**
 * @brief Створення іконки програми.
 * @return QIcon з іконкою замка.
 *
 * Публічний метод для отримання іконки програми.
 * Генерує іконку розміром 128x128 пікселів та
 * повертає її як QIcon для використання у вікні.
 */
QIcon IconGenerator::createProgramIcon() {
    return QIcon(generateLockIcon(128));
}

/**
 * @brief Генерація зображення замка.
 * @param size Розмір іконки в пікселях.
 * @return QPixmap з намальованою іконкою.
 *
 * Функція створює квадратне зображення з замком, що складається з:
 *
 * 1. Прозорий фон:
 *    - Pixmap заповнюється прозорістю для коректного відображення.
 *
 * 2. Бірюзове коло (фон):
 *    - Радіальний градієнт від центру до країв.
 *    - Кольори: #00E6E8 (світлий центр) → #00CED1 (середина) → #009999 (темні краї).
 *    - Форма: еліпс з відступом 8px від країв.
 *
 * 3. Чорний замок:
 *    a) Тіло замка:
 *       - Закруглений прямокутник (#1a1a1a).
 *       - Розташування: нижня третина кола.
 *       - Заокруглення: 8px.
 *
 *    b) Дужка замка:
 *       - Дуга зверху над тілом.
 *       - Кут: 180° (напівколо).
 *       - Без заповнення (тільки контур).
 *
 *    c) Замкова щілина:
 *       - Вертикальна бірюзова лінія (#00CED1).
 *       - Кружечок зверху лінії (імітація отвору для ключа).
 *
 * Всі розміри розраховуються пропорційно до параметра size,
 * що дозволяє генерувати іконки будь-якого розміру з правильними пропорціями.
 */
QPixmap IconGenerator::generateLockIcon(int size) {
    // ============ Створення прозорого pixmap ============
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    // ============ Ініціалізація painter з антиаліасингом ============
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // ============ Розрахунок розмірів ============
    int center = size / 2;           // Центр іконки.
    int radius = size / 2 - 8;       // Радіус кола (з відступом 8px).

    // ============ Фон: радіальний градієнт ============
    // Градієнт йде від центру (світлий) до країв (темний).
    QRadialGradient gradient(center, center, radius);
    gradient.setColorAt(0, QColor(0, 230, 232));      // Центр: світло-бірюзовий.
    gradient.setColorAt(0.5, QColor(0, 206, 209));    // Середина: основний бірюзовий.
    gradient.setColorAt(1, QColor(0, 153, 153));      // Краї: темно-бірюзовий.

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(8, 8, size - 16, size - 16);

    // ============ Замок: підготовка пензля ============
    painter.setPen(QPen(QColor("#1a1a1a"), size / 21));
    painter.setBrush(QColor("#1a1a1a"));

    // ============ Тіло замка (закруглений прямокутник) ============
    int lockWidth = size * 11 / 32;      // Ширина замка (~34% розміру іконки).
    int lockHeight = size * 5 / 16;      // Висота замка (~31% розміру іконки).
    int lockX = (size - lockWidth) / 2;  // Центрування по горизонталі.
    int lockY = size * 15 / 32;          // Розташування: трохи нижче центру.

    painter.drawRoundedRect(lockX, lockY, lockWidth, lockHeight, 8, 8);

    // ============ Дужка замка (дуга зверху) ============
    painter.setBrush(Qt::NoBrush);       // Тільки контур, без заповнення.

    int arcWidth = size * 7 / 32;        // Ширина дужки (~22% розміру іконки).
    int arcHeight = size * 35 / 128;     // Висота дужки (~27% розміру іконки).
    int arcX = (size - arcWidth) / 2;    // Центрування дужки.
    int arcY = size * 19 / 64;           // Розташування: над тілом замка.

    // Малюємо дугу 180° (напівколо зверху).
    // Параметри: x, y, width, height, startAngle (0°), spanAngle (180° * 16).
    painter.drawArc(arcX, arcY, arcWidth, arcHeight, 0, 180 * 16);

    // ============ Замкова щілина (бірюзова лінія + точка) ============
    painter.setPen(QPen(QColor(0, 206, 209), size / 32));

    // Вертикальна лінія.
    painter.drawLine(center, size * 9 / 16, center, size * 11 / 16);

    // Кружечок зверху.
    painter.drawEllipse(center - 3, size * 17 / 32, 6, 6);

    return pixmap;
}
