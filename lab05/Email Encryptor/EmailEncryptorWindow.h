/*
 * EmailEncryptorWindow.h
 *
 * Заголовочний файл головного вікна Email-шифратора.
 * Містить оголошення класу головного вікна з графічним інтерфейсом.
 */

#ifndef EMAILENCRYPTORWINDOW_H
#define EMAILENCRYPTORWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QTabWidget>
#include <QGroupBox>
#include <QDateEdit>
#include "EncryptionEngine.h"

/**
 * @class EmailEncryptorWindow
 * @brief Головне вікно програми Email-шифратора.
 *
 * Надає графічний інтерфейс для:
 * - Генерації ключів шифрування.
 * - Шифрування повідомлень та файлів.
 * - Розшифрування повідомлень та файлів.
 * - Пояснення алгоритму роботи.
 */
class EmailEncryptorWindow : public QMainWindow {
    Q_OBJECT

public:
    // ========== Конструктор та деструктор ==========

    /**
     * @brief Конструктор головного вікна.
     * @param parent Батьківський віджет.
     */
    explicit EmailEncryptorWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор.
     */
    ~EmailEncryptorWindow();

private slots:
    // ========== Обробники вкладки генерації ключа ==========

    /**
     * @brief Генерація ключа шифрування.
     */
    void generateKey();

    /**
     * @brief Заповнення форми прикладом.
     */
    void fillKeyExample();

    /**
     * @brief Збереження ключа у файл.
     */
    void saveKey();

    /**
     * @brief Очищення вкладки генерації ключа.
     */
    void clearKeyTab();

    // ========== Обробники вкладки шифрування ==========

    /**
     * @brief Завантаження ключа для шифрування.
     */
    void loadEncryptionKey();

    /**
     * @brief Шифрування повідомлення.
     */
    void encryptMessage();

    /**
     * @brief Вибір файлу для шифрування.
     */
    void selectFileToEncrypt();

    /**
     * @brief Шифрування файлу.
     */
    void encryptFile();

    /**
     * @brief Збереження зашифрованого повідомлення.
     */
    void saveEncryptedMessage();

    /**
     * @brief Очищення вкладки шифрування.
     */
    void clearEncryptTab();

    // ========== Обробники вкладки розшифрування ==========

    /**
     * @brief Завантаження ключа для розшифрування.
     */
    void loadDecryptionKey();

    /**
     * @brief Розшифрування повідомлення.
     */
    void decryptMessage();

    /**
     * @brief Вибір файлу для розшифрування.
     */
    void selectFileToDecrypt();

    /**
     * @brief Розшифрування файлу.
     */
    void decryptFile();

    /**
     * @brief Збереження розшифрованого повідомлення.
     */
    void saveDecryptedFile();

    /**
     * @brief Очищення вкладки розшифрування.
     */
    void clearDecryptTab();

private:
    // ========== Методи створення UI ==========

    /**
     * @brief Налаштування головного інтерфейсу.
     */
    void setupUI();

    /**
     * @brief Створення вкладки генерації ключа.
     * @return Віджет вкладки.
     */
    QWidget* createKeyTab();

    /**
     * @brief Створення вкладки шифрування.
     * @return Віджет вкладки.
     */
    QWidget* createEncryptTab();

    /**
     * @brief Створення вкладки розшифрування.
     * @return Віджет вкладки.
     */
    QWidget* createDecryptTab();

    /**
     * @brief Створення вкладки покрокового алгоритму.
     * @return Віджет вкладки.
     */
    QWidget* createStepByStepTab();

    /**
     * @brief Генерація HTML-пояснення алгоритму.
     * @return HTML-текст з поясненням.
     */
    QString generateAlgorithmExplanation();

    // ========== Допоміжні методи ==========

    /**
     * @brief Застосування темної теми.
     */
    void applyDarkTheme();

    /**
     * @brief Створення іконки програми.
     * @return Іконка програми.
     */
    QIcon createProgramIcon();

    // ========== UI елементи ==========

    /// Головний віджет з вкладками.
    QTabWidget* tabWidget;

    // ===== ВКЛАДКА 1: ГЕНЕРАЦІЯ КЛЮЧА =====

    /// Поле введення email.
    QLineEdit* key_emailEdit;

    /// Поле введення імені.
    QLineEdit* key_firstNameEdit;

    /// Поле введення прізвища.
    QLineEdit* key_lastNameEdit;

    /// Поле вибору дати народження.
    QDateEdit* key_birthDateEdit;

    /// Поле відображення згенерованого ключа.
    QTextEdit* key_generatedKeyEdit;

    /// Кнопка генерації ключа.
    QPushButton* key_generateBtn;

    /// Кнопка заповнення прикладом.
    QPushButton* key_exampleBtn;

    /// Кнопка збереження ключа.
    QPushButton* key_saveBtn;

    /// Кнопка очищення.
    QPushButton* key_clearBtn;

    // ===== ВКЛАДКА 2: ШИФРУВАННЯ =====

    /// Поле шляху до ключа шифрування.
    QLineEdit* encrypt_keyPathEdit;

    /// Кнопка завантаження ключа.
    QPushButton* encrypt_loadKeyBtn;

    /// Поле введення повідомлення.
    QTextEdit* encrypt_messageEdit;

    /// Поле відображення зашифрованого повідомлення.
    QTextEdit* encrypt_encryptedEdit;

    /// Кнопка шифрування повідомлення.
    QPushButton* encrypt_messageBtn;

    /// Кнопка збереження зашифрованого повідомлення.
    QPushButton* encrypt_saveMessageBtn;

    /// Поле шляху до файлу.
    QLineEdit* encrypt_filePathEdit;

    /// Кнопка вибору файлу.
    QPushButton* encrypt_selectFileBtn;

    /// Кнопка шифрування файлу.
    QPushButton* encrypt_fileBtn;

    /// Мітка інформації про файл.
    QLabel* encrypt_fileInfoLabel;

    /// Кнопка очищення.
    QPushButton* encrypt_clearBtn;

    // ===== ВКЛАДКА 3: РОЗШИФРУВАННЯ =====

    /// Поле шляху до ключа розшифрування.
    QLineEdit* decrypt_keyPathEdit;

    /// Кнопка завантаження ключа.
    QPushButton* decrypt_loadKeyBtn;

    /// Поле введення зашифрованого повідомлення.
    QTextEdit* decrypt_encryptedEdit;

    /// Поле відображення розшифрованого повідомлення.
    QTextEdit* decrypt_decryptedEdit;

    /// Кнопка розшифрування повідомлення.
    QPushButton* decrypt_messageBtn;

    /// Поле шляху до зашифрованого файлу.
    QLineEdit* decrypt_filePathEdit;

    /// Кнопка вибору файлу.
    QPushButton* decrypt_selectFileBtn;

    /// Кнопка розшифрування файлу.
    QPushButton* decrypt_fileBtn;

    /// Кнопка збереження розшифрованого повідомлення.
    QPushButton* decrypt_saveFileBtn;

    /// Мітка інформації про файл.
    QLabel* decrypt_fileInfoLabel;

    /// Кнопка очищення.
    QPushButton* decrypt_clearBtn;

    // ========== Дані програми ==========

    /// Об'єкт движка шифрування.
    EncryptionEngine* encryptionEngine;

    /// Згенерований ключ.
    EncryptionKey currentKey;

    /// Завантажений ключ для шифрування.
    QString encryptionKey;

    /// Завантажений ключ для розшифрування.
    QString decryptionKey;

    /// Шлях до файлу для шифрування.
    QString encryptFilePath;

    /// Дані файлу для шифрування.
    QByteArray encryptFileData;

    /// Шлях до файлу для розшифрування.
    QString decryptFilePath;

    /// Дані зашифрованого файлу.
    QByteArray decryptFileData;

    /// Розшифровані дані файлу.
    QByteArray decryptedFileData;
};

#endif // EMAILENCRYPTORWINDOW_H
