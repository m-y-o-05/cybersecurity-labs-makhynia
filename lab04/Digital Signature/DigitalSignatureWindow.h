/*
 * DigitalSignatureWindow.h
 *
 * Заголовочний файл головного вікна програми цифрових підписів.
 * Містить оголошення класу головного вікна з графічним інтерфейсом.
 *
 * Функціональні можливості:
 * - 4 вкладки: Генерація ключів, Створення підпису, Перевірка підпису, Алгоритм.
 * - Підтримка різних алгоритмів хешування (SHA-256, SHA-512, MD5).
 * - Візуалізація процесу створення та перевірки підписів.
 * - Демонстрація виявлення підробок.
 */

#ifndef DIGITALSIGNATUREWINDOW_H
#define DIGITALSIGNATUREWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QDateEdit>
#include <QTabWidget>
#include <QComboBox>
#include <QGroupBox>
#include "DigitalSignatureEngine.h"

/**
 * @class DigitalSignatureWindow
 * @brief Головне вікно програми для цифрових підписів.
 *
 * Надає графічний інтерфейс для роботи з цифровими підписами:
 * - Генерація пари ключів.
 * - Створення цифрових підписів.
 * - Перевірка підписів.
 * - Виявлення підробок.
 */
class DigitalSignatureWindow : public QMainWindow {
    Q_OBJECT

public:
    // ========== Конструктор та деструктор ==========

    /**
     * @brief Конструктор головного вікна.
     * @param parent Батьківський віджет.
     */
    explicit DigitalSignatureWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор.
     */
    ~DigitalSignatureWindow();

private slots:
    // ========== Обробники вкладки генерації ключів ==========

    /**
     * @brief Генерація пари ключів.
     *
     * Створює приватний та публічний ключі на основі персональних даних.
     */
    void generateKeys();

    /**
     * @brief Заповнення форми прикладом.
     */
    void fillKeysExample();

    /**
     * @brief Збереження приватного ключа у файл.
     */
    void savePrivateKey();

    /**
     * @brief Збереження публічного ключа у файл.
     */
    void savePublicKey();

    /**
     * @brief Очищення вкладки генерації ключів.
     */
    void clearKeysTab();

    // ========== Обробники вкладки створення підпису ==========

    /**
     * @brief Вибір документа для підписання.
     */
    void selectDocumentToSign();

    /**
     * @brief Завантаження приватного ключа з файлу.
     */
    void loadPrivateKey();

    /**
     * @brief Створення цифрового підпису.
     */
    void createSignature();

    /**
     * @brief Збереження підпису у файл.
     */
    void saveSignature();

    /**
     * @brief Очищення вкладки створення підпису.
     */
    void clearSignTab();

    // ========== Обробники вкладки перевірки підпису ==========

    /**
     * @brief Вибір документа для перевірки.
     */
    void selectDocumentToVerify();

    /**
     * @brief Завантаження підпису з файлу.
     */
    void loadSignature();

    /**
     * @brief Завантаження публічного ключа з файлу.
     */
    void loadPublicKey();

    /**
     * @brief Перевірка підпису.
     */
    void verifySignature();

    /**
     * @brief Очищення вкладки перевірки.
     */
    void clearVerifyTab();

    // ========== Обробники вкладки алгоритму ==========

    /**
     * @brief Показ покрокового алгоритму.
     */
    void showStepByStepAlgorithm();

    // ========== Обробники зміни налаштувань ==========

    /**
     * @brief Зміна алгоритму хешування.
     * @param index Індекс вибраного алгоритму.
     */
    void onHashAlgorithmChanged(int index);

private:
    // ========== Методи створення UI ==========

    /**
     * @brief Налаштування головного інтерфейсу.
     */
    void setupUI();

    /**
     * @brief Створення вкладки генерації ключів.
     * @return Віджет вкладки.
     */
    QWidget* createKeysTab();

    /**
     * @brief Створення вкладки створення підпису.
     * @return Віджет вкладки.
     */
    QWidget* createSignTab();

    /**
     * @brief Створення вкладки перевірки підпису.
     * @return Віджет вкладки.
     */
    QWidget* createVerifyTab();

    /**
     * @brief Створення вкладки покрокового алгоритму.
     * @return Віджет вкладки.
     */
    QWidget* createAlgorithmTab();

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

    /**
     * @brief Відображення результату перевірки підпису.
     * @param result Результат перевірки.
     */
    void displayVerificationResult(const SignatureResult& result);

    // ========== UI елементи ==========

    /// Головний віджет з вкладками.
    QTabWidget* tabWidget;

    // ===== ВКЛАДКА 1: ГЕНЕРАЦІЯ КЛЮЧІВ =====

    /// Поле введення імені.
    QLineEdit* keys_firstNameEdit;

    /// Поле введення прізвища.
    QLineEdit* keys_lastNameEdit;

    /// Поле вибору дати народження.
    QDateEdit* keys_birthDateEdit;

    /// Поле введення секретного слова.
    QLineEdit* keys_secretWordEdit;

    /// Вибір алгоритму хешування.
    QComboBox* keys_hashAlgorithmCombo;

    /// Поле відображення приватного ключа.
    QTextEdit* keys_privateKeyEdit;

    /// Поле відображення публічного ключа.
    QTextEdit* keys_publicKeyEdit;

    /// Кнопка генерації ключів.
    QPushButton* keys_generateBtn;

    /// Кнопка заповнення прикладом.
    QPushButton* keys_exampleBtn;

    /// Кнопка очищення.
    QPushButton* keys_clearBtn;

    /// Кнопка збереження приватного ключа.
    QPushButton* keys_savePrivateBtn;

    /// Кнопка збереження публічного ключа.
    QPushButton* keys_savePublicBtn;

    // ===== ВКЛАДКА 2: СТВОРЕННЯ ПІДПИСУ =====

    /// Поле шляху до документа.
    QLineEdit* sign_documentPathEdit;

    /// Кнопка вибору документа.
    QPushButton* sign_selectDocumentBtn;

    /// Поле шляху до приватного ключа.
    QLineEdit* sign_privateKeyPathEdit;

    /// Кнопка завантаження приватного ключа.
    QPushButton* sign_loadPrivateKeyBtn;

    /// Поле відображення хешу документа.
    QTextEdit* sign_documentHashEdit;

    /// Поле відображення створеного підпису.
    QTextEdit* sign_signatureEdit;

    /// Кнопка створення підпису.
    QPushButton* sign_createBtn;

    /// Кнопка збереження підпису.
    QPushButton* sign_saveBtn;

    /// Кнопка очищення.
    QPushButton* sign_clearBtn;

    /// Мітка інформації про документ.
    QLabel* sign_documentInfoLabel;

    // ===== ВКЛАДКА 3: ПЕРЕВІРКА ПІДПИСУ =====

    /// Поле шляху до документа.
    QLineEdit* verify_documentPathEdit;

    /// Кнопка вибору документа.
    QPushButton* verify_selectDocumentBtn;

    /// Поле шляху до підпису.
    QLineEdit* verify_signaturePathEdit;

    /// Кнопка завантаження підпису.
    QPushButton* verify_loadSignatureBtn;

    /// Поле шляху до публічного ключа.
    QLineEdit* verify_publicKeyPathEdit;

    /// Кнопка завантаження публічного ключа.
    QPushButton* verify_loadPublicKeyBtn;

    /// Поле відображення результату перевірки.
    QTextEdit* verify_resultEdit;

    /// Кнопка перевірки підпису.
    QPushButton* verify_verifyBtn;

    /// Кнопка очищення.
    QPushButton* verify_clearBtn;

    /// Мітка інформації про документ.
    QLabel* verify_documentInfoLabel;

    /// Група деталей перевірки.
    QGroupBox* verify_detailsGroup;

    /// Поле оригінального хешу.
    QLineEdit* verify_originalHashEdit;

    /// Поле поточного хешу.
    QLineEdit* verify_currentHashEdit;

    /// Поле розшифрованого хешу.
    QLineEdit* verify_decryptedHashEdit;

    // ===== ВКЛАДКА 4: ПОКРОКОВИЙ АЛГОРИТМ =====

    /// Текстове поле з поясненням алгоритму.
    QTextEdit* algorithm_textEdit;

    // ========== Дані програми ==========

    /// Об'єкт движка цифрових підписів.
    DigitalSignatureEngine* signatureEngine;

    /// Згенеровані ключі.
    KeyPair currentKeys;

    /// Шлях до документа для підписання.
    QString signDocumentPath;

    /// Вміст документа для підписання.
    QByteArray signDocumentData;

    /// Завантажений приватний ключ.
    QString loadedPrivateKey;

    /// Створений підпис.
    QString createdSignature;

    /// Шлях до документа для перевірки.
    QString verifyDocumentPath;

    /// Вміст документа для перевірки.
    QByteArray verifyDocumentData;

    /// Завантажений підпис.
    QString loadedSignature;

    /// Завантажений публічний ключ.
    QString loadedPublicKey;
};

#endif // DIGITALSIGNATUREWINDOW_H
