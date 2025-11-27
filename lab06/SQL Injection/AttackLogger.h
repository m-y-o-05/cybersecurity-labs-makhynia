/*
 * AttackLogger.h
 *
 * Система логування спроб SQL-ін'єкцій.
 * Записує всі підозрілі запити для аналізу.
 */

#ifndef ATTACKLOGGER_H
#define ATTACKLOGGER_H

#include <QString>
#include <QDateTime>
#include <QVector>

/**
 * @struct AttackLog
 * @brief Структура для зберігання інформації про атаку.
 */
struct AttackLog {
    int id;
    QString attackType;        // Тип атаки (UNION, OR 1=1, DROP тощо).
    QString queryText;         // Повний SQL запит.
    QString userInput;         // Ввід користувача.
    bool isBlocked;            // Чи заблоковано атаку.
    QDateTime timestamp;       // Час атаки.
    QString ipAddress;         // IP адреса (опціонально).
    QString additionalInfo;    // Додаткова інформація.
};

/**
 * @class AttackLogger
 * @brief Клас для логування та аналізу SQL-ін'єкцій.
 */
class AttackLogger
{
public:
    /**
     * @brief Логування спроби атаки.
     * @param attackType Тип атаки.
     * @param queryText SQL запит.
     * @param userInput Ввід користувача.
     * @param isBlocked Чи заблоковано.
     * @param additionalInfo Додаткова інформація.
     */
    static void logAttack(const QString& attackType,
                          const QString& queryText,
                          const QString& userInput,
                          bool isBlocked,
                          const QString& additionalInfo = "");

    /**
     * @brief Отримання всіх логів атак.
     * @return Вектор логів.
     */
    static QVector<AttackLog> getAllLogs();

    /**
     * @brief Отримання логів за типом.
     * @param attackType Тип атаки.
     * @return Вектор логів.
     */
    static QVector<AttackLog> getLogsByType(const QString& attackType);

    /**
     * @brief Детекція підозрілих патернів у вводі.
     * @param userInput Ввід користувача.
     * @return Тип виявленої атаки (або пустий рядок).
     */
    static QString detectAttackPattern(const QString& userInput);

    /**
     * @brief Очищення старих логів.
     * @param daysOld Видалити логи старші за N днів.
     * @return Кількість видалених записів.
     */
    static int clearOldLogs(int daysOld = 30);

    /**
     * @brief Статистика атак.
     * @return Рядок зі статистикою.
     */
    static QString getAttackStatistics();

private:
    /**
     * @brief Перевірка наявності SQL ключових слів.
     * @param input Ввід користувача.
     * @return true, якщо знайдено підозрілі слова.
     */
    static bool containsSQLKeywords(const QString& input);
};

#endif // ATTACKLOGGER_H
