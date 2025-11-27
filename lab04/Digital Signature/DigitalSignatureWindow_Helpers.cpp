/*
 * DigitalSignatureWindow_Helpers.cpp
 *
 * Файл з допоміжними методами головного вікна.
 * Містить функції відображення результатів, теми оформлення та алгоритму.
 */

#include "DigitalSignatureWindow.h"
#include <QMessageBox>

/*
 * ============================================================================
 * ЗАСТОСУВАННЯ ТЕМНОЇ ТЕМИ
 * ============================================================================
 */

/**
 * @brief Застосування темної теми з бірюзовими акцентами.
 *
 * Встановлює CSS-стилі для всіх елементів інтерфейсу:
 * - Темний фон (#1a1a1a).
 * - Бірюзові акценти (#00CED1).
 * - Градієнти для кнопок.
 * - Стилізація таблиць, вкладок, полів введення.
 */
void DigitalSignatureWindow::applyDarkTheme() {
    QString darkTheme = R"(
        /* ========== Головне вікно ========== */
        QMainWindow {
            background: #1a1a1a;
        }

        QWidget {
            background: #1a1a1a;
            color: #e0e0e0;
        }

        /* ========== Вкладки ========== */
        QTabWidget::pane {
            border: 2px solid #00CED1;
            border-radius: 10px;
            background: #1a1a1a;
            padding: 10px;
        }

        QTabBar::tab {
            background: #2a2a2a;
            color: #e0e0e0;
            padding: 12px 25px;
            margin-right: 5px;
            border: 1px solid #3a3a3a;
            border-bottom: none;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            font-size: 13px;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00CED1, stop:1 #008B8B);
            color: #000000;
            font-weight: bold;
        }

        QTabBar::tab:hover:!selected {
            background: #3a3a3a;
            color: #00E5E8;
        }

        /* ========== Групи (QGroupBox) ========== */
        QGroupBox {
            border: 2px solid #00CED1;
            border-radius: 10px;
            margin-top: 15px;
            padding-top: 15px;
            background: #1a1a1a;
            font-size: 13px;
            font-weight: bold;
            color: #00E5E8;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 15px;
            padding: 5px 10px;
            background: #1a1a1a;
            color: #00CED1;
        }

        /* ========== Поля введення ========== */
        QLineEdit, QTextEdit, QDateEdit, QComboBox {
            background: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 8px;
            color: #e0e0e0;
            font-size: 12px;
        }

        QLineEdit:focus, QTextEdit:focus, QDateEdit:focus, QComboBox:focus {
            border-color: #00CED1;
            background: #2f2f2f;
        }

        QLineEdit:read-only {
            background: #252525;
            color: #999;
        }

        /* ========== ComboBox ========== */
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }

        QComboBox::down-arrow {
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTQgNkw4IDEwTDEyIDYiIHN0cm9rZT0iIzAwQ0VEMSIgc3Ryb2tlLXdpZHRoPSIyIiBzdHJva2UtbGluZWNhcD0icm91bmQiIHN0cm9rZS1saW5lam9pbj0icm91bmQiLz4KPC9zdmc+);
        }

        QComboBox QAbstractItemView {
            background: #2a2a2a;
            border: 2px solid #00CED1;
            selection-background-color: #00CED1;
            selection-color: #000000;
        }

        /* ========== Кнопки ========== */
        QPushButton {
            background: #3a3a3a;
            border: 2px solid #4a4a4a;
            border-radius: 8px;
            color: #e0e0e0;
            padding: 10px;
            font-size: 13px;
        }

        QPushButton:hover {
            background: #4a4a4a;
            border-color: #00CED1;
            color: #00E5E8;
        }

        QPushButton:pressed {
            background: #2a2a2a;
        }

        /* ========== Календар ========== */
        QCalendarWidget QWidget {
            background: #2a2a2a;
            color: #e0e0e0;
        }

        QCalendarWidget QAbstractItemView:enabled {
            background: #2a2a2a;
            selection-background-color: #00CED1;
            selection-color: #000000;
        }
    )";

    setStyleSheet(darkTheme);
}

/*
 * ============================================================================
 * ВІДОБРАЖЕННЯ РЕЗУЛЬТАТУ ПЕРЕВІРКИ
 * ============================================================================
 */

/**
 * @brief Відображення результату перевірки підпису.
 *
 * Показує:
 * - Результат перевірки (дійсний/підроблений).
 * - Детальну інформацію про хеші.
 * - Технічні деталі процесу.
 *
 * @param result Результат перевірки.
 */
void DigitalSignatureWindow::displayVerificationResult(const SignatureResult& result) {
    // ========== КРОК 1: Формуємо головне повідомлення ==========
    QString resultMessage;

    if (result.isValid) {
        // ДІЙСНИЙ ПІДПИС.
        resultMessage = QString(
                            "═══════════════════════════════════════\n"
                            "       ✅ ПІДПИС ДІЙСНИЙ\n"
                            "═══════════════════════════════════════\n\n"
                            "%1\n\n"
                            "Технічні деталі:\n"
                            "• Поточний хеш документа співпадає з оригінальним\n"
                            "• Підпис створений власником приватного ключа\n"
                            "• Документ не був змінений після підписання\n\n"
                            "Висновок:\n"
                            "Цей документ є автентичним і не був модифікований."
                            ).arg(result.message);

        // Встановлюємо зелений стиль.
        verify_resultEdit->setStyleSheet(
            "QTextEdit {"
            "   background: rgba(74, 222, 128, 0.1);"
            "   color: #4ade80;"
            "   border: 3px solid #22c55e;"
            "   border-radius: 8px;"
            "   padding: 15px;"
            "   font-size: 11px;"
            "   font-family: 'Courier New', monospace;"
            "   font-weight: bold;"
            "}"
            );

        // Показуємо додаткове повідомлення.
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Результат перевірки");
        msgBox.setText("✅ ПІДПИС ДІЙСНИЙ");
        msgBox.setInformativeText(
            "Документ пройшов перевірку успішно!\n\n"
            "Підпис створений власником приватного ключа,\n"
            "і документ не був змінений після підписання."
            );
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

    } else {
        // ПІДРОБЛЕНИЙ ПІДПИС.
        resultMessage = QString(
                            "═══════════════════════════════════════\n"
                            "       ❌ ПІДПИС ПІДРОБЛЕНИЙ\n"
                            "═══════════════════════════════════════\n\n"
                            "%1\n\n"
                            "Технічні деталі:\n"
                            "• Поточний хеш документа НЕ співпадає з оригінальним\n"
                            "• Документ був змінений після підписання, або\n"
                            "• Підпис не відповідає публічному ключу, або\n"
                            "• Використано неправильний ключ для перевірки\n\n"
                            "⚠ ПОПЕРЕДЖЕННЯ:\n"
                            "Не довіряйте цьому документу!\n"
                            "Він може бути підробленим або модифікованим."
                            ).arg(result.message);

        // Встановлюємо червоний стиль.
        verify_resultEdit->setStyleSheet(
            "QTextEdit {"
            "   background: rgba(248, 113, 113, 0.1);"
            "   color: #f87171;"
            "   border: 3px solid #dc2626;"
            "   border-radius: 8px;"
            "   padding: 15px;"
            "   font-size: 11px;"
            "   font-family: 'Courier New', monospace;"
            "   font-weight: bold;"
            "}"
            );

        // Показуємо попередження.
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Результат перевірки");
        msgBox.setText("❌ ПІДПИС ПІДРОБЛЕНИЙ");
        msgBox.setInformativeText(
            "⚠️ ПОПЕРЕДЖЕННЯ!\n\n"
            "Документ не пройшов перевірку.\n\n"
            "Можливі причини:\n"
            "• Документ був змінений після підписання\n"
            "• Використано неправильний публічний ключ\n"
            "• Підпис підроблено\n\n"
            "Не довіряйте цьому документу!"
            );
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    verify_resultEdit->setPlainText(resultMessage);

    // ========== КРОК 2: Відображаємо технічні деталі ==========
    verify_originalHashEdit->setText(result.originalHash);
    verify_currentHashEdit->setText(result.currentHash);
    verify_decryptedHashEdit->setText(result.decryptedHash);

    // Показуємо групу деталей.
    verify_detailsGroup->setVisible(true);

    // ========== КРОК 3: Підсвічуємо різницю в хешах ==========
    if (result.isValid) {
        // Всі хеші зелені (співпадають).
        verify_currentHashEdit->setStyleSheet(
            "QLineEdit {"
            "   background: rgba(74, 222, 128, 0.2);"
            "   color: #4ade80;"
            "   border: 2px solid #22c55e;"
            "   font-family: 'Courier New', monospace;"
            "   font-size: 10px;"
            "   font-weight: bold;"
            "}"
            );
        verify_decryptedHashEdit->setStyleSheet(
            "QLineEdit {"
            "   background: rgba(74, 222, 128, 0.2);"
            "   color: #4ade80;"
            "   border: 2px solid #22c55e;"
            "   font-family: 'Courier New', monospace;"
            "   font-size: 10px;"
            "   font-weight: bold;"
            "}"
            );
    } else {
        // Хеші червоні (не співпадають).
        verify_currentHashEdit->setStyleSheet(
            "QLineEdit {"
            "   background: rgba(248, 113, 113, 0.2);"
            "   color: #f87171;"
            "   border: 2px solid #dc2626;"
            "   font-family: 'Courier New', monospace;"
            "   font-size: 10px;"
            "   font-weight: bold;"
            "}"
            );
        verify_decryptedHashEdit->setStyleSheet(
            "QLineEdit {"
            "   background: rgba(248, 113, 113, 0.2);"
            "   color: #f87171;"
            "   border: 2px solid #dc2626;"
            "   font-family: 'Courier New', monospace;"
            "   font-size: 10px;"
            "   font-weight: bold;"
            "}"
            );
    }
}

/*
 * ============================================================================
 * ПОКАЗ ПОКРОКОВОГО АЛГОРИТМУ
 * ============================================================================
 */

/**
 * @brief Відображення детального пояснення алгоритму цифрових підписів.
 *
 * Показує:
 * - Теоретичні основи цифрових підписів.
 * - Покрокове пояснення процесу.
 * - Конкретні приклади з числами.
 * - Візуалізацію процесу підписання та перевірки.
 */
void DigitalSignatureWindow::showStepByStepAlgorithm() {
    QString algorithm = "<div style='color: #e0e0e0; font-family: Arial; font-size: 13px;'>";
    algorithm += "<h2 style='color: #00CED1;'>Покроковий алгоритм цифрових підписів</h2>";

    // ========== ВСТУП ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.15); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00CED1;'>Що таке цифровий підпис?</h3>";
    algorithm += "<p><b>Цифровий підпис</b> - це криптографічний механізм, який забезпечує:</p>";
    algorithm += "<ul>";
    algorithm += "<li><b>Автентичність:</b> Підтверджує, хто створив документ</li>";
    algorithm += "<li><b>Цілісність:</b> Гарантує, що документ не був змінений</li>";
    algorithm += "<li><b>Неспростовність:</b> Автор не може заперечити підписання</li>";
    algorithm += "</ul>";
    algorithm += "<p>Це електронний аналог власноручного підпису, але значно безпечніший!</p>";
    algorithm += "</div>";

    // ========== ЕТАП 1: Генерація ключів ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 1: Генерація пари ключів</h3>";

    algorithm += "<p><b>Крок 1.1:</b> Збираємо персональні дані</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Ім'я: \"Іван\"\n";
    algorithm += "Прізвище: \"Петренко\"\n";
    algorithm += "Дата народження: \"15031995\"\n";
    algorithm += "Секретне слово: \"MySecretWord123\"";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 1.2:</b> Генеруємо приватний ключ (SHA-256)</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Об'єднані дані: \"ІванПетренко15031995MySecretWord123\"\n";
    algorithm += "        ↓ SHA-256 хеш\n";
    algorithm += "Приватний ключ (hex): FD3EF50CF78EAD89BF7A022F207CFBE83449FBF8FB9FB02D5525D9FC956E66F4\n";
    algorithm += "Приватний ключ (число): 114791953579291264319502555278940817062856495916023447260439655857874950237940\n\n";
    algorithm += "ВАЖЛИВО: Зберігайте приватний ключ в секреті!";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 1.3:</b> Обчислюємо публічний ключ (модульна арифметика + хешування)</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Крок 1: Обчислюємо числове значення\n";
    algorithm += "Формула: publicKeyNum = (privateKey × 7) mod 1000007\n\n";
    algorithm += "privateKey = 114791953579291264319502555278940817062856495916023447260439655857874950237940\n";
    algorithm += "        ↓\n";
    algorithm += "(privateKey × 7) mod 1000007 = 245594\n\n";
    algorithm += "Крок 2: Генеруємо hex-ключ з числового значення\n";
    algorithm += "publicKeyNum = 245594\n";
    algorithm += "        ↓ SHA-256 хеш від числа\n";
    algorithm += "Публічний ключ (hex): C1E8E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5E5C5\n";
    algorithm += "                      (перші 64 символи)\n\n";
    algorithm += "Тепер приватний і публічний ключі мають однакову довжину\n";
    algorithm += "   для коректної роботи XOR операцій!";
    algorithm += "</pre>";

    algorithm += "<p><b>Математична основа:</b></p>";
    algorithm += "<ul>";
    algorithm += "<li>7 - множник (просте число)</li>";
    algorithm += "<li>1000007 - модуль (велике просте число)</li>";
    algorithm += "<li>З публічного ключа важко відновити приватний (однонаправлена функція)</li>";
    algorithm += "<li>У реальних системах (RSA) використовуються набагато більші числа</li>";
    algorithm += "</ul>";
    algorithm += "</div>";

    // ========== ЕТАП 2: Створення підпису ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.08); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 2: Створення цифрового підпису</h3>";

    algorithm += "<p><b>Крок 2.1:</b> Обчислюємо хеш документа</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Документ: \"Договір купівлі-продажу на 1000 грн\"\n";
    algorithm += "        ↓ SHA-256\n";
    algorithm += "Хеш: A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1E3F5A7B9C1D3E5F7A9B1C3D5E7F9A1B3\n\n";
    algorithm += "Будь-яка зміна документа → інший хеш!";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 2.2:</b> \"Шифруємо\" хеш приватним ключем</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "У реальній RSA: signature = hash^d mod n\n";
    algorithm += "У нашій спрощеній версії: signature = hash XOR privateKey\n\n";
    algorithm += "Хеш документа:    A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1E3F5A7B9C1D3E5F7A9B1C3D5E7F9A1B3\n";
    algorithm += "Приватний ключ:   FD3EF50CF78EAD89BF7A022F207CFBE83449FBF8FB9FB02D5525D9FC956E66F4\n";
    algorithm += "        ↓ XOR\n";
    algorithm += "Підпис:           5A8D3CDD127C053D79A2E3DC8528B43FD77C5C413A4A1F22FC94149E72976747\n\n";
    algorithm += "Це і є цифровий підпис!";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 2.3:</b> Передаємо документ + підпис</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Документ: contract.pdf\n";
    algorithm += "Підпис: contract.sig\n";
    algorithm += "Публічний ключ: public_key.txt\n\n";
    algorithm += "Отримувач може перевірити підпис за допомогою публічного ключа.";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== ЕТАП 3: Перевірка підпису ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 3: Перевірка цифрового підпису</h3>";

    algorithm += "<p><b>Крок 3.1:</b> Обчислюємо хеш отриманого документа</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Отриманий документ: \"Договір купівлі-продажу на 1000 грн\"\n";
    algorithm += "        ↓ SHA-256\n";
    algorithm += "Поточний хеш: A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1E3F5A7B9C1D3E5F7A9B1C3D5E7F9A1B3";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 3.2:</b> \"Розшифровуємо\" підпис публічним ключем</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "У реальній RSA: hash = signature^e mod n\n";
    algorithm += "У нашій версії: hash = signature XOR publicKey\n\n";
    algorithm += "Підпис:          5A8D3CDD127C053D79A2E3DC8528B43FD77C5C413A4A1F22FC94149E72976747\n";
    algorithm += "Публічний ключ:  FD3EF50CF78EAD89BF7A022F207CFBE83449FBF8FB9FB02D5525D9FC956E66F4\n";
    algorithm += "        ↓ XOR\n";
    algorithm += "Оригінальний хеш: A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1E3F5A7B9C1D3E5F7A9B1C3D5E7F9A1B3";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 3.3:</b> Порівнюємо хеші</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Поточний хеш:     A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1E3F5A7B9C1D3E5F7A9B1C3D5E7F9A1B3\n";
    algorithm += "Оригінальний хеш: A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1E3F5A7B9C1D3E5F7A9B1C3D5E7F9A1B3\n";
    algorithm += "        ↓\n";
    algorithm += "Співпадають? <span style='color: #4ade80;'>ТАК</span>\n\n";
    algorithm += "Результат: <span style='color: #4ade80;'><b>ПІДПИС ДІЙСНИЙ</b></span>";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== ЕТАП 4: Виявлення підробки ==========
    algorithm += "<div style='background: rgba(220, 38, 38, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #f87171;'>Етап 4: Виявлення підробки</h3>";

    algorithm += "<p><b>Сценарій 1:</b> Документ був змінений</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Оригінал: \"Договір на 1000 грн\"\n";
    algorithm += "Підробка: \"Договір на <span style='color: #f87171;'>10000</span> грн\" ← Змінено!\n\n";
    algorithm += "Хеш оригіналу:  A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1E3F5A7B9C1D3E5F7A9B1C3D5E7F9A1B3\n";
    algorithm += "Хеш підробки:   <span style='color: #f87171;'>3E8F7A2C9B1D4F6E8A2C5D7F9B1E4A6C8E1F3B5D7A9C2E4F6B8D1A3C5E7F9B2</span> ← Інший!\n\n";
    algorithm += "Результат: <span style='color: #f87171;'><b>ПІДПИС ПІДРОБЛЕНИЙ</b></span>";
    algorithm += "</pre>";

    algorithm += "<p><b>Сценарій 2:</b> Неправильний ключ</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Використано публічний ключ іншої особи\n";
    algorithm += "        ↓\n";
    algorithm += "Розшифрований хеш не співпадає з поточним\n";
    algorithm += "        ↓\n";
    algorithm += "Результат: <span style='color: #f87171;'><b>ПІДПИС ПІДРОБЛЕНИЙ</b></span>";
    algorithm += "</pre>";

    algorithm += "<p><b>Чому це працює?</b></p>";
    algorithm += "<ul>";
    algorithm += "<li>Тільки власник приватного ключа може створити правильний підпис</li>";
    algorithm += "<li>Будь-яка зміна документа змінює хеш</li>";
    algorithm += "<li>Неможливо підробити підпис без приватного ключа</li>";
    algorithm += "<li>Публічний ключ дозволяє перевірити, але НЕ створити підпис</li>";
    algorithm += "</ul>";
    algorithm += "</div>";

    // ========== Конкретний приклад ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.15); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00CED1;'>Конкретний приклад: Підпис договору</h3>";

    algorithm += "<p><b>Ситуація:</b> Іван підписує договір з Петром</p>";

    algorithm += "<p><b>1. Іван створює підпис:</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Документ: \"Договір між Іваном і Петром на 1000 грн\"\n";
    algorithm += "Приватний ключ Івана: FD3EF50CF78EAD89BF7A022F207CFBE8...\n";
    algorithm += "        ↓\n";
    algorithm += "Хеш документа: A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1...\n";
    algorithm += "        ↓ Шифрування приватним ключем\n";
    algorithm += "Підпис: 5A8D3CDD127C053D79A2E3DC8528B43F...";
    algorithm += "</pre>";

    algorithm += "<p><b>2. Іван відправляє Петру:</b></p>";
    algorithm += "<ul>";
    algorithm += "<li>Договір (contract.pdf)</li>";
    algorithm += "<li>Підпис (contract.sig)</li>";
    algorithm += "<li>Публічний ключ (ivan_public.txt)</li>";
    algorithm += "</ul>";

    algorithm += "<p><b>3. Петро перевіряє:</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Хеш отриманого документа: A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1...\n";
    algorithm += "Розшифрований підпис (публічним ключем): A7B3C9D1E5F2A8B4C6D8E1F3A5B7C9D1...\n";
    algorithm += "        ↓\n";
    algorithm += "Співпадають! ✅\n\n";
    algorithm += "Петро впевнений, що:\n";
    algorithm += "• Договір підписав саме Іван (автентичність)\n";
    algorithm += "• Договір не змінювався (цілісність)\n";
    algorithm += "• Іван не може заперечити підпис (неспростовність)";
    algorithm += "</pre>";

    algorithm += "<p><b>4. Спроба підробки:</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Якщо хтось змінить \"1000 грн\" на \"10000 грн\":\n";
    algorithm += "        ↓\n";
    algorithm += "Новий хеш: <span style='color: #f87171;'>3E8F7A2C9B1D4F6E...</span> (інший!)\n";
    algorithm += "Підпис залишився: 5A8D3CDD127C053D...\n";
    algorithm += "        ↓\n";
    algorithm += "Хеші не співпадають ❌\n\n";
    algorithm += "Підробка виявлена!";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== Порівняння з RSA ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.08); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Порівняння: наша реалізація vs RSA</h3>";

    algorithm += "<table style='border-collapse: collapse; width: 100%; margin: 10px 0;'>";
    algorithm += "<tr style='background: rgba(0, 206, 209, 0.2);'>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>Аспект</th>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>Наша реалізація</th>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>RSA (реальна система)</th>";
    algorithm += "</tr>";

    algorithm += "<tr style='background: rgba(0, 206, 209, 0.05);'>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'><b>Приватний ключ</b></td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>SHA-256 хеш даних</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>Випадкове просте число (d)</td>";
    algorithm += "</tr>";

    algorithm += "<tr>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'><b>Публічний ключ</b></td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>(privateKey × 7) mod 1000007</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>Пара чисел (n, e)</td>";
    algorithm += "</tr>";

    algorithm += "<tr style='background: rgba(0, 206, 209, 0.05);'>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'><b>Шифрування</b></td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>XOR операція</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>hash^d mod n</td>";
    algorithm += "</tr>";

    algorithm += "<tr>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'><b>Розшифрування</b></td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>XOR операція</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>signature^e mod n</td>";
    algorithm += "</tr>";

    algorithm += "<tr style='background: rgba(0, 206, 209, 0.05);'>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'><b>Безпека</b></td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>Навчальна (спрощена)</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>Криптографічно стійка</td>";
    algorithm += "</tr>";

    algorithm += "<tr>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'><b>Розмір ключа</b></td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>256 біт</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>2048-4096 біт</td>";
    algorithm += "</tr>";

    algorithm += "</table>";

    algorithm += "<p><b>Висновок:</b> Наша реалізація демонструє ПРИНЦИПИ роботи, "
                 "але для реального використання потрібна повноцінна криптографічна система (RSA, ECDSA).</p>";
    algorithm += "</div>";

    // ========== Висновки ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.12); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00CED1;'>Висновки</h3>";
    algorithm += "<ul>";
    algorithm += "<li><b>Асиметричне шифрування:</b> Два різні ключі (приватний + публічний)</li>";
    algorithm += "<li><b>Безпека:</b> Тільки власник приватного ключа може створити підпис</li>";
    algorithm += "<li><b>Перевірка:</b> Будь-хто з публічним ключем може перевірити підпис</li>";
    algorithm += "<li><b>Цілісність:</b> Будь-яка зміна документа → недійсний підпис</li>";
    algorithm += "<li><b>Математична основа:</b> Модульна арифметика та однонаправлені функції</li>";
    algorithm += "<li><b>Застосування:</b> Електронні договори, сертифікати, blockchain</li>";
    algorithm += "</ul>";
    algorithm += "</div>";

    algorithm += "</div>";

    // Встановлюємо HTML-контент.
    algorithm_textEdit->setHtml(algorithm);
}
