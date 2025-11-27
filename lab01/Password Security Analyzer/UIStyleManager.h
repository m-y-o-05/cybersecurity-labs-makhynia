/*
 * UIStyleManager.h.
 *
 * Заголовочний файл менеджера стилів інтерфейсу.
 * Містить оголошення класу для централізованого управління.
 */

#ifndef UISTYLEMANAGER_H
#define UISTYLEMANAGER_H

#include <QString>
#include <QColor>

/**
 * @class UIStyleManager.
 * @brief Клас для централізованого управління стилями інтерфейсу.
 *
 * Цей клас надає статичні методи для отримання CSS-стилів
 * для різних елементів інтерфейсу програми.
 */
class UIStyleManager {
public:
    /**
     * @brief Отримання головного stylesheet для програми.
     * @return QString з повним CSS-стилем для всього вікна.
     *
     * Повертає стилі для:
     * - Головного вікна (градієнтний фон).
     * - Групових блоків (QGroupBox).
     * - Міток (QLabel).
     * - Полів вводу (QLineEdit, QDateEdit).
     * - Текстових полів (QTextEdit).
     * - Прогрес-барів (QProgressBar).
     * - Скролбарів (QScrollBar).
     */
    static QString getMainStyleSheet();

    /**
     * @brief Отримання стилю для міток акцентного кольору.
     * @return QString з CSS-стилем для QLabel.
     *
     * Використовується для міток поруч з полями вводу.
     * Колір: бірюзовий (#00E5E8), жирний шрифт 14px.
     */
    static QString getLabelStyle();

    /**
     * @brief Отримання стилю для головної кнопки.
     * @return QString з CSS-стилем для основної кнопки дії.
     *
     * Кнопка з бірюзовим градієнтом, використовується для
     * основної дії (наприклад, "Аналізувати пароль").
     * Включає ефекти наведення та натискання.
     */
    static QString getPrimaryButtonStyle();

    /**
     * @brief Отримання стилю для вторинної кнопки.
     * @return QString з CSS-стилем для допоміжної кнопки.
     *
     * Кнопка з темно-сірим градієнтом, використовується для
     * допоміжних дій (наприклад, "Приклад").
     * Має помірний контраст з фоном.
     */
    static QString getSecondaryButtonStyle();

    /**
     * @brief Отримання стилю для кнопки видалення/небезпечної дії.
     * @return QString з CSS-стилем для кнопки очищення.
     *
     * Кнопка з червоним градієнтом, використовується для
     * дій видалення або скидання (наприклад, "Очистити").
     * Червоний колір сигналізує про деструктивну дію.
     */
    static QString getDangerButtonStyle();

    /**
     * @brief Отримання стилю для маленької іконкової кнопки.
     * @return QString з CSS для компактної кнопки з іконкою.
     *
     * Використовується для маленьких кнопок, таких як
     * кнопка показу/приховування пароля.
     */
    static QString getIconButtonStyle();

    /**
     * @brief Отримання стилю для діалогових вікон.
     * @return QString з CSS-стилем для QMessageBox.
     *
     * Застосовує темну тему до діалогових вікон для
     * узгодженості з загальним дизайном програми.
     * Включає стилізацію кнопок всередині діалогу.
     */
    static QString getMessageBoxStyle();

    /**
     * @brief Визначення кольору для оцінки пароля.
     * @param score Числова оцінка від 1 до 10.
     * @return QColor відповідно до рівня надійності.
     *
     * Повертає колір залежно від оцінки:
     * - 1-3: Червоний #F44336 (небезпечно).
     * - 4-5: Оранжевий #FF9800 (слабко).
     * - 6-7: Жовтий #FFEB3B (середньо).
     * - 8: Світло-зелений #8BC34A (добре).
     * - 9-10: Бірюзовий #00CED1 (відмінно).
     */
    static QColor getScoreColor(int score);

    /**
     * @brief Отримання стилю прогрес-бару на основі оцінки.
     * @param score Числова оцінка від 1 до 10.
     * @return QString з CSS-стилем для QProgressBar::chunk.
     *
     * Генерує градієнт для прогрес-бару, де початковий колір
     * залежить від оцінки, а кінцевий - бірюзовий.
     */
    static QString getProgressBarStyle(int score);

private:
    // ============ Константи кольорів теми ============

    /**
     * @brief Основний бірюзовий колір теми.
     */
    static constexpr const char* COLOR_PRIMARY = "#00CED1";

    /**
     * @brief Світліший відтінок бірюзового.
     */
    static constexpr const char* COLOR_PRIMARY_LIGHT = "#00E5E8";

    /**
     * @brief Темніший відтінок бірюзового.
     */
    static constexpr const char* COLOR_PRIMARY_DARK = "#009999";

    /**
     * @brief Темний фон програми.
     */
    static constexpr const char* COLOR_BACKGROUND_DARK = "#1a1a1a";

    /**
     * @brief Світлий текст на темному фоні.
     */
    static constexpr const char* COLOR_TEXT_LIGHT = "#e0e0e0";
};

#endif // UISTYLEMANAGER_H
