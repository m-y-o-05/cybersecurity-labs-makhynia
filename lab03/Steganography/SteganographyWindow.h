/*
 * SteganographyWindow.h
 *
 * Заголовочний файл головного вікна програми стеганографії.
 * Містить оголошення класу головного вікна з графічним інтерфейсом.
 *
 * Функціональні можливості:
 * - 4 вкладки: Приховування, Витягування, Аналіз, Алгоритм.
 * - Візуальний попередній перегляд зображень.
 * - Генерація ключів шифрування.
 * - Детальний аналіз результатів.
 */

#ifndef STEGANOGRAPHYWINDOW_H
#define STEGANOGRAPHYWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QDateEdit>
#include <QTabWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QCheckBox>
#include "SteganographyEngine.h"

/**
 * @class SteganographyWindow
 * @brief Головне вікно програми для стеганографії.
 *
 * Надає графічний інтерфейс для роботи зі стеганографією:
 * - Приховування повідомлень в зображеннях.
 * - Витягування повідомлень з зображень.
 * - Аналіз якості стеганографії.
 * - Навчальна інформація про алгоритм.
 */
class SteganographyWindow : public QMainWindow {
    Q_OBJECT

public:
    // ========== Конструктор та деструктор ==========

    /**
     * @brief Конструктор головного вікна.
     * @param parent Батьківський віджет (nullptr за замовчуванням).
     */
    explicit SteganographyWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор.
     *
     * Звільняє виділені ресурси.
     */
    ~SteganographyWindow();

private slots:
    // ========== Обробники подій (Slots) ==========

    /**
     * @brief Вибір вхідного зображення для приховування.
     *
     * Відкриває діалог вибору файлу та завантажує зображення.
     */
    void selectInputImage();

    /**
     * @brief Приховування повідомлення в зображенні.
     *
     * Виконує процес стеганографії:
     * 1. Перевіряє наявність зображення та тексту.
     * 2. Генерує ключ (якщо потрібно шифрування).
     * 3. Приховує повідомлення.
     * 4. Відображає результат.
     */
    void hideMessage();

    /**
     * @brief Вибір зображення для витягування повідомлення.
     *
     * Відкриває діалог вибору файлу.
     */
    void selectExtractImage();

    /**
     * @brief Витягування прихованого повідомлення.
     *
     * Виконує процес витягування:
     * 1. Завантажує зображення.
     * 2. Витягує біти з пікселів.
     * 3. Дешифрує (якщо потрібно).
     * 4. Відображає повідомлення.
     */
    void extractMessage();

    /**
     * @brief Аналіз змін в зображенні.
     *
     * Порівнює оригінальне та модифіковане зображення:
     * - Обчислює PSNR, MSE.
     * - Підраховує змінені пікселі.
     * - Розраховує використання ємності.
     */
    void analyzeImage();

    /**
     * @brief Заповнення форми прикладом персональних даних.
     *
     * Автоматично заповнює поля для демонстрації роботи програми.
     */
    void fillPersonalDataExample();

    /**
     * @brief Показ покрокового алгоритму стеганографії.
     *
     * Відображає детальне пояснення алгоритму LSB.
     */
    void showStepByStepAlgorithm();

    /**
     * @brief Очищення всіх полів вкладки приховування.
     */
    void clearHideTab();

    /**
     * @brief Очищення всіх полів вкладки витягування.
     */
    void clearExtractTab();

private:
    // ========== Методи створення UI ==========

    /**
     * @brief Налаштування головного інтерфейсу.
     *
     * Створює всі вкладки та елементи управління.
     */
    void setupUI();

    /**
     * @brief Створення вкладки приховування повідомлення.
     * @return Віджет вкладки.
     */
    QWidget* createHideTab();

    /**
     * @brief Створення вкладки витягування повідомлення.
     * @return Віджет вкладки.
     */
    QWidget* createExtractTab();

    /**
     * @brief Створення вкладки аналізу зображення.
     * @return Віджет вкладки.
     */
    QWidget* createAnalysisTab();

    /**
     * @brief Створення вкладки покрокового алгоритму.
     * @return Віджет вкладки.
     */
    QWidget* createAlgorithmTab();

    // ========== Допоміжні методи ==========

    /**
     * @brief Застосування темної теми з бірюзовими акцентами.
     *
     * Встановлює CSS-стилі для всіх елементів інтерфейсу.
     */
    void applyDarkTheme();

    /**
     * @brief Відображення попереднього перегляду зображення.
     * @param label Мітка для відображення.
     * @param image Зображення для показу.
     * @param info Додаткова інформація.
     */
    void displayImagePreview(QLabel* label, const QImage& image, const QString& info);

    /**
     * @brief Відображення результатів аналізу в таблиці.
     * @param result Структура з результатами.
     */
    void displayAnalysisResults(const ImageAnalysisResult& result);

    /**
     * @brief Створення іконки програми.
     * @return Іконка у вигляді QIcon.
     */
    QIcon createProgramIcon();

    // ========== UI елементи ==========

    /// Головний віджет з вкладками.
    QTabWidget* tabWidget;

    // ===== ВКЛАДКА 1: ПРИХОВУВАННЯ =====

    /// Поле введення імені.
    QLineEdit* hide_firstNameEdit;

    /// Поле введення прізвища.
    QLineEdit* hide_lastNameEdit;

    /// Поле вибору дати народження.
    QDateEdit* hide_birthDateEdit;

    /// Поле введення телефону.
    QLineEdit* hide_phoneEdit;

    /// Поле введення повідомлення для приховування.
    QTextEdit* hide_messageEdit;

    /// Мітка для показу вхідного зображення.
    QLabel* hide_imageLabel;

    /// Мітка для показу результату (модифікованого зображення).
    QLabel* hide_resultImageLabel;

    /// Кнопка вибору вхідного зображення.
    QPushButton* hide_selectImageBtn;

    /// Кнопка приховування повідомлення.
    QPushButton* hide_hideBtn;

    /// Кнопка заповнення прикладом.
    QPushButton* hide_exampleBtn;

    /// Кнопка очищення форми.
    QPushButton* hide_clearBtn;

    /// Поле показу шляху до зображення.
    QLineEdit* hide_imagePathEdit;

    /// Чекбокс "Шифрувати повідомлення".
    QCheckBox* hide_encryptCheckBox;

    /// Поле показу згенерованого ключа.
    QLineEdit* hide_generatedKeyEdit;

    /// Прогрес-бар процесу приховування.
    QProgressBar* hide_progressBar;

    // ===== ВКЛАДКА 2: ВИТЯГУВАННЯ =====

    /// Мітка для показу зображення з повідомленням.
    QLabel* extract_imageLabel;

    /// Поле показу витягнутого повідомлення.
    QTextEdit* extract_messageEdit;

    /// Кнопка вибору зображення.
    QPushButton* extract_selectImageBtn;

    /// Кнопка витягування повідомлення.
    QPushButton* extract_extractBtn;

    /// Кнопка очищення форми.
    QPushButton* extract_clearBtn;

    /// Поле показу шляху до зображення.
    QLineEdit* extract_imagePathEdit;

    /// Чекбокс "Розшифрувати повідомлення".
    QCheckBox* extract_decryptCheckBox;

    /// Поле введення ключа дешифрування.
    QLineEdit* extract_keyEdit;

    /// Прогрес-бар процесу витягування.
    QProgressBar* extract_progressBar;

    // ===== ВКЛАДКА 3: АНАЛІЗ ЗОБРАЖЕННЯ =====

    /// Таблиця порівняльних даних.
    QTableWidget* analysisTable;

    /// Мітка оригінального зображення.
    QLabel* analysis_originalImageLabel;

    /// Мітка модифікованого зображення.
    QLabel* analysis_modifiedImageLabel;

    /// Текстове поле детального аналізу.
    QTextEdit* analysisDetailsEdit;

    /// Кнопка запуску аналізу.
    QPushButton* analyzeBtn;

    // ===== ВКЛАДКА 4: ПОКРОКОВИЙ АЛГОРИТМ =====

    /// Текстове поле з поясненням алгоритму.
    QTextEdit* algorithmEdit;

    // ========== Дані програми ==========

    /// Об'єкт движка стеганографії.
    SteganographyEngine* stegoEngine;

    /// Оригінальне зображення.
    QImage originalImage;

    /// Модифіковане зображення (з повідомленням).
    QImage modifiedImage;

    /// Шлях до поточного зображення (приховування).
    QString currentImagePath;

    /// Шлях до зображення для витягування.
    QString extractImagePath;
};

#endif // STEGANOGRAPHYWINDOW_H
