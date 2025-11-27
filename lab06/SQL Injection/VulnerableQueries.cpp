/*
 * VulnerableQueries.cpp
 *
 * Реалізація вразливий SQL-запитів.
 * Демонструє реальні приклади SQL-ін'єкцій.
 */

#include "VulnerableQueries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>

/**
 * @brief Вразлива авторизація користувача.
 *
 * Проблема: Використовує String Concatenation замість Prepared Statements.
 *
 * Як працює атака:
 * 1. Нормальний запит:
 *    SELECT * FROM users WHERE username='admin' AND password='password123'
 *
 * 2. Атака (SQL Injection):
 *    username: admin' OR '1'='1
 *    password: [будь-що]
 *    Результат: SELECT * FROM users WHERE username='admin' OR '1'='1' AND password='...'
 *    Пояснення: '1'='1' завжди TRUE → обхід автентифікації.
 *
 * 3. Інші варіанти атак:
 *    - username: admin'--
 *      (-- коментує решту запиту, пароль не перевіряється)
 *    - username: ' OR 1=1--
 *      (повертає ВСІХ користувачів)
 *
 * @param username Ім'я користувача.
 * @param password Пароль.
 * @return true, якщо знайдено користувача.
 */
bool VulnerableQueries::vulnerableLogin(const QString& username,
                                        const QString& password)
{
    // Пряме підставлення у SQL.
    QString queryString = QString(
                              "SELECT * FROM users WHERE username='%1' AND password='%2'"
                              ).arg(username)
                              .arg(password);

    // Виводимо запит для аналізу.
    qDebug() << "[VULNERABLE] Login query:" << queryString;

    QSqlQuery query(DatabaseManager::getDatabase());

    // Виконуємо вразливий запит.
    if (!query.exec(queryString)) {
        qWarning() << "[VULNERABLE] Login failed:" << query.lastError().text();
        return false;
    }

    // Перевірка: чи знайдено хоч один запис?
    bool success = query.next();

    if (success) {
        qDebug() << "[VULNERABLE] Login successful for user:"
                 << query.value("username").toString();
    }

    return success;
}

/**
 * @brief Вразливий пошук студентів за ім'ям.
 *
 * Проблема: Використовує LIKE з необробленим вводом.
 *
 * Як працює атака (UNION-based SQL Injection):
 * 1. Нормальний запит:
 *    SELECT * FROM students WHERE first_name LIKE '%Іван%' OR last_name LIKE '%Іван%'
 *
 * 2. Атака (витік даних з іншої таблиці):
 *    searchTerm: %' UNION SELECT id, username, password, role, '1', 1.0, 'hacked', '000' FROM users --
 *
 *    Результат:
 *    SELECT * FROM students WHERE first_name LIKE '%%'
 *    UNION SELECT id, username, password, role, '1', 1.0, 'hacked', '000' FROM users --'
 *
 * 3. Інші атаки:
 *    - searchTerm: %' OR '1'='1
 *      (повертає ВСІХ студентів)
 *    - searchTerm: %'; DROP TABLE students; --
 *      (спроба видалити таблицю!)
 *
 * @param searchTerm Пошуковий запит.
 * @return Список знайдених записів.
 */
QVector<Student> VulnerableQueries::vulnerableSearchStudents(const QString& searchTerm)
{
    // Пряме підставлення у LIKE.
    QString queryString = QString(
                              "SELECT * FROM students "
                              "WHERE first_name LIKE '%%1%' OR last_name LIKE '%%1%'"
                              ).arg(searchTerm);

    qDebug() << "[VULNERABLE] Search query:" << queryString;

    return executeStudentQuery(queryString);
}

/**
 * @brief Вразлива фільтрація студентів за факультетом.
 *
 * Проблема: Використовує WHERE = з необробленим вводом.
 *
 * Як працює атака:
 * 1. Нормальний запит:
 *    SELECT * FROM students WHERE faculty='Інформатика'
 *
 * 2. Атака (Boolean-based SQL Injection):
 *    faculty: Інформатика' OR '1'='1
 *
 *    Результат:
 *    SELECT * FROM students WHERE faculty='Інформатика' OR '1'='1'
 *
 * 3. Інший варіант атаки:
 *    faculty: ' OR 1=1 --
 *    (пустий факультет + завжди TRUE → всі записи)
 *
 * @param faculty Назва факультету.
 * @return Список студентів.
 */
QVector<Student> VulnerableQueries::vulnerableFilterByFaculty(const QString& faculty)
{
    // Пряме підставлення у WHERE.
    QString queryString = QString(
                              "SELECT * FROM students WHERE faculty='%1'"
                              ).arg(faculty);

    qDebug() << "[VULNERABLE] Filter query:" << queryString;

    return executeStudentQuery(queryString);
}

/**
 * @brief Вразливе додавання коментаря.
 *
 * Проблема: Використовує INSERT з необробленим текстом.
 *
 * Як працює атака (Stacked Queries):
 * 1. Нормальний запит:
 *    INSERT INTO comments (student_id, comment_text, rating)
 *    VALUES (1, 'Чудова система!', 5)
 *
 * 2. Атака (спроба виконати додаткові команди):
 *    commentText: Test'); DROP TABLE students; --
 *
 *    Результат:
 *    INSERT INTO comments (student_id, comment_text, rating)
 *    VALUES (1, 'Test'); DROP TABLE students; --', 5)
 *
 * 3. Інші атаки:
 *    - commentText: '); UPDATE students SET grade=5.0 WHERE id=1; --
 *      (зміна оцінок)
 *    - commentText: '); DELETE FROM comments; --
 *      (видалення всіх коментарів)
 *
 * @param studentId ID студента.
 * @param commentText Текст коментаря.
 * @param rating Рейтинг (1-5).
 * @return true, якщо "успішно" вставлено.
 */
bool VulnerableQueries::vulnerableAddComment(int studentId,
                                             const QString& commentText,
                                             int rating)
{
    // Пряме підставлення у INSERT.
    QString queryString = QString(
                              "INSERT INTO comments (student_id, comment_text, rating) "
                              "VALUES (%1, '%2', %3)"
                              ).arg(studentId)
                              .arg(commentText)
                              .arg(rating);

    qDebug() << "[VULNERABLE] Original query:" << queryString;

    // ========== ДЕМОНСТРАЦІЯ STACKED QUERIES ==========

    QSqlQuery query(DatabaseManager::getDatabase());
    bool anySuccess = false;
    int successfulStatements = 0;
    bool criticalCommandExecuted = false;

    // Крок 1: Розділяємо запит по крапці з комою.
    QStringList sqlStatements = queryString.split(';', Qt::SkipEmptyParts);

    qDebug() << "[VULNERABLE] Found" << sqlStatements.size() << "statements";

    for (int i = 0; i < sqlStatements.size(); ++i) {
        QString statement = sqlStatements[i].trimmed();

        qDebug() << "[VULNERABLE] Processing statement" << (i+1) << "of" << sqlStatements.size();
        qDebug() << "Original:" << statement;

        // Пропускаємо пусті рядки.
        if (statement.isEmpty()) {
            qDebug() << "Skipped: empty statement";
            continue;
        }

        // Видаляємо SQL коментарі (все після --).
        int commentPos = statement.indexOf("--");
        if (commentPos != -1) {
            qDebug() << "Found SQL comment at position:" << commentPos;
            statement = statement.left(commentPos).trimmed();
            qDebug() << "After comment removal:" << statement;
        }

        // Якщо після видалення коментарів нічого не залишилось.
        if (statement.isEmpty()) {
            qDebug() << "Skipped: empty after comment removal";
            continue;
        }

        // Логіка очищення.
        QString upperStatement = statement.toUpper();

        // Для INSERT, UPDATE, DELETE - очищуємо тільки останню частину.
        if (upperStatement.startsWith("INSERT") ||
            upperStatement.startsWith("UPDATE") ||
            upperStatement.startsWith("DELETE")) {

            // Видаляємо тільки залишкові символи в кінці від оригінального запиту.
            // Шукаємо закриваючу дужку VALUES.
            if (upperStatement.contains("VALUES")) {
                int lastClosingParen = statement.lastIndexOf(')');
                if (lastClosingParen != -1) {
                    // Залишаємо все до останньої дужки включно.
                    statement = statement.left(lastClosingParen + 1).trimmed();
                }
            }

            // Для UPDATE/DELETE просто видаляємо кінцеві лапки/коми.
            QRegularExpression endGarbage("[',\\s]+$");
            statement = statement.remove(endGarbage).trimmed();

        } else if (upperStatement.startsWith("DROP") ||
                   upperStatement.startsWith("TRUNCATE") ||
                   upperStatement.startsWith("ALTER")) {

            // Для DDL команд - мінімальне очищення.
            statement = statement.trimmed();

            // Видаляємо тільки явні залишки від VALUES.
            QRegularExpression ddlGarbage("[',\\)\\s]+$");
            statement = statement.remove(ddlGarbage).trimmed();
        } else {
            // Для інших - стандартне очищення.
            QRegularExpression trailingGarbage("['\"\\),\\s]+$");
            statement = statement.remove(trailingGarbage).trimmed();
        }

        qDebug() << "After cleanup:" << statement;

        // Фінальна перевірка.
        if (statement.isEmpty()) {
            qDebug() << "Skipped: empty after cleanup";
            continue;
        }

        // Перевірка на валідність SQL.
        if (statement.length() < 5) {
            qDebug() << "Skipped: too short to be valid SQL";
            continue;
        }

        qDebug() << "[VULNERABLE] EXECUTING STATEMENT" << (i+1) << ":";
        qDebug() << statement;

        // Виконуємо SQL команду.
        if (!query.exec(statement)) {
            qWarning() << "[VULNERABLE] Statement FAILED:";
            qWarning() << "Error:" << query.lastError().text();
            qWarning() << "Error type:" << query.lastError().type();
            qWarning() << "Query:" << statement;
        } else {
            anySuccess = true;
            successfulStatements++;
            qDebug() << "[VULNERABLE] Statement executed successfully!";

            // Перевіряємо тип виконаної команди.
            QString type = statement.toUpper();

            if (type.startsWith("DROP")) {
                qCritical() << "CRITICAL: DROP TABLE EXECUTED!";
                criticalCommandExecuted = true;
            }
            else if (type.startsWith("DELETE")) {
                qWarning() << "DANGEROUS: DELETE operation executed!";
                qWarning() << "Rows affected:" << query.numRowsAffected();
                criticalCommandExecuted = true;
            }
            else if (type.startsWith("UPDATE")) {
                qWarning() << "DANGEROUS: UPDATE operation executed!";
                qWarning() << "Rows affected:" << query.numRowsAffected();
                criticalCommandExecuted = true;
            }
            else if (type.startsWith("INSERT")) {
                qDebug() << "INSERT operation executed!";
                qDebug() << "Inserted ID:" << query.lastInsertId().toInt();
            }
            else if (type.startsWith("TRUNCATE")) {
                qCritical() << "CRITICAL: TRUNCATE TABLE EXECUTED!";
                criticalCommandExecuted = true;
            }
        }
    }

    qDebug() << "";
    qDebug() << "[VULNERABLE] Summary:";
    qDebug() << "Total statements parsed:" << sqlStatements.size();
    qDebug() << "Statements executed successfully:" << successfulStatements;
    qDebug() << "Critical command executed:" << (criticalCommandExecuted ? "YES️" : "NO");
    qDebug() << "Overall success:" << (anySuccess ? "YES" : "NO");
    qDebug() << "";

    return anySuccess;
}

/**
 * @brief Вразливий пошук студентів за діапазоном оцінок.
 *
 * Проблема: Використовує BETWEEN з необробленими значеннями.
 *
 * Як працює атака:
 * 1. Нормальний запит:
 *    SELECT * FROM students WHERE grade BETWEEN 4.0 AND 5.0
 *
 * 2. Атака (обхід фільтрації):
 *    minGrade: 0 OR 1=1 --
 *    maxGrade: [будь-що]
 *
 *    Результат:
 *    SELECT * FROM students WHERE grade BETWEEN 0 OR 1=1 -- AND 5.0
 *
 * 3. Інший варіант:
 *    minGrade: 0 UNION SELECT * FROM users --
 *    (спроба витягти дані користувачів)
 *
 * @param minGrade Мінімальна оцінка.
 * @param maxGrade Максимальна оцінка.
 * @return Список студентів
 */
QVector<Student> VulnerableQueries::vulnerableSearchByGrade(const QString& minGrade,
                                                            const QString& maxGrade)
{
    // Пряме підставлення числових значень.
    QString queryString = QString(
                              "SELECT * FROM students WHERE grade BETWEEN %1 AND %2"
                              ).arg(minGrade)
                              .arg(maxGrade);

    qDebug() << "[VULNERABLE] Grade search query:" << queryString;

    return executeStudentQuery(queryString);
}

/**
 * @brief Допоміжна функція для виконання SELECT запитів.
 *
 * Виконує SQL запит та конвертує результати у вектор Student.
 *
 * @param queryString SQL запит.
 * @return Вектор студентів.
 */
QVector<Student> VulnerableQueries::executeStudentQuery(const QString& queryString)
{
    QVector<Student> students;
    QSqlQuery query(DatabaseManager::getDatabase());

    // Виконуємо запит.
    if (!query.exec(queryString)) {
        qWarning() << "[VULNERABLE] Query failed:" << query.lastError().text();
        return students;
    }

    // Обробка результатів.
    while (query.next()) {
        Student student;

        // Безпечне читання полів (перевірка на NULL).
        student.id = query.value("id").toInt();
        student.firstName = query.value("first_name").toString();
        student.lastName = query.value("last_name").toString();
        student.faculty = query.value("faculty").toString();
        student.course = query.value("course").toInt();
        student.grade = query.value("grade").toDouble();
        student.email = query.value("email").toString();
        student.phone = query.value("phone").toString();

        students.append(student);
    }

    qDebug() << "[VULNERABLE] Found" << students.size() << "records";
    return students;
}
