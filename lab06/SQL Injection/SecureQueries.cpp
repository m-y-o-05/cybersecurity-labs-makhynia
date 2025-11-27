/*
 * SecureQueries.cpp
 *
 * Реалізація захищених SQL-запитів.
 * Демонструє правильні методи використання Prepared Statements.
 */

#include "SecureQueries.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSet>

/**
 * @brief Захищена авторизація користувача.
 *
 * ЗАХИСТ: Використовує Prepared Statements (Parametrized Queries).
 *
 * Як працює Prepared Statement:
 * 1. Створюємо SQL шаблон з плейсхолдерами:
 *    SELECT * FROM users WHERE username=:username AND password=:password
 *
 * 2. Підставляємо параметри через bindValue():
 *    query.bindValue(":username", username);
 *    query.bindValue(":password", password);
 *
 * 3. БД автоматично екранує спецсимволи:
 *    - Одинарні лапки (') → екрановані.
 *    - Подвійні дефіси (--) → екрановані.
 *    - UNION, DROP тощо → трактуються як TEXT, не команди.
 *
 * @param username Ім'я користувача.
 * @param password Пароль.
 * @return true, якщо користувач справжній.
 */
bool SecureQueries::secureLogin(const QString& username, const QString& password)
{
    // ========== КРОК 1: Створюємо Prepared Statement ==========
    QSqlQuery query(DatabaseManager::getDatabase());

    // SQL з плейсхолдерами (:username, :password).
    QString queryString = "SELECT * FROM users WHERE username=:username AND password=:password";

    // Підготовка запиту.
    if (!query.prepare(queryString)) {
        qWarning() << "[SECURE] Failed to prepare login query:"
                   << query.lastError().text();
        return false;
    }

    // ========== КРОК 2: Підставляємо параметри безпечно ==========
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    qDebug() << "[SECURE] Login attempt for user:" << username;

    // ========== КРОК 3: Виконуємо запит ==========
    if (!query.exec()) {
        qWarning() << "[SECURE] Login query failed:" << query.lastError().text();
        return false;
    }

    // ========== КРОК 4: Перевірка результату ==========
    bool success = query.next();

    if (success) {
        QString foundUsername = query.value("username").toString();
        QString role = query.value("role").toString();
        qDebug() << "[SECURE] Login successful! User:" << foundUsername
                 << "Role:" << role;
    } else {
        qDebug() << "[SECURE] Login failed: Invalid credentials";
    }

    return success;
}

/**
 * @brief Захищений пошук студентів за ім'ям.
 *
 * Захист: Prepared Statements + валідація довжини.
 *
 * Додаткові захисні механізми:
 * 1. Обмеження довжини пошукового запиту (max 100 символів).
 * 2. Prepared Statement для екранування.
 * 3. LIKE з плейсхолдерами.
 *
 * @param searchTerm Пошуковий запит.
 * @return Список знайдених студентів.
 */
QVector<Student> SecureQueries::secureSearchStudents(const QString& searchTerm)
{
    // ========== ВАЛІДАЦІЯ 1: Обмеження довжини ==========
    if (searchTerm.length() > 100) {
        qWarning() << "[SECURE] Search term too long! Length:"
                   << searchTerm.length();
        return QVector<Student>();
    }

    // ========== ВАЛІДАЦІЯ 2: Перевірка на пустий запит ==========
    if (searchTerm.trimmed().isEmpty()) {
        qDebug() << "[SECURE] Empty search term";
        return QVector<Student>();
    }

    // ========== Створення Prepared Statement ==========
    QSqlQuery query(DatabaseManager::getDatabase());

    // SQL з плейсхолдерами для LIKE.
    QString queryString =
        "SELECT * FROM students "
        "WHERE first_name LIKE :searchTerm OR last_name LIKE :searchTerm";

    if (!query.prepare(queryString)) {
        qWarning() << "[SECURE] Failed to prepare search query:"
                   << query.lastError().text();
        return QVector<Student>();
    }

    // Додаємо wildcards (%) до пошукового терміну.
    QString searchPattern = "%" + searchTerm + "%";

    // Підставляємо параметр безпечно.
    query.bindValue(":searchTerm", searchPattern);

    qDebug() << "[SECURE] Searching for:" << searchTerm;

    // Виконуємо та повертаємо результат.
    return executeSecureStudentQuery(query);
}

/**
 * @brief Захищена фільтрація за факультетом.
 *
 * Захист: Prepared Statements + Whitelist валідація.
 *
 * Подвійний захист:
 * 1. Whitelist - дозволені тільки існуючі факультети.
 * 2. Prepared Statement - екранування на рівні БД.
 *
 * @param faculty Назва факультету.
 * @return Список студентів з цього факультету.
 */
QVector<Student> SecureQueries::secureFilterByFaculty(const QString& faculty)
{
    // ========== ЗАХИСТ 1: WHITELIST ВАЛІДАЦІЯ ==========
    if (!isValidFaculty(faculty)) {
        qWarning() << "[SECURE] Invalid faculty name (not in whitelist):"
                   << faculty;
        return QVector<Student>();
    }

    // ========== ЗАХИСТ 2: PREPARED STATEMENT ==========
    QSqlQuery query(DatabaseManager::getDatabase());

    QString queryString = "SELECT * FROM students WHERE faculty=:faculty";

    if (!query.prepare(queryString)) {
        qWarning() << "[SECURE] Failed to prepare filter query:"
                   << query.lastError().text();
        return QVector<Student>();
    }

    // Підставляємо параметр.
    query.bindValue(":faculty", faculty);

    qDebug() << "[SECURE] Filtering by faculty:" << faculty;

    return executeSecureStudentQuery(query);
}

/**
 * @brief Захищине додавання коментаря.
 *
 * Захист: Prepared Statements + валідація рейтингу.
 *
 * Додаткові перевірки:
 * 1. studentId > 0 (захист від від'ємних ID).
 * 2. rating у діапазоні 1-5.
 * 3. Довжина коментаря ≤ 1000 символів.
 * 4. Prepared Statement для INSERT.
 *
 * @param studentId ID студента.
 * @param commentText Текст коментаря.
 * @param rating Рейтинг (1-5).
 * @return true, якщо успішно додано.
 */
bool SecureQueries::secureAddComment(int studentId,
                                     const QString& commentText,
                                     int rating)
{
    // ========== ВАЛІДАЦІЯ 1: Student ID ==========
    if (studentId <= 0) {
        qWarning() << "[SECURE] Invalid student ID:" << studentId;
        return false;
    }

    // ========== ВАЛІДАЦІЯ 2: Rating ==========
    // Рейтинг має бути 1-5.
    if (rating < 1 || rating > 5) {
        qWarning() << "[SECURE] Invalid rating (must be 1-5):" << rating;
        return false;
    }

    // ========== ВАЛІДАЦІЯ 3: Довжина коментаря ==========
    // Захист від DoS через надто довгі коментарі.
    if (commentText.length() > 1000) {
        qWarning() << "[SECURE] Comment too long! Length:"
                   << commentText.length();
        return false;
    }

    // Перевірка на порожній коментар.
    if (commentText.trimmed().isEmpty()) {
        qWarning() << "[SECURE] Empty comment text";
        return false;
    }

    // ========== PREPARED STATEMENT для INSERT ==========
    QSqlQuery query(DatabaseManager::getDatabase());

    QString queryString =
        "INSERT INTO comments (student_id, comment_text, rating) "
        "VALUES (:studentId, :commentText, :rating)";

    if (!query.prepare(queryString)) {
        qWarning() << "[SECURE] Failed to prepare insert query:"
                   << query.lastError().text();
        return false;
    }

    // Підставляємо параметри безпечно.
    query.bindValue(":studentId", studentId);
    query.bindValue(":commentText", commentText);
    query.bindValue(":rating", rating);

    qDebug() << "[SECURE] Adding comment for student:" << studentId;

    // Виконуємо INSERT.
    if (!query.exec()) {
        qWarning() << "[SECURE] Failed to insert comment:"
                   << query.lastError().text();
        return false;
    }

    qDebug() << "[SECURE] Comment added successfully";
    return true;
}

/**
 * @brief Захищений пошук за діапазоном оцінок.
 *
 * Захист: Prepared Statements + валідація типів даних.
 *
 * Додаткові перевірки:
 * 1. Перевірка діапазону (0.0 - 5.0).
 * 2. minGrade ≤ maxGrade (логічна валідація).
 * 3. Використання double замість QString (type safety).
 *
 * @param minGrade Мінімальна оцінка (валідована).
 * @param maxGrade Максимальна оцінка (валідована).
 * @return Список студентів у діапазоні.
 */
QVector<Student> SecureQueries::secureSearchByGrade(double minGrade, double maxGrade)
{
    // ========== ВАЛІДАЦІЯ 1: Діапазон оцінок ==========
    // Оцінки у системі від 0.0 до 5.0
    if (minGrade < 0.0 || minGrade > 5.0 ||
        maxGrade < 0.0 || maxGrade > 5.0) {
        qWarning() << "[SECURE] Invalid grade range (must be 0.0-5.0):"
                   << minGrade << "-" << maxGrade;
        return QVector<Student>();
    }

    // ========== ВАЛІДАЦІЯ 2: Логічна перевірка ==========
    if (minGrade > maxGrade) {
        qWarning() << "[SECURE] Invalid range: min > max:"
                   << minGrade << ">" << maxGrade;
        return QVector<Student>();
    }

    // ========== PREPARED STATEMENT з BETWEEN ==========
    QSqlQuery query(DatabaseManager::getDatabase());

    QString queryString =
        "SELECT * FROM students WHERE grade BETWEEN :minGrade AND :maxGrade";

    if (!query.prepare(queryString)) {
        qWarning() << "[SECURE] Failed to prepare grade query:"
                   << query.lastError().text();
        return QVector<Student>();
    }

    // Підставляємо числові параметри.
    query.bindValue(":minGrade", minGrade);
    query.bindValue(":maxGrade", maxGrade);

    qDebug() << "[SECURE] Searching grades between:"
             << minGrade << "-" << maxGrade;

    return executeSecureStudentQuery(query);
}

/**
 * @brief Допоміжна функція для виконання SELECT з Prepared Statement.
 *
 * Виконує вже підготовлений query та конвертує результати у Student.
 *
 * @param query Підготовлений QSqlQuery (з bindValue).
 * @return Вектор студентів.
 */
QVector<Student> SecureQueries::executeSecureStudentQuery(QSqlQuery& query)
{
    QVector<Student> students;

    // Виконуємо безпечний запит.
    if (!query.exec()) {
        qWarning() << "[SECURE] Query execution failed:"
                   << query.lastError().text();
        return students;
    }

    // Обробка результатів.
    while (query.next()) {
        Student student;

        // Безпечне читання полів з перевіркою на NULL.
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

    qDebug() << "[SECURE] Found" << students.size() << "records";
    return students;
}

/**
 * @brief Валідація назви факультету через WHITELIST.
 *
 * WHITELIST - список дозволених значень.
 * Це один з найсильніших методів захисту!
 *
 * Принцип:
 * - Дозволяємо тільки відомі, перевірені значення.
 * - Все інше блокується автоматично.
 * - Навіть якщо атакуючий знайде вразливість у Prepared Statement
 *   (теоретично неможливо), whitelist його зупинить.
 *
 * @param faculty Назва факультету.
 * @return true, якщо факультет у whitelist.
 */
bool SecureQueries::isValidFaculty(const QString& faculty)
{
    // ========== WHITELIST дозволених факультетів ==========
    // QSet для швидкого пошуку O(1).
    static const QSet<QString> validFaculties = {
        "Інформатика",
        "Математика",
        "Фізика",
        "Економіка",
        "Хімія",
        "Біологія"
    };

    // Перевірка: чи є faculty у whitelist?
    bool isValid = validFaculties.contains(faculty);

    if (!isValid) {
        qWarning() << "[SECURE] Faculty not in whitelist:" << faculty;
        qDebug() << "[SECURE] Allowed faculties:" << validFaculties;
    }

    return isValid;
}
