/*
 * SteganographyWindow_Helpers.cpp
 *
 * Файл з допоміжними методами головного вікна.
 * Містить функції відображення результатів, теми оформлення та алгоритму.
 */

#include "SteganographyWindow.h"
#include <QTableWidget>
#include <QTableWidgetItem>

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
void SteganographyWindow::applyDarkTheme() {
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
        QLineEdit, QTextEdit, QDateEdit {
            background: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 8px;
            color: #e0e0e0;
            font-size: 12px;
        }

        QLineEdit:focus, QTextEdit:focus, QDateEdit:focus {
            border-color: #00CED1;
            background: #2f2f2f;
        }

        QLineEdit:read-only {
            background: #252525;
            color: #999;
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

        /* ========== Таблиці ========== */
        QTableWidget {
            background: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            gridline-color: #3a3a3a;
            color: #e0e0e0;
        }

        QTableWidget::item {
            padding: 8px;
        }

        QTableWidget::item:selected {
            background: rgba(0, 206, 209, 0.3);
        }

        QHeaderView::section {
            background: #3a3a3a;
            color: #00E5E8;
            padding: 8px;
            border: 1px solid #4a4a4a;
            font-weight: bold;
        }

        /* ========== Чекбокси ========== */
        QCheckBox {
            spacing: 8px;
        }

        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border: 2px solid #3a3a3a;
            border-radius: 4px;
            background: #2a2a2a;
        }

        QCheckBox::indicator:checked {
            background: #00CED1;
            border-color: #00CED1;
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEzLjMzMzMgNEw2IDExLjMzMzNMMi42NjY2NyA4IiBzdHJva2U9IiMxYTFhMWEiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLWxpbmVjYXA9InJvdW5kIiBzdHJva2UtbGluZWpvaW49InJvdW5kIi8+Cjwvc3ZnPg==);
        }

        /* ========== Прогрес-бар ========== */
        QProgressBar {
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            text-align: center;
            background: #2a2a2a;
            color: #e0e0e0;
            font-weight: bold;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00CED1, stop:1 #009999);
            border-radius: 6px;
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
 * ВІДОБРАЖЕННЯ РЕЗУЛЬТАТІВ АНАЛІЗУ
 * ============================================================================
 */

/**
 * @brief Відображення результатів аналізу в таблиці та текстовому полі.
 *
 * Заповнює:
 * 1. Таблицю порівняльних даних (8 рядків × 3 колонки).
 * 2. Детальний HTML-звіт з поясненнями.
 *
 * @param result Структура з результатами аналізу.
 */
void SteganographyWindow::displayAnalysisResults(const ImageAnalysisResult& result) {
    // ========== ЗАПОВНЕННЯ ТАБЛИЦІ ==========

    // Рядок 0: Розмір (пікселі).
    analysisTable->setItem(0, 0, new QTableWidgetItem("Розмір (пікселі)"));
    analysisTable->setItem(0, 1, new QTableWidgetItem(
                                     QString("%1 x %2").arg(result.originalWidth).arg(result.originalHeight)
                                     ));
    analysisTable->setItem(0, 2, new QTableWidgetItem(
                                     QString("%1 x %2").arg(result.modifiedWidth).arg(result.modifiedHeight)
                                     ));

    // Рядок 1: Розмір файлу.
    analysisTable->setItem(1, 0, new QTableWidgetItem("Розмір файлу"));
    analysisTable->setItem(1, 1, new QTableWidgetItem(
                                     QString("%1 КБ").arg(result.originalFileSize / 1024.0, 0, 'f', 2)
                                     ));
    analysisTable->setItem(1, 2, new QTableWidgetItem(
                                     QString("%1 КБ").arg(result.modifiedFileSize / 1024.0, 0, 'f', 2)
                                     ));

    // Рядок 2: Довжина повідомлення.
    analysisTable->setItem(2, 0, new QTableWidgetItem("Довжина повідомлення"));
    analysisTable->setItem(2, 1, new QTableWidgetItem("-"));
    analysisTable->setItem(2, 2, new QTableWidgetItem(
                                     QString("%1 символів").arg(result.messageLength)
                                     ));

    // Рядок 3: Використана ємність.
    analysisTable->setItem(3, 0, new QTableWidgetItem("Використана ємність"));
    analysisTable->setItem(3, 1, new QTableWidgetItem("-"));
    analysisTable->setItem(3, 2, new QTableWidgetItem(
                                     QString("%1% (%2/%3)")
                                         .arg(result.capacityUsed, 0, 'f', 2)
                                         .arg(result.messageLength)
                                         .arg(result.maxCapacity, 0, 'f', 0)
                                     ));

    // Рядок 4: PSNR (якість).
    analysisTable->setItem(4, 0, new QTableWidgetItem("PSNR (якість)"));
    analysisTable->setItem(4, 1, new QTableWidgetItem("-"));
    analysisTable->setItem(4, 2, new QTableWidgetItem(
                                     QString("%1 dB").arg(result.psnr, 0, 'f', 2)
                                     ));

    // Рядок 5: MSE (помилка).
    analysisTable->setItem(5, 0, new QTableWidgetItem("MSE (помилка)"));
    analysisTable->setItem(5, 1, new QTableWidgetItem("-"));
    analysisTable->setItem(5, 2, new QTableWidgetItem(
                                     QString("%1").arg(result.mse, 0, 'f', 4)
                                     ));

    // Рядок 6: Змінених пікселів.
    analysisTable->setItem(6, 0, new QTableWidgetItem("Змінених пікселів"));
    analysisTable->setItem(6, 1, new QTableWidgetItem("-"));
    analysisTable->setItem(6, 2, new QTableWidgetItem(
                                     QString("%1").arg(result.modifiedPixels)
                                     ));

    // Рядок 7: Візуальна різниця.
    analysisTable->setItem(7, 0, new QTableWidgetItem("Візуальна різниця"));
    analysisTable->setItem(7, 1, new QTableWidgetItem("-"));
    QString visualDiff = result.psnr > 40 ? "Непомітна ✅" :
                             result.psnr > 30 ? "Мінімальна" : "Помітна";
    analysisTable->setItem(7, 2, new QTableWidgetItem(visualDiff));

    // ========== ФОРМУВАННЯ ДЕТАЛЬНОГО HTML-ЗВІТУ ==========
    QString details = "<div style='color: #e0e0e0; font-family: Arial;'>";
    details += "<h3 style='color: #00CED1;'>Детальний аналіз стеганографії</h3>";

    // ========== Блок 1: Загальна інформація ==========
    details += "<div style='background: rgba(0, 206, 209, 0.1); "
               "padding: 15px; border-radius: 8px; margin: 10px 0;'>";
    details += "<h4 style='color: #00E5E8;'>Загальна інформація</h4>";
    details += QString(
                   "<p><b>Розмір зображення:</b> %1 x %2 пікселів (%3 пікселів загалом)</p>"
                   ).arg(result.originalWidth)
                   .arg(result.originalHeight)
                   .arg(result.originalWidth * result.originalHeight);

    details += QString("<p><b>Довжина повідомлення:</b> %1 символів</p>")
                   .arg(result.messageLength);

    details += QString(
                   "<p><b>Використано бітів:</b> %1 / %2 (з 3 бітів на піксель)</p>"
                   ).arg(result.bitsUsed)
                   .arg(result.originalWidth * result.originalHeight * 3);
    details += "</div>";

    // ========== Блок 2: Якість стеганографії ==========
    details += "<div style='background: rgba(0, 206, 209, 0.08); "
               "padding: 15px; border-radius: 8px; margin: 10px 0;'>";
    details += "<h4 style='color: #00E5E8;'>Якість стеганографії</h4>";
    details += QString("<p><b>PSNR (Peak Signal-to-Noise Ratio):</b> %1 dB</p>")
                   .arg(result.psnr, 0, 'f', 2);

    // Інтерпретація PSNR
    if (result.psnr > 40) {
        details += "<p style='color: #4ade80;'>"
                   "✅ Відмінна якість! Зміни практично непомітні для людського ока."
                   "</p>";
    } else if (result.psnr > 30) {
        details += "<p style='color: #fbbf24;'>"
                   "⚠️ Добра якість. Зміни мінімальні."
                   "</p>";
    } else {
        details += "<p style='color: #f87171;'>"
                   "❌ Низька якість. Зміни можуть бути помітні."
                   "</p>";
    }

    details += QString("<p><b>MSE (Mean Squared Error):</b> %1</p>")
                   .arg(result.mse, 0, 'f', 4);

    details += QString("<p><b>Змінених пікселів:</b> %1 (%2%)</p>")
                   .arg(result.modifiedPixels)
                   .arg((result.modifiedPixels * 100.0) /
                            (result.originalWidth * result.originalHeight), 0, 'f', 2);
    details += "</div>";

    // ========== Блок 3: Використання ємності ==========
    details += "<div style='background: rgba(0, 206, 209, 0.1); "
               "padding: 15px; border-radius: 8px; margin: 10px 0;'>";
    details += "<h4 style='color: #00E5E8;'>Використання ємності</h4>";
    details += QString("<p><b>Максимальна ємність:</b> %1 символів</p>")
                   .arg(result.maxCapacity, 0, 'f', 0);

    details += QString("<p><b>Використано:</b> %1% ємності</p>")
                   .arg(result.capacityUsed, 0, 'f', 2);

    // Інтерпретація використання ємності.
    if (result.capacityUsed < 50) {
        details += "<p style='color: #4ade80;'>"
                   "✅ Низьке використання ємності. Можна приховати більше даних."
                   "</p>";
    } else if (result.capacityUsed < 80) {
        details += "<p style='color: #fbbf24;'>"
                   "⚠️ Середнє використання ємності."
                   "</p>";
    } else {
        details += "<p style='color: #f87171;'>"
                   "❌ Високе використання ємності!"
                   "</p>";
    }
    details += "</div>";

    // ========== Блок 4: Пояснення LSB методу ==========
    details += "<div style='background: rgba(0, 206, 209, 0.15); "
               "padding: 15px; border-radius: 8px; margin: 10px 0;'>";
    details += "<h4 style='color: #00CED1;'>Чому використовуються молодші біти?</h4>";
    details += "<p><b>Метод LSB (Least Significant Bit):</b></p>";
    details += "<ul>";
    details += "<li>Кожен колірний канал (R, G, B) має значення від 0 до 255 (8 біт)</li>";
    details += "<li>Молодший біт (LSB) має найменший вплив на значення кольору</li>";
    details += "<li>Зміна молодшого біту змінює значення кольору максимум на 1</li>";
    details += "<li>Така зміна непомітна для людського ока "
               "(наприклад, RGB(150,200,75) → RGB(151,201,74))</li>";
    details += "<li>Це дозволяє приховувати дані без помітної деградації зображення</li>";
    details += "</ul>";
    details += "</div>";

    // ========== Блок 5: Висновки ==========
    details += "<div style='background: rgba(0, 206, 209, 0.12); "
               "padding: 15px; border-radius: 8px; margin: 10px 0;'>";
    details += "<h4 style='color: #00CED1;'>Висновки</h4>";
    details += "<ul>";
    details += QString("<li><b>Зображення візуально не змінилося:</b> %1</li>")
                   .arg(result.psnr > 40 ? "Так ✅" : "Є незначні відмінності ⚠️");

    details += QString("<li><b>Повідомлення успішно приховано:</b> %1 символів</li>")
                   .arg(result.messageLength);

    details += QString("<li><b>Стійкість до виявлення:</b> %1</li>")
                   .arg(result.capacityUsed < 50 ? "Висока ✅" : "Середня ⚠️");
    details += "</ul>";
    details += "</div>";

    details += "</div>";

    // Встановлюємо HTML-контент.
    analysisDetailsEdit->setHtml(details);
}

/*
 * ============================================================================
 * ПОКАЗ ПОКРОКОВОГО АЛГОРИТМУ
 * ============================================================================
 */

/**
 * @brief Відображення детального пояснення алгоритму LSB.
 *
 * Показує:
 * - Теоретичні основи стеганографії.
 * - Покрокове пояснення методу LSB.
 * - Конкретні приклади з числами.
 * - Візуалізацію процесу приховування та витягування.
 */
void SteganographyWindow::showStepByStepAlgorithm() {
    QString algorithm = "<div style='color: #e0e0e0; font-family: Arial; font-size: 13px;'>";
    algorithm += "<h2 style='color: #00CED1;'>Покроковий алгоритм стеганографії LSB</h2>";

    // ========== ЕТАП 1: Підготовка повідомлення ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 1: Підготовка повідомлення</h3>";
    algorithm += "<p><b>Крок 1.1:</b> Додаємо маркери початку та кінця</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Оригінальне повідомлення: \"Привіт\"\n";
    algorithm += "З маркерами: \"&lt;&lt;START&gt;&gt;Привіт&lt;&lt;END&gt;&gt;\"";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 1.2:</b> Конвертуємо текст у двійковий формат (UTF-16)</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Символ 'П' (Unicode: 1055) → 0000010000011111\n";
    algorithm += "Символ 'р' (Unicode: 1088) → 0000010001000000\n";
    algorithm += "Символ 'и' (Unicode: 1080) → 0000010000111000\n";
    algorithm += "...та інші символи";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== ЕТАП 2: Читання пікселів ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.08); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 2: Читання пікселів зображення</h3>";
    algorithm += "<p><b>Крок 2.1:</b> Зчитуємо піксель з зображення</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Приклад піксела (x=0, y=0):\n";
    algorithm += "Червоний канал (R): 150 → 10010110 в двійковому\n";
    algorithm += "Зелений канал (G): 200 → 11001000 в двійковому\n";
    algorithm += "Синій канал (B):   75  → 01001011 в двійковому\n";
    algorithm += "                            ↑\n";
    algorithm += "                     Молодший біт (LSB)";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== ЕТАП 3: Модифікація молодших бітів ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 3: Модифікація молодших бітів</h3>";
    algorithm += "<p><b>Чому молодші біти?</b></p>";
    algorithm += "<ul>";
    algorithm += "<li>Молодший біт має найменший вплив на значення кольору</li>";
    algorithm += "<li>Зміна LSB змінює колір максимум на ±1</li>";
    algorithm += "<li>Така зміна абсолютно непомітна для людського ока</li>";
    algorithm += "</ul>";

    algorithm += "<p><b>Крок 3.1:</b> Заміна молодших бітів бітами повідомлення</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Оригінальні значення пікселя:\n";
    algorithm += "R: 150 (1001011<span style='color: #f87171;'>0</span>)  "
                 "G: 200 (1100100<span style='color: #f87171;'>0</span>)  "
                 "B: 75 (0100101<span style='color: #f87171;'>1</span>)\n\n";
    algorithm += "Біти повідомлення для приховування: 0, 0, 1\n\n";
    algorithm += "Модифіковані значення:\n";
    algorithm += "R: 150 (1001011<span style='color: #4ade80;'>0</span>)  "
                 "G: 200 (1100100<span style='color: #4ade80;'>0</span>)  "
                 "B: 75 (0100101<span style='color: #4ade80;'>1</span>)\n\n";
    algorithm += "Результат: R=150, G=200, B=75 (без змін або зміна ±1)";
    algorithm += "</pre>";

    algorithm += "<p><b>Приклад змін:</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "RGB(150, 200, 75)  →  RGB(151, 201, 74)\n";
    algorithm += "                      ↓    ↓    ↓\n";
    algorithm += "                     +1   +1   -1\n\n";
    algorithm += "Ця зміна абсолютно непомітна!";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== ЕТАП 4: Збереження ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.08); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 4: Збереження модифікованого зображення</h3>";
    algorithm += "<p><b>Крок 4.1:</b> Зберігаємо зображення в форматі PNG</p>";
    algorithm += "<ul>";
    algorithm += "<li><b>Чому PNG?</b> Формат без втрат якості (lossless)</li>";
    algorithm += "<li><b>Не використовуємо JPEG!</b> JPEG використовує компресію з втратами, "
                 "що знищить приховані дані</li>";
    algorithm += "<li>PNG зберігає кожен піксель точно, включаючи модифіковані LSB</li>";
    algorithm += "</ul>";
    algorithm += "</div>";

    // ========== ЕТАП 5: Витягування ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.1); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Етап 5: Витягування повідомлення (зворотний процес)</h3>";
    algorithm += "<p><b>Крок 5.1:</b> Зчитуємо молодші біти з пікселів</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Піксель (0,0): R=151, G=201, B=74\n";
    algorithm += "               1001011<span style='color: #4ade80;'>1</span>  "
                 "1100100<span style='color: #4ade80;'>1</span>  "
                 "0100101<span style='color: #4ade80;'>0</span>\n";
    algorithm += "                      ↓        ↓        ↓\n";
    algorithm += "Витягнуті біти: 1, 1, 0";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 5.2:</b> Групуємо біти по 16 (один символ UTF-16)</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Біти: 0000010000011111...\n";
    algorithm += "      ↓\n";
    algorithm += "0000010000011111 → 1055 (Unicode) → 'П'\n";
    algorithm += "0000010001000000 → 1088 (Unicode) → 'р'\n";
    algorithm += "...та інші";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 5.3:</b> Шукаємо маркери та витягуємо текст</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Знайдено: \"&lt;&lt;START&gt;&gt;Привіт&lt;&lt;END&gt;&gt;...\"\n";
    algorithm += "           ↓\n";
    algorithm += "Витягнуте повідомлення: \"Привіт\"";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== Конкретний приклад ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.15); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00CED1;'>Конкретний приклад: Літера 'A'</h3>";
    algorithm += "<p><b>Текст для приховування:</b> \"A\"</p>";

    algorithm += "<p><b>Крок 1:</b> Конвертуємо 'A' в двійковий формат</p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "'A' → Unicode 65 → 0000000001000001 (16 біт)";
    algorithm += "</pre>";

    algorithm += "<p><b>Крок 2:</b> Приховуємо в 6 пікселях (3 канали × 6 = 18 біт, потрібно 16)</p>";
    algorithm += "<table style='border-collapse: collapse; width: 100%; margin: 10px 0;'>";
    algorithm += "<tr style='background: rgba(0, 206, 209, 0.2);'>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>Піксель</th>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>Канал</th>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>До</th>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>Біт</th>";
    algorithm += "<th style='padding: 8px; border: 1px solid #555;'>Після</th>";
    algorithm += "</tr>";
    algorithm += "<tr style='background: rgba(0, 206, 209, 0.05);'>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>0</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>R</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>150 (10010110)</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555; color: #4ade80;'>0</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>150 (10010110)</td>";
    algorithm += "</tr>";
    algorithm += "<tr>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>0</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>G</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>200 (11001000)</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555; color: #4ade80;'>0</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>200 (11001000)</td>";
    algorithm += "</tr>";
    algorithm += "<tr style='background: rgba(0, 206, 209, 0.05);'>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>0</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>B</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>75 (01001011)</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555; color: #4ade80;'>0</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>74 (01001010)</td>";
    algorithm += "</tr>";
    algorithm += "<tr>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>1</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>R</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>180 (10110100)</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555; color: #4ade80;'>0</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>180 (10110100)</td>";
    algorithm += "</tr>";
    algorithm += "<tr style='background: rgba(0, 206, 209, 0.05);'>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>...</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>...</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>...</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>...</td>";
    algorithm += "<td style='padding: 8px; border: 1px solid #555;'>...</td>";
    algorithm += "</tr>";
    algorithm += "</table>";
    algorithm += "<p style='color: #4ade80;'>Результат: Літера 'A' успішно прихована у молодших бітах!</p>";
    algorithm += "</div>";

    // ========== Додаткове шифрування ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.08); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00E5E8;'>Додатково: Шифрування перед приховуванням</h3>";
    algorithm += "<p>Для підвищення безпеки можна зашифрувати повідомлення перед приховуванням:</p>";
    algorithm += "<ol>";
    algorithm += "<li>Генеруємо ключ на основі персональних даних (SHA-256)</li>";
    algorithm += "<li>Шифруємо текст за допомогою XOR-шифру</li>";
    algorithm += "<li>Приховуємо зашифрований текст у зображенні</li>";
    algorithm += "<li>При витягуванні: спочатку витягуємо, потім розшифровуємо</li>";
    algorithm += "</ol>";
    algorithm += "<p><b>Приклад генерації ключа:</b></p>";
    algorithm += "<pre style='background: #2a2a2a; padding: 10px; border-radius: 5px;'>";
    algorithm += "Ім'я: \"Іван\"\n";
    algorithm += "Прізвище: \"Петренко\"\n";
    algorithm += "Дата: \"15031995\"\n";
    algorithm += "Телефон: \"+380123456789\"\n";
    algorithm += "        ↓ SHA-256\n";
    algorithm += "Ключ: \"A20B2BA0E3715320\"";
    algorithm += "</pre>";
    algorithm += "</div>";

    // ========== Висновки ==========
    algorithm += "<div style='background: rgba(0, 206, 209, 0.12); "
                 "padding: 20px; border-radius: 10px; margin: 15px 0;'>";
    algorithm += "<h3 style='color: #00CED1;'>Висновки</h3>";
    algorithm += "<ul>";
    algorithm += "<li><b>Непомітність:</b> Зміни молодших бітів не впливають на візуальне сприйняття</li>";
    algorithm += "<li><b>Ємність:</b> Можна приховати великі обсяги даних</li>";
    algorithm += "<li><b>Простота:</b> Алгоритм легко реалізувати</li>";
    algorithm += "<li><b>Формат:</b> Необхідно використовувати PNG (lossless формат)</li>";
    algorithm += "<li><b>Безпека:</b> Можна додатково шифрувати повідомлення</li>";
    algorithm += "</ul>";
    algorithm += "</div>";

    algorithm += "</div>";

    // Встановлюємо HTML-контент.
    algorithmEdit->setHtml(algorithm);
}
