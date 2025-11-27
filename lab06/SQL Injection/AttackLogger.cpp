/*
 * AttackLogger.cpp
 *
 * Реалізація системи логування SQL-ін'єкцій.
 * Детекція, запис та аналіз спроб атак.
 */

#include "AttackLogger.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>

/**
 * @brief Логування спроби SQL-ін'єкції.
 *
 * Записує всю інформацію про спробу атаки у таблицю attack_logs.
 * Це дозволяє:
 * - Відстежувати патерни атак.
 * - Аналізувати, які методи використовують атакуючі.
 * - Покращувати захист на основі реальних даних.
 * - Виявляти систематичні спроби зламу.
 *
 * Процес логування:
 * 1. Детекція типу атаки (якщо не вказано).
 * 2. Запис у БД через Prepared Statement (іронія - захист логів від ін'єкцій!).
 * 3. Виведення у консоль для негайного моніторингу.
 *
 * @param attackType Тип атаки (UNION, OR 1=1, DROP, тощо).
 * @param queryText Повний SQL запит, що був спробований.
 * @param userInput Оригінальний ввід користувача.
 * @param isBlocked Чи успішно заблоковано атаку.
 * @param additionalInfo Додаткова інформація (IP, браузер тощо).
 */
void AttackLogger::logAttack(const QString& attackType,
                             const QString& queryText,
                             const QString& userInput,
                             bool isBlocked,
                             const QString& additionalInfo)
{
    // ========== КРОК 1: Автоматична детекція типу атаки ==========
    QString detectedType = attackType;
    if (detectedType.isEmpty()) {
        detectedType = detectAttackPattern(userInput);
        if (detectedType.isEmpty()) {
            detectedType = "UNKNOWN";
        }
    }

    // ========== КРОК 2: Виведення у консоль для моніторингу ==========
    qWarning() << "========================================";
    qWarning() << "[ATTACK DETECTED]";
    qWarning() << "Type:" << detectedType;
    qWarning() << "Blocked:" << (isBlocked ? "YES" : "NO");
    qWarning() << "User Input:" << userInput;
    qWarning() << "Query:" << queryText;
    qWarning() << "Time:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    if (!additionalInfo.isEmpty()) {
        qWarning() << "Additional Info:" << additionalInfo;
    }
    qWarning() << "========================================";

    // ========== КРОК 3: Запис у БД==========
    QSqlQuery query(DatabaseManager::getDatabase());

    QString insertQuery =
        "INSERT INTO attack_logs "
        "(attack_type, query_text, user_input, is_blocked, ip_address, additional_info) "
        "VALUES (:attackType, :queryText, :userInput, :isBlocked, :ipAddress, :additionalInfo)";

    if (!query.prepare(insertQuery)) {
        qCritical() << "[LOGGER] Failed to prepare log insert:"
                    << query.lastError().text();
        return;
    }

    // Підставляємо параметри.
    query.bindValue(":attackType", detectedType);
    query.bindValue(":queryText", queryText);
    query.bindValue(":userInput", userInput);
    query.bindValue(":isBlocked", isBlocked);
    query.bindValue(":ipAddress", "127.0.0.1");
    query.bindValue(":additionalInfo", additionalInfo);

    // Виконуємо INSERT.
    if (!query.exec()) {
        qCritical() << "[LOGGER] Failed to insert log:"
                    << query.lastError().text();
        return;
    }

    qDebug() << "[LOGGER] Attack logged successfully with ID:"
             << query.lastInsertId().toInt();
}

/**
 * @brief Отримання всіх логів атак з БД.
 *
 * Повертає всі записи з таблиці attack_logs для аналізу.
 * Сортування за часом (найновіші першими).
 *
 * @return Вектор структур AttackLog.
 */
QVector<AttackLog> AttackLogger::getAllLogs()
{
    QVector<AttackLog> logs;
    QSqlQuery query(DatabaseManager::getDatabase());

    // Сортування за часом (DESC = спочатку нові).
    QString queryString =
        "SELECT * FROM attack_logs ORDER BY timestamp DESC";

    if (!query.exec(queryString)) {
        qWarning() << "[LOGGER] Failed to fetch logs:"
                   << query.lastError().text();
        return logs;
    }

    // Читання результатів.
    while (query.next()) {
        AttackLog log;
        log.id = query.value("id").toInt();
        log.attackType = query.value("attack_type").toString();
        log.queryText = query.value("query_text").toString();
        log.userInput = query.value("user_input").toString();
        log.isBlocked = query.value("is_blocked").toBool();
        log.timestamp = query.value("timestamp").toDateTime();
        log.ipAddress = query.value("ip_address").toString();
        log.additionalInfo = query.value("additional_info").toString();

        logs.append(log);
    }

    qDebug() << "[LOGGER] Retrieved" << logs.size() << "logs";
    return logs;
}

/**
 * @brief Отримання логів за типом атаки.
 *
 * Фільтрує логи за конкретним типом (UNION, OR, DROP тощо).
 * Корисно для аналізу конкретних векторів атак.
 *
 * @param attackType Тип атаки для фільтрації.
 * @return Вектор відфільтрованих логів.
 */
QVector<AttackLog> AttackLogger::getLogsByType(const QString& attackType)
{
    QVector<AttackLog> logs;
    QSqlQuery query(DatabaseManager::getDatabase());

    // Використовуємо Prepared Statement для фільтрації.
    QString queryString =
        "SELECT * FROM attack_logs WHERE attack_type=:attackType "
        "ORDER BY timestamp DESC";

    if (!query.prepare(queryString)) {
        qWarning() << "[LOGGER] Failed to prepare type filter:"
                   << query.lastError().text();
        return logs;
    }

    query.bindValue(":attackType", attackType);

    if (!query.exec()) {
        qWarning() << "[LOGGER] Failed to fetch logs by type:"
                   << query.lastError().text();
        return logs;
    }

    // Читання результатів.
    while (query.next()) {
        AttackLog log;
        log.id = query.value("id").toInt();
        log.attackType = query.value("attack_type").toString();
        log.queryText = query.value("query_text").toString();
        log.userInput = query.value("user_input").toString();
        log.isBlocked = query.value("is_blocked").toBool();
        log.timestamp = query.value("timestamp").toDateTime();
        log.ipAddress = query.value("ip_address").toString();
        log.additionalInfo = query.value("additional_info").toString();

        logs.append(log);
    }

    qDebug() << "[LOGGER] Found" << logs.size() << "logs of type:" << attackType;
    return logs;
}

/**
 * @brief Автоматична детекція типу SQL-ін'єкції.
 *
 * Аналізує ввід користувача та визначає тип атаки.
 * Використовує регулярні вирази та ключові слова.
 *
 * Типи атак, що детектуються:
 * 1. UNION-based - об'єднання з іншою таблицею.
 * 2. Boolean-based - OR 1=1, AND 1=1 тощо.
 * 3. Time-based - SLEEP, WAITFOR тощо.
 * 4. Stacked queries - DROP, DELETE, UPDATE.
 * 5. Comment injection - подвійний дефіс, слеш-зірка.
 * 6. LIKE injection - символ відсотка у незвичайних місцях.
 *
 * Алгоритм детекції:
 * 1. Перевірка на UNION (найпопулярніша атака).
 * 2. Перевірка на Boolean операції (OR 1=1).
 * 3. Перевірка на небезпечні команди (DROP, DELETE).
 * 4. Перевірка на коментарі (дефіс-дефіс, слеш-зірка).
 * 5. Перевірка на часові атаки (SLEEP).
 *
 * @param userInput Ввід користувача для аналізу.
 * @return Назва типу атаки або пустий рядок.
 */
QString AttackLogger::detectAttackPattern(const QString& userInput)
{
    // Приводимо до upper case для аналізу (case-insensitive).
    QString input = userInput.toUpper();

    // ========== ДЕТЕКЦІЯ 1: UNION-based injection ==========
    // Найпопулярніший тип атаки для витягування даних.
    // Приклад: ' UNION SELECT username, password FROM users --
    if (input.contains("UNION") && input.contains("SELECT")) {
        qDebug() << "[DETECTOR] UNION-based injection detected";
        return "UNION-BASED";
    }

    // ========== ДЕТЕКЦІЯ 2: Boolean-based injection ==========
    // Використовує логічні операції для обходу перевірок.
    // Приклади: OR 1=1, OR '1'='1', AND 1=1
    QRegularExpression boolPattern(
        "((OR|AND)\\s+(\\d+\\s*=\\s*\\d+|'\\d+'\\s*=\\s*'\\d+'))",
        QRegularExpression::CaseInsensitiveOption
    );
    if (boolPattern.match(input).hasMatch()) {
        qDebug() << "[DETECTOR] Boolean-based injection detected";
        return "BOOLEAN-BASED (OR 1=1)";
    }

    // ========== ДЕТЕКЦІЯ 3: Stacked queries (множинні команди) ==========
    // Спроба виконати додаткові SQL команди.
    // Приклади: '; DROP TABLE --, '); DELETE FROM --
    QStringList dangerousCommands = {
        "DROP", "DELETE", "TRUNCATE", "ALTER", "CREATE", "INSERT INTO USERS"
    };
    for (const QString& cmd : dangerousCommands) {
        if (input.contains(cmd)) {
            qDebug() << "[DETECTOR] Stacked query injection detected:" << cmd;
            return "STACKED-QUERY (" + cmd + ")";
        }
    }

    // ========== ДЕТЕКЦІЯ 4: Comment injection ==========
    // Використання коментарів для обходу решти запиту.
    // Приклади: admin'--, admin'/*
    if (input.contains("--") || input.contains("/*") || input.contains("*/")) {
        qDebug() << "[DETECTOR] Comment-based injection detected";
        return "COMMENT-BASED";
    }

    // ========== ДЕТЕКЦІЯ 5: Time-based blind injection ==========
    // Використання затримок для детекції вразливостей.
    // Приклади: SLEEP(5), WAITFOR DELAY
    QStringList timeCommands = {"SLEEP", "WAITFOR", "BENCHMARK"};
    for (const QString& cmd : timeCommands) {
        if (input.contains(cmd)) {
            qDebug() << "[DETECTOR] Time-based injection detected";
            return "TIME-BASED";
        }
    }

    // ========== ДЕТЕКЦІЯ 6: Підозрілі символи ==========.
    // Перевірка на множинні одинарні лапки або крапки з комою
    int quoteCount = userInput.count("'");
    int semicolonCount = userInput.count(";");

    if (quoteCount >= 3 || semicolonCount >= 2) {
        qDebug() << "[DETECTOR] Suspicious characters detected";
        return "SUSPICIOUS-CHARACTERS";
    }

    // ========== ДЕТЕКЦІЯ 7: Загальна перевірка SQL ключових слів ==========
    if (containsSQLKeywords(input)) {
        qDebug() << "[DETECTOR] SQL keywords detected";
        return "SQL-KEYWORDS";
    }

    // Якщо нічого не виявлено.
    return "";
}

/**
 * @brief Очищення старих логів з БД.
 *
 * Видаляє записи старші за вказану кількість днів.
 * Це запобігає переповненню БД та покращує продуктивність.
 *
 * @param daysOld Видалити логи старші за N днів (за замовчуванням 30).
 * @return Кількість видалених записів.
 */
int AttackLogger::clearOldLogs(int daysOld)
{
    QSqlQuery query(DatabaseManager::getDatabase());

    // Обчислюємо дату відсікання.
    QDateTime cutoffDate = QDateTime::currentDateTime().addDays(-daysOld);

    // Використовуємо Prepared Statement для DELETE.
    QString deleteQuery =
        "DELETE FROM attack_logs WHERE timestamp < :cutoffDate";

    if (!query.prepare(deleteQuery)) {
        qWarning() << "[LOGGER] Failed to prepare delete query:"
                   << query.lastError().text();
        return 0;
    }

    query.bindValue(":cutoffDate", cutoffDate);

    if (!query.exec()) {
        qWarning() << "[LOGGER] Failed to delete old logs:"
                   << query.lastError().text();
        return 0;
    }

    int deletedCount = query.numRowsAffected();
    qDebug() << "[LOGGER] Deleted" << deletedCount << "old logs (older than"
             << daysOld << "days)";

    return deletedCount;
}

/**
 * @brief Генерація статистики атак.
 *
 * Створює текстовий звіт про всі зареєстровані атаки:
 * - Загальна кількість атак.
 * - Кількість заблокованих / незаблокованих.
 * - Розподіл за типами.
 * - Найпопулярніші типи атак.
 *
 * @return QString з форматованою статистикою.
 */
QString AttackLogger::getAttackStatistics()
{
    QString stats;
    QSqlQuery query(DatabaseManager::getDatabase());

    // ========== СТАТИСТИКА 1: Загальна кількість ==========
    query.exec("SELECT COUNT(*) FROM attack_logs");
    int totalAttacks = 0;
    if (query.next()) {
        totalAttacks = query.value(0).toInt();
    }

    // ========== СТАТИСТИКА 2: Заблоковані / Незаблоковані ==========
    query.exec("SELECT COUNT(*) FROM attack_logs WHERE is_blocked=1");
    int blockedCount = 0;
    if (query.next()) {
        blockedCount = query.value(0).toInt();
    }

    int unblockedCount = totalAttacks - blockedCount;

    // ========== СТАТИСТИКА 3: Розподіл за типами ==========
    query.exec(
        "SELECT attack_type, COUNT(*) as count "
        "FROM attack_logs "
        "GROUP BY attack_type "
        "ORDER BY count DESC"
    );

    QString typeBreakdown = "\nРозподіл за типами:\n";
    while (query.next()) {
        QString type = query.value("attack_type").toString();
        int count = query.value("count").toInt();
        double percentage = (totalAttacks > 0) ?
                           (count * 100.0 / totalAttacks) : 0.0;

        typeBreakdown += QString("   • %1: %2 (%3%)\n")
                         .arg(type)
                         .arg(count)
                         .arg(percentage, 0, 'f', 1);
    }

    // ========== СТАТИСТИКА 4: Останні атаки ==========
    query.exec(
        "SELECT timestamp FROM attack_logs "
        "ORDER BY timestamp DESC LIMIT 1"
    );
    QString lastAttack = "N/A";
    if (query.next()) {
        lastAttack = query.value(0).toDateTime()
                     .toString("yyyy-MM-dd HH:mm:ss");
    }

    // ========== ФОРМАТУВАННЯ ЗВІТУ ==========
    stats = QString(
        "═══════════════════════════════════════\n"
        "СТАТИСТИКА SQL-ІН'ЄКЦІЙ\n"
        "═══════════════════════════════════════\n\n"
        "Загальна кількість атак: %1\n"
        "Заблоковано: %2 (%3%)\n"
        "Не заблоковано: %4 (%5%)\n"
        "%6\n"
        "Остання атака: %7\n"
        "═══════════════════════════════════════\n"
    ).arg(totalAttacks)
     .arg(blockedCount)
     .arg(totalAttacks > 0 ? (blockedCount * 100.0 / totalAttacks) : 0.0, 0, 'f', 1)
     .arg(unblockedCount)
     .arg(totalAttacks > 0 ? (unblockedCount * 100.0 / totalAttacks) : 0.0, 0, 'f', 1)
     .arg(typeBreakdown)
     .arg(lastAttack);

    return stats;
}

/**
 * @brief Перевірка наявності SQL ключових слів.
 *
 * Допоміжна функція для детекції підозрілого вводу.
 * Перевіряє, чи містить ввід SQL команди.
 *
 * @param input Ввід користувача.
 * @return true, якщо знайдено SQL ключові слова.
 */
bool AttackLogger::containsSQLKeywords(const QString& input)
{
    // Список найпопулярніших SQL команд для детекції.
    static const QStringList sqlKeywords = {
        "SELECT", "INSERT", "UPDATE", "DELETE", "DROP",
        "CREATE", "ALTER", "TRUNCATE", "EXEC", "EXECUTE",
        "UNION", "JOIN", "WHERE", "HAVING", "GROUP BY",
        "ORDER BY", "LIMIT", "OFFSET"
    };

    QString upperInput = input.toUpper();

    // Перевірка кожного ключового слова.
    for (const QString& keyword : sqlKeywords) {
        // Перевіряємо, чи є слово окремо (не частина іншого слова).
        QRegularExpression wordBoundary(
            "\\b" + keyword + "\\b",
            QRegularExpression::CaseInsensitiveOption
        );

        if (wordBoundary.match(upperInput).hasMatch()) {
            qDebug() << "[DETECTOR] SQL keyword found:" << keyword;
            return true;
        }
    }

    return false;
}
