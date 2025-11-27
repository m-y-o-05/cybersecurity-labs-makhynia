/*
 * EmailEncryptorWindow_StepByStep.cpp
 *
 * Файл з реалізацією вкладки покрокового алгоритму.
 * Містить текстове пояснення роботи Email-шифратора.
 */

#include "EmailEncryptorWindow.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QScrollArea>

/*
 * ============================================================================
 * СТВОРЕННЯ ВКЛАДКИ ПОКРОКОВОГО АЛГОРИТМУ
 * ============================================================================
 */

QWidget* EmailEncryptorWindow::createStepByStepTab() {
    QWidget* stepTab = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(stepTab);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Створюємо текстове поле з HTML-контентом.
    QTextEdit* textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    // Генеруємо HTML-контент з поясненням алгоритму.
    QString algorithm = generateAlgorithmExplanation();
    textEdit->setHtml(algorithm);

    mainLayout->addWidget(textEdit);

    return stepTab;
}

/*
 * ============================================================================
 * ГЕНЕРАЦІЯ HTML-ПОЯСНЕННЯ АЛГОРИТМУ
 * ============================================================================
 */

QString EmailEncryptorWindow::generateAlgorithmExplanation() {
    QString algorithm = "<div style='color: #e0e0e0; font-family: Arial; font-size: 13px;'>";
    algorithm += "<h2 style='color: #00CED1; text-align: center;'>Покроковий алгоритм Email-шифратора</h2>";

    // ========== ВСТУП ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.15); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00CED1;'>Що таке симетричне шифрування?</h3>";
    algorithm += "<p><b>Симетричне шифрування</b> - це криптографічний метод, де:</p>";
    algorithm += "<ul>";
    algorithm += "<li><b>Один ключ:</b> Використовується для шифрування та розшифрування</li>";
    algorithm += "<li><b>Швидкість:</b> Працює набагато швидше за асиметричне шифрування</li>";
    algorithm += "<li><b>Секретність:</b> Ключ повинен залишатися таємним для обох сторін</li>";
    algorithm += "</ul>";
    algorithm += "<p>Це як замок з ключем: той самий ключ відкриває і закриває!</p>";
    algorithm += "</div>";

    // ========== ЕТАП 1: Генерація ключа ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 1: Генерація ключа шифрування</h3>";

    algorithm += "<p><b>Крок 1.1:</b> Збираємо персональні дані</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Email: \"ivan.petrenko@gmail.com\"\n";
    algorithm += "Ім'я: \"Іван\"\n";
    algorithm += "Прізвище: \"Петренко\"\n";
    algorithm += "Дата народження: \"15.03.1995\" → \"15031995\"";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 1.2:</b> Об'єднуємо дані в один рядок</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Персональні дані = Ім'я + Прізвище + Дата\n";
    algorithm += "Результат: \"ІванПетренко15031995\"";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 1.3:</b> Генеруємо ключ за допомогою SHA-256</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Вхід: \"ІванПетренко15031995\"\n";
    algorithm += "        ↓ SHA-256 хешування\n";
    algorithm += "Ключ (hex): FD3EF50CF78EAD89BF7A022F207CFBE8...\n";
    algorithm += "Довжина: 64 символи (256 біт)\n\n";
    algorithm += "ВАЖЛИВО: Цей ключ використовується для шифрування!\n";
    algorithm += "Зберігайте його в безпечному місці!";
    algorithm += "</pre>";

    algorithm += "<p><b>Що таке SHA-256?</b></p>";
    algorithm += "<ul>";
    algorithm += "<li><b>SHA</b> - Secure Hash Algorithm (Безпечний алгоритм хешування)</li>";
    algorithm += "<li><b>256</b> - довжина хешу в бітах</li>";
    algorithm += "<li><b>Однонаправлена функція:</b> неможливо відновити оригінальні дані з хешу</li>";
    algorithm += "<li><b>Детермінований:</b> однакові дані завжди дають однаковий хеш</li>";
    algorithm += "<li><b>Унікальний:</b> навіть мінімальна зміна даних повністю змінює хеш</li>";
    algorithm += "</ul>";

    algorithm += "<p><b>Приклад зміни:</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "SHA256(\"ІванПетренко15031995\") = FD3EF50CF78EAD89...\n";
    algorithm += "SHA256(\"ІванПетренко15031996\") = A7B2C9E4F1D8A3B5... ← Повністю інший!";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== ЕТАП 2: Шифрування повідомлення ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.08); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 2: Шифрування повідомлення</h3>";

    algorithm += "<p><b>Крок 2.1:</b> Підготовка повідомлення</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Повідомлення: \"Зустрічаємося завтра о 15:00\"\n";
    algorithm += "        ↓ Конвертація в байти (UTF-8)\n";
    algorithm += "Байти: [208, 151, 209, 131, 209, 129, 209, 130...]";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 2.2:</b> Підготовка ключа</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Ключ (hex): FD3EF50CF78EAD89BF7A022F207CFBE8...\n";
    algorithm += "        ↓ Конвертація з hex в байти\n";
    algorithm += "Байти ключа: [253, 62, 245, 12, 247, 142, 173, 137...]\n\n";
    algorithm += "Ключ повторюється циклічно до довжини повідомлення:";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 2.3:</b> Операція XOR (побітове виключне АБО)</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Для кожного байта:\n";
    algorithm += "Зашифрований[i] = Повідомлення[i] XOR Ключ[i]\n\n";
    algorithm += "Приклад:\n";
    algorithm += "Байт повідомлення: 208 (11010000 в binary)\n";
    algorithm += "Байт ключа:        253 (11111101 в binary)\n";
    algorithm += "                   ──────────────────────\n";
    algorithm += "XOR результат:      45 (00101101 в binary)\n\n";
    algorithm += "Таблиця істинності XOR:\n";
    algorithm += "   0 XOR 0 = 0\n";
    algorithm += "   0 XOR 1 = 1\n";
    algorithm += "   1 XOR 0 = 1\n";
    algorithm += "   1 XOR 1 = 0";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 2.4:</b> Кодування в Base64</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Зашифровані байти: [45, 123, 89, 201, 157...]\n";
    algorithm += "        ↓ Base64 кодування\n";
    algorithm += "Результат: \"LXtZyZ1kPm4xQz8=\"\n\n";
    algorithm += "Base64 дозволяє передавати бінарні дані як текст!";
    algorithm += "</pre>";

    algorithm += "<p><b>Чому XOR ідеально підходить для шифрування?</b></p>";
    algorithm += "<ul>";
    algorithm += "<li><b>Симетричність:</b> A XOR B XOR B = A (той самий ключ для шифр./розшифр.)</li>";
    algorithm += "<li><b>Швидкість:</b> Дуже швидка операція на рівні процесора</li>";
    algorithm += "<li><b>Ефективність:</b> Не змінює розмір даних</li>";
    algorithm += "<li><b>Безпека:</b> З випадковим ключем - криптографічно стійкий</li>";
    algorithm += "</ul>";
    algorithm += "</div>";

    // ========== ЕТАП 3: Розшифрування повідомлення ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 3: Розшифрування повідомлення</h3>";

    algorithm += "<p><b>Крок 3.1:</b> Отримання зашифрованого повідомлення</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Зашифроване (Base64): \"LXtZyZ1kPm4xQz8=\"\n";
    algorithm += "        ↓ Декодування з Base64\n";
    algorithm += "Зашифровані байти: [45, 123, 89, 201, 157...]";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 3.2:</b> Підготовка того самого ключа</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Ключ (той самий!): FD3EF50CF78EAD89BF7A022F207CFBE8...\n";
    algorithm += "        ↓ Конвертація в байти\n";
    algorithm += "Байти ключа: [253, 62, 245, 12, 247, 142, 173, 137...]";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 3.3:</b> Операція XOR (знову!)</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Для кожного байта:\n";
    algorithm += "Розшифрований[i] = Зашифрований[i] XOR Ключ[i]\n\n";
    algorithm += "Приклад:\n";
    algorithm += "Зашифрований байт:  45 (00101101 в binary)\n";
    algorithm += "Байт ключа:        253 (11111101 в binary)\n";
    algorithm += "                   ──────────────────────\n";
    algorithm += "XOR результат:     208 (11010000 в binary)\n\n";
    algorithm += "Отримали оригінальний байт повідомлення!";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 3.4:</b> Конвертація в текст</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Розшифровані байти: [208, 151, 209, 131, 209, 129...]\n";
    algorithm += "        ↓ Конвертація з UTF-8\n";
    algorithm += "Результат: \"Зустрічаємося завтра о 15:00\"\n\n";
    algorithm += "Повідомлення успішно розшифровано!";
    algorithm += "</pre>";

    algorithm += "<p><b>Магія симетричного шифрування:</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Оригінал XOR Ключ = Зашифрований\n";
    algorithm += "Зашифрований XOR Ключ = Оригінал\n\n";
    algorithm += "Приклад з числами:\n";
    algorithm += "5 XOR 3 = 6\n";
    algorithm += "6 XOR 3 = 5  ← Повернулись до оригіналу!";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== ЕТАП 4: Шифрування файлів ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.08); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 4: Шифрування файлів</h3>";

    algorithm += "<p><b>Крок 4.1:</b> Читання файлу</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Файл: document.pdf (розмір: 1,234,567 байт)\n";
    algorithm += "        ↓ Читання всіх байтів\n";
    algorithm += "Дані файлу: [37, 80, 68, 70, 45, 49, 46, 55...]";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 4.2:</b> XOR шифрування</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Для КОЖНОГО байта файлу:\n";
    algorithm += "Зашифрований[i] = Файл[i] XOR Ключ[i mod довжина_ключа]\n\n";
    algorithm += "Приклад для 3-х байтів:\n";
    algorithm += "Файл:     [37,  80,  68]\n";
    algorithm += "Ключ:     [253, 62,  245] (циклічно)\n";
    algorithm += "         ────────────────────\n";
    algorithm += "Результат: [220, 110, 177]";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 4.3:</b> Збереження зашифрованого файлу</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Зашифровані дані: [220, 110, 177, 89, 123...]\n";
    algorithm += "        ↓ Збереження у файл\n";
    algorithm += "Файл: document.pdf.encrypted\n\n";
    algorithm += "Розмір залишається незмінним: 1,234,567 байт";
    algorithm += "</pre>";

    algorithm += "<p><b>Розшифрування файлу - той самий процес!</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Зашифрований_файл XOR Ключ = Оригінальний_файл\n\n";
    algorithm += "Працює для будь-яких типів файлів:\n";
    algorithm += "1. Документи (PDF, DOCX, TXT)\n";
    algorithm += "2. Зображення (JPG, PNG, GIF)\n";
    algorithm += "3. Відео (MP4, AVI)\n";
    algorithm += "4. Архіви (ZIP, RAR)\n";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== БЕЗПЕКА ==========
    algorithm += "<div style='background: rgba(220, 38, 38, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #f87171;'>Безпека та обмеження</h3>";

    algorithm += "<p><b>Переваги реалізації:</b></p>";
    algorithm += "<ul>";
    algorithm += "<li><b>Простота:</b> Легко зрозуміти принципи роботи</li>";
    algorithm += "<li><b>Швидкість:</b> XOR - дуже швидка операція</li>";
    algorithm += "<li><b>Демонстраційна:</b> Показує основи криптографії</li>";
    algorithm += "</ul>";

    algorithm += "<p><b>Обмеження:</b></p>";
    algorithm += "<ul>";
    algorithm += "<li><b>Детермінований ключ:</b> Генерується з персональних даних (передбачувано)</li>";
    algorithm += "<li><b>Відсутність солі:</b> Немає випадкової компоненти</li>";
    algorithm += "<li><b>Простий XOR:</b> Без додаткових криптографічних перетворень</li>";
    algorithm += "<li><b>Немає аутентифікації:</b> Неможливо перевірити цілісність</li>";
    algorithm += "</ul>";

    // ========== Висновки ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.12); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00CED1;'>Висновки</h3>";
    algorithm += "<ul>";
    algorithm += "<li><b>Симетричне шифрування:</b> Один ключ для шифрування та розшифрування</li>";
    algorithm += "<li><b>SHA-256:</b> Генерує стійкий ключ з персональних даних</li>";
    algorithm += "<li><b>XOR операція:</b> Проста, швидка і симетрична</li>";
    algorithm += "<li><b>Base64:</b> Дозволяє передавати бінарні дані як текст</li>";
    algorithm += "<li><b>Універсальність:</b> Працює з текстом і файлами</li>";
    algorithm += "<li><b>Освітня цінність:</b> Демонструє основи криптографії</li>";
    algorithm += "<li><b>Обмеження:</b> Не використовувати для реального захисту даних</li>";
    algorithm += "</ul>";

    algorithm += "</div>";

    return algorithm;
}
