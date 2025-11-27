/*
 * MainWindow.cpp.
 *
 * Реалізація головного вікна програми.
 */

#include "MainWindow.h"
#include "UIStyleManager.h"
#include "ReportGenerator.h"
#include "IconGenerator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFont>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>

/**
 * @brief Конструктор головного вікна.
 * @param parent Батьківський віджет.
 *
 * Ініціалізує всі компоненти вікна:
 * 1. Створює аналізатор паролів.
 * 2. Налаштовує інтерфейс користувача.
 * 3. Встановлює заголовок та розмір вікна.
 * 4. Застосовує іконку програми.
 * 5. Застосовує темну тему через UIStyleManager.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), analyzer(new PasswordAnalyzer()) {

    // ============ Налаштування UI ============
    setupUI();

    // ============ Налаштування вікна ============
    setWindowTitle("Аналізатор безпеки паролів");
    resize(900, 600);

    // ============ Встановлення іконки через IconGenerator ============
    setWindowIcon(IconGenerator::createProgramIcon());

    // ============ Застосування головної теми через UIStyleManager ============
    setStyleSheet(UIStyleManager::getMainStyleSheet());
}

/**
 * @brief Деструктор головного вікна.
 *
 * Звільняє пам'ять, виділену під аналізатор паролів.
 * Всі UI елементи видаляються автоматично Qt-ієрархією.
 */
MainWindow::~MainWindow() {
    delete analyzer;
}

/**
 * @brief Налаштування користувацького інтерфейсу.
 *
 * Головний метод створення UI, який:
 * 1. Створює центральний віджет та головний layout.
 * 2. Додає заголовок програми з тінню.
 * 3. Створює групу полів вводу.
 * 4. Створює панель кнопок.
 * 5. Створює групу результатів.
 *
 * Всі елементи розміщуються у вертикальному layout
 * з відступами 20px та проміжками 15px.
 */
void MainWindow::setupUI() {
    // ============ Створення центрального віджета ============
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // ============ Головний вертикальний layout ============
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ============ Заголовок програми ============
    QLabel* titleLabel = new QLabel("Аналізатор безпеки паролів", this);

    // Налаштування шрифту заголовка.
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Стилізація заголовка з бірюзовим градієнтом.
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:0.5 #00CED1, stop:1 #00B8BA);"
        "   padding: 15px;"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 rgba(0, 206, 209, 0.1), stop:1 rgba(0, 184, 186, 0.1));"
        "   border-radius: 15px;"
        "   font-size: 24pt;"
        "   font-weight: bold;"
        "}"
        );

    // ============ Додавання тіні до заголовка ============
    QGraphicsDropShadowEffect* titleShadow = new QGraphicsDropShadowEffect();
    titleShadow->setBlurRadius(20);
    titleShadow->setColor(QColor(0, 206, 209, 100));
    titleShadow->setOffset(0, 5);
    titleLabel->setGraphicsEffect(titleShadow);

    mainLayout->addWidget(titleLabel);

    // ============ Група введення даних ============
    QGroupBox* inputGroup = new QGroupBox("Введіть дані для аналізу", this);
    QGridLayout* inputLayout = new QGridLayout(inputGroup);
    inputLayout->setSpacing(12);
    inputLayout->setVerticalSpacing(15);

    QString labelStyle = UIStyleManager::getLabelStyle();

    // ============ Поле пароля з кнопкою показу/приховування ============
    QLabel* passwordLabel = new QLabel("Пароль:", this);
    passwordLabel->setStyleSheet(labelStyle);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Введіть пароль для аналізу");

    // Кнопка показу/приховування пароля.
    QPushButton* showPasswordButton = new QPushButton("🔒", this);
    showPasswordButton->setMaximumWidth(50);
    showPasswordButton->setToolTip("Показати/приховати пароль");

    // Застосування стилю через UIStyleManager.
    showPasswordButton->setStyleSheet(UIStyleManager::getIconButtonStyle());

    // Підключення обробника для зміни режиму відображення пароля.
    connect(showPasswordButton, &QPushButton::clicked, [this, showPasswordButton]() {
        if (passwordEdit->echoMode() == QLineEdit::Password) {
            passwordEdit->setEchoMode(QLineEdit::Normal);
            showPasswordButton->setText("🔓");
        } else {
            passwordEdit->setEchoMode(QLineEdit::Password);
            showPasswordButton->setText("🔒");
        }
    });

    inputLayout->addWidget(passwordLabel, 0, 0);
    inputLayout->addWidget(passwordEdit, 0, 1);
    inputLayout->addWidget(showPasswordButton, 0, 2);

    // ============ Поле для імені ============
    QLabel* firstNameLabel = new QLabel("Ім'я:", this);
    firstNameLabel->setStyleSheet(labelStyle);
    firstNameEdit = new QLineEdit(this);
    firstNameEdit->setPlaceholderText("Іван");
    inputLayout->addWidget(firstNameLabel, 1, 0);
    inputLayout->addWidget(firstNameEdit, 1, 1, 1, 2);

    // ============ Поле для прізвища ============
    QLabel* lastNameLabel = new QLabel("Прізвище:", this);
    lastNameLabel->setStyleSheet(labelStyle);
    lastNameEdit = new QLineEdit(this);
    lastNameEdit->setPlaceholderText("Петренко");
    inputLayout->addWidget(lastNameLabel, 2, 0);
    inputLayout->addWidget(lastNameEdit, 2, 1, 1, 2);

    // ============ Поле для дати народження ============
    QLabel* birthDateLabel = new QLabel("Дата народження:", this);
    birthDateLabel->setStyleSheet(labelStyle);
    birthDateEdit = new QDateEdit(this);
    birthDateEdit->setDate(QDate(2000, 1, 1));
    birthDateEdit->setDisplayFormat("dd.MM.yyyy");
    birthDateEdit->setCalendarPopup(true);
    inputLayout->addWidget(birthDateLabel, 3, 0);
    inputLayout->addWidget(birthDateEdit, 3, 1, 1, 2);

    // ============ Поле для email (опціонально) ============
    QLabel* emailLabel = new QLabel("Email (опціонально):", this);
    emailLabel->setStyleSheet(labelStyle);
    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("example@email.com");
    inputLayout->addWidget(emailLabel, 4, 0);
    inputLayout->addWidget(emailEdit, 4, 1, 1, 2);

    mainLayout->addWidget(inputGroup);

    // ============ Панель кнопок ============
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    // Головна кнопка "Аналізувати пароль".
    analyzeButton = new QPushButton("Аналізувати пароль", this);
    analyzeButton->setMinimumHeight(45);

    // Застосування стилю.
    analyzeButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:0.5 #00B8BA, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 15px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:0.5 #00CED1, stop:1 #00B8BA);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00B8BA, stop:0.5 #009999, stop:1 #007F82);"
        "}"
        );

    QGraphicsDropShadowEffect* buttonShadow = new QGraphicsDropShadowEffect();
    buttonShadow->setBlurRadius(15);
    buttonShadow->setColor(QColor(0, 206, 209, 150));
    buttonShadow->setOffset(0, 5);
    analyzeButton->setGraphicsEffect(buttonShadow);

    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzePassword);

    // Кнопка "Приклад".
    exampleButton = new QPushButton("Приклад", this);
    exampleButton->setMinimumHeight(45);
    exampleButton->setStyleSheet(UIStyleManager::getSecondaryButtonStyle());
    connect(exampleButton, &QPushButton::clicked, this, &MainWindow::fillExampleData);

    // Кнопка "Очистити".
    clearButton = new QPushButton("Очистити", this);
    clearButton->setMinimumHeight(45);
    clearButton->setStyleSheet(UIStyleManager::getDangerButtonStyle());
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearResults);

    buttonLayout->addWidget(analyzeButton, 3);
    buttonLayout->addWidget(exampleButton, 1);
    buttonLayout->addWidget(clearButton, 1);

    mainLayout->addLayout(buttonLayout);

    // ============ Група результатів ============
    QGroupBox* resultsGroup = new QGroupBox("Результати аналізу", this);
    QVBoxLayout* resultsLayout = new QVBoxLayout(resultsGroup);

    // ============ Панель оцінки з прогрес-баром ============
    QHBoxLayout* scoreLayout = new QHBoxLayout();

    scoreLabel = new QLabel("Оцінка: --/10", this);
    QFont scoreFont;
    scoreFont.setPointSize(16);
    scoreFont.setBold(true);
    scoreLabel->setFont(scoreFont);
    scoreLabel->setStyleSheet("QLabel { color: #00CED1; }");

    scoreBar = new QProgressBar(this);
    scoreBar->setMinimum(0);
    scoreBar->setMaximum(10);
    scoreBar->setValue(0);
    scoreBar->setTextVisible(true);
    scoreBar->setMinimumHeight(35);

    strengthLabel = new QLabel("Рівень: --", this);
    strengthLabel->setFont(scoreFont);
    strengthLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    strengthLabel->setStyleSheet("QLabel { color: #00E5E8; }");

    scoreLayout->addWidget(scoreLabel);
    scoreLayout->addWidget(scoreBar, 1);
    scoreLayout->addWidget(strengthLabel);
    scoreLayout->setAlignment(Qt::AlignVCenter);

    resultsLayout->addLayout(scoreLayout);

    // ============ Текстове поле результатів ============
    resultsText = new QTextEdit(this);
    resultsText->setReadOnly(true);
    resultsText->setMinimumHeight(180);
    resultsText->setMaximumHeight(250);
    resultsText->setPlaceholderText("Введіть дані та натисніть 'Аналізувати пароль' для отримання результатів...");
    resultsLayout->addWidget(resultsText);

    mainLayout->addWidget(resultsGroup);
}

/**
 * @brief Слот для заповнення форми тестовими даними.
 *
 * Заповнює всі поля форми прикладом даних для демонстрації
 * роботи програми та швидкого тестування.
 *
 * Приклад даних:
 * - Пароль: "ivan1995" (слабкий пароль з персональними даними).
 * - Ім'я: "Іван".
 * - Прізвище: "Петренко".
 * - Дата народження: 15.03.1995.
 * - Email: "ivan.petrenko@example.com".
 */
void MainWindow::fillExampleData() {
    passwordEdit->setText("ivan1995");
    firstNameEdit->setText("Іван");
    lastNameEdit->setText("Петренко");
    birthDateEdit->setDate(QDate(1995, 3, 15));
    emailEdit->setText("ivan.petrenko@example.com");
}

/**
 * @brief Слот для очищення всіх полів та результатів.
 *
 * Виконує повне скидання форми до початкового стану:
 *
 * 1. Очищення полів вводу:
 *    - Пароль, ім'я, прізвище, email → порожні рядки.
 *    - Дата народження → 01.01.2000 (значення за замовчуванням).
 *
 * 2. Очищення результатів:
 *    - Текстове поле результатів → порожнє.
 *    - Мітка оцінки → "Оцінка: --/10".
 *    - Прогрес-бар → 0.
 *    - Мітка рівня → "Рівень: --".
 *
 * 3. Встановлення фокусу на поле пароля для зручності користувача.
 */
void MainWindow::clearResults() {
    // ============ Очищення полів вводу ============
    passwordEdit->clear();
    firstNameEdit->clear();
    lastNameEdit->clear();
    birthDateEdit->setDate(QDate(2000, 1, 1));
    emailEdit->clear();

    // ============ Очищення результатів ============
    resultsText->clear();
    scoreLabel->setText("Оцінка: --/10");
    scoreBar->setValue(0);
    strengthLabel->setText("Рівень: --");

    // ============ Встановлення фокусу на перше поле ============
    passwordEdit->setFocus();
}

/**
 * @brief Слот для аналізу введеного пароля.
 *
 * Головний метод обробки запиту на аналіз пароля:
 *
 * 1. Перевірка на порожній пароль:
 *    - Якщо пароль не введено, показується стилізоване
 *      повідомлення з підказкою.
 *
 * 2. Збір даних з форми:
 *    - Пароль, ім'я, прізвище, дата народження, email.
 *
 * 3. Виклик аналізатора:
 *    - analyzer->analyzePassword(...) виконує всю логіку аналізу.
 *
 * 4. Відображення результатів:
 *    - displayResults(result) форматує та виводить результати.
 *
 * Метод використовує UIStyleManager для стилізації повідомлення про помилку.
 */
void MainWindow::analyzePassword() {
    QString password = passwordEdit->text();

    // ============ Перевірка на порожній пароль ============
    if (password.isEmpty()) {
        // Створення стилізованого повідомлення про помилку.
        QMessageBox messageBox(this);
        messageBox.setWindowTitle("Помилка введення");
        messageBox.setWindowIcon(IconGenerator::createProgramIcon());

        // Заголовок повідомлення (HTML з бірюзовим кольором).
        messageBox.setText("<h3 style='color: #00CED1;'>Пароль не введено</h3>");

        // Текст повідомлення з підказкою.
        messageBox.setInformativeText(
            "<p style='color: #ffffff;'>Будь ласка, введіть пароль у відповідне поле "
            "для початку аналізу.</p>"
            "<p style='color: #00CED1; font-size: 11px;'><i>Підказка: Ви також можете "
            "натиснути кнопку \"Приклад\" для швидкого тестування.</i></p>"
            );

        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);

        // Застосування темної теми до повідомлення через UIStyleManager.
        messageBox.setStyleSheet(UIStyleManager::getMessageBoxStyle());

        messageBox.exec();
        passwordEdit->setFocus();
        return;
    }

    // ============ Збір даних з форми ============
    QString firstName = firstNameEdit->text();
    QString lastName = lastNameEdit->text();
    QDate birthDate = birthDateEdit->date();
    QString email = emailEdit->text();

    // ============ Виклик аналізатора паролів ============
    // analyzer виконує всю логіку перевірки безпеки.
    AnalysisResult result = analyzer->analyzePassword(
        password,
        firstName,
        lastName,
        birthDate,
        email
        );

    // ============ Відображення результатів аналізу ============
    displayResults(result);
}

/**
 * @brief Відображення результатів аналізу пароля.
 * @param result Структура з результатами аналізу.
 *
 * Метод оновлює всі елементи UI результатами аналізу:
 *
 * 1. Оновлення мітки оцінки:
 *    - Використовує ReportGenerator::formatScore для додавання емоджі.
 *
 * 2. Оновлення прогрес-бару:
 *    - Встановлює значення (0-10).
 *    - Застосовує колір через UIStyleManager::getProgressBarStyle.
 *
 * 3. Оновлення мітки рівня надійності:
 *    - Текстовий опис: "Дуже слабкий", "Слабкий", "Середній",
 *      "Надійний", "Дуже надійний".
 *
 * 4. Генерація HTML-звіту:
 *    - ReportGenerator::generateHTMLReport створює детальний звіт.
 *    - Звіт виводиться у resultsText.
 *
 * 5. Прокрутка до початку результатів для зручності читання.
 */
void MainWindow::displayResults(const AnalysisResult& result) {
    // ============ Оновлення мітки оцінки з емоджі ============
    scoreLabel->setText(ReportGenerator::formatScore(result.score));

    // ============ Оновлення прогрес-бару ============
    scoreBar->setValue(result.score);

    // Встановлення кольору прогрес-бару залежно від оцінки.
    scoreBar->setStyleSheet(UIStyleManager::getProgressBarStyle(result.score));

    // ============ Оновлення мітки рівня надійності ============
    strengthLabel->setText(QString("Рівень: %1").arg(result.strengthLevel));

    // ============ Генерація та відображення HTML-звіту ============
    QString report = ReportGenerator::generateHTMLReport(result);
    resultsText->setHtml(report);

    // ============ Прокрутка до початку результатів ============
    resultsText->verticalScrollBar()->setValue(0);
}
