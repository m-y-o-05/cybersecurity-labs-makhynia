/*
 * EmailEncryptorWindow_UI.cpp
 *
 * Файл з методами створення UI вкладок.
 * Містить реалізацію всіх 4 вкладок програми.
 */

#include "EmailEncryptorWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>
#include <QApplication>

/*
 * ============================================================================
 * ВКЛАДКА 1: ГЕНЕРАЦІЯ КЛЮЧА
 * ============================================================================
 */

/**
 * @brief Створення вкладки генерації ключа шифрування.
 *
 * Структура вкладки (вертикальний layout):
 * 1. Блок персональних даних (email, ім'я, прізвище, дата народження).
 * 2. Інформаційне повідомлення про алгоритм SHA-256.
 * 3. Кнопки дій (Генерувати, Приклад, Очистити).
 * 4. Поле відображення згенерованого ключа.
 * 5. Кнопки для роботи з ключем (Зберегти, Копіювати).
 *
 * Користувацький сценарій:
 * 1. Користувач вводить персональні дані.
 * 2. Натискає "Генерувати ключ".
 * 3. Отримує SHA-256 ключ у hex-форматі.
 * 4. Може зберегти ключ у файл або скопіювати.
 *
 * @return Віджет вкладки з усіма елементами.
 */
QWidget* EmailEncryptorWindow::createKeyTab() {
    // Створюємо головний віджет вкладки.
    QWidget* keyTab = new QWidget();

    // Налаштовуємо вертикальний layout.
    QVBoxLayout* mainLayout = new QVBoxLayout(keyTab);
    mainLayout->setSpacing(15);

    // Стиль для міток полів.
    QString labelStyle = "QLabel { color: #00E5E8; font-weight: bold; }";

    // ========== БЛОК 1: Персональні дані ==========
    // Група для введення даних користувача (email, ім'я, прізвище, дата).
    QGroupBox* personalGroup = new QGroupBox("Персональні дані (для генерації ключа)", this);
    QGridLayout* personalLayout = new QGridLayout(personalGroup);

    // Поле Email.
    QLabel* emailLabel = new QLabel("Email:", this);
    emailLabel->setStyleSheet(labelStyle);
    key_emailEdit = new QLineEdit(this);
    key_emailEdit->setPlaceholderText("ivan.petrenko@gmail.com");

    // Поле Ім'я.
    QLabel* firstNameLabel = new QLabel("Ім'я:", this);
    firstNameLabel->setStyleSheet(labelStyle);
    key_firstNameEdit = new QLineEdit(this);
    key_firstNameEdit->setPlaceholderText("Іван");

    // Поле Прізвище.
    QLabel* lastNameLabel = new QLabel("Прізвище:", this);
    lastNameLabel->setStyleSheet(labelStyle);
    key_lastNameEdit = new QLineEdit(this);
    key_lastNameEdit->setPlaceholderText("Петренко");

    // Поле Дата народження.
    QLabel* birthDateLabel = new QLabel("Дата народження:", this);
    birthDateLabel->setStyleSheet(labelStyle);
    key_birthDateEdit = new QDateEdit(this);
    key_birthDateEdit->setDate(QDate(1995, 1, 1));           // Початкова дата.
    key_birthDateEdit->setDisplayFormat("dd.MM.yyyy");        // Формат відображення.
    key_birthDateEdit->setCalendarPopup(true);                // Спливаючий календар.
    key_birthDateEdit->setMinimumDate(QDate(1900, 1, 1));    // Мінімальна дата.
    key_birthDateEdit->setMaximumDate(QDate::currentDate()); // Максимальна дата (сьогодні).

    // Розміщення елементів у сітці.
    // Рядок 0: Email.
    personalLayout->addWidget(emailLabel, 0, 0);
    personalLayout->addWidget(key_emailEdit, 0, 1, 1, 3);

    // Рядок 1: Ім'я та Прізвище.
    personalLayout->addWidget(firstNameLabel, 1, 0);
    personalLayout->addWidget(key_firstNameEdit, 1, 1);
    personalLayout->addWidget(lastNameLabel, 1, 2);
    personalLayout->addWidget(key_lastNameEdit, 1, 3);

    // Рядок 2: Дата народження.
    personalLayout->addWidget(birthDateLabel, 2, 0);
    personalLayout->addWidget(key_birthDateEdit, 2, 1);

    mainLayout->addWidget(personalGroup);

    // ========== БЛОК 2: Інформаційне повідомлення ==========
    // Жовта інформаційна панель з поясненням алгоритму.
    QLabel* infoLabel = new QLabel(
        "Ключ генерується на основі SHA-256 хешу від ваших персональних даних.\n"
        "Формула: SHA256(Ім'я + Прізвище + ДатаНародження)\n\n"
        "Приклад: SHA256(\"ІванПетренко15031995\") = ключ шифрування", this);

    // Стиль: жовтий текст на напівпрозорому жовтому фоні.
    infoLabel->setStyleSheet(
        "QLabel { color: #fbbf24; background: rgba(251, 191, 36, 0.1); "
        "padding: 12px; border-radius: 8px; font-size: 12px; }");
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    // ========== БЛОК 3: Кнопки дій ==========
    // Горизонтальний ряд з 3 кнопками.
    QHBoxLayout* actionsLayout = new QHBoxLayout();

    // Кнопка "Генерувати ключ" (головна дія, бірюзовий градієнт).
    key_generateBtn = new QPushButton("Генерувати ключ", this);
    key_generateBtn->setMinimumHeight(50);
    key_generateBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00CED1, stop:1 #009999); "
        "color: #000000; font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00E5E8, stop:1 #00B8B8); }");
    // Підключаємо обробник події натискання.
    connect(key_generateBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::generateKey);
    actionsLayout->addWidget(key_generateBtn);

    // Кнопка "Приклад" (заповнює форму тестовими даними).
    key_exampleBtn = new QPushButton("Приклад", this);
    key_exampleBtn->setMinimumHeight(50);
    connect(key_exampleBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::fillKeyExample);
    actionsLayout->addWidget(key_exampleBtn);

    // Кнопка "Очистити" (червоний градієнт).
    key_clearBtn = new QPushButton("Очистити", this);
    key_clearBtn->setMinimumHeight(50);
    key_clearBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #dc2626, stop:1 #991b1b); "
        "color: #ffffff; font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ef4444, stop:1 #dc2626); }");
    connect(key_clearBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::clearKeyTab);
    actionsLayout->addWidget(key_clearBtn);

    mainLayout->addLayout(actionsLayout);

    // ========== БЛОК 4: Згенерований ключ ==========
    // Група для відображення результату генерації.
    QGroupBox* keyGroup = new QGroupBox("Згенерований ключ шифрування", this);
    QVBoxLayout* keyLayout = new QVBoxLayout(keyGroup);

    // Текстове поле для відображення ключа (тільки для читання).
    key_generatedKeyEdit = new QTextEdit(this);
    key_generatedKeyEdit->setReadOnly(true);
    key_generatedKeyEdit->setPlaceholderText(
        "Ключ шифрування з'явиться тут після генерації...\n\n"
        "Цей ключ використовується для шифрування та розшифрування ваших повідомлень.");
    key_generatedKeyEdit->setMinimumHeight(250);

    // Стиль: зелений текст на темному фоні (monospace шрифт для hex).
    key_generatedKeyEdit->setStyleSheet(
        "QTextEdit { background: #2a2a2a; color: #4ade80; border: 2px solid #22c55e; "
        "border-radius: 8px; padding: 10px; font-family: 'Courier New', monospace; font-size: 11px; }");
    keyLayout->addWidget(key_generatedKeyEdit);

    // Кнопки для роботи з ключем.
    QHBoxLayout* keyBtnsLayout = new QHBoxLayout();

    // Кнопка "Зберегти ключ".
    key_saveBtn = new QPushButton("Зберегти ключ", this);
    key_saveBtn->setEnabled(false);
    key_saveBtn->setMinimumHeight(40);
    key_saveBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4ade80, stop:1 #22c55e); "
        "color: #000000; font-size: 13px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #86efac, stop:1 #4ade80); } "
        "QPushButton:disabled { background: #444444; color: #888888; }");
    connect(key_saveBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::saveKey);
    keyBtnsLayout->addWidget(key_saveBtn);

    // Кнопка "Копіювати" (копіює ключ у буфер обміну).
    QPushButton* copyKeyBtn = new QPushButton("Копіювати", this);
    copyKeyBtn->setMinimumHeight(40);
    copyKeyBtn->setMinimumWidth(120);

    // Lambda-функція для копіювання.
    connect(copyKeyBtn, &QPushButton::clicked, [this]() {
        // Перевірка: чи є згенерований ключ.
        if (!key_generatedKeyEdit->toPlainText().isEmpty()) {
            // Копіюємо hex-ключ у системний буфер обміну.
            QApplication::clipboard()->setText(currentKey.keyHex);
            QMessageBox::information(this, "Успіх", "Ключ скопійовано у буфер обміну!");
        }
    });
    keyBtnsLayout->addWidget(copyKeyBtn);

    keyLayout->addLayout(keyBtnsLayout);
    mainLayout->addWidget(keyGroup);

    // ========== Активація кнопки збереження після генерації ==========
    // Lambda-функція: активує кнопку "Зберегти", коли ключ згенеровано.
    connect(key_generateBtn, &QPushButton::clicked, [this]() {
        if (!currentKey.keyHex.isEmpty()) {
            key_saveBtn->setEnabled(true);
        }
    });

    mainLayout->addStretch();

    return keyTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 2: ШИФРУВАННЯ
 * ============================================================================
 */

/**
 * @brief Створення вкладки шифрування повідомлень та файлів
 *
 * Структура вкладки:
 * 1. Ключ шифрування.
 * 2. Повідомлення для шифрування.
 * 3. Кнопки дій (Шифрування, Збереження, Очистити, Вибір файла).
 * 4. Поле зашифрованого повідомлення.
 * 5. Поле інофрмації про файл.
 *
 * Користувацький сценарій:
 * 1. Завантажити ключ шифрування з файлу.
 * 2. Ввести повідомлення АБО вибрати файл.
 * 3. Натиснути кнопку шифрування.
 * 4. Зберегти або скопіювати результат.
 *
 * @return Віджет вкладки з усіма елементами.
 */
QWidget* EmailEncryptorWindow::createEncryptTab() {
    QWidget* encryptTab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(encryptTab);
    mainLayout->setSpacing(15);

    // ========== ВЕРХНЯ ЧАСТИНА: Ключ + Повідомлення ==========
    // Горизонтальний layout.
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(15);

    // ========== ЛІВА ЧАСТИНА ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // --- Група: Ключ шифрування ---
    QGroupBox* keyGroup = new QGroupBox("Ключ шифрування", this);
    QVBoxLayout* keyLayout = new QVBoxLayout(keyGroup);

    // Поле для відображення шляху до файлу ключа (тільки для читання).
    encrypt_keyPathEdit = new QLineEdit(this);
    encrypt_keyPathEdit->setReadOnly(true);
    encrypt_keyPathEdit->setPlaceholderText("Завантажте ключ шифрування з файлу...");
    keyLayout->addWidget(encrypt_keyPathEdit);

    // Кнопка завантаження ключа (відкриває діалог вибору файлу).
    encrypt_loadKeyBtn = new QPushButton("Завантажити ключ", this);
    encrypt_loadKeyBtn->setMinimumHeight(40);
    encrypt_loadKeyBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00CED1, stop:1 #009999); "
        "color: #000000; font-size: 13px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00E5E8, stop:1 #00B8B8); }");
    connect(encrypt_loadKeyBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::loadEncryptionKey);
    keyLayout->addWidget(encrypt_loadKeyBtn);

    leftLayout->addWidget(keyGroup);

    // --- Група: Повідомлення для шифрування ---
    QGroupBox* messageGroup = new QGroupBox("Повідомлення для шифрування", this);
    QVBoxLayout* messageLayout = new QVBoxLayout(messageGroup);

    // Багаторядкове текстове поле для введення повідомлення.
    encrypt_messageEdit = new QTextEdit(this);
    encrypt_messageEdit->setPlaceholderText(
        "Введіть текст повідомлення для шифрування...\n\nПриклад: Зустрічаємося завтра о 15:00");
    encrypt_messageEdit->setMinimumHeight(200);
    encrypt_messageEdit->setStyleSheet(
        "QTextEdit { background: #2a2a2a; color: #e0e0e0; border: 2px solid #3a3a3a; "
        "border-radius: 8px; padding: 10px; font-size: 12px; }");
    messageLayout->addWidget(encrypt_messageEdit);

    // Кнопка шифрування повідомлення.
    encrypt_messageBtn = new QPushButton("Зашифрувати повідомлення", this);
    encrypt_messageBtn->setMinimumHeight(40);
    encrypt_messageBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00CED1, stop:1 #009999); "
        "color: #000000; font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00E5E8, stop:1 #00B8B8); }");
    connect(encrypt_messageBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::encryptMessage);
    messageLayout->addWidget(encrypt_messageBtn);

    leftLayout->addWidget(messageGroup);
    leftLayout->addStretch();

    // ========== ПРАВА ЧАСТИНА ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // --- Група: Зашифроване повідомлення (результат) ---
    QGroupBox* encryptedGroup = new QGroupBox("Зашифроване повідомлення", this);
    QVBoxLayout* encryptedLayout = new QVBoxLayout(encryptedGroup);

    // Поле для відображення результату шифрування (тільки для читання).
    encrypt_encryptedEdit = new QTextEdit(this);
    encrypt_encryptedEdit->setReadOnly(true);
    encrypt_encryptedEdit->setPlaceholderText(
        "Зашифроване повідомлення з'явиться тут...\n\nФормат: Base64\n"
        "Приклад: U2FsdGVkX1+vupppZksvRf5pq5g5XjFRIipRkwB0K1Y96Qsv2L...");
    encrypt_encryptedEdit->setMinimumHeight(300);

    // Стиль: жовтий текст (шифротекст), monospace шрифт.
    encrypt_encryptedEdit->setStyleSheet(
        "QTextEdit { background: #2a2a2a; color: #fbbf24; border: 2px solid #f59e0b; "
        "border-radius: 8px; padding: 10px; font-family: 'Courier New', monospace; font-size: 11px; }");
    encryptedLayout->addWidget(encrypt_encryptedEdit);

    // Кнопки для роботи з результатом.
    QHBoxLayout* encryptedBtnsLayout = new QHBoxLayout();

    // Кнопка збереження.
    encrypt_saveMessageBtn = new QPushButton("Зберегти повідомлення", this);
    encrypt_saveMessageBtn->setEnabled(false);
    encrypt_saveMessageBtn->setMinimumHeight(40);
    encrypt_saveMessageBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4ade80, stop:1 #22c55e); "
        "color: #000000; font-size: 13px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #86efac, stop:1 #4ade80); } "
        "QPushButton:disabled { background: #444444; color: #888888; }");
    connect(encrypt_saveMessageBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::saveEncryptedMessage);
    encryptedBtnsLayout->addWidget(encrypt_saveMessageBtn);

    // Кнопка копіювання.
    QPushButton* copyEncryptedBtn = new QPushButton("Копіювати", this);
    copyEncryptedBtn->setMinimumHeight(40);
    copyEncryptedBtn->setMinimumWidth(120);

    // Lambda: копіює весь текст з результатного поля.
    connect(copyEncryptedBtn, &QPushButton::clicked, [this]() {
        if (!encrypt_encryptedEdit->toPlainText().isEmpty()) {
            QApplication::clipboard()->setText(encrypt_encryptedEdit->toPlainText());
            QMessageBox::information(this, "Успіх", "Зашифроване повідомлення скопійовано у буфер обміну!");
        }
    });
    encryptedBtnsLayout->addWidget(copyEncryptedBtn);

    encryptedLayout->addLayout(encryptedBtnsLayout);
    rightLayout->addWidget(encryptedGroup);
    rightLayout->addStretch();

    // Lambda: активує кнопку збереження після успішного шифрування.
    connect(encrypt_messageBtn, &QPushButton::clicked, [this]() {
        if (!encrypt_encryptedEdit->toPlainText().isEmpty()) {
            encrypt_saveMessageBtn->setEnabled(true);
        }
    });

    // Додаємо ліву та праву частини до верхнього layout.
    topLayout->addLayout(leftLayout, 1);
    topLayout->addLayout(rightLayout, 1);

    mainLayout->addLayout(topLayout);

    // ========== НИЖНЯ ЧАСТИНА: Шифрування файлів ==========
    QGroupBox* fileGroup = new QGroupBox("Шифрування файлів", this);
    QVBoxLayout* fileLayout = new QVBoxLayout(fileGroup);

    // Поле для відображення шляху до вибраного файлу.
    encrypt_filePathEdit = new QLineEdit(this);
    encrypt_filePathEdit->setReadOnly(true);
    encrypt_filePathEdit->setPlaceholderText("Виберіть файл для шифрування...");
    fileLayout->addWidget(encrypt_filePathEdit);

    // Кнопки для роботи з файлами.
    QHBoxLayout* fileBtnsLayout = new QHBoxLayout();

    // Кнопка вибору файлу (відкриває діалог вибору).
    encrypt_selectFileBtn = new QPushButton("Вибрати файл", this);
    encrypt_selectFileBtn->setMinimumHeight(40);
    connect(encrypt_selectFileBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::selectFileToEncrypt);
    fileBtnsLayout->addWidget(encrypt_selectFileBtn);

    // Кнопка шифрування файлу.
    encrypt_fileBtn = new QPushButton("Зашифрувати файл", this);
    encrypt_fileBtn->setMinimumHeight(40);
    encrypt_fileBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00CED1, stop:1 #009999); "
        "color: #000000; font-size: 13px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00E5E8, stop:1 #00B8B8); }");
    connect(encrypt_fileBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::encryptFile);
    fileBtnsLayout->addWidget(encrypt_fileBtn);

    fileLayout->addLayout(fileBtnsLayout);

    // Мітка для відображення інформації про вибраний файл
    // (ім'я файлу, розмір).
    encrypt_fileInfoLabel = new QLabel("", this);
    encrypt_fileInfoLabel->setStyleSheet(
        "QLabel { color: #a0a0a0; padding: 8px; background: #2a2a2a; "
        "border: 1px solid #3a3a3a; border-radius: 5px; font-size: 11px; }");
    fileLayout->addWidget(encrypt_fileInfoLabel);

    mainLayout->addWidget(fileGroup);

    // ========== Кнопка очищення вкладки ==========
    encrypt_clearBtn = new QPushButton("Очистити", this);
    encrypt_clearBtn->setMinimumHeight(50);
    encrypt_clearBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #dc2626, stop:1 #991b1b); "
        "color: #ffffff; font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ef4444, stop:1 #dc2626); }");
    connect(encrypt_clearBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::clearEncryptTab);
    mainLayout->addWidget(encrypt_clearBtn);

    return encryptTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 3: РОЗШИФРУВАННЯ
 * ============================================================================
 */

/**
 * @brief Створення вкладки розшифрування повідомлень та файлів
 *
 * * Структура вкладки:
 * 1. Ключ розшифрування.
 * 2. Повідомлення для розшифрування.
 * 3. Кнопки дій (Розфрування, Збереження, Очистити, Вибір файла).
 * 4. Поле Розшифрованого повідомлення.
 * 5. Поле інофрмації про файл.
 *
 * Користувацький сценарій:
 * 1. Завантажити той самий ключ, що використовувався для шифрування.
 * 2. Вставити зашифроване повідомлення або вибрати .encrypted файл.
 * 3. Натиснути кнопку розшифрування.
 * 4. Отримати оригінальний текст/файл.
 *
 * @return Віджет вкладки з усіма елементами.
 */
QWidget* EmailEncryptorWindow::createDecryptTab() {
    QWidget* decryptTab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(decryptTab);
    mainLayout->setSpacing(15);

    // ========== ВЕРХНЯ ЧАСТИНА: Ключ + Повідомлення ==========
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(15);

    // ========== ЛІВА ЧАСТИНА ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // --- Група: Ключ розшифрування ---
    // ВАЖЛИВО: Це має бути той самий ключ, що використовувався для шифрування!
    QGroupBox* keyGroup = new QGroupBox("Ключ розшифрування", this);
    QVBoxLayout* keyLayout = new QVBoxLayout(keyGroup);

    decrypt_keyPathEdit = new QLineEdit(this);
    decrypt_keyPathEdit->setReadOnly(true);
    decrypt_keyPathEdit->setPlaceholderText("Завантажте ключ розшифрування з файлу...");
    keyLayout->addWidget(decrypt_keyPathEdit);

    decrypt_loadKeyBtn = new QPushButton("Завантажити ключ", this);
    decrypt_loadKeyBtn->setMinimumHeight(40);
    decrypt_loadKeyBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00CED1, stop:1 #009999); "
        "color: #000000; font-size: 13px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00E5E8, stop:1 #00B8B8); }");
    connect(decrypt_loadKeyBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::loadDecryptionKey);
    keyLayout->addWidget(decrypt_loadKeyBtn);

    leftLayout->addWidget(keyGroup);

    // --- Група: Зашифроване повідомлення (вхідні дані) ---
    QGroupBox* encryptedGroup = new QGroupBox("Зашифроване повідомлення", this);
    QVBoxLayout* encryptedLayout = new QVBoxLayout(encryptedGroup);

    // Поле для вставки зашифрованого тексту (Base64).
    decrypt_encryptedEdit = new QTextEdit(this);
    decrypt_encryptedEdit->setPlaceholderText(
        "Вставте зашифроване повідомлення для розшифрування...\n\n"
        "Формат: Base64\nПриклад: U2FsdGVkX1+vupppZksvRf5pq5g5XjFRIipRkwB0K1Y96Qsv2L...");
    decrypt_encryptedEdit->setMinimumHeight(200);

    // Стиль: жовтий текст (для шифротексту).
    decrypt_encryptedEdit->setStyleSheet(
        "QTextEdit { background: #2a2a2a; color: #fbbf24; border: 2px solid #f59e0b; "
        "border-radius: 8px; padding: 10px; font-family: 'Courier New', monospace; font-size: 11px; }");
    encryptedLayout->addWidget(decrypt_encryptedEdit);

    // Кнопка розшифрування.
    decrypt_messageBtn = new QPushButton("Розшифрувати повідомлення", this);
    decrypt_messageBtn->setMinimumHeight(40);
    decrypt_messageBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00CED1, stop:1 #009999); "
        "color: #000000; font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00E5E8, stop:1 #00B8B8); }");
    connect(decrypt_messageBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::decryptMessage);
    encryptedLayout->addWidget(decrypt_messageBtn);

    leftLayout->addWidget(encryptedGroup);
    leftLayout->addStretch();

    // ========== ПРАВА ЧАСТИНА ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // --- Група: Розшифроване повідомлення (результат) ---
    QGroupBox* decryptedGroup = new QGroupBox("Розшифроване повідомлення", this);
    QVBoxLayout* decryptedLayout = new QVBoxLayout(decryptedGroup);

    // Поле для відображення оригінального тексту (тільки для читання).
    decrypt_decryptedEdit = new QTextEdit(this);
    decrypt_decryptedEdit->setReadOnly(true);
    decrypt_decryptedEdit->setPlaceholderText(
        "Розшифроване повідомлення з'явиться тут...\n\nПриклад: Зустрічаємося завтра о 15:00");
    decrypt_decryptedEdit->setMinimumHeight(300);

    // Стиль: зелений текст (успішно розшифровано).
    decrypt_decryptedEdit->setStyleSheet(
        "QTextEdit { background: #2a2a2a; color: #4ade80; border: 2px solid #22c55e; "
        "border-radius: 8px; padding: 10px; font-size: 12px; }");
    decryptedLayout->addWidget(decrypt_decryptedEdit);

    // Кнопки для роботи з результатом.
    QHBoxLayout* decryptedBtnsLayout = new QHBoxLayout();

    // Кнопка збереження.
    decrypt_saveFileBtn = new QPushButton("Зберегти повідомлення", this);
    decrypt_saveFileBtn->setEnabled(false);
    decrypt_saveFileBtn->setMinimumHeight(40);
    decrypt_saveFileBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4ade80, stop:1 #22c55e); "
        "color: #000000; font-size: 13px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #86efac, stop:1 #4ade80); } "
        "QPushButton:disabled { background: #444444; color: #888888; }");
    connect(decrypt_saveFileBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::saveDecryptedFile);
    decryptedBtnsLayout->addWidget(decrypt_saveFileBtn);

    // Кнопка копіювання.
    QPushButton* copyDecryptedBtn = new QPushButton("Копіювати", this);
    copyDecryptedBtn->setMinimumHeight(40);
    copyDecryptedBtn->setMinimumWidth(120);
    connect(copyDecryptedBtn, &QPushButton::clicked, [this]() {
        if (!decrypt_decryptedEdit->toPlainText().isEmpty()) {
            QApplication::clipboard()->setText(decrypt_decryptedEdit->toPlainText());
            QMessageBox::information(this, "Успіх", "Розшифроване повідомлення скопійовано у буфер обміну!");
        }
    });
    decryptedBtnsLayout->addWidget(copyDecryptedBtn);

    decryptedLayout->addLayout(decryptedBtnsLayout);
    rightLayout->addWidget(decryptedGroup);
    rightLayout->addStretch();

    // Lambda: активує кнопку збереження після успішного розшифрування.
    connect(decrypt_messageBtn, &QPushButton::clicked, [this]() {
        if (!decrypt_decryptedEdit->toPlainText().isEmpty()) {
            decrypt_saveFileBtn->setEnabled(true);
        }
    });

    topLayout->addLayout(leftLayout, 1);
    topLayout->addLayout(rightLayout, 1);

    mainLayout->addLayout(topLayout);

    // ========== НИЖНЯ ЧАСТИНА: Розшифрування файлів ==========
    QGroupBox* fileGroup = new QGroupBox("Розшифрування файлів", this);
    QVBoxLayout* fileLayout = new QVBoxLayout(fileGroup);

    // Поле для відображення шляху до зашифрованого файлу.
    decrypt_filePathEdit = new QLineEdit(this);
    decrypt_filePathEdit->setReadOnly(true);
    decrypt_filePathEdit->setPlaceholderText("Виберіть зашифрований файл для розшифрування...");
    fileLayout->addWidget(decrypt_filePathEdit);

    // Кнопки для роботи з файлами.
    QHBoxLayout* fileBtnsLayout = new QHBoxLayout();

    // Кнопка вибору .encrypted файлу.
    decrypt_selectFileBtn = new QPushButton("Вибрати файл", this);
    decrypt_selectFileBtn->setMinimumHeight(40);
    connect(decrypt_selectFileBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::selectFileToDecrypt);
    fileBtnsLayout->addWidget(decrypt_selectFileBtn);

    // Кнопка розшифрування файлу.
    decrypt_fileBtn = new QPushButton("Розшифрувати файл", this);
    decrypt_fileBtn->setMinimumHeight(40);
    decrypt_fileBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00CED1, stop:1 #009999); "
        "color: #000000; font-size: 13px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00E5E8, stop:1 #00B8B8); }");
    connect(decrypt_fileBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::decryptFile);
    fileBtnsLayout->addWidget(decrypt_fileBtn);

    fileLayout->addLayout(fileBtnsLayout);

    // Мітка для відображення інформації про файл
    decrypt_fileInfoLabel = new QLabel("", this);
    decrypt_fileInfoLabel->setStyleSheet(
        "QLabel { color: #a0a0a0; padding: 8px; background: #2a2a2a; "
        "border: 1px solid #3a3a3a; border-radius: 5px; font-size: 11px; }");
    fileLayout->addWidget(decrypt_fileInfoLabel);

    mainLayout->addWidget(fileGroup);

    // ========== Кнопка очищення вкладки ==========
    decrypt_clearBtn = new QPushButton("Очистити", this);
    decrypt_clearBtn->setMinimumHeight(50);
    decrypt_clearBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #dc2626, stop:1 #991b1b); "
        "color: #ffffff; font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ef4444, stop:1 #dc2626); }");
    connect(decrypt_clearBtn, &QPushButton::clicked, this, &EmailEncryptorWindow::clearDecryptTab);
    mainLayout->addWidget(decrypt_clearBtn);

    return decryptTab;
}
