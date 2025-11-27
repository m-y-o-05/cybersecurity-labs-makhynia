/*
 * DatabaseManager.cpp
 *
 * Реалізація менеджера бази даних.
 * Створює та наповнює тестову базу даних студентів.
 */

#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>

/**
 * @brief Конструктор - підключення до SQLite.
 */
DatabaseManager::DatabaseManager()
{
    // Використовуємо SQLite (локальна файлова БД).
    db = QSqlDatabase::addDatabase("QSQLITE");

    // Шлях до файлу бази даних.
    QString dbPath = QCoreApplication::applicationDirPath() + "/students.db";
    db.setDatabaseName(dbPath);

    qDebug() << "Database path:" << dbPath;
}

/**
 * @brief Деструктор - закриття з'єднання.
 */
DatabaseManager::~DatabaseManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

/**
 * @brief Ініціалізація бази даних.
 *
 * Процес:
 * 1. Відкриває з'єднання з БД.
 * 2. Створює таблиці (students, users, comments, attack_logs).
 * 3. Наповнює тестовими даними.
 *
 * @return true, якщо все успішно.
 */
bool DatabaseManager::initializeDatabase()
{
    // ========== КРОК 1: Відкриття БД ==========
    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database opened successfully";

    // ========== КРОК 2: Створення таблиць ==========
    if (!createTables()) {
        qCritical() << "Failed to create tables";
        return false;
    }

    // ========== КРОК 3: Наповнення даними ==========
    if (!populateTables()) {
        qCritical() << "Failed to populate tables";
        return false;
    }

    qDebug() << "Database initialized successfully";
    return true;
}

/**
 * @brief Створення таблиць бази даних.
 *
 * Створює 4 таблиці:
 * 1. students - інформація про студентів.
 * 2. users - облікові записи (логін/пароль).
 * 3. comments - коментарі/відгуки.
 * 4. attack_logs - логи спроб SQL-ін'єкцій.
 *
 * @return true, якщо успішно
 */
bool DatabaseManager::createTables()
{
    QSqlQuery query(db);

    // ========== ТАБЛИЦЯ 1: СТУДЕНТИ ==========
    // Містить персональну інформацію про студентів.
    QString createStudentsTable = R"(
        CREATE TABLE IF NOT EXISTS students (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            faculty TEXT NOT NULL,
            course INTEGER NOT NULL,
            grade REAL NOT NULL,
            email TEXT NOT NULL UNIQUE,
            phone TEXT NOT NULL
        )
    )";

    if (!query.exec(createStudentsTable)) {
        qCritical() << "Failed to create students table:"
                    << query.lastError().text();
        return false;
    }

    // ========== ТАБЛИЦЯ 2: КОРИСТУВАЧІ ==========
    // Містить облікові записи для авторизації.
    QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            role TEXT NOT NULL DEFAULT 'student',
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createUsersTable)) {
        qCritical() << "Failed to create users table:"
                    << query.lastError().text();
        return false;
    }

    // ========== ТАБЛИЦЯ 3: КОМЕНТАРІ ==========
    // Система відгуків/коментарів студентів.
    QString createCommentsTable = R"(
        CREATE TABLE IF NOT EXISTS comments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            student_id INTEGER NOT NULL,
            comment_text TEXT NOT NULL,
            rating INTEGER CHECK(rating >= 1 AND rating <= 5),
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (student_id) REFERENCES students(id)
        )
    )";

    if (!query.exec(createCommentsTable)) {
        qCritical() << "Failed to create comments table:"
                    << query.lastError().text();
        return false;
    }

    // ========== ТАБЛИЦЯ 4: ЛОГИ АТАК ==========
    // Зберігає всі спроби SQL-ін'єкцій для аналізу.
    QString createAttackLogsTable = R"(
        CREATE TABLE IF NOT EXISTS attack_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            attack_type TEXT NOT NULL,
            query_text TEXT NOT NULL,
            user_input TEXT NOT NULL,
            is_blocked BOOLEAN NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            ip_address TEXT,
            additional_info TEXT
        )
    )";

    if (!query.exec(createAttackLogsTable)) {
        qCritical() << "Failed to create attack_logs table:"
                    << query.lastError().text();
        return false;
    }

    qDebug() << "All tables created successfully";
    return true;
}

/**
 * @brief Наповнення таблиць тестовими даними.
 *
 * Створює:
 * - 20 студентів (різні факультети, курси, оцінки).
 * - 5 користувачів (admin, викладач, студенти).
 * - 10 коментарів.
 *
 * @return true, якщо успішно.
 */
bool DatabaseManager::populateTables()
{
    QSqlQuery query(db);

    // Перевірка: чи таблиця вже заповнена?
    query.exec("SELECT COUNT(*) FROM students");
    if (query.next() && query.value(0).toInt() > 0) {
        qDebug() << "Tables already populated, skipping...";
        return true;
    }

    // ========== НАПОВНЕННЯ 1: СТУДЕНТИ ==========
    // Масив тестових студентів (20 записів).
    QVector<Student> students = {
        {0, "Іван", "Петренко", "Інформатика", 3, 4.5, "ivan.petrenko@university.edu", "+380501234567"},
        {0, "Марія", "Коваленко", "Математика", 2, 4.8, "maria.kovalenko@university.edu", "+380502345678"},
        {0, "Олександр", "Шевченко", "Фізика", 4, 3.9, "oleksandr.shevchenko@university.edu", "+380503456789"},
        {0, "Анна", "Мельник", "Інформатика", 1, 4.2, "anna.melnyk@university.edu", "+380504567890"},
        {0, "Дмитро", "Бойко", "Економіка", 3, 3.7, "dmytro.boyko@university.edu", "+380505678901"},
        {0, "Олена", "Ткаченко", "Математика", 2, 4.6, "olena.tkachenko@university.edu", "+380506789012"},
        {0, "Андрій", "Кравченко", "Інформатика", 4, 4.9, "andriy.kravchenko@university.edu", "+380507890123"},
        {0, "Софія", "Мороз", "Фізика", 1, 4.1, "sofia.moroz@university.edu", "+380508901234"},
        {0, "Максим", "Полтавець", "Економіка", 3, 3.5, "maksym.poltavets@university.edu", "+380509012345"},
        {0, "Вікторія", "Лисенко", "Математика", 2, 4.7, "viktoria.lysenko@university.edu", "+380500123456"},
        {0, "Богдан", "Савченко", "Інформатика", 4, 4.3, "bohdan.savchenko@university.edu", "+380501234560"},
        {0, "Катерина", "Гончар", "Фізика", 1, 4.0, "kateryna.honchar@university.edu", "+380502345671"},
        {0, "Ярослав", "Павленко", "Економіка", 3, 3.8, "yaroslav.pavlenko@university.edu", "+380503456782"},
        {0, "Дарина", "Романенко", "Математика", 2, 4.4, "daryna.romanenko@university.edu", "+380504567893"},
        {0, "Ілля", "Захарченко", "Інформатика", 4, 4.6, "illya.zakharchenko@university.edu", "+380505678904"},
        {0, "Аліна", "Костенко", "Фізика", 1, 3.9, "alina.kostenko@university.edu", "+380506789015"},
        {0, "Владислав", "Приходько", "Економіка", 3, 4.1, "vladyslav.prykhodko@university.edu", "+380507890126"},
        {0, "Юлія", "Левченко", "Математика", 2, 4.8, "yulia.levchenko@university.edu", "+380508901237"},
        {0, "Тарас", "Сидоренко", "Інформатика", 4, 4.5, "taras.sydorenko@university.edu", "+380509012348"},
        {0, "Олеся", "Кириленко", "Фізика", 1, 4.2, "olesia.kyrylenko@university.edu", "+380500123459"}
    };

    // Вставка студентів у БД.
    for (const Student& student : students) {
        QString insertQuery = QString(
                                  "INSERT INTO students (first_name, last_name, faculty, course, grade, email, phone) "
                                  "VALUES ('%1', '%2', '%3', %4, %5, '%6', '%7')"
                                  ).arg(student.firstName)
                                  .arg(student.lastName)
                                  .arg(student.faculty)
                                  .arg(student.course)
                                  .arg(student.grade)
                                  .arg(student.email)
                                  .arg(student.phone);

        if (!query.exec(insertQuery)) {
            qWarning() << "Failed to insert student:" << query.lastError().text();
        }
    }

    qDebug() << "Inserted" << students.size() << "students";

    // ========== НАПОВНЕННЯ 2: КОРИСТУВАЧІ ==========
    // Тестові користувачі.
    QVector<User> users = {
        {0, "admin", "admin123", "admin"},
        {0, "teacher", "teacher123", "teacher"},
        {0, "ivan", "password123", "student"},
        {0, "maria", "qwerty", "student"},
        {0, "test", "test", "student"}
    };

    // Вставка користувачів.
    for (const User& user : users) {
        QString insertQuery = QString(
                                  "INSERT INTO users (username, password, role) "
                                  "VALUES ('%1', '%2', '%3')"
                                  ).arg(user.username)
                                  .arg(user.password)
                                  .arg(user.role);

        if (!query.exec(insertQuery)) {
            qWarning() << "Failed to insert user:" << query.lastError().text();
        }
    }

    qDebug() << "Inserted" << users.size() << "users";

    // ========== НАПОВНЕННЯ 3: КОМЕНТАРІ ==========
    // Тестові відгуки студентів.
    QVector<QString> comments = {
        "Відмінна система! Дуже зручно користуватися.",
        "Швидкий доступ до інформації про оцінки.",
        "Хотілося б більше функцій для аналізу успішності.",
        "Чудовий інтерфейс, все інтуїтивно зрозуміло.",
        "Іноді є проблеми з швидкістю завантаження.",
        "Дякую за можливість переглядати свій прогрес!",
        "Було б добре додати графіки успішності.",
        "Чудова робота розробників!",
        "Система працює стабільно та без помилок.",
        "Рекомендую всім студентам користуватися!"
    };

    // Вставка коментарів (прив'язуємо до випадкових студентів).
    for (int i = 0; i < comments.size(); ++i) {
        int studentId = (i % 20) + 1;  // ID від 1 до 20
        int rating = 3 + (i % 3);      // Рейтинг 3-5

        QString insertQuery = QString(
                                  "INSERT INTO comments (student_id, comment_text, rating) "
                                  "VALUES (%1, '%2', %3)"
                                  ).arg(studentId)
                                  .arg(comments[i])
                                  .arg(rating);

        if (!query.exec(insertQuery)) {
            qWarning() << "Failed to insert comment:" << query.lastError().text();
        }
    }

    qDebug() << "Inserted" << comments.size() << "comments";

    return true;
}

/**
 * @brief Отримання з'єднання з базою даних.
 *
 * Статичний метод для доступу до БД з інших класів.
 *
 * @return QSqlDatabase
 */
QSqlDatabase DatabaseManager::getDatabase()
{
    return QSqlDatabase::database();
}
