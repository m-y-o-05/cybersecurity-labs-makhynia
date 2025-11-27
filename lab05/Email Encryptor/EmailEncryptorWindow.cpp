/*
 * EmailEncryptorWindow.cpp
 *
 * Основний файл головного вікна Email-шифратора.
 * Містить конструктор, деструктор та базову ініціалізацію.
 */

#include "EmailEncryptorWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QPixmap>
#include <QClipboard>
#include <QApplication>

/*
 * ============================================================================
 * КОНСТРУКТОР ТА ДЕСТРУКТОР
 * ============================================================================
 */

/**
 * @brief Конструктор головного вікна.
 */
EmailEncryptorWindow::EmailEncryptorWindow(QWidget *parent)
    : QMainWindow(parent),
    encryptionEngine(new EncryptionEngine())
{
    // Створюємо та налаштовуємо UI.
    setupUI();

    // Застосовуємо темну тему оформлення.
    applyDarkTheme();

    // Встановлюємо заголовок вікна.
    setWindowTitle("Email-шифратор - Безпечна комунікація");

    // Встановлюємо іконку програми.
    setWindowIcon(createProgramIcon());

    // Встановлюємо початковий розмір вікна.
    resize(1400, 850);
}

/**
 * @brief Деструктор.
 */
EmailEncryptorWindow::~EmailEncryptorWindow() {
    delete encryptionEngine;
}

/*
 * ============================================================================
 * НАЛАШТУВАННЯ UI
 * ============================================================================
 */

/**
 * @brief Налаштування головного інтерфейсу.
 */
void EmailEncryptorWindow::setupUI() {
    // Створюємо центральний віджет.
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Створюємо головний вертикальний layout.
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ========== Заголовок програми ==========
    QLabel* titleLabel = new QLabel(
        "Email-шифратор - Безпечна комунікація",
        this
        );

    // Налаштовуємо шрифт заголовка.
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Стилізуємо заголовок.
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #00CED1;"
        "   padding: 15px;"
        "   background: rgba(0, 206, 209, 0.1);"
        "   border-radius: 10px;"
        "}"
        );

    mainLayout->addWidget(titleLabel);

    // ========== Створення віджета з вкладками ==========
    tabWidget = new QTabWidget(this);

    // Додаємо всі 4 вкладки.
    tabWidget->addTab(createKeyTab(), "Генерація ключа");
    tabWidget->addTab(createEncryptTab(), "Шифрування");
    tabWidget->addTab(createDecryptTab(), "Розшифрування");
    tabWidget->addTab(createStepByStepTab(), "Покроковий алгоритм");

    mainLayout->addWidget(tabWidget);
}

/*
 * ============================================================================
 * СТВОРЕННЯ ІКОНКИ ПРОГРАМИ
 * ============================================================================
 */

/**
 * @brief Створення іконки програми.
 */
QIcon EmailEncryptorWindow::createProgramIcon() {
    // Створюємо pixmap 128x128 з прозорим фоном.
    QPixmap pixmap(128, 128);
    pixmap.fill(Qt::transparent);

    // Створюємо об'єкт для малювання.
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // ========== Малюємо бірюзове коло з градієнтом ==========
    QRadialGradient gradient(64, 64, 64);
    gradient.setColorAt(0, QColor(0, 230, 232));
    gradient.setColorAt(0.5, QColor(0, 206, 209));
    gradient.setColorAt(1, QColor(0, 153, 153));

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(8, 8, 112, 112);

    // ========== Малюємо емодзі замка ==========
    painter.setPen(QPen(QColor("#1a1a1a"), 6));
    painter.setFont(QFont("Arial", 48, QFont::Bold));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "🔐");

    return QIcon(pixmap);
}
