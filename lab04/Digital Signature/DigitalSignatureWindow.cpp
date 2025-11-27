/*
 * DigitalSignatureWindow.cpp
 *
 * Основний файл головного вікна програми цифрових підписів.
 * Містить конструктор, деструктор та базову ініціалізацію.
 */

#include "DigitalSignatureWindow.h"
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
 *
 * Виконує повну ініціалізацію вікна:
 * 1. Створює об'єкт движка цифрових підписів.
 * 2. Налаштовує графічний інтерфейс.
 * 3. Застосовує темну тему.
 * 4. Встановлює параметри вікна.
 *
 * @param parent Батьківський віджет.
 */
DigitalSignatureWindow::DigitalSignatureWindow(QWidget *parent)
    : QMainWindow(parent),
    signatureEngine(new DigitalSignatureEngine())
{
    // Створюємо та налаштовуємо UI.
    setupUI();

    // Застосовуємо темну тему оформлення.
    applyDarkTheme();

    // Встановлюємо заголовок вікна.
    setWindowTitle("Цифрові підписи - Система електронного підпису документів");

    // Встановлюємо іконку програми.
    setWindowIcon(createProgramIcon());

    // Встановлюємо початковий розмір вікна.
    resize(1400, 800);
}

/**
 * @brief Деструктор.
 *
 * Звільняє виділену пам'ять.
 */
DigitalSignatureWindow::~DigitalSignatureWindow() {
    // Видаляємо об'єкт движка.
    delete signatureEngine;
}

/*
 * ============================================================================
 * НАЛАШТУВАННЯ UI
 * ============================================================================
 */

/**
 * @brief Налаштування головного інтерфейсу.
 *
 * Створює:
 * - Центральний віджет.
 * - Заголовок програми.
 * - Віджет з вкладками (tabs).
 * - Всі 4 вкладки: Генерація ключів, Створення підпису, Перевірка, Алгоритм.
 */
void DigitalSignatureWindow::setupUI() {
    // Створюємо центральний віджет.
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Створюємо головний вертикальний layout.
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ========== Заголовок програми ==========
    QLabel* titleLabel = new QLabel(
        "Цифрові підписи - Система електронного підпису документів",
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
    tabWidget->addTab(createKeysTab(), "Генерація ключів");
    tabWidget->addTab(createSignTab(), "Створення підпису");
    tabWidget->addTab(createVerifyTab(), "Перевірка підпису");
    tabWidget->addTab(createAlgorithmTab(), "Покроковий алгоритм");

    mainLayout->addWidget(tabWidget);
}

/*
 * ============================================================================
 * СТВОРЕННЯ ІКОНКИ ПРОГРАМИ
 * ============================================================================
 */

/**
 * @brief Створення іконки програми.
 *
 * Малює іконку з бірюзовим градієнтом та емодзі замка.
 *
 * @return Іконка програми.
 */
QIcon DigitalSignatureWindow::createProgramIcon() {
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

/*
 * ============================================================================
 * ОБРОБНИК ЗМІНИ АЛГОРИТМУ ХЕШУВАННЯ
 * ============================================================================
 */

/**
 * @brief Обробник зміни алгоритму хешування.
 *
 * Викликається при виборі нового алгоритму в ComboBox.
 *
 * @param index Індекс вибраного алгоритму (0=SHA256, 1=SHA512, 2=MD5).
 */
void DigitalSignatureWindow::onHashAlgorithmChanged(int index) {
    HashAlgorithm algorithm;

    switch (index) {
    case 0:
        algorithm = HashAlgorithm::SHA256;
        break;
    case 1:
        algorithm = HashAlgorithm::SHA512;
        break;
    case 2:
        algorithm = HashAlgorithm::MD5;
        break;
    default:
        algorithm = HashAlgorithm::SHA256;
    }

    // Встановлюємо новий алгоритм.
    signatureEngine->setHashAlgorithm(algorithm);

    // Показуємо повідомлення.
    QString algorithmName = DigitalSignatureEngine::getAlgorithmName(algorithm);

    // Попередження для MD5.
    if (algorithm == HashAlgorithm::MD5) {
        QMessageBox::warning(
            this,
            "Попередження",
            "MD5 є застарілим алгоритмом!\n\n"
            "MD5 вважається криптографічно ненадійним і не рекомендується "
            "для використання в реальних системах.\n\n"
            "Використовується тільки для демонстраційних цілей."
            );
    }

    qDebug() << "Hash algorithm changed to:" << algorithmName;
}
