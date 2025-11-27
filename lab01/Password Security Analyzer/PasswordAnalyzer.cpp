/*
 * PasswordAnalyzer.cpp.
 *
 * Реалізація класу аналізатора паролів.
 * Містить логіку перевірки безпеки паролів та генерації рекомендацій.
 */

#include "PasswordAnalyzer.h"
#include <QDebug>
#include <algorithm>

/**
 * @brief Конструктор аналізатора паролів.
 *
 * Ініціалізує словник поширених слів при створенні об'єкта.
 */
PasswordAnalyzer::PasswordAnalyzer() {
    initializeDictionary();
}

/**
 * @brief Ініціалізація словника поширених слів.
 *
 * Заповнює масив commonWords популярними словами та паролями,
 * які часто використовуються і легко підбираються.
 * Включає українські та англійські слова, а також числові комбінації.
 */
void PasswordAnalyzer::initializeDictionary() {
    commonWords = {
        // Популярні англійські паролі та слова.
        "password", "qwerty", "admin", "user", "login",
        "love", "life", "hello", "world", "ukraine",
        "football", "dragon", "master",

        // Українські слова.
        "паролі", "пароль", "користувач", "вхід",

        // Прості числові комбінації.
        "123456", "111111", "000000"
    };
}

/**
 * @brief Нормалізація тексту для порівняння.
 * @param text Вхідний текст.
 * @return Нормалізований текст у нижньому регістрі з заміненими символами.
 *
 * Функція виконує наступні перетворення:
 * - Переводить весь текст у нижній регістр.
 * - Замінює цифри та символи на схожі літери (leetspeak):
 *   - 1 -> i
 *   - 0 -> o
 *   - 3 -> e
 *   - 4 -> a
 *   - 5 -> s
 *   - @ -> a
 *   - $ -> s
 *
 * Це дозволяє виявити персональні дані навіть якщо користувач
 * спробував їх приховати заміною символів.
 */
QString PasswordAnalyzer::normalize(const QString& text) {
    QString normalized = text.toLower();

    // Заміна схожих символів (leetspeak).
    normalized.replace('1', 'i');
    normalized.replace('0', 'o');
    normalized.replace('3', 'e');
    normalized.replace('4', 'a');
    normalized.replace('5', 's');
    normalized.replace('@', 'a');
    normalized.replace('$', 's');

    return normalized;
}

/**
 * @brief Транслітерація українського тексту в англійський.
 * @param text Український текст.
 * @return Транслітерований текст латиницею.
 */
QString PasswordAnalyzer::transliterateUkrToEng(const QString& text) {
    QString result;

    for (int i = 0; i < text.length(); i++) {
        QChar ch = text[i];

        // Українські літери в нижньому регістрі.
        if (ch == QString("а")[0]) result += "a";
        else if (ch == QString("б")[0]) result += "b";
        else if (ch == QString("в")[0]) result += "v";
        else if (ch == QString("г")[0]) result += "h";
        else if (ch == QString("ґ")[0]) result += "g";
        else if (ch == QString("д")[0]) result += "d";
        else if (ch == QString("е")[0]) result += "e";
        else if (ch == QString("є")[0]) result += "ye";
        else if (ch == QString("ж")[0]) result += "zh";
        else if (ch == QString("з")[0]) result += "z";
        else if (ch == QString("и")[0]) result += "y";
        else if (ch == QString("і")[0]) result += "i";
        else if (ch == QString("ї")[0]) result += "yi";
        else if (ch == QString("й")[0]) result += "y";
        else if (ch == QString("к")[0]) result += "k";
        else if (ch == QString("л")[0]) result += "l";
        else if (ch == QString("м")[0]) result += "m";
        else if (ch == QString("н")[0]) result += "n";
        else if (ch == QString("о")[0]) result += "o";
        else if (ch == QString("п")[0]) result += "p";
        else if (ch == QString("р")[0]) result += "r";
        else if (ch == QString("с")[0]) result += "s";
        else if (ch == QString("т")[0]) result += "t";
        else if (ch == QString("у")[0]) result += "u";
        else if (ch == QString("ф")[0]) result += "f";
        else if (ch == QString("х")[0]) result += "kh";
        else if (ch == QString("ц")[0]) result += "ts";
        else if (ch == QString("ч")[0]) result += "ch";
        else if (ch == QString("ш")[0]) result += "sh";
        else if (ch == QString("щ")[0]) result += "shch";
        else if (ch == QString("ь")[0]) result += "";
        else if (ch == QString("ю")[0]) result += "yu";
        else if (ch == QString("я")[0]) result += "ya";

        // Українські літери у верхньому регістрі.
        else if (ch == QString("А")[0]) result += "A";
        else if (ch == QString("Б")[0]) result += "B";
        else if (ch == QString("В")[0]) result += "V";
        else if (ch == QString("Г")[0]) result += "H";
        else if (ch == QString("Ґ")[0]) result += "G";
        else if (ch == QString("Д")[0]) result += "D";
        else if (ch == QString("Е")[0]) result += "E";
        else if (ch == QString("Є")[0]) result += "Ye";
        else if (ch == QString("Ж")[0]) result += "Zh";
        else if (ch == QString("З")[0]) result += "Z";
        else if (ch == QString("И")[0]) result += "Y";
        else if (ch == QString("І")[0]) result += "I";
        else if (ch == QString("Ї")[0]) result += "Yi";
        else if (ch == QString("Й")[0]) result += "Y";
        else if (ch == QString("К")[0]) result += "K";
        else if (ch == QString("Л")[0]) result += "L";
        else if (ch == QString("М")[0]) result += "M";
        else if (ch == QString("Н")[0]) result += "N";
        else if (ch == QString("О")[0]) result += "O";
        else if (ch == QString("П")[0]) result += "P";
        else if (ch == QString("Р")[0]) result += "R";
        else if (ch == QString("С")[0]) result += "S";
        else if (ch == QString("Т")[0]) result += "T";
        else if (ch == QString("У")[0]) result += "U";
        else if (ch == QString("Ф")[0]) result += "F";
        else if (ch == QString("Х")[0]) result += "Kh";
        else if (ch == QString("Ц")[0]) result += "Ts";
        else if (ch == QString("Ч")[0]) result += "Ch";
        else if (ch == QString("Ш")[0]) result += "Sh";
        else if (ch == QString("Щ")[0]) result += "Shch";
        else if (ch == QString("Ь")[0]) result += "";
        else if (ch == QString("Ю")[0]) result += "Yu";
        else if (ch == QString("Я")[0]) result += "Ya";

        // Інші символи залишаємо без змін.
        else result += ch;
    }

    return result;
}

/**
 * @brief Перевірка наявності персональних даних у паролі.
 * @param password Пароль для перевірки.
 * @param data Персональні дані (ім'я, прізвище тощо).
 * @return true якщо дані знайдено у паролі.
 *
 * Функція виконує три типи перевірок:
 * 1. Пряма перевірка наявності даних.
 * 2. Перевірка з нормалізацією (ігнорування регістру та leetspeak).
 * 3. Перевірка зворотного запису (reverse).
 */
bool PasswordAnalyzer::containsPersonalData(const QString& password, const QString& data) {
    // Якщо дані порожні, перевірка не потрібна.
    if (data.isEmpty()) return false;

    // Нормалізація для порівняння.
    QString normalizedPassword = normalize(password);
    QString normalizedData = normalize(data);

    // Пряма перевірка наявності даних.
    if (normalizedPassword.contains(normalizedData)) return true;

    // Перевірка зворотного запису.
    QString reversed = normalizedData;
    std::reverse(reversed.begin(), reversed.end());
    if (normalizedPassword.contains(reversed)) return true;

    // Транслітерація українського тексту в англійський та перевірка.
    QString transliterated = transliterateUkrToEng(data);
    QString normalizedTranslit = normalize(transliterated);

    // Перевірка транслітерованого варіанту.
    if (normalizedPassword.contains(normalizedTranslit)) return true;

    // Перевірка зворотного запису транслітерованого варіанту.
    QString reversedTranslit = normalizedTranslit;
    std::reverse(reversedTranslit.begin(), reversedTranslit.end());
    if (normalizedPassword.contains(reversedTranslit)) return true;

    return false;
}

/**
 * @brief Перевірка наявності дати в різних форматах.
 * @param password Пароль для перевірки.
 * @param date Дата для пошуку.
 * @return true якщо дата знайдена у будь-якому форматі.
 *
 * Функція перевіряє наявність дати у наступних форматах:
 * - ddMMyyyy (наприклад: 15061995).
 * - ddMMyy (наприклад: 150695).
 * - dd.MM.yyyy (наприклад: 15.06.1995).
 * - dd/MM/yyyy (наприклад: 15/06/1995).
 * - yyyyMMdd (наприклад: 19950615).
 * - dd-MM-yyyy (наприклад: 15-06-1995).
 * - yyyy (наприклад: 1995).
 * - ddMM (наприклад: 1506).
 */
bool PasswordAnalyzer::containsDateVariants(const QString& password, const QDate& date) {
    // Якщо дата невалідна, перевірка не потрібна.
    if (!date.isValid()) return false;

    // Генерація всіх можливих форматів дати.
    QStringList dateFormats;
    dateFormats << date.toString("ddMMyyyy")
                << date.toString("ddMMyy")
                << date.toString("dd.MM.yyyy")
                << date.toString("dd/MM/yyyy")
                << date.toString("yyyyMMdd")
                << date.toString("dd-MM-yyyy")
                << date.toString("yyyy")
                << QString::number(date.year())
                << date.toString("ddMM");

    QString normalizedPassword = normalize(password);

    // Перевірка кожного формату.
    for (const QString& format : dateFormats) {
        if (normalizedPassword.contains(format.toLower())) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Перевірка наявності словникових слів.
 * @param password Пароль для перевірки.
 * @return true якщо знайдено поширені слова.
 *
 * Функція перевіряє пароль на наявність слів зі словника.
 * Враховуються тільки слова довжиною >= 4 символи.
 * Перевірка виконується з нормалізацією, щоб виявити
 * слова навіть якщо вони написані з leetspeak.
 */
bool PasswordAnalyzer::containsDictionaryWords(const QString& password) {
    QString normalizedPassword = normalize(password);

    // Перевірка кожного слова зі словника.
    for (const QString& word : commonWords) {
        // Враховуємо тільки слова довжиною >= 4 символи.
        if (word.length() >= 4 && normalizedPassword.contains(normalize(word))) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Перевірка наявності послідовностей символів.
 * @param password Пароль для перевірки.
 * @return true якщо знайдено послідовності.
 *
 * Функція перевіряє наявність послідовностей довжиною 3+ символи:
 * - Числові: 123, 234, 987, 876 тощо.
 * - Латиниця: abc, bcd, zyx, yxw тощо.
 * - Кирилиця: абв, бвг, яюї, іїй тощо.
 */
bool PasswordAnalyzer::containsSequences(const QString& password) {
    QString lower = password.toLower();

    // Перевірка числових послідовностей (прямих та зворотних).
    for (int i = 0; i <= lower.length() - 3; i++) {
        bool isSequence = true;
        bool isReverseSequence = true;

        // Перевірка на 3 послідовні символи.
        for (int j = 0; j < 2; j++) {
            QChar current = lower[i + j];
            QChar next = lower[i + j + 1];

            // Перевірка числової послідовності (пряма).
            if (current.isDigit() && next.isDigit()) {
                if (next.unicode() - current.unicode() != 1) {
                    isSequence = false;
                }
                if (current.unicode() - next.unicode() != 1) {
                    isReverseSequence = false;
                }
            } else {
                isSequence = false;
                isReverseSequence = false;
                break;
            }
        }

        if (isSequence || isReverseSequence) {
            return true;
        }
    }

    // Перевірка букв латиниці.
    for (int i = 0; i <= lower.length() - 3; i++) {
        bool isSequence = true;
        bool isReverseSequence = true;

        for (int j = 0; j < 2; j++) {
            QChar current = lower[i + j];
            QChar next = lower[i + j + 1];

            // Латинські букви a-z.
            if (current >= 'a' && current <= 'z' && next >= 'a' && next <= 'z') {
                if (next.unicode() - current.unicode() != 1) {
                    isSequence = false;
                }
                if (current.unicode() - next.unicode() != 1) {
                    isReverseSequence = false;
                }
            } else {
                isSequence = false;
                isReverseSequence = false;
                break;
            }
        }

        if (isSequence || isReverseSequence) {
            return true;
        }
    }

    // Перевірка кирилиці (українська абетка).
    QString cyrillicAlphabet = QString("абвгґдеєжзиіїйклмнопрстуфхцчшщьюя");

    for (int i = 0; i <= lower.length() - 3; i++) {
        bool isSequence = true;

        for (int j = 0; j < 3; j++) {
            QChar ch = lower[i + j];
            int pos = cyrillicAlphabet.indexOf(ch);

            if (pos == -1) {
                isSequence = false;
                break;
            }

            // Перевірка чи наступний символ йде по порядку.
            if (j < 2) {
                QChar nextCh = lower[i + j + 1];
                int nextPos = cyrillicAlphabet.indexOf(nextCh);

                // Пряма або зворотна послідовність.
                if (nextPos != pos + 1 && nextPos != pos - 1) {
                    isSequence = false;
                    break;
                }

                // Перевірка що послідовність однакова (або тільки пряма, або тільки зворотна).
                if (j > 0) {
                    int prevPos = cyrillicAlphabet.indexOf(lower[i + j - 1]);
                    int direction1 = cyrillicAlphabet.indexOf(lower[i + j]) - prevPos;
                    int direction2 = nextPos - pos;

                    if (direction1 != direction2) {
                        isSequence = false;
                        break;
                    }
                }
            }
        }

        if (isSequence) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Розрахунок оцінки складності пароля.
 * @param password Пароль для оцінки.
 * @return Оцінка від 1 до 10 балів.
 *
 * Функція оцінює складність пароля за наступними критеріями:
 *
 * 1. Довжина пароля (максимум 4 бали):
 *    - >= 16 символів: 4 бали.
 *    - >= 12 символів: 3 бали.
 *    - >= 8 символів: 2 бали.
 *    - >= 6 символів: 1 бал.
 *
 * 2. Наявність великих літер (A-Z, А-Я): 1 бал.
 * 3. Наявність малих літер (a-z, а-я): 1 бал.
 * 4. Наявність цифр (0-9): 1 бал.
 * 5. Наявність спеціальних символів: 2 бали.
 * 6. Висока різноманітність символів (>70% унікальних): 1 бал.
 *
 * Максимальна оцінка: 10 балів.
 */
int PasswordAnalyzer::calculateComplexityScore(const QString& password) {
    int score = 0;
    int length = password.length();

    // ============ Оцінка довжини (максимум 4 бали) ============
    if (length >= 16) {
        score += 4;
    } else if (length >= 12) {
        score += 3;
    } else if (length >= 8) {
        score += 2;
    } else if (length >= 6) {
        score += 1;
    }

    // ============ Наявність великих літер (1 бал) ============
    // Перевіряємо як англійські (A-Z), так і українські (А-Я, І, Ї, Є).
    if (password.contains(QRegularExpression("[A-ZА-ЯІЇЄ]"))) {
        score += 1;
    }

    // ============ Наявність малих літер (1 бал) ============
    // Перевіряємо як англійські (a-z), так і українські (а-я, і, ї, є).
    if (password.contains(QRegularExpression("[a-zа-яіїє]"))) {
        score += 1;
    }

    // ============ Наявність цифр (1 бал) ============
    if (password.contains(QRegularExpression("[0-9]"))) {
        score += 1;
    }

    // ============ Наявність спеціальних символів (2 бали) ============
    // Включає: !@#$%^&*()_+-=[]{};':"\\|,.<>/?
    if (password.contains(QRegularExpression("[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]"))) {
        score += 2;
    }

    // ============ Різноманітність символів (1 бал) ============
    // Якщо >= 70% символів унікальні, додаємо бал.
    QSet<QChar> uniqueChars;
    for (QChar ch : password) {
        uniqueChars.insert(ch);
    }
    if (uniqueChars.size() >= length * 0.7) {
        score += 1;
    }

    // Обмежуємо максимальну оцінку до 10 балів.
    return qMin(score, 10);
}

/**
 * @brief Визначення текстового рівня надійності.
 * @param score Числова оцінка від 1 до 10.
 * @return Текстовий опис рівня надійності.
 *
 * Конвертує числову оцінку у зрозумілий текстовий опис:
 * - 1-3 балів: "Дуже слабкий".
 * - 4-5 балів: "Слабкий".
 * - 6-7 балів: "Середній".
 * - 8 балів: "Надійний".
 * - 9-10 балів: "Дуже надійний".
 */
QString PasswordAnalyzer::determineStrengthLevel(int score) {
    if (score <= 3) {
        return "Дуже слабкий";
    } else if (score <= 5) {
        return "Слабкий";
    } else if (score <= 7) {
        return "Середній";
    } else if (score <= 8) {
        return "Надійний";
    } else {
        return "Дуже надійний";
    }
}

/**
 * @brief Генерація рекомендацій для покращення пароля.
 * @param result Результати попереднього аналізу.
 * @return Список персоналізованих рекомендацій.
 *
 * Функція аналізує результати перевірки пароля та генерує
 * список конкретних рекомендацій для підвищення безпеки.
 *
 * Рекомендації включають:
 * - Попередження про персональні дані.
 * - Поради щодо довжини пароля.
 * - Вимоги до різноманітності символів.
 * - Попередження про словникові слова.
 * - Загальні рекомендації з безпеки.
 */
QStringList PasswordAnalyzer::generateRecommendations(const AnalysisResult& result) {
    QStringList recommendations;

    // ============ Рекомендації щодо персональних даних ============
    if (result.containsName || result.containsBirthDate || result.containsBirthYear) {
        recommendations << "Уникайте використання особистих даних (імені, дати народження)!";
        recommendations << "Персональні дані легко вгадати або знайти в соціальних мережах.";
    }

    // ============ Рекомендації щодо довжини ============
    if (result.length < 8) {
        recommendations << "Збільште довжину пароля мінімум до 12 символів.";
    } else if (result.length < 12) {
        recommendations << "Рекомендуємо використовувати пароль довжиною 16+ символів для максимальної безпеки.";
    }

    // ============ Рекомендації щодо різноманітності символів ============

    // Якщо немає великих літер.
    if (!result.hasUpperCase) {
        recommendations << "Додайте великі літери (A-Z, А-Я).";
    }

    // Якщо немає малих літер.
    if (!result.hasLowerCase) {
        recommendations << "Додайте малі літери (a-z, а-я).";
    }

    // Якщо немає цифр.
    if (!result.hasDigits) {
        recommendations << "Додайте цифри (0-9).";
    }

    // Якщо немає спеціальних символів.
    if (!result.hasSpecialChars) {
        recommendations << "Додайте спеціальні символи (!@#$%^&* тощо).";
    }

    // ============ Рекомендації щодо словникових слів ============
    if (result.hasDictionaryWords) {
        recommendations << "Уникайте поширених слів зі словника.";
        recommendations << "Використовуйте випадкову комбінацію символів або парольну фразу.";
    }

    // ============ Рекомендації щодо послідовностей ============
    if (result.hasSequences) {
        recommendations << "Уникайте послідовностей символів (123, abc, абв тощо).";
        recommendations << "Послідовності легко вгадати за допомогою автоматизованих атак.";
    }

    // ============ Загальні рекомендації ============
    if (result.score < 7) {
        recommendations << "Розгляньте використання менеджера паролів для генерації надійних паролів.";
        recommendations << "Приклад надійного пароля: T#9kL$mP2@xQ7wN!";
    }

    // ============ Якщо пароль надійний ============
    if (recommendations.isEmpty()) {
        recommendations << "Чудово! Ваш пароль відповідає високим стандартам безпеки.";
        recommendations << "Не забувайте регулярно змінювати паролі та використовувати двофакторну автентифікацію.";
    }

    return recommendations;
}

/**
 * @brief Основний метод аналізу пароля.
 * @param password Пароль для аналізу.
 * @param firstName Ім'я користувача.
 * @param lastName Прізвище користувача.
 * @param birthDate Дата народження.
 * @param email Email адреса.
 * @return AnalysisResult структура з повними результатами аналізу.
 *
 * Це головна функція аналізатора, яка виконує комплексну перевірку:
 *
 * 1. Базовий аналіз складності:
 *    - Довжина пароля.
 *    - Наявність різних типів символів.
 *    - Перевірка словникових слів.
 *
 * 2. Перевірка персональних даних:
 *    - Ім'я та прізвище.
 *    - Дата народження та рік.
 *    - Частина email адреси.
 *
 * 3. Розрахунок фінальної оцінки:
 *    - Базова оцінка складності.
 *    - Штрафи за використання персональних даних.
 *    - Штрафи за словникові слова.
 *
 * 4. Генерація рекомендацій для покращення пароля.
 */
AnalysisResult PasswordAnalyzer::analyzePassword(const QString& password,
                                                 const QString& firstName,
                                                 const QString& lastName,
                                                 const QDate& birthDate,
                                                 const QString& email) {
    AnalysisResult result;

    // ============ Базовий аналіз складності ============
    result.length = password.length();
    result.hasUpperCase = password.contains(QRegularExpression("[A-ZА-ЯІЇЄ]"));
    result.hasLowerCase = password.contains(QRegularExpression("[a-zа-яіїє]"));
    result.hasDigits = password.contains(QRegularExpression("[0-9]"));
    result.hasSpecialChars = password.contains(QRegularExpression("[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]"));
    result.hasDictionaryWords = containsDictionaryWords(password);
    result.hasSequences = containsSequences(password);

    // ============ Перевірка персональних даних ============
    result.containsName = false;

    // Перевірка імені.
    if (!firstName.isEmpty()) {
        if (containsPersonalData(password, firstName)) {
            result.containsName = true;
            result.personalDataFound << QString("Ім'я: %1").arg(firstName);
        }
    }

    // Перевірка прізвища.
    if (!lastName.isEmpty()) {
        if (containsPersonalData(password, lastName)) {
            result.containsName = true;
            result.personalDataFound << QString("Прізвище: %1").arg(lastName);
        }
    }

    // Перевірка дати народження.
    result.containsBirthDate = containsDateVariants(password, birthDate);
    result.containsBirthYear = false;

    if (birthDate.isValid()) {
        // Перевірка року окремо.
        QString year = QString::number(birthDate.year());
        if (password.contains(year)) {
            result.containsBirthYear = true;
            result.personalDataFound << QString("Рік народження: %1").arg(year);
        }

        // Додавання інформації про повну дату.
        if (result.containsBirthDate) {
            result.personalDataFound << QString("Дата народження: %1").arg(birthDate.toString("dd.MM.yyyy"));
        }
    }

    // Перевірка email.
    if (!email.isEmpty()) {
        // Витягуємо частину email до символу @.
        QString emailName = email.split('@').first();
        if (containsPersonalData(password, emailName)) {
            result.personalDataFound << QString("Частина email: %1").arg(emailName);
        }
    }

    // ============ Розрахунок базового балу складності ============
    int complexityScore = calculateComplexityScore(password);

    // ============ Зменшення балу за використання персональних даних ============
    int penalty = 0;

    // Штраф за ім'я або прізвище.
    if (result.containsName) penalty += 3;

    // Штраф за дату або рік народження.
    if (result.containsBirthDate || result.containsBirthYear) penalty += 3;

    // Штраф за словникові слова.
    if (result.hasDictionaryWords) penalty += 2;

    // Штраф за послідовності.
    if (result.hasSequences) penalty += 2;

    // Фінальна оцінка (мінімум 1 бал).
    result.score = qMax(1, complexityScore - penalty);

    // ============ Визначення рівня надійності та генерація рекомендацій ============
    result.strengthLevel = determineStrengthLevel(result.score);
    result.recommendations = generateRecommendations(result);

    return result;
}
