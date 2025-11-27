/*
 * DatabaseManager.h
 *
 * Менеджер бази даних - ініціалізація та наповнення.
 * Створює тестову базу даних з студентами.
 */

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QVector>

/**
 * @struct Student
 * @brief Структура для зберігання інформації про студента.
 */
struct Student {
    int id;
    QString firstName;
    QString lastName;
    QString faculty;
    int course;
    double grade;
    QString email;
    QString phone;
};

/**
 * @struct User
 * @brief Структура для зберігання облікових записів.
 */
struct User {
    int id;
    QString username;
    QString password;
    QString role;
};

/**
 * @class DatabaseManager
 * @brief Клас для управління базою даних.
 */
class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    /**
     * @brief Ініціалізація бази даних.
     * @return true, якщо успішно.
     */
    bool initializeDatabase();

    /**
     * @brief Створення таблиць.
     * @return true, якщо успішно.
     */
    bool createTables();

    /**
     * @brief Наповнення таблиць тестовими даними.
     * @return true, якщо успішно.
     */
    bool populateTables();

    /**
     * @brief Отримання з'єднання з БД.
     * @return QSqlDatabase.
     */
    static QSqlDatabase getDatabase();

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
