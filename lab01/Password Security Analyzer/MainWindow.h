/*
 * MainWindow.h.
 *
 * Заголовочний файл головного вікна програми.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QDateEdit>
#include <QProgressBar>
#include "PasswordAnalyzer.h"

/**
 * @class MainWindow.
 * @brief Головне вікно програми для аналізу безпеки паролів.
 *
 * Клас фокусується на логіці UI та взаємодії з користувачем.
 * Стилі, звіти та іконки винесені в окремі класи.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Конструктор головного вікна.
     * @param parent Батьківський віджет (за замовчуванням nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор головного вікна.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Слот для аналізу введеного пароля.
     */
    void analyzePassword();

    /**
     * @brief Слот для очищення результатів аналізу.
     */
    void clearResults();

    /**
     * @brief Слот для заповнення полів прикладами даних.
     */
    void fillExampleData();

private:
    /**
     * @brief Налаштування користувацького інтерфейсу.
     */
    void setupUI();

    /**
     * @brief Відображення результатів аналізу.
     * @param result Структура з результатами аналізу пароля.
     */
    void displayResults(const AnalysisResult& result);

    // ============ UI елементи ============

    QLineEdit* passwordEdit;        ///< Поле для вводу пароля.
    QLineEdit* firstNameEdit;       ///< Поле для вводу імені.
    QLineEdit* lastNameEdit;        ///< Поле для вводу прізвища.
    QDateEdit* birthDateEdit;       ///< Поле для вводу дати народження.
    QLineEdit* emailEdit;           ///< Поле для вводу email.

    QPushButton* analyzeButton;     ///< Кнопка аналізу пароля.
    QPushButton* clearButton;       ///< Кнопка очищення результатів.
    QPushButton* exampleButton;     ///< Кнопка заповнення прикладом.

    QTextEdit* resultsText;         ///< Текстове поле з результатами.
    QLabel* scoreLabel;             ///< Мітка з оцінкою.
    QProgressBar* scoreBar;         ///< Прогрес-бар надійності.
    QLabel* strengthLabel;          ///< Мітка з рівнем надійності.

    PasswordAnalyzer* analyzer;     ///< Об'єкт для аналізу паролів.
};

#endif // MAINWINDOW_H
