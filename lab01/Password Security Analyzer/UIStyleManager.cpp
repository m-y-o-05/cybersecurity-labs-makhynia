/*
 * UIStyleManager.cpp.
 *
 * Реалізація класу для централізованого управління стилями інтерфейсу.
 * Містить всі CSS-стилі програми у вигляді raw string literals.
 */

#include "UIStyleManager.h"

/**
 * @brief Отримання головного stylesheet для всієї програми.
 * @return QString з повним CSS для всіх основних елементів UI.
 *
 * Цей метод повертає CSS, який застосовується до всього вікна
 * та визначає загальний вигляд програми.
 *
 * Основні елементи стилізації:
 * 1. QMainWindow - градієнтний темний фон.
 * 2. QGroupBox - напівпрозорі блоки з бірюзовою рамкою.
 * 3. QLabel - світлий текст.
 * 4. QLineEdit/QDateEdit - темні поля з бірюзовою рамкою при фокусі.
 * 5. QTextEdit - текстове поле для результатів.
 * 6. QProgressBar - прогрес-бар з бірюзовим градієнтом.
 * 7. QScrollBar - тонкі скролбари з бірюзовим повзунком.
 */
QString UIStyleManager::getMainStyleSheet() {
    return R"(
        /* ============ Головне вікно ============ */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #0a0a0a, stop:0.3 #1a1a1a, stop:0.7 #1e1e1e, stop:1 #252525);
        }

        /* ============ Групові блоки ============ */
        QGroupBox {
            background: rgba(255, 255, 255, 0.03);
            border: 2px solid rgba(0, 206, 209, 0.3);
            border-radius: 15px;
            margin-top: 15px;
            padding: 20px;
            color: #e0e0e0;
            font-size: 14px;
            font-weight: bold;
        }

        /* ============ Заголовки групових блоків ============ */
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 5px 15px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00CED1, stop:0.5 #00B8BA, stop:1 #009999);
            border-radius: 8px;
            color: #000000;
        }

        /* ============ Мітки (Labels) ============ */
        QLabel {
            color: #e0e0e0;
            font-size: 13px;
        }

        /* ============ Поля вводу ============ */
        QLineEdit, QDateEdit {
            background: rgba(255, 255, 255, 0.05);
            border: 2px solid rgba(0, 206, 209, 0.3);
            border-radius: 10px;
            padding: 12px 15px;
            color: #ffffff;
            font-size: 13px;
            selection-background-color: #00CED1;
        }

        /* Поля вводу в фокусі */
        QLineEdit:focus, QDateEdit:focus {
            border: 2px solid #00CED1;
            background: rgba(255, 255, 255, 0.08);
        }

        /* Placeholder текст */
        QLineEdit::placeholder {
            color: rgba(255, 255, 255, 0.3);
        }

        /* ============ Текстове поле результатів ============ */
        QTextEdit {
            background: rgba(255, 255, 255, 0.03);
            border: 2px solid rgba(0, 206, 209, 0.3);
            border-radius: 12px;
            padding: 15px;
            color: #e0e0e0;
            font-size: 13px;
            selection-background-color: #00CED1;
        }

        /* ============ Прогрес-бар ============ */
        QProgressBar {
            background: rgba(255, 255, 255, 0.05);
            border: none;
            border-radius: 15px;
            text-align: center;
            color: white;
            font-weight: bold;
        }

        /* Заповнена частина прогрес-бару */
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00CED1, stop:0.5 #00B8BA, stop:1 #009999);
            border-radius: 15px;
        }

        /* ============ Скролбари ============ */
        QScrollBar:vertical {
            background: rgba(255, 255, 255, 0.03);
            width: 12px;
            border-radius: 6px;
        }

        /* Повзунок скролбару */
        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00CED1, stop:1 #009999);
            border-radius: 6px;
            min-height: 20px;
        }

        /* Повзунок скролбару при наведенні */
        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00E5E8, stop:1 #00B8BA);
        }
    )";
}

/**
 * @brief Отримання стилю для акцентних міток.
 * @return QString з CSS для міток бірюзового кольору.
 *
 * Використовується для міток, які вказують на поля вводу
 * (наприклад, "Пароль:", "Ім'я:", "Прізвище:").
 */
QString UIStyleManager::getLabelStyle() {
    return "QLabel { color: #00E5E8; font-size: 14px; font-weight: bold; }";
}

/**
 * @brief Отримання стилю для головної кнопки дії.
 * @return QString з CSS для кнопки з бірюзовим градієнтом.
 *
 * Головна кнопка виділяється бірюзовим градієнтом та чорним текстом.
 * Використовується для основної дії програми ("Аналізувати пароль").
 *
 * Стани кнопки:
 * - Звичайний: бірюзовий градієнт #00CED1 → #009999.
 * - Наведення (hover): світліший градієнт #00E5E8 → #00B8BA.
 * - Натискання (pressed): темніший градієнт #00B8BA → #007F82.
 */
QString UIStyleManager::getPrimaryButtonStyle() {
    return R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00CED1, stop:0.5 #00B8BA, stop:1 #009999);
            color: #000000;
            font-size: 16px;
            font-weight: bold;
            border-radius: 15px;
            border: none;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00E5E8, stop:0.5 #00CED1, stop:1 #00B8BA);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00B8BA, stop:0.5 #009999, stop:1 #007F82);
        }
    )";
}

/**
 * @brief Отримання стилю для вторинної кнопки.
 * @return QString з CSS для кнопки з сірим градієнтом.
 *
 * Вторинна кнопка має менш виражений темно-сірий градієнт.
 * Використовується для допоміжних дій ("Приклад").
 *
 * Стани кнопки:
 * - Звичайний: темно-сірий градієнт з тонкою рамкою.
 * - Наведення: світліший сірий з бірюзовою рамкою.
 * - Натискання: темніший сірий градієнт.
 */
QString UIStyleManager::getSecondaryButtonStyle() {
    return R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3a3a3a, stop:1 #2d2d2d);
            border: 1px solid #555555;
            color: #e0e0e0;
            border-radius: 12px;
            padding: 12px 20px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4a4a4a, stop:1 #3d3d3d);
            border: 1px solid #00CED1;
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2a2a2a, stop:1 #1d1d1d);
        }
    )";
}

/**
 * @brief Отримання стилю для кнопки небезпечної дії.
 * @return QString з CSS для кнопки з червоним градієнтом.
 *
 * Кнопка з червоним градієнтом сигналізує про деструктивну дію.
 * Використовується для очищення даних ("Очистити").
 *
 * Стани кнопки:
 * - Звичайний: червоний градієнт #dc2626 → #991b1b.
 * - Наведення: яскравіший червоний #ef4444 → #dc2626.
 * - Натискання: темніший червоний #b91c1c → #7f1d1d.
 */
QString UIStyleManager::getDangerButtonStyle() {
    return R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #dc2626, stop:1 #991b1b);
            color: #ffffff;
            border-radius: 12px;
            padding: 12px 20px;
            font-size: 14px;
            font-weight: bold;
            border: none;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ef4444, stop:1 #dc2626);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #b91c1c, stop:1 #7f1d1d);
        }
    )";
}

/**
 * @brief Отримання стилю для маленької іконкової кнопки.
 * @return QString з CSS для компактної кнопки.
 *
 * Маленька кнопка з темно-сірим градієнтом.
 * Використовується для кнопок з іконками (емоджі),
 * таких як кнопка показу/приховування пароля.
 *
 * Стани кнопки:
 * - Звичайний: темно-сірий градієнт з тонкою рамкою.
 * - Наведення: світліший сірий з бірюзовою рамкою.
 * - Натискання: темніший сірий градієнт.
 */
QString UIStyleManager::getIconButtonStyle() {
    return R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3a3a3a, stop:0.5 #2d2d2d, stop:1 #252525);
            color: #e0e0e0;
            border: 1px solid #555555;
            border-radius: 12px;
            padding: 12px 8px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4a4a4a, stop:0.5 #3d3d3d, stop:1 #353535);
            border: 1px solid #00CED1;
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2a2a2a, stop:0.5 #1d1d1d, stop:1 #1a1a1a);
        }
    )";
}

/**
 * @brief Отримання стилю для діалогових вікон.
 * @return QString з CSS для QMessageBox.
 *
 * Застосовує темну тему до стандартних діалогових вікон Qt
 * для узгодженості з загальним дизайном програми.
 *
 * Стилізує:
 * - Фон діалогу (темний градієнт з бірюзовою рамкою).
 * - Текст діалогу (світлий).
 * - Кнопки діалогу (бірюзовий градієнт як у головної кнопки).
 */
QString UIStyleManager::getMessageBoxStyle() {
    return R"(
        QMessageBox {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #0a0a0a, stop:0.5 #1a1a1a, stop:1 #1e1e1e);
            border: 2px solid #00CED1;
            border-radius: 10px;
        }
        QMessageBox QLabel {
            color: #e0e0e0;
            font-size: 13px;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00CED1, stop:0.5 #00B8BA, stop:1 #009999);
            color: #000000;
            padding: 10px 25px;
            border-radius: 8px;
            border: none;
            font-weight: bold;
            min-width: 100px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00E5E8, stop:0.5 #00CED1, stop:1 #00B8BA);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00B8BA, stop:0.5 #009999, stop:1 #007F82);
        }
    )";
}

/**
 * @brief Визначення кольору на основі оцінки безпеки.
 * @param score Числова оцінка від 1 до 10.
 * @return QColor відповідний рівню надійності пароля.
 *
 * Функція використовує систему кольорового кодування
 * від червоного (небезпечно) до бірюзового (відмінно):
 *
 * Розподіл кольорів:
 * - 1-3 балів: Червоний (244, 67, 54) - дуже слабкий пароль.
 * - 4-5 балів: Оранжевий (255, 152, 0) - слабкий пароль.
 * - 6-7 балів: Жовтий (255, 235, 59) - середній пароль.
 * - 8 балів: Світло-зелений (139, 195, 74) - надійний пароль.
 * - 9-10 балів: Бірюзовий (0, 206, 209) - дуже надійний пароль.
 */
QColor UIStyleManager::getScoreColor(int score) {
    if (score <= 3) return QColor(244, 67, 54);      // Червоний.
    else if (score <= 5) return QColor(255, 152, 0); // Оранжевий.
    else if (score <= 7) return QColor(255, 235, 59);// Жовтий.
    else if (score <= 8) return QColor(139, 195, 74);// Світло-зелений.
    else return QColor(0, 206, 209);                 // Бірюзовий.
}

/**
 * @brief Генерація стилю прогрес-бару на основі оцінки.
 * @param score Числова оцінка від 1 до 10.
 * @return QString з CSS для QProgressBar::chunk.
 *
 * Створює градієнт для прогрес-бару, де:
 * - Початковий колір залежить від оцінки (червоний → бірюзовий).
 * - Проміжний колір: #00CED1 (основний бірюзовий).
 * - Кінцевий колір: #009999 (темний бірюзовий).
 *
 * Це створює плавний візуальний перехід, де слабкі паролі
 * показуються з червоним відтінком, а надійні - з бірюзовим.
 */
QString UIStyleManager::getProgressBarStyle(int score) {
    QColor scoreColor = getScoreColor(score);
    return QString(
               "QProgressBar::chunk {"
               "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
               "       stop:0 %1, stop:0.5 #00CED1, stop:1 #009999);"
               "   border-radius: 15px;"
               "}"
               ).arg(scoreColor.name());
}
