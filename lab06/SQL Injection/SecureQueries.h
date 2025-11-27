/*
 * SecureQueries.h
 *
 * SQL запити з використанням Prepared Statements.
 * Демонструє правильні методи захисту від SQL-ін'єкцій.
 */

#ifndef SECUREQUERIES_H
#define SECUREQUERIES_H

#include <QString>
#include <QVector>
#include <QSqlQuery>
#include "DatabaseManager.h"

/**
 * @class SecureQueries
 * @brief Клас з захищеними SQL-запитами.
 *
 * Використовує правильні методи захисту:
 * 1. Prepared Statements (Parametrized Queries).
 * 2. Валідація вхідних даних.
 * 3. Мінімальні привілеї для БД.
 * 4. Логування підозрілих запитів.
 *
 * Принцип роботи Prepared Statements:
 * - Структура SQL відокремлена від даних.
 * - Параметри передаються окремо через bind().
 * - БД автоматично екранує спецсимволи.
 * - SQL-ін'єкції неможливі.
 */
class SecureQueries
{
public:
    /**
     * @brief Захищена авторизація користувача.
     *
     * Захист: Використовує Prepared Statements.
     *
     * Як працює:
     * 1. Створюємо шаблон SQL з плейсхолдерами (:username, :password).
     * 2. Підставляємо значення через bindValue().
     * 3. БД автоматично екранує всі спецсимволи.
     *
     * @param username Ім'я користувача (буде екрановано).
     * @param password Пароль (буде екрановано).
     * @return true, якщо користувач знайдений.
     */
    static bool secureLogin(const QString& username, const QString& password);

    /**
     * @brief Захищений пошук студентів.
     *
     * Захист: Prepared Statements + валідація довжини.
     *
     * @param searchTerm Пошуковий запит (буде екрановано).
     * @return Список студентів (без leaked даних).
     */
    static QVector<Student> secureSearchStudents(const QString& searchTerm);

    /**
     * @brief Захищена фільтрація за факультетом.
     *
     * Захист: Prepared Statements + whitelist валідація.
     *
     * Додатковий захист:
     * - Перевірка факультету за whitelist (дозволені значення).
     * - Навіть якщо Prepared Statement обійти, whitelist зупинить атаку.
     *
     * @param faculty Назва факультету (валідується + екранується).
     * @return Список студентів.
     */
    static QVector<Student> secureFilterByFaculty(const QString& faculty);

    /**
     * @brief Захищене додавання коментаря.
     *
     * Захист: Prepared Statements + валідація rating.
     *
     * @param studentId ID студента (валідується).
     * @param commentText Текст коментаря (екранується).
     * @param rating Рейтинг (валідується 1-5).
     * @return true, якщо успішно додано.
     */
    static bool secureAddComment(int studentId,
                                 const QString& commentText,
                                 int rating);

    /**
     * @brief Захищений пошук за діапазоном оцінок.
     *
     * Захист: Prepared Statements + валідація типів.
     *
     * Додатковий захист:
     * - Перевірка, що minGrade та maxGrade - справді числа.
     * - Перевірка діапазону (0.0 - 5.0).
     *
     * @param minGrade Мінімальна оцінка (валідується + екранується).
     * @param maxGrade Максимальна оцінка (валідується + екранується).
     * @return Список студентів.
     */
    static QVector<Student> secureSearchByGrade(double minGrade, double maxGrade);

private:
    /**
     * @brief Допоміжна функція для виконання SELECT з Prepared Statement.
     * @param query Prepared query об'єкт.
     * @return Вектор студентів.
     */
    static QVector<Student> executeSecureStudentQuery(QSqlQuery& query);

    /**
     * @brief Валідація назви факультету (whitelist).
     * @param faculty Назва факультету.
     * @return true, якщо дозволено.
     */
    static bool isValidFaculty(const QString& faculty);
};

#endif // SECUREQUERIES_H
