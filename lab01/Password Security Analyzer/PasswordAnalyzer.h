/*
 * PasswordAnalyzer.h.
 *
 * Заголовочний файл аналізатора паролів.
 * Містить оголошення класу PasswordAnalyzer та структури AnalysisResult.
 */

#ifndef PASSWORDANALYZER_H
#define PASSWORDANALYZER_H

#include <QString>
#include <QDate>
#include <QMap>
#include <QVector>
#include <QRegularExpression>
#include <QStringList>
#include <QSet>

/**
 * @struct AnalysisResult.
 * @brief Структура для зберігання результатів аналізу пароля.
 *
 * Містить всю інформацію про аналіз пароля:
 * - оцінку безпеки.
 * - виявлені проблеми.
 * - характеристики пароля.
 * - рекомендації для покращення.
 */
struct AnalysisResult {
    int score;                          ///< Загальна оцінка від 1 до 10.
    bool containsName;                  ///< Чи містить ім'я або прізвище.
    bool containsBirthDate;             ///< Чи містить дату народження.
    bool containsBirthYear;             ///< Чи містить рік народження.
    int length;                         ///< Довжина пароля.
    bool hasUpperCase;                  ///< Чи є великі літери (A-Z, А-Я).
    bool hasLowerCase;                  ///< Чи є малі літери (a-z, а-я).
    bool hasDigits;                     ///< Чи є цифри (0-9).
    bool hasSpecialChars;               ///< Чи є спеціальні символи (!@#$%^&* тощо).
    bool hasDictionaryWords;            ///< Чи є поширені словникові слова.
    bool hasSequences;                  ///< Чи є послідовності (123, abc, 987 тощо).
    QStringList personalDataFound;      ///< Список знайдених персональних даних.
    QStringList recommendations;        ///< Список рекомендацій для покращення.
    QString strengthLevel;              ///< Рівень надійності (текстовий опис).
};

/**
 * @class PasswordAnalyzer.
 * @brief Клас для аналізу безпеки паролів.
 *
 * Виконує комплексний аналіз паролів на:
 * - Складність (довжина, різноманітність символів).
 * - Наявність персональних даних.
 * - Використання словникових слів.
 * - Підрахунок оцінки безпеки.
 *
 * Генерує детальний звіт з рекомендаціями для покращення пароля.
 */
class PasswordAnalyzer {
public:
    /**
     * @brief Конструктор аналізатора.
     *
     * Ініціалізує словник поширених слів.
     */
    PasswordAnalyzer();

    /**
     * @brief Основний метод аналізу пароля.
     * @param password Пароль для аналізу.
     * @param firstName Ім'я користувача.
     * @param lastName Прізвище користувача.
     * @param birthDate Дата народження.
     * @param email Email адреса.
     * @return AnalysisResult структура з результатами аналізу.
     *
     * Виконує повний аналіз пароля з урахуванням персональних даних
     * та генерує детальний звіт з оцінкою та рекомендаціями.
     */
    AnalysisResult analyzePassword(const QString& password,
                                   const QString& firstName,
                                   const QString& lastName,
                                   const QDate& birthDate,
                                   const QString& email = QString());

private:
    /**
     * @brief Перевірка наявності персональних даних у паролі.
     * @param password Пароль для перевірки.
     * @param data Персональні дані (ім'я, прізвище тощо).
     * @return true якщо дані знайдено у паролі.
     *
     * Перевіряє наявність даних з урахуванням:
     * - Різних регістрів.
     * - Заміни символів (1->i, 0->o, @->a тощо).
     * - Зворотного запису (reverse).
     */
    bool containsPersonalData(const QString& password, const QString& data);

    /**
     * @brief Розрахунок оцінки складності пароля.
     * @param password Пароль для оцінки.
     * @return Оцінка від 1 до 10.
     *
     * Оцінює пароль за критеріями:
     * - Довжина (до 4 балів).
     * - Наявність великих літер (1 бал).
     * - Наявність малих літер (1 бал).
     * - Наявність цифр (1 бал).
     * - Наявність спеціальних символів (2 бали).
     * - Різноманітність символів (1 бал).
     */
    int calculateComplexityScore(const QString& password);

    /**
     * @brief Перевірка наявності словникових слів.
     * @param password Пароль для перевірки.
     * @return true якщо знайдено поширені слова.
     *
     * Перевіряє пароль на наявність слів зі словника
     * з урахуванням нормалізації (leetspeak тощо).
     */
    bool containsDictionaryWords(const QString& password);

    /**
     * @brief Перевірка наявності послідовностей символів.
     * @param password Пароль для перевірки.
     * @return true якщо знайдено послідовності.
     *
     * Перевіряє пароль на наявність:
     * - Числових послідовностей (123, 987, 456 тощо).
     * - Букв латиниці (abc, xyz, zyx тощо).
     * - Букв кирилиці (абв, ґде, яюї тощо).
     * У прямому та зворотному порядку.
     */
    bool containsSequences(const QString& password);

    /**
     * @brief Генерація рекомендацій для покращення пароля.
     * @param result Результати попереднього аналізу.
     * @return Список рекомендацій.
     *
     * Аналізує результати та генерує персоналізовані
     * рекомендації для підвищення безпеки пароля.
     */
    QStringList generateRecommendations(const AnalysisResult& result);

    /**
     * @brief Визначення текстового рівня надійності.
     * @param score Числова оцінка від 1 до 10.
     * @return Текстовий опис рівня надійності.
     *
     * Конвертує числову оцінку у зрозумілий опис:
     * - 1-3: Дуже слабкий.
     * - 4-5: Слабкий.
     * - 6-7: Середній.
     * - 8: Надійний.
     * - 9-10: Дуже надійний.
     */
    QString determineStrengthLevel(int score);

    /**
     * @brief Нормалізація тексту для порівняння.
     * @param text Текст для нормалізації.
     * @return Нормалізований текст.
     *
     * Приводить текст до єдиного формату:
     * - Переводить у нижній регістр.
     * - Замінює схожі символи (1->i, 0->o, 3->e тощо).
     * - Дозволяє виявити приховані персональні дані.
     */
    QString normalize(const QString& text);

    /**
     * @brief Перевірка наявності дати в різних форматах.
     * @param password Пароль для перевірки.
     * @param date Дата для пошуку.
     * @return true якщо дата знайдена у будь-якому форматі.
     *
     * Перевіряє наявність дати у форматах:
     * - ddMMyyyy, ddMMyy.
     * - dd.MM.yyyy, dd/MM/yyyy.
     * - yyyyMMdd, yyyy.
     * - ddMM.
     */
    bool containsDateVariants(const QString& password, const QDate& date);

    /**
     * @brief Ініціалізація словника поширених слів.
     *
     * Заповнює словник популярними паролями та словами
     * українською та англійською мовами.
     */
    void initializeDictionary();

    /**
     * @brief Транслітерація українського тексту в англійський.
     * @param text Український текст.
     * @return Транслітерований текст латиницею.
     */
    QString transliterateUkrToEng(const QString& text);

    // ============ Дані класу ============

    /**
     * @brief Словник поширених слів та паролів.
     *
     * Містить популярні слова, які часто використовуються
     * у паролях і легко підбираються.
     */
    QVector<QString> commonWords;
};

#endif // PASSWORDANALYZER_H
