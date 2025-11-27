/*
 * MainWindow.h
 *
 * Реалізація головного вікна застосунку/
 * UI для демонстрації SQL-ін'єкцій.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QLabel>
#include <QGroupBox>
#include <QIcon>
#include "DatabaseManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // ========== UI КОМПОНЕНТИ ==========
    QTabWidget* tabWidget;

    // --- ВКЛАДКА 1: Авторизація ---
    QLineEdit* login_usernameEdit;
    QLineEdit* login_passwordEdit;
    QPushButton* login_vulnerableBtn;
    QPushButton* login_secureBtn;
    QTextEdit* login_resultEdit;

    // --- ВКЛАДКА 2: Пошук студентів ---
    QLineEdit* search_termEdit;
    QPushButton* search_vulnerableBtn;
    QPushButton* search_secureBtn;
    QTableWidget* search_resultsTable;
    QLabel* search_infoLabel;

    // --- ВКЛАДКА 3: Фільтрація ---
    QComboBox* filter_facultyCombo;
    QLineEdit* filter_customEdit;
    QPushButton* filter_vulnerableBtn;
    QPushButton* filter_secureBtn;
    QTableWidget* filter_resultsTable;

    // --- ВКЛАДКА 4: Коментарі ---
    QLineEdit* comment_studentIdEdit;
    QTextEdit* comment_textEdit;
    QComboBox* comment_ratingCombo;
    QPushButton* comment_vulnerableBtn;
    QPushButton* comment_secureBtn;
    QTextEdit* comment_resultEdit;

    // --- ВКЛАДКА 5: Логи атак ---
    QTableWidget* logs_tableWidget;
    QPushButton* logs_refreshBtn;
    QPushButton* logs_clearBtn;
    QTextEdit* logs_statisticsEdit;
    QComboBox* logs_filterCombo;

    // --- ВКЛАДКА 6: Інформація ---
    QTextEdit* info_textEdit;

    // ========== МЕТОДИ СТВОРЕННЯ UI ==========
    void setupUI();
    void applyModernDarkTheme();
    QIcon createProgramIcon();

    QWidget* createLoginTab();
    QWidget* createSearchTab();
    QWidget* createFilterTab();
    QWidget* createCommentsTab();
    QWidget* createLogsTab();
    QWidget* createInfoTab();

    // ========== ДОПОМІЖНІ МЕТОДИ ==========
    void displayStudentsInTable(const QVector<Student>& students,
                                QTableWidget* table);

private slots:
    // --- Обробники авторизації ---
    void onVulnerableLogin();
    void onSecureLogin();

    // --- Обробники пошуку ---
    void onVulnerableSearch();
    void onSecureSearch();

    // --- Обробники фільтрації ---
    void onVulnerableFilter();
    void onSecureFilter();

    // --- Обробники коментарів ---
    void onVulnerableComment();
    void onSecureComment();

    // --- Обробники логів ---
    void onRefreshLogs();
    void onClearLogs();
    void onFilterLogs(int index);
};

#endif // MAINWINDOW_H
