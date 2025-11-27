/*
 * SteganographyWindow_UI.cpp
 *
 * Файл з методами створення UI вкладок.
 * Містить реалізацію всіх 4 вкладок програми.
 */

#include "SteganographyWindow.h"
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
 * ВКЛАДКА 1: ПРИХОВУВАННЯ ПОВІДОМЛЕННЯ
 * ============================================================================
 */

/**
 * @brief Створення вкладки приховування повідомлення.
 *
 * Структура:
 * - Ліва частина: Форми введення даних.
 *   - Персональні дані (для генерації ключа).
 *   - Згенерований ключ шифрування.
 *   - Текст для приховування.
 *   - Кнопки дій.
 * - Права частина: Зображення.
 *   - Вхідне зображення.
 *   - Результат (модифіковане зображення).
 *
 * @return Віджет вкладки.
 */
QWidget* SteganographyWindow::createHideTab() {
    QWidget* hideTab = new QWidget();

    // ========== ГОЛОВНИЙ LAYOUT: ГОРИЗОНТАЛЬНИЙ ==========
    QHBoxLayout* mainLayout = new QHBoxLayout(hideTab);
    mainLayout->setSpacing(15);

    // Стиль для міток.
    QString labelStyle = "QLabel { color: #00E5E8; font-weight: bold; }";

    // ========== ЛІВА ЧАСТИНА: Форми введення ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // ========== Група 1: Персональні дані ==========
    QGroupBox* personalGroup = new QGroupBox(
        "Персональні дані (для генерації ключа шифрування)",
        this
        );
    QGridLayout* personalLayout = new QGridLayout(personalGroup);

    // --- Поле: Ім'я ---
    QLabel* firstNameLabel = new QLabel("Ім'я:", this);
    firstNameLabel->setStyleSheet(labelStyle);
    hide_firstNameEdit = new QLineEdit(this);
    hide_firstNameEdit->setPlaceholderText("Введіть ім'я");

    // --- Поле: Прізвище ---
    QLabel* lastNameLabel = new QLabel("Прізвище:", this);
    lastNameLabel->setStyleSheet(labelStyle);
    hide_lastNameEdit = new QLineEdit(this);
    hide_lastNameEdit->setPlaceholderText("Введіть прізвище");

    // --- Поле: Дата народження ---
    QLabel* birthDateLabel = new QLabel("Дата народження:", this);
    birthDateLabel->setStyleSheet(labelStyle);
    hide_birthDateEdit = new QDateEdit(this);
    hide_birthDateEdit->setDate(QDate(2000, 1, 1));
    hide_birthDateEdit->setDisplayFormat("dd.MM.yyyy");
    hide_birthDateEdit->setCalendarPopup(true);

    // --- Поле: Телефон ---
    QLabel* phoneLabel = new QLabel("Телефон:", this);
    phoneLabel->setStyleSheet(labelStyle);
    hide_phoneEdit = new QLineEdit(this);
    hide_phoneEdit->setPlaceholderText("+380XXXXXXXXX");

    // Додаємо поля в сітку (2 ряди × 4 колонки)
    personalLayout->addWidget(firstNameLabel, 0, 0);
    personalLayout->addWidget(hide_firstNameEdit, 0, 1);
    personalLayout->addWidget(lastNameLabel, 0, 2);
    personalLayout->addWidget(hide_lastNameEdit, 0, 3);
    personalLayout->addWidget(birthDateLabel, 1, 0);
    personalLayout->addWidget(hide_birthDateEdit, 1, 1);
    personalLayout->addWidget(phoneLabel, 1, 2);
    personalLayout->addWidget(hide_phoneEdit, 1, 3);

    // --- Кнопка: Заповнити прикладом ---
    hide_exampleBtn = new QPushButton("Заповнити прикладом", this);
    hide_exampleBtn->setMinimumHeight(40);
    personalLayout->addWidget(hide_exampleBtn, 2, 0, 1, 2);
    connect(hide_exampleBtn, &QPushButton::clicked,
            this, &SteganographyWindow::fillPersonalDataExample);

    // --- Чекбокс: Шифрування ---
    hide_encryptCheckBox = new QCheckBox(
        "Шифрувати повідомлення перед приховуванням",
        this
        );
    hide_encryptCheckBox->setChecked(true);
    hide_encryptCheckBox->setStyleSheet(
        "QCheckBox { color: #00E5E8; font-weight: bold; }"
        );
    personalLayout->addWidget(hide_encryptCheckBox, 2, 2, 1, 2);

    leftLayout->addWidget(personalGroup);

    // ========== Група 2: Згенерований ключ ==========
    QGroupBox* keyGroup = new QGroupBox(
        "Згенерований ключ шифрування",
        this
        );
    QHBoxLayout* keyLayout = new QHBoxLayout(keyGroup);

    QLabel* keyLabel = new QLabel("Ключ:", this);
    keyLabel->setStyleSheet(labelStyle);
    keyLayout->addWidget(keyLabel);

    // --- Поле відображення ключа ---
    hide_generatedKeyEdit = new QLineEdit(this);
    hide_generatedKeyEdit->setReadOnly(true);
    hide_generatedKeyEdit->setPlaceholderText(
        "Ключ з'явиться після приховування повідомлення"
        );
    hide_generatedKeyEdit->setStyleSheet(
        "QLineEdit {"
        "   background: #2a2a2a;"
        "   border: 2px solid #00CED1;"
        "   color: #00E5E8;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "}"
        );
    keyLayout->addWidget(hide_generatedKeyEdit);

    // --- Кнопка: Копіювати ключ ---
    QPushButton* copyKeyBtn = new QPushButton("Копіювати", this);
    copyKeyBtn->setMinimumWidth(120);
    copyKeyBtn->setToolTip("Копіювати ключ у буфер обміну");
    copyKeyBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:1 #00B8B8);"
        "}"
        "QPushButton:disabled {"
        "   background: #444444;"
        "   color: #888888;"
        "}"
        );

    // Обробник копіювання.
    connect(copyKeyBtn, &QPushButton::clicked, [this]() {
        if (!hide_generatedKeyEdit->text().isEmpty()) {
            QApplication::clipboard()->setText(hide_generatedKeyEdit->text());
            QMessageBox::information(this, "Успіх",
                                     "Ключ скопійовано у буфер обміну!");
        }
    });
    keyLayout->addWidget(copyKeyBtn);

    leftLayout->addWidget(keyGroup);

    // ========== Група 3: Текст для приховування ==========
    QGroupBox* messageGroup = new QGroupBox(
        "Текст для приховування",
        this
        );
    QVBoxLayout* messageLayout = new QVBoxLayout(messageGroup);

    hide_messageEdit = new QTextEdit(this);
    hide_messageEdit->setPlaceholderText(
        "Введіть текст, який потрібно приховати в зображенні..."
        );
    hide_messageEdit->setMinimumHeight(150);
    messageLayout->addWidget(hide_messageEdit);

    leftLayout->addWidget(messageGroup);

    // ========== Кнопки дій ==========
    QHBoxLayout* actionsLayout = new QHBoxLayout();

    // --- Кнопка: Приховати повідомлення ---
    hide_hideBtn = new QPushButton("Приховати повідомлення", this);
    hide_hideBtn->setMinimumHeight(50);
    hide_hideBtn->setStyleSheet(
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
    connect(hide_hideBtn, &QPushButton::clicked,
            this, &SteganographyWindow::hideMessage);
    actionsLayout->addWidget(hide_hideBtn);

    // --- Кнопка: Очистити ---
    hide_clearBtn = new QPushButton("Очистити", this);
    hide_clearBtn->setMinimumHeight(50);
    hide_clearBtn->setStyleSheet(
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
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #b91c1c, stop:1 #7f1d1d);"
        "}"
        );
    connect(hide_clearBtn, &QPushButton::clicked,
            this, &SteganographyWindow::clearHideTab);
    actionsLayout->addWidget(hide_clearBtn);

    leftLayout->addLayout(actionsLayout);
    leftLayout->addStretch();

    // ========== ПРАВА ЧАСТИНА: Зображення ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // ========== Група 1: Вхідне зображення ==========
    QGroupBox* inputImageGroup = new QGroupBox(
        "Вхідне зображення",
        this
        );
    QVBoxLayout* inputImageLayout = new QVBoxLayout(inputImageGroup);

    // Контейнер для центрування зображення.
    QHBoxLayout* inputImageCenterLayout = new QHBoxLayout();
    inputImageCenterLayout->addStretch();

    hide_imageLabel = new QLabel(this);
    hide_imageLabel->setFixedSize(250, 180);
    hide_imageLabel->setStyleSheet(
        "QLabel {"
        "   background: #2a2a2a;"
        "   border: 2px dashed #00CED1;"
        "   border-radius: 10px;"
        "}"
        );
    hide_imageLabel->setAlignment(Qt::AlignCenter);
    hide_imageLabel->setText("Натисніть кнопку\nдля вибору зображення");
    hide_imageLabel->setScaledContents(false);

    inputImageCenterLayout->addWidget(hide_imageLabel);
    inputImageCenterLayout->addStretch();
    inputImageLayout->addLayout(inputImageCenterLayout);

    // Поле шляху до файлу.
    hide_imagePathEdit = new QLineEdit(this);
    hide_imagePathEdit->setReadOnly(true);
    hide_imagePathEdit->setPlaceholderText("Шлях до зображення");
    inputImageLayout->addWidget(hide_imagePathEdit);

    // Кнопка вибору зображення.
    hide_selectImageBtn = new QPushButton("Вибрати зображення", this);
    hide_selectImageBtn->setMinimumHeight(40);
    hide_selectImageBtn->setStyleSheet(
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
    connect(hide_selectImageBtn, &QPushButton::clicked,
            this, &SteganographyWindow::selectInputImage);
    inputImageLayout->addWidget(hide_selectImageBtn);

    rightLayout->addWidget(inputImageGroup);

    // ========== Група 2: Результат ==========
    QGroupBox* resultImageGroup = new QGroupBox(
        "Зображення з прихованим повідомленням",
        this
        );
    QVBoxLayout* resultImageLayout = new QVBoxLayout(resultImageGroup);

    // Контейнер для центрування результату.
    QHBoxLayout* resultImageCenterLayout = new QHBoxLayout();
    resultImageCenterLayout->addStretch();

    hide_resultImageLabel = new QLabel(this);
    hide_resultImageLabel->setFixedSize(250, 180);
    hide_resultImageLabel->setStyleSheet(
        "QLabel {"
        "   background: #2a2a2a;"
        "   border: 2px solid #4ade80;"
        "   border-radius: 10px;"
        "}"
        );
    hide_resultImageLabel->setAlignment(Qt::AlignCenter);
    hide_resultImageLabel->setText("Результат з'явиться\nпісля приховування");
    hide_resultImageLabel->setScaledContents(false);

    resultImageCenterLayout->addWidget(hide_resultImageLabel);
    resultImageCenterLayout->addStretch();
    resultImageLayout->addLayout(resultImageCenterLayout);

    // Прогрес-бар.
    hide_progressBar = new QProgressBar(this);
    hide_progressBar->setVisible(false);
    resultImageLayout->addWidget(hide_progressBar);

    // Кнопка збереження результату.
    QPushButton* saveBtn = new QPushButton("Зберегти результат", this);
    saveBtn->setMinimumHeight(40);
    saveBtn->setEnabled(false);
    saveBtn->setStyleSheet(
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

    // Обробник збереження.
    connect(saveBtn, &QPushButton::clicked, [this, saveBtn]() {
        if (modifiedImage.isNull()) return;

        QString fileName = QFileDialog::getSaveFileName(
            this,
            "Зберегти зображення",
            "",
            "PNG (*.png);;JPEG (*.jpg)"
            );

        if (!fileName.isEmpty()) {
            if (modifiedImage.save(fileName)) {
                QMessageBox::information(this, "Успіх",
                                         "Зображення успішно збережено!");
            }
        }
    });
    resultImageLayout->addWidget(saveBtn);

    // Активація кнопки збереження.
    connect(hide_hideBtn, &QPushButton::clicked, [saveBtn]() {
        saveBtn->setEnabled(true);
    });
    connect(hide_selectImageBtn, &QPushButton::clicked, [saveBtn]() {
        saveBtn->setEnabled(false);
    });

    rightLayout->addWidget(resultImageGroup);
    rightLayout->addStretch();

    // ========== Додаємо дві частини до головного layout ==========
    mainLayout->addLayout(leftLayout, 3);
    mainLayout->addLayout(rightLayout, 2);

    return hideTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 2: ВИТЯГУВАННЯ ПОВІДОМЛЕННЯ
 * ============================================================================
 */

/**
 * @brief Створення вкладки витягування повідомлення.
 *
 * Структура:
 * - Ліва частина: Введення та результати.
 *   - Ключ шифрування (опціонально).
 *   - Витягнуте повідомлення.
 *   - Кнопки дій.
 * - Права частина: Зображення.
 *   - Зображення з прихованим повідомленням.
 *   - Прогрес-бар.
 *
 * @return Віджет вкладки.
 */
QWidget* SteganographyWindow::createExtractTab() {
    QWidget* extractTab = new QWidget();

    // ========== ГОЛОВНИЙ LAYOUT: ГОРИЗОНТАЛЬНИЙ ==========
    QHBoxLayout* mainLayout = new QHBoxLayout(extractTab);
    mainLayout->setSpacing(15);

    QString labelStyle = "QLabel { color: #00E5E8; font-weight: bold; }";

    // ========== ЛІВА ЧАСТИНА: Форми введення ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    // ========== Група 1: Ключ шифрування ==========
    QGroupBox* keyGroup = new QGroupBox(
        "Ключ шифрування (якщо повідомлення зашифроване)",
        this
        );
    QVBoxLayout* keyLayout = new QVBoxLayout(keyGroup);

    // --- Чекбокс: Розшифрувати ---
    extract_decryptCheckBox = new QCheckBox(
        "Розшифрувати повідомлення після витягування",
        this
        );
    extract_decryptCheckBox->setChecked(true);
    extract_decryptCheckBox->setStyleSheet(
        "QCheckBox { color: #00E5E8; font-weight: bold; }"
        );
    keyLayout->addWidget(extract_decryptCheckBox);

    QHBoxLayout* keyInputLayout = new QHBoxLayout();

    QLabel* keyLabel = new QLabel("Ключ:", this);
    keyLabel->setStyleSheet(labelStyle);
    keyInputLayout->addWidget(keyLabel);

    // --- Поле введення ключа ---
    extract_keyEdit = new QLineEdit(this);
    extract_keyEdit->setPlaceholderText(
        "Введіть ключ шифрування (наприклад: A5B3F7E2D9C1A8B4)"
        );
    extract_keyEdit->setToolTip(
        "Залиште порожнім, якщо не знаєте ключ або "
        "повідомлення не було зашифроване"
        );
    keyInputLayout->addWidget(extract_keyEdit);

    // --- Кнопка очищення ключа ---
    QPushButton* clearKeyBtn = new QPushButton("✖", this);
    clearKeyBtn->setMaximumWidth(40);
    clearKeyBtn->setToolTip("Очистити ключ");
    clearKeyBtn->setStyleSheet(
        "QPushButton {"
        "   background: #444444;"
        "   color: #e0e0e0;"
        "   border-radius: 5px;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: #555555;"
        "}"
        );
    connect(clearKeyBtn, &QPushButton::clicked,
            extract_keyEdit, &QLineEdit::clear);
    keyInputLayout->addWidget(clearKeyBtn);

    keyLayout->addLayout(keyInputLayout);
    leftLayout->addWidget(keyGroup);

    // ========== Група 2: Витягнуте повідомлення ==========
    QGroupBox* resultGroup = new QGroupBox(
        "Витягнуте повідомлення",
        this
        );
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);

    extract_messageEdit = new QTextEdit(this);
    extract_messageEdit->setReadOnly(true);
    extract_messageEdit->setPlaceholderText(
        "Тут з'явиться витягнуте повідомлення..."
        );
    extract_messageEdit->setMinimumHeight(300);
    extract_messageEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #999999;"
        "   border: 2px solid #4ade80;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   font-size: 13px;"
        "   font-weight: normal;"
        "}"
        "QTextEdit::placeholder {"
        "   color: #666666;"
        "}"
        );
    resultLayout->addWidget(extract_messageEdit);

    leftLayout->addWidget(resultGroup);

    // ========== Кнопки дій ==========
    QHBoxLayout* actionsLayout = new QHBoxLayout();

    // --- Кнопка: Витягти повідомлення ---
    extract_extractBtn = new QPushButton("Витягти повідомлення", this);
    extract_extractBtn->setMinimumHeight(50);
    extract_extractBtn->setStyleSheet(
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
    connect(extract_extractBtn, &QPushButton::clicked,
            this, &SteganographyWindow::extractMessage);
    actionsLayout->addWidget(extract_extractBtn);

    // --- Кнопка: Очистити ---
    extract_clearBtn = new QPushButton("Очистити", this);
    extract_clearBtn->setMinimumHeight(50);
    extract_clearBtn->setStyleSheet(
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
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #b91c1c, stop:1 #7f1d1d);"
        "}"
        );
    connect(extract_clearBtn, &QPushButton::clicked,
            this, &SteganographyWindow::clearExtractTab);
    actionsLayout->addWidget(extract_clearBtn);

    leftLayout->addLayout(actionsLayout);
    leftLayout->addStretch();

    // ========== ПРАВА ЧАСТИНА: Зображення ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);

    // ========== Група: Зображення з повідомленням ==========
    QGroupBox* imageGroup = new QGroupBox(
        "Зображення з прихованим повідомленням",
        this
        );
    QVBoxLayout* imageLayout = new QVBoxLayout(imageGroup);

    // Контейнер для центрування.
    QHBoxLayout* imageCenterLayout = new QHBoxLayout();
    imageCenterLayout->addStretch();

    extract_imageLabel = new QLabel(this);
    extract_imageLabel->setFixedSize(250, 180);
    extract_imageLabel->setStyleSheet(
        "QLabel {"
        "   background: #2a2a2a;"
        "   border: 2px dashed #a78bfa;"
        "   border-radius: 10px;"
        "}"
        );
    extract_imageLabel->setAlignment(Qt::AlignCenter);
    extract_imageLabel->setText("Натисніть кнопку\nдля вибору зображення");
    extract_imageLabel->setScaledContents(false);

    imageCenterLayout->addWidget(extract_imageLabel);
    imageCenterLayout->addStretch();
    imageLayout->addLayout(imageCenterLayout);

    // Поле шляху.
    extract_imagePathEdit = new QLineEdit(this);
    extract_imagePathEdit->setReadOnly(true);
    extract_imagePathEdit->setPlaceholderText("Шлях до зображення");
    imageLayout->addWidget(extract_imagePathEdit);

    // Кнопка вибору зображення.
    extract_selectImageBtn = new QPushButton("Вибрати зображення", this);
    extract_selectImageBtn->setMinimumHeight(40);
    extract_selectImageBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #a78bfa, stop:1 #8b5cf6);"
        "   color: #000000;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #c4b5fd, stop:1 #a78bfa);"
        "}"
        );
    connect(extract_selectImageBtn, &QPushButton::clicked,
            this, &SteganographyWindow::selectExtractImage);
    imageLayout->addWidget(extract_selectImageBtn);

    rightLayout->addWidget(imageGroup);

    // ========== Прогрес-бар ==========
    extract_progressBar = new QProgressBar(this);
    extract_progressBar->setVisible(false);
    extract_progressBar->setMinimumHeight(30);
    rightLayout->addWidget(extract_progressBar);

    rightLayout->addStretch();

    // ========== Додаємо дві частини до головного layout ==========
    mainLayout->addLayout(leftLayout, 3);
    mainLayout->addLayout(rightLayout, 2);

    return extractTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 3: АНАЛІЗ ЗОБРАЖЕННЯ
 * ============================================================================
 */

/**
 * @brief Створення вкладки аналізу зображення.
 *
 * Структура:
 * - Верхня частина: Два зображення (оригінал + модифіковане).
 * - Кнопка аналізу (на всю ширину).
 * - Інформаційне повідомлення.
 * - Нижня частина: Результати аналізу (таблиця + детальний опис).
 *
 * @return Віджет вкладки.
 */
QWidget* SteganographyWindow::createAnalysisTab() {
    QWidget* analysisTab = new QWidget();

    // ========== ГОЛОВНИЙ LAYOUT: ВЕРТИКАЛЬНИЙ ==========
    QVBoxLayout* mainLayout = new QVBoxLayout(analysisTab);
    mainLayout->setSpacing(15);

    QString labelStyle = "QLabel { color: #00E5E8; font-weight: bold; }";

    // ========== ВЕРХНЯ ЧАСТИНА: Зображення ==========
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(15);

    // ========== Оригінальне зображення ==========
    QVBoxLayout* origLayout = new QVBoxLayout();

    QGroupBox* origGroup = new QGroupBox(
        "Оригінальне зображення",
        this
        );
    QVBoxLayout* origGroupLayout = new QVBoxLayout(origGroup);

    QLabel* origLabel = new QLabel("Оригінальне зображення:", this);
    origLabel->setStyleSheet(labelStyle);
    origLabel->setAlignment(Qt::AlignCenter);
    origGroupLayout->addWidget(origLabel);

    // Контейнер для центрування.
    QHBoxLayout* origImageCenterLayout = new QHBoxLayout();
    origImageCenterLayout->addStretch();

    analysis_originalImageLabel = new QLabel(this);
    analysis_originalImageLabel->setFixedSize(250, 180);
    analysis_originalImageLabel->setStyleSheet(
        "QLabel {"
        "   background: #2a2a2a;"
        "   border: 2px solid #00CED1;"
        "   border-radius: 10px;"
        "}"
        );
    analysis_originalImageLabel->setAlignment(Qt::AlignCenter);
    analysis_originalImageLabel->setText("Оригінальне\nзображення");
    analysis_originalImageLabel->setScaledContents(false);

    origImageCenterLayout->addWidget(analysis_originalImageLabel);
    origImageCenterLayout->addStretch();
    origGroupLayout->addLayout(origImageCenterLayout);

    // Кнопка завантаження оригіналу.
    QPushButton* loadOrigBtn = new QPushButton(
        "Завантажити оригінал",
        this
        );
    loadOrigBtn->setMinimumHeight(40);
    loadOrigBtn->setStyleSheet(
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
    connect(loadOrigBtn, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "Оригінальне зображення",
            "",
            "Images (*.png *.jpg *.jpeg *.bmp)"
            );

        if (!fileName.isEmpty()) {
            originalImage.load(fileName);
            if (!originalImage.isNull()) {
                QPixmap pixmap = QPixmap::fromImage(originalImage);
                analysis_originalImageLabel->setPixmap(
                    pixmap.scaled(analysis_originalImageLabel->size(),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation)
                    );
            }
        }
    });
    origGroupLayout->addWidget(loadOrigBtn);

    origLayout->addWidget(origGroup);
    topLayout->addLayout(origLayout);

    // ========== Модифіковане зображення ==========
    QVBoxLayout* modLayout = new QVBoxLayout();

    QGroupBox* modGroup = new QGroupBox(
        "Модифіковане зображення",
        this
        );
    QVBoxLayout* modGroupLayout = new QVBoxLayout(modGroup);

    QLabel* modLabel = new QLabel("Зображення з повідомленням:", this);
    modLabel->setStyleSheet(labelStyle);
    modLabel->setAlignment(Qt::AlignCenter);
    modGroupLayout->addWidget(modLabel);

    // Контейнер для центрування.
    QHBoxLayout* modImageCenterLayout = new QHBoxLayout();
    modImageCenterLayout->addStretch();

    analysis_modifiedImageLabel = new QLabel(this);
    analysis_modifiedImageLabel->setFixedSize(250, 180);
    analysis_modifiedImageLabel->setStyleSheet(
        "QLabel {"
        "   background: #2a2a2a;"
        "   border: 2px solid #4ade80;"
        "   border-radius: 10px;"
        "}"
        );
    analysis_modifiedImageLabel->setAlignment(Qt::AlignCenter);
    analysis_modifiedImageLabel->setText("Модифіковане\nзображення");
    analysis_modifiedImageLabel->setScaledContents(false);

    modImageCenterLayout->addWidget(analysis_modifiedImageLabel);
    modImageCenterLayout->addStretch();
    modGroupLayout->addLayout(modImageCenterLayout);

    // Кнопка завантаження модифікованого.
    QPushButton* loadModBtn = new QPushButton(
        "Завантажити модифіковане",
        this
        );
    loadModBtn->setMinimumHeight(40);
    loadModBtn->setStyleSheet(
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
        );
    connect(loadModBtn, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "Модифіковане зображення",
            "",
            "Images (*.png *.jpg *.jpeg *.bmp)"
            );

        if (!fileName.isEmpty()) {
            modifiedImage.load(fileName);
            if (!modifiedImage.isNull()) {
                QPixmap pixmap = QPixmap::fromImage(modifiedImage);
                analysis_modifiedImageLabel->setPixmap(
                    pixmap.scaled(analysis_modifiedImageLabel->size(),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation)
                    );
            }
        }
    });
    modGroupLayout->addWidget(loadModBtn);

    modLayout->addWidget(modGroup);
    topLayout->addLayout(modLayout);

    mainLayout->addLayout(topLayout);

    // ========== КНОПКА АНАЛІЗУ ==========
    analyzeBtn = new QPushButton("Провести аналіз", this);
    analyzeBtn->setMinimumHeight(55);
    analyzeBtn->setStyleSheet(
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
    connect(analyzeBtn, &QPushButton::clicked,
            this, &SteganographyWindow::analyzeImage);
    mainLayout->addWidget(analyzeBtn);

    // ========== Інформаційне повідомлення ==========
    QLabel* infoLabel = new QLabel(
        "💡 Для аналізу завантажте оригінальне та модифіковане зображення, "
        "потім натисніть \"Провести аналіз\"",
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
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    // ========== НИЖНЯ ЧАСТИНА: Результати ==========
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(15);

    // ========== Таблиця порівняння ==========
    QGroupBox* tableGroup = new QGroupBox("Порівняльна таблиця", this);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);

    analysisTable = new QTableWidget(this);
    analysisTable->setColumnCount(3);
    analysisTable->setRowCount(8);
    analysisTable->setHorizontalHeaderLabels({
        "Параметр",
        "Оригінал",
        "Модифіковане"
    });

    QStringList rowHeaders = {
        "Розмір (пікселі)",
        "Розмір файлу",
        "Довжина повідомлення",
        "Використана ємність",
        "PSNR (якість)",
        "MSE (помилка)",
        "Змінених пікселів",
        "Візуальна різниця"
    };
    analysisTable->setVerticalHeaderLabels(rowHeaders);

    analysisTable->horizontalHeader()->setStretchLastSection(true);
    analysisTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    analysisTable->setAlternatingRowColors(true);
    analysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    analysisTable->setSelectionMode(QAbstractItemView::SingleSelection);
    analysisTable->setMinimumHeight(150);
    analysisTable->setStyleSheet(
        "QTableWidget {"
        "   background: #2a2a2a;"
        "   border: 2px solid #3a3a3a;"
        "   border-radius: 8px;"
        "   gridline-color: #3a3a3a;"
        "   color: #e0e0e0;"
        "}"
        "QTableWidget::item {"
        "   padding: 8px;"
        "}"
        "QTableWidget::item:selected {"
        "   background: rgba(0, 206, 209, 0.3);"
        "}"
        "QHeaderView::section {"
        "   background: #3a3a3a;"
        "   color: #00E5E8;"
        "   padding: 8px;"
        "   border: 1px solid #4a4a4a;"
        "   font-weight: bold;"
        "}"
        );

    tableLayout->addWidget(analysisTable);
    bottomLayout->addWidget(tableGroup, 1);

    // ========== Детальний аналіз ==========
    QGroupBox* detailsGroup = new QGroupBox("Детальний аналіз", this);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsGroup);

    analysisDetailsEdit = new QTextEdit(this);
    analysisDetailsEdit->setReadOnly(true);
    analysisDetailsEdit->setMinimumHeight(150);
    analysisDetailsEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   border: 2px solid #3a3a3a;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   color: #e0e0e0;"
        "   font-size: 13px;"
        "}"
        );
    detailsLayout->addWidget(analysisDetailsEdit);

    bottomLayout->addWidget(detailsGroup, 1);

    mainLayout->addLayout(bottomLayout);

    return analysisTab;
}

/*
 * ============================================================================
 * ВКЛАДКА 4: ПОКРОКОВИЙ АЛГОРИТМ
 * ============================================================================
 */

/**
 * @brief Створення вкладки покрокового алгоритму.
 *
 * Відображає детальне пояснення алгоритму LSB стеганографії
 * з прикладами та візуалізацією.
 *
 * @return Віджет вкладки.
 */
QWidget* SteganographyWindow::createAlgorithmTab() {
    QWidget* algorithmTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(algorithmTab);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // ========== Група з поясненням алгоритму ==========
    QGroupBox* infoGroup = new QGroupBox(
        "Покроковий алгоритм стеганографії LSB",
        this
        );
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);

    algorithmEdit = new QTextEdit(this);
    algorithmEdit->setReadOnly(true);
    algorithmEdit->setStyleSheet(
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

    infoLayout->addWidget(algorithmEdit);
    layout->addWidget(infoGroup);

    // ========== Відразу показуємо алгоритм ==========
    showStepByStepAlgorithm();

    return algorithmTab;
}
