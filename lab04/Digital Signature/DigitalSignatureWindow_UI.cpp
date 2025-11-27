/*
 * DigitalSignatureWindow_UI.cpp
 *
 * Файл з методами створення UI вкладок
 * Містить реалізацію всіх 4 вкладок програми
 *
 * Автор: Student Project
 * Версія: 1.0
 * Дата: 2025
 */

#include "DigitalSignatureWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>
#include <QApplication>

/*
 * ============================================================================
 * ВКЛАДКА 1: ГЕНЕРАЦІЯ КЛЮЧІВ
 * ============================================================================
 */

/**
 * @brief Створення вкладки генерації ключів (приватного та публічного)
 *
 * Користувацький сценарій:
 * 1. Заповнити персональні дані (ім'я, прізвище, дата, секрет).
 * 2. Вибрати алгоритм хешування (SHA-256/SHA-512/MD5).
 * 3. Натиснути "Генерувати ключі".
 * 4. Отримати пару ключів (приватний + публічний).
 * 5. Зберегти ключі у файли або скопіювати.
 *
 * @return Віджет вкладки з усіма елементами.
 */
QWidget* DigitalSignatureWindow::createKeysTab() {
    QWidget* keysTab = new QWidget();

    // ========== ГОЛОВНИЙ LAYOUT==========
    QHBoxLayout* mainLayout = new QHBoxLayout(keysTab);
    mainLayout->setSpacing(15);

    // Стиль для міток (бірюзовий колір, жирний шрифт).
    QString labelStyle = "QLabel { color: #00E5E8; font-weight: bold; }";

    // ========== ЛІВА ЧАСТИНА: Форми введення даних ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // ========== Група 1: Персональні дані ==========
    // Поля для введення інформації про користувача.
    QGroupBox* personalGroup = new QGroupBox(
        "Персональні дані (для генерації ключів)",
        this
        );
    QGridLayout* personalLayout = new QGridLayout(personalGroup);

    // --- Поле: Ім'я ---
    QLabel* firstNameLabel = new QLabel("Ім'я:", this);
    firstNameLabel->setStyleSheet(labelStyle);
    keys_firstNameEdit = new QLineEdit(this);
    keys_firstNameEdit->setPlaceholderText("Введіть ім'я");

    // --- Поле: Прізвище ---
    QLabel* lastNameLabel = new QLabel("Прізвище:", this);
    lastNameLabel->setStyleSheet(labelStyle);
    keys_lastNameEdit = new QLineEdit(this);
    keys_lastNameEdit->setPlaceholderText("Введіть прізвище");

    // --- Поле: Дата народження ---
    // QDateEdit з спливаючим календарем для зручного вибору дати
    QLabel* birthDateLabel = new QLabel("Дата народження:", this);
    birthDateLabel->setStyleSheet(labelStyle);
    keys_birthDateEdit = new QDateEdit(this);
    keys_birthDateEdit->setDate(QDate(2000, 1, 1));
    keys_birthDateEdit->setDisplayFormat("dd.MM.yyyy");
    keys_birthDateEdit->setCalendarPopup(true);

    // --- Поле: Секретне слово ---
    // Додатковий фактор безпеки для генерації ключа.
    // За замовчуванням приховане (password mode).
    QLabel* secretWordLabel = new QLabel("Секретне слово:", this);
    secretWordLabel->setStyleSheet(labelStyle);
    keys_secretWordEdit = new QLineEdit(this);
    keys_secretWordEdit->setPlaceholderText("Введіть секретне слово");
    keys_secretWordEdit->setEchoMode(QLineEdit::Password);

    // Розміщення полів у сітці (2 ряди × 4 колонки).
    // Рядок 0: Ім'я | [поле] | Прізвище | [поле].
    // Рядок 1: Дата | [поле] | Секретне слово | [поле+кнопка].
    personalLayout->addWidget(firstNameLabel, 0, 0);
    personalLayout->addWidget(keys_firstNameEdit, 0, 1);
    personalLayout->addWidget(lastNameLabel, 0, 2);
    personalLayout->addWidget(keys_lastNameEdit, 0, 3);
    personalLayout->addWidget(birthDateLabel, 1, 0);
    personalLayout->addWidget(keys_birthDateEdit, 1, 1);
    personalLayout->addWidget(secretWordLabel, 1, 2);

    // Створюємо контейнер для секретного слова з кнопкою показу/приховування.
    QHBoxLayout* secretWordLayout = new QHBoxLayout();
    secretWordLayout->setSpacing(5);
    secretWordLayout->addWidget(keys_secretWordEdit);

    // --- Кнопка показу/приховування секретного слова ---
    // Перемикає між режимами Password (***) та Normal (текст).
    QPushButton* toggleSecretBtn = new QPushButton("🔓️", this);
    toggleSecretBtn->setMaximumWidth(40);
    toggleSecretBtn->setMinimumHeight(32);
    toggleSecretBtn->setToolTip("Показати/Приховати секретне слово");
    toggleSecretBtn->setCursor(Qt::PointingHandCursor);  // Курсор-рука при наведенні.
    toggleSecretBtn->setStyleSheet(
        "QPushButton {"
        "   background: #3a3a3a;"
        "   border: 2px solid #4a4a4a;"
        "   border-radius: 5px;"
        "   font-size: 16px;"
        "   padding: 5px;"
        "}"
        "QPushButton:hover {"
        "   background: #4a4a4a;"
        "   border-color: #00CED1;"
        "}"
        "QPushButton:pressed {"
        "   background: #2a2a2a;"
        "}"
        );

    // Lambda-функція для перемикання видимості.
    connect(toggleSecretBtn, &QPushButton::clicked, [this, toggleSecretBtn]() {
        if (keys_secretWordEdit->echoMode() == QLineEdit::Password) {
            // Показуємо текст.
            keys_secretWordEdit->setEchoMode(QLineEdit::Normal);
            toggleSecretBtn->setText("🔒");
            toggleSecretBtn->setToolTip("Приховати секретне слово");
        } else {
            // Приховуємо текст
            keys_secretWordEdit->setEchoMode(QLineEdit::Password);
            toggleSecretBtn->setText("🔓️");
            toggleSecretBtn->setToolTip("Показати секретне слово");
        }
    });

    secretWordLayout->addWidget(toggleSecretBtn);
    personalLayout->addLayout(secretWordLayout, 1, 3);

    leftLayout->addWidget(personalGroup);

    // ========== Група 2: Вибір алгоритму хешування ==========
    // Користувач може обрати один з трьох алгоритмів.
    QGroupBox* algorithmGroup = new QGroupBox(
        "Алгоритм хешування",
        this
        );
    QHBoxLayout* algorithmLayout = new QHBoxLayout(algorithmGroup);

    QLabel* algorithmLabel = new QLabel("Алгоритм:", this);
    algorithmLabel->setStyleSheet(labelStyle);
    algorithmLayout->addWidget(algorithmLabel);

    // ComboBox з вибором алгоритму.
    keys_hashAlgorithmCombo = new QComboBox(this);
    keys_hashAlgorithmCombo->addItem("SHA-256 (рекомендовано)");
    keys_hashAlgorithmCombo->addItem("SHA-512 (більш безпечний)");
    keys_hashAlgorithmCombo->addItem("MD5 (застарілий, для демонстрації)");
    keys_hashAlgorithmCombo->setCurrentIndex(0);

    // Підключаємо обробник зміни алгоритму.
    // Викликається коли користувач змінює вибір у випадаючому списку.
    connect(keys_hashAlgorithmCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &DigitalSignatureWindow::onHashAlgorithmChanged);

    algorithmLayout->addWidget(keys_hashAlgorithmCombo);

    leftLayout->addWidget(algorithmGroup);

    // ========== Інформаційне повідомлення ==========
    // Жовта панель з важливою інформацією про безпеку.
    QLabel* infoLabel = new QLabel(
        "Приватний ключ зберігайте в секреті!\n"
        "Публічний ключ можна передавати іншим для перевірки підписів.",
        this
        );
    infoLabel->setStyleSheet(
        "QLabel {"
        "   color: #fbbf24;"
        "   background: rgba(251, 191, 36, 0.1);"
        "   padding: 12px;"
        "   border-radius: 8px;"
        "   font-size: 12px;"
        "}"
        );
    infoLabel->setWordWrap(true);
    leftLayout->addWidget(infoLabel);

    // ========== Кнопки дій ==========
    QHBoxLayout* actionsLayout = new QHBoxLayout();

    // --- Кнопка: Генерувати ключі (головна дія) ---
    keys_generateBtn = new QPushButton("Генерувати ключі", this);
    keys_generateBtn->setMinimumHeight(50);
    keys_generateBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:1 #00B8B8);"
        "}"
        );
    connect(keys_generateBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::generateKeys);
    actionsLayout->addWidget(keys_generateBtn);

    // --- Кнопка: Заповнити прикладом ---
    // Швидке заповнення форми тестовими даними.
    keys_exampleBtn = new QPushButton("Приклад", this);
    keys_exampleBtn->setMinimumHeight(50);
    connect(keys_exampleBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::fillKeysExample);
    actionsLayout->addWidget(keys_exampleBtn);

    // --- Кнопка: Очистити ---
    // Скидає всі поля до початкового стану.
    keys_clearBtn = new QPushButton("Очистити", this);
    keys_clearBtn->setMinimumHeight(50);
    keys_clearBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #ef4444, stop:1 #dc2626);"
        "}"
        );
    connect(keys_clearBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::clearKeysTab);
    actionsLayout->addWidget(keys_clearBtn);

    leftLayout->addLayout(actionsLayout);
    leftLayout->addStretch();

    // ========== ПРАВА ЧАСТИНА: Відображення згенерованих ключів ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // ========== Група 1: Приватний ключ (СЕКРЕТНИЙ!) ==========
    // ВАЖЛИВО: Приватний ключ повинен зберігатися в таємниці!
    // Використовується для створення цифрових підписів.
    QGroupBox* privateKeyGroup = new QGroupBox(
        "Приватний ключ",
        this
        );
    QVBoxLayout* privateKeyLayout = new QVBoxLayout(privateKeyGroup);

    // Текстове поле для відображення приватного ключа.
    keys_privateKeyEdit = new QTextEdit(this);
    keys_privateKeyEdit->setReadOnly(true);
    keys_privateKeyEdit->setPlaceholderText(
        "Приватний ключ з'явиться тут після генерації..."
        );
    keys_privateKeyEdit->setMinimumHeight(120);
    keys_privateKeyEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #ff6b6b;"
        "   border: 2px solid #dc2626;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 11px;"
        "}"
        );
    privateKeyLayout->addWidget(keys_privateKeyEdit);

    // Кнопки для роботи з приватним ключем.
    QHBoxLayout* privateKeyBtnsLayout = new QHBoxLayout();

    // Кнопка збереження приватного ключа (спочатку неактивна).
    keys_savePrivateBtn = new QPushButton("Зберегти приватний ключ", this);
    keys_savePrivateBtn->setEnabled(false);
    keys_savePrivateBtn->setMinimumHeight(40);
    keys_savePrivateBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: #ffffff;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #ef4444, stop:1 #dc2626);"
        "}"
        "QPushButton:disabled {"
        "   background: #444444;"
        "   color: #888888;"
        "}"
        );
    connect(keys_savePrivateBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::savePrivateKey);
    privateKeyBtnsLayout->addWidget(keys_savePrivateBtn);

    // Кнопка копіювання приватного ключа.
    QPushButton* copyPrivateBtn = new QPushButton("Копіювати", this);
    copyPrivateBtn->setMinimumHeight(40);
    copyPrivateBtn->setMinimumWidth(120);

    // Lambda: копіює приватний ключ у буфер обміну.
    connect(copyPrivateBtn, &QPushButton::clicked, [this]() {
        if (!keys_privateKeyEdit->toPlainText().isEmpty()) {
            QApplication::clipboard()->setText(keys_privateKeyEdit->toPlainText());
            QMessageBox::information(this, "Успіх",
                                     "Приватний ключ скопійовано у буфер обміну!");
        }
    });
    privateKeyBtnsLayout->addWidget(copyPrivateBtn);

    privateKeyLayout->addLayout(privateKeyBtnsLayout);
    rightLayout->addWidget(privateKeyGroup);

    // ========== Група 2: Публічний ключ (ПУБЛІЧНИЙ) ==========
    // Публічний ключ можна вільно передавати іншим.
    // Використовується для перевірки цифрових підписів.
    QGroupBox* publicKeyGroup = new QGroupBox(
        "Публічний ключ",
        this
        );
    QVBoxLayout* publicKeyLayout = new QVBoxLayout(publicKeyGroup);

    // Текстове поле для відображення публічного ключа.
    keys_publicKeyEdit = new QTextEdit(this);
    keys_publicKeyEdit->setReadOnly(true);
    keys_publicKeyEdit->setPlaceholderText(
        "Публічний ключ з'явиться тут після генерації..."
        );
    keys_publicKeyEdit->setMinimumHeight(120);
    keys_publicKeyEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #4ade80;"
        "   border: 2px solid #22c55e;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 11px;"
        "}"
        );
    publicKeyLayout->addWidget(keys_publicKeyEdit);

    // Кнопки для роботи з публічним ключем.
    QHBoxLayout* publicKeyBtnsLayout = new QHBoxLayout();

    // Кнопка збереження публічного ключа.
    keys_savePublicBtn = new QPushButton("Зберегти публічний ключ", this);
    keys_savePublicBtn->setEnabled(false);
    keys_savePublicBtn->setMinimumHeight(40);
    keys_savePublicBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #4ade80, stop:1 #22c55e);"
        "   color: #000000;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #86efac, stop:1 #4ade80);"
        "}"
        "QPushButton:disabled {"
        "   background: #444444;"
        "   color: #888888;"
        "}"
        );
    connect(keys_savePublicBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::savePublicKey);
    publicKeyBtnsLayout->addWidget(keys_savePublicBtn);

    // Кнопка копіювання публічного ключа.
    QPushButton* copyPublicBtn = new QPushButton("Копіювати", this);
    copyPublicBtn->setMinimumHeight(40);
    copyPublicBtn->setMinimumWidth(120);
    connect(copyPublicBtn, &QPushButton::clicked, [this]() {
        if (!keys_publicKeyEdit->toPlainText().isEmpty()) {
            QApplication::clipboard()->setText(keys_publicKeyEdit->toPlainText());
            QMessageBox::information(this, "Успіх",
                                     "Публічний ключ скопійовано у буфер обміну!");
        }
    });
    publicKeyBtnsLayout->addWidget(copyPublicBtn);

    publicKeyLayout->addLayout(publicKeyBtnsLayout);
    rightLayout->addWidget(publicKeyGroup);
    rightLayout->addStretch();

    // ========== Активація кнопок збереження після генерації ==========
    // Lambda: коли ключі згенеровані, активуємо кнопки збереження.
    connect(keys_generateBtn, &QPushButton::clicked, [this]() {
        keys_savePrivateBtn->setEnabled(true);
        keys_savePublicBtn->setEnabled(true);
    });

    // ========== Додаємо дві частини до головного layout (50/50) ==========
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    return keysTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 2: СТВОРЕННЯ ПІДПИСУ
 * ============================================================================
 */

/**
 * @brief Створення вкладки створення цифрового підпису.
 *
 * Користувацький сценарій:
 * 1. Вибрати документ для підписання (будь-який файл).
 * 2. Завантажити приватний ключ (з вкладки 1).
 * 3. Натиснути "Створити підпис".
 * 4. Отримати хеш документа та цифровий підпис.
 * 5. Зберегти підпис у файл .sig.
 *
 * Алгоритм створення підпису:
 * 1. Хешування документа (SHA-256/SHA-512/MD5).
 * 2. "Шифрування" хешу приватним ключем.
 * 3. Результат = цифровий підпис.
 *
 * @return Віджет вкладки з усіма елементами.
 */
QWidget* DigitalSignatureWindow::createSignTab() {
    QWidget* signTab = new QWidget();

    // ========== ГОЛОВНИЙ LAYOUT==========
    QHBoxLayout* mainLayout = new QHBoxLayout(signTab);
    mainLayout->setSpacing(15);

    QString labelStyle = "QLabel { color: #00E5E8; font-weight: bold; }";

    // ========== ЛІВА ЧАСТИНА: Введення даних ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // ========== Група 1: Вибір документа для підписання ==========
    // Користувач вибирає будь-який файл, який потрібно підписати.
    QGroupBox* documentGroup = new QGroupBox(
        "Документ для підписання",
        this
        );
    QVBoxLayout* documentLayout = new QVBoxLayout(documentGroup);

    // Поле для відображення шляху до вибраного файлу.
    sign_documentPathEdit = new QLineEdit(this);
    sign_documentPathEdit->setReadOnly(true);
    sign_documentPathEdit->setPlaceholderText("Виберіть документ...");
    documentLayout->addWidget(sign_documentPathEdit);

    // Кнопка вибору документа (відкриває діалог вибору файлу).
    sign_selectDocumentBtn = new QPushButton("Вибрати документ", this);
    sign_selectDocumentBtn->setMinimumHeight(40);
    sign_selectDocumentBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:1 #00B8B8);"
        "}"
        );
    connect(sign_selectDocumentBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::selectDocumentToSign);
    documentLayout->addWidget(sign_selectDocumentBtn);

    // Мітка для відображення інформації про вибраний файл
    // (ім'я файлу, розмір, тип).
    sign_documentInfoLabel = new QLabel("", this);
    sign_documentInfoLabel->setStyleSheet(
        "QLabel {"
        "   color: #00E5E8;"
        "   padding: 8px;"
        "   background: rgba(0, 206, 209, 0.1);"
        "   border-radius: 5px;"
        "}"
        );
    documentLayout->addWidget(sign_documentInfoLabel);

    leftLayout->addWidget(documentGroup);

    // ========== Група 2: Завантаження приватного ключа ==========
    // ВАЖЛИВО: Потрібен приватний ключ, згенерований на вкладці 1.
    QGroupBox* keyGroup = new QGroupBox(
        "Приватний ключ",
        this
        );
    QVBoxLayout* keyLayout = new QVBoxLayout(keyGroup);

    // Поле для відображення шляху до файлу ключа.
    sign_privateKeyPathEdit = new QLineEdit(this);
    sign_privateKeyPathEdit->setReadOnly(true);
    sign_privateKeyPathEdit->setPlaceholderText(
        "Завантажте приватний ключ з файлу..."
        );
    keyLayout->addWidget(sign_privateKeyPathEdit);

    // Кнопка завантаження приватного ключа
    sign_loadPrivateKeyBtn = new QPushButton(
        "Завантажити приватний ключ",
        this
        );
    sign_loadPrivateKeyBtn->setMinimumHeight(40);
    connect(sign_loadPrivateKeyBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::loadPrivateKey);
    keyLayout->addWidget(sign_loadPrivateKeyBtn);

    leftLayout->addWidget(keyGroup);

    // ========== Кнопки дій (ліва частина) ==========
    QVBoxLayout* leftActionsLayout = new QVBoxLayout();

    // Кнопка створення підпису (головна дія).
    sign_createBtn = new QPushButton("Створити підпис", this);
    sign_createBtn->setMinimumHeight(50);
    sign_createBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:1 #00B8B8);"
        "}"
        );
    connect(sign_createBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::createSignature);
    leftActionsLayout->addWidget(sign_createBtn);

    // Кнопка очищення.
    sign_clearBtn = new QPushButton("Очистити", this);
    sign_clearBtn->setMinimumHeight(50);
    sign_clearBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #ef4444, stop:1 #dc2626);"
        "}"
        );
    connect(sign_clearBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::clearSignTab);
    leftActionsLayout->addWidget(sign_clearBtn);

    leftLayout->addLayout(leftActionsLayout);
    leftLayout->addStretch();

    // ========== ПРАВА ЧАСТИНА: Результати ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // ========== Група 3: Хеш документа ==========
    // Відображає результат хешування документа.
    // Це проміжний крок перед створенням підпису.
    QGroupBox* hashGroup = new QGroupBox(
        "Хеш документа",
        this
        );
    QVBoxLayout* hashLayout = new QVBoxLayout(hashGroup);

    sign_documentHashEdit = new QTextEdit(this);
    sign_documentHashEdit->setReadOnly(true);
    sign_documentHashEdit->setPlaceholderText(
        "Хеш документа з'явиться тут після вибору файлу..."
        );
    sign_documentHashEdit->setMinimumHeight(120);
    sign_documentHashEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #a78bfa;"
        "   border: 2px solid #8b5cf6;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 11px;"
        "}"
        );
    hashLayout->addWidget(sign_documentHashEdit);

    rightLayout->addWidget(hashGroup);

    // ========== Група 4: Цифровий підпис (результат) ==========
    // Відображає згенерований цифровий підпис.
    QGroupBox* signatureGroup = new QGroupBox(
        "Цифровий підпис",
        this
        );
    QVBoxLayout* signatureLayout = new QVBoxLayout(signatureGroup);

    sign_signatureEdit = new QTextEdit(this);
    sign_signatureEdit->setReadOnly(true);
    sign_signatureEdit->setPlaceholderText(
        "Підпис з'явиться тут після створення..."
        );
    sign_signatureEdit->setMinimumHeight(120);
    sign_signatureEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #4ade80;"
        "   border: 2px solid #22c55e;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 11px;"
        "}"
        );
    signatureLayout->addWidget(sign_signatureEdit);

    // Кнопка збереження підпису (спочатку неактивна).
    sign_saveBtn = new QPushButton("Зберегти підпис", this);
    sign_saveBtn->setEnabled(false);
    sign_saveBtn->setMinimumHeight(40);
    sign_saveBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #4ade80, stop:1 #22c55e);"
        "   color: #000000;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #86efac, stop:1 #4ade80);"
        "}"
        "QPushButton:disabled {"
        "   background: #444444;"
        "   color: #888888;"
        "}"
        );
    connect(sign_saveBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::saveSignature);
    signatureLayout->addWidget(sign_saveBtn);

    rightLayout->addWidget(signatureGroup);
    rightLayout->addStretch();

    // ========== Активація кнопки збереження після створення підпису ==========
    // Lambda: коли підпис створено, активуємо кнопку збереження.
    connect(sign_createBtn, &QPushButton::clicked, [this]() {
        if (!createdSignature.isEmpty()) {
            sign_saveBtn->setEnabled(true);
        }
    });

    // ========== Додаємо дві частини до головного layout (50/50) ==========
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    return signTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 3: ПЕРЕВІРКА ПІДПИСУ
 * ============================================================================
 */

/**
 * @brief Створення вкладки перевірки цифрового підпису
 *
 * Користувацький сценарій:
 * 1. Завантажити документ (той самий, що був підписаний).
 * 2. Завантажити файл підпису (.sig).
 * 3. Завантажити публічний ключ автора (не секретний!).
 * 4. Натиснути "Перевірити підпис".
 * 5. Отримати результат: ✅ ДІЙСНИЙ або ❌ НЕДІЙСНИЙ.
 *
 * Алгоритм перевірки:
 * 1. Хешування документа → currentHash.
 * 2. "Розшифрування" підпису публічним ключем → originalHash.
 * 3. Порівняння: currentHash == originalHash?.
 *    - Якщо ТАК → підпис дійсний ✅.
 *    - Якщо НІ → підпис недійсний ❌.
 *
 * @return Віджет вкладки з усіма елементами.
 */
QWidget* DigitalSignatureWindow::createVerifyTab() {
    QWidget* verifyTab = new QWidget();

    // ========== ГОЛОВНИЙ LAYOUT==========
    QHBoxLayout* mainLayout = new QHBoxLayout(verifyTab);
    mainLayout->setSpacing(15);

    QString labelStyle = "QLabel { color: #00E5E8; font-weight: bold; }";

    // ========== ЛІВА ЧАСТИНА: Введення даних для перевірки ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // ========== Група 1: Вибір документа для перевірки ==========
    // ВАЖЛИВО: Це має бути той самий документ, що був підписаний!
    // Навіть мінімальна зміна документа зробить підпис недійсним.
    QGroupBox* documentGroup = new QGroupBox(
        "Документ для перевірки",
        this
        );
    QVBoxLayout* documentLayout = new QVBoxLayout(documentGroup);

    verify_documentPathEdit = new QLineEdit(this);
    verify_documentPathEdit->setReadOnly(true);
    verify_documentPathEdit->setPlaceholderText("Виберіть документ...");
    documentLayout->addWidget(verify_documentPathEdit);

    verify_selectDocumentBtn = new QPushButton("Вибрати документ", this);
    verify_selectDocumentBtn->setMinimumHeight(40);
    verify_selectDocumentBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:1 #00B8B8);"
        "}"
        );
    connect(verify_selectDocumentBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::selectDocumentToVerify);
    documentLayout->addWidget(verify_selectDocumentBtn);

    verify_documentInfoLabel = new QLabel("", this);
    verify_documentInfoLabel->setStyleSheet(
        "QLabel {"
        "   color: #00E5E8;"
        "   padding: 8px;"
        "   background: rgba(0, 206, 209, 0.1);"
        "   border-radius: 5px;"
        "}"
        );
    documentLayout->addWidget(verify_documentInfoLabel);

    leftLayout->addWidget(documentGroup);

    // ========== Група 2: Завантаження підпису ==========
    // Файл підпису (.sig), створений на вкладці 2.
    QGroupBox* signatureGroup = new QGroupBox(
        "Цифровий підпис",
        this
        );
    QVBoxLayout* signatureLayout = new QVBoxLayout(signatureGroup);

    verify_signaturePathEdit = new QLineEdit(this);
    verify_signaturePathEdit->setReadOnly(true);
    verify_signaturePathEdit->setPlaceholderText(
        "Завантажте файл підпису..."
        );
    signatureLayout->addWidget(verify_signaturePathEdit);

    verify_loadSignatureBtn = new QPushButton(
        "Завантажити підпис",
        this
        );
    verify_loadSignatureBtn->setMinimumHeight(40);
    connect(verify_loadSignatureBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::loadSignature);
    signatureLayout->addWidget(verify_loadSignatureBtn);

    leftLayout->addWidget(signatureGroup);

    // ========== Група 3: Завантаження публічного ключа ==========
    // ВАЖЛИВО: Потрібен публічний ключ автора підпису (не приватний!).
    // Публічний ключ можна вільно передавати.
    QGroupBox* keyGroup = new QGroupBox(
        "Публічний ключ",
        this
        );
    QVBoxLayout* keyLayout = new QVBoxLayout(keyGroup);

    verify_publicKeyPathEdit = new QLineEdit(this);
    verify_publicKeyPathEdit->setReadOnly(true);
    verify_publicKeyPathEdit->setPlaceholderText(
        "Завантажте публічний ключ..."
        );
    keyLayout->addWidget(verify_publicKeyPathEdit);

    verify_loadPublicKeyBtn = new QPushButton(
        "Завантажити публічний ключ",
        this
        );
    verify_loadPublicKeyBtn->setMinimumHeight(40);
    connect(verify_loadPublicKeyBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::loadPublicKey);
    keyLayout->addWidget(verify_loadPublicKeyBtn);

    leftLayout->addWidget(keyGroup);

    // ========== Кнопки дій (ліва частина) ==========
    QVBoxLayout* leftActionsLayout = new QVBoxLayout();

    // Кнопка перевірки підпису (головна дія).
    verify_verifyBtn = new QPushButton("Перевірити підпис", this);
    verify_verifyBtn->setMinimumHeight(50);
    verify_verifyBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:1 #00B8B8);"
        "}"
        );
    connect(verify_verifyBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::verifySignature);
    leftActionsLayout->addWidget(verify_verifyBtn);

    // Кнопка очищення.
    verify_clearBtn = new QPushButton("Очистити", this);
    verify_clearBtn->setMinimumHeight(50);
    verify_clearBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #ef4444, stop:1 #dc2626);"
        "}"
        );
    connect(verify_clearBtn, &QPushButton::clicked,
            this, &DigitalSignatureWindow::clearVerifyTab);
    leftActionsLayout->addWidget(verify_clearBtn);

    leftLayout->addLayout(leftActionsLayout);
    leftLayout->addStretch();

    // ========== ПРАВА ЧАСТИНА: Результати перевірки ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // ========== Група 4: Результат перевірки ==========
    // Відображає: ✅ ПІДПИС ДІЙСНИЙ або ❌ ПІДПИС НЕДІЙСНИЙ.
    // + детальна інформація про перевірку.
    QGroupBox* resultGroup = new QGroupBox(
        "Результат перевірки",
        this
        );
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);

    verify_resultEdit = new QTextEdit(this);
    verify_resultEdit->setReadOnly(true);
    verify_resultEdit->setPlaceholderText(
        "Результат перевірки з'явиться тут..."
        );
    verify_resultEdit->setMinimumHeight(360);
    verify_resultEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #e0e0e0;"
        "   border: 2px solid #3a3a3a;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   font-size: 11px;"
        "   font-family: 'Courier New', monospace;"
        "}"
        );
    resultLayout->addWidget(verify_resultEdit);

    rightLayout->addWidget(resultGroup);

    // ========== Група 5: Деталі перевірки (технічна інформація) ==========
    // Показує хеші для технічного аналізу.
    // Спочатку прихована, показується після перевірки.
    verify_detailsGroup = new QGroupBox(
        "Деталі перевірки (технічна інформація)",
        this
        );
    verify_detailsGroup->setVisible(false);
    QGridLayout* detailsLayout = new QGridLayout(verify_detailsGroup);

    // --- Оригінальний хеш (з підпису) ---
    // Це хеш, який був у документі на момент підписання.
    QLabel* originalHashLabel = new QLabel("Оригінальний хеш:", this);
    originalHashLabel->setStyleSheet(labelStyle);
    detailsLayout->addWidget(originalHashLabel, 0, 0);

    verify_originalHashEdit = new QLineEdit(this);
    verify_originalHashEdit->setReadOnly(true);
    verify_originalHashEdit->setStyleSheet(
        "QLineEdit {"
        "   background: #2a2a2a;"
        "   color: #a78bfa;"
        "   border: 2px solid #8b5cf6;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 10px;"
        "}"
        );
    detailsLayout->addWidget(verify_originalHashEdit, 0, 1);

    // --- Поточний хеш (обчислений зараз) ---
    // Це хеш поточного стану документа.
    QLabel* currentHashLabel = new QLabel("Поточний хеш:", this);
    currentHashLabel->setStyleSheet(labelStyle);
    detailsLayout->addWidget(currentHashLabel, 1, 0);

    verify_currentHashEdit = new QLineEdit(this);
    verify_currentHashEdit->setReadOnly(true);
    verify_currentHashEdit->setStyleSheet(
        "QLineEdit {"
        "   background: #2a2a2a;"
        "   color: #4ade80;"
        "   border: 2px solid #22c55e;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 10px;"
        "}"
        );
    detailsLayout->addWidget(verify_currentHashEdit, 1, 1);

    // --- Розшифрований хеш (з підпису через публічний ключ) ---
    // Результат "розшифрування" підпису публічним ключем.
    QLabel* decryptedHashLabel = new QLabel("Розшифрований хеш:", this);
    decryptedHashLabel->setStyleSheet(labelStyle);
    detailsLayout->addWidget(decryptedHashLabel, 2, 0);

    verify_decryptedHashEdit = new QLineEdit(this);
    verify_decryptedHashEdit->setReadOnly(true);
    verify_decryptedHashEdit->setStyleSheet(
        "QLineEdit {"
        "   background: #2a2a2a;"
        "   color: #fbbf24;"
        "   border: 2px solid #f59e0b;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 10px;"
        "}"
        );
    detailsLayout->addWidget(verify_decryptedHashEdit, 2, 1);

    rightLayout->addWidget(verify_detailsGroup);
    rightLayout->addStretch();

    // ========== Додаємо дві частини до головного layout (50/50) ==========
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    return verifyTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 4: ПОКРОКОВИЙ АЛГОРИТМ
 * ============================================================================
 */

/**
 * @brief Створення вкладки покрокового алгоритму цифрових підписів
 *
 * Вкладка містить детальне текстове пояснення:
 * - Що таке цифровий підпис.
 * - Як працює алгоритм генерації ключів.
 * - Як створюється підпис.
 * - Як перевіряється підпис.
 * - Приклади з конкретними даними.
 * - Математичні основи (хешування, симетричне шифрування).
 *
 * @return Віджет вкладки з текстовим поясненням.
 */
QWidget* DigitalSignatureWindow::createAlgorithmTab() {
    QWidget* algorithmTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(algorithmTab);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // ========== Група з поясненням алгоритму ==========
    QGroupBox* infoGroup = new QGroupBox(
        "Покроковий алгоритм цифрових підписів",
        this
        );
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);

    // Текстове поле з детальним поясненням (тільки для читання).
    algorithm_textEdit = new QTextEdit(this);
    algorithm_textEdit->setReadOnly(true);
    algorithm_textEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   border: 2px solid #3a3a3a;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   color: #e0e0e0;"
        "   font-size: 13px;"
        "   line-height: 1.6;"
        "}"
        );

    infoLayout->addWidget(algorithm_textEdit);
    layout->addWidget(infoGroup);

    // ========== Відразу показуємо алгоритм ==========
    // Метод заповнює текстове поле детальним поясненням.
    // з прикладами, формулами та покроковими інструкціями.
    showStepByStepAlgorithm();

    return algorithmTab;
}
