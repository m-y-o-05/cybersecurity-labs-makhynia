/*
 * VulnerableQueries.h
 *
 * SQL запити без захисту.
 * Демонструє різні типи SQL-ін'єкцій.
 */

#ifndef VULNERABLEQUERIES_H
#define VULNERABLEQUERIES_H

#include <QString>
#include <QVector>
#include <QSqlQuery>
#include "DatabaseManager.h"

/**
 * @class VulnerableQueries
 * @brief Клас з вразливими SQL-запитами.
 *
 * Цей клас демонструє типові помилки, що призводять до SQL-ін'єкцій:
 * - Пряме підставлення користувацького вводу в SQL.
 * - Відсутність валідації вхідних даних.
 * - Використання конкатенації рядків замість prepared statements.
 */
class VulnerableQueries
{
public:
    /**
     * @brief Вразлива авторизація користувача.
     *
     * Небезпека: Пряме підставлення username і password у SQL.
     *
     * @param username Ім'я користувача.
     * @param password Пароль.
     * @return true, якщо "успішно".
     */
    static bool vulnerableLogin(const QString& username, const QString& password);

    /**
     * @brief Вразливий пошук студентів.
     *
     * Небезпека: Пряме підставлення searchTerm у LIKE.
     *
     * @param searchTerm Пошуковий запит.
     * @return Список студентів.
     */
    static QVector<Student> vulnerableSearchStudents(const QString& searchTerm);

    /**
     * @brief Вразлива фільтрація за факультетом.
     *
     * Небезпека: Пряме підставлення faculty у WHERE.
     *
     * @param faculty Назва факультету.)
     * @return Список студентів.
     */
    static QVector<Student> vulnerableFilterByFaculty(const QString& faculty);

    /**
     * @brief Вразливе додавання коментаря.
     *
     * Небезпека: Пряме підставлення commentText у INSERT.
     *
     * @param studentId ID студента.
     * @param commentText Текст коментаря.
     * @param rating Рейтинг.
     * @return true, якщо "успішно".
     */
    static bool vulnerableAddComment(int studentId,
                                     const QString& commentText,
                                     int rating);

    /**
     * @brief Вразливий пошук за діапазоном оцінок.
     *
     * Небезпека: Пряме підставлення minGrade і maxGrade.
     *
     * @param minGrade Мінімальна оцінка.
     * @param maxGrade Максимальна оцінка.
     * @return Список студентів.
     */
    static QVector<Student> vulnerableSearchByGrade(const QString& minGrade,
                                                    const QString& maxGrade);

private:
    /**
     * @brief Допоміжна функція для виконання SELECT запитів.
     * @param queryString SQL запит.
     * @return Список студентів.
     */
    static QVector<Student> executeStudentQuery(const QString& queryString);
};

#endif // VULNERABLEQUERIES_H
