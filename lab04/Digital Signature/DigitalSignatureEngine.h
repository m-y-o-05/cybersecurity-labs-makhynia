/*
 * DigitalSignatureEngine.h
 *
 * Заголовочний файл для реалізації алгоритмів цифрових підписів.
 * Містить класи для генерації ключів, створення та перевірки підписів.
 *
 * Основні можливості:
 * - Генерація пари ключів (приватний + публічний).
 * - Створення цифрового підпису.
 * - Перевірка підпису.
 * - Виявлення підробок.
 * - Підтримка SHA-256, SHA-512, MD5.
 */

#ifndef DIGITALSIGNATUREENGINE_H
#define DIGITALSIGNATUREENGINE_H

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

/**
 * @struct KeyPair
 * @brief Структура для зберігання пари ключів.
 */
struct KeyPair {
    QString privateKey;   ///< Приватний ключ (hex-формат).
    QString publicKey;    ///< Публічний ключ (hex-формат).
    qint64 privateKeyNum; ///< Приватний ключ (числове значення).
    qint64 publicKeyNum;  ///< Публічний ключ (числове значення).
};

/**
 * @struct SignatureResult
 * @brief Результат перевірки підпису.
 */
struct SignatureResult {
    bool isValid;           ///< Чи дійсний підпис.
    QString originalHash;   ///< Оригінальний хеш документа.
    QString currentHash;    ///< Поточний хеш документа.
    QString decryptedHash;  ///< Розшифрований хеш з підпису.
    QString message;        ///< Повідомлення про результат.
};

/**
 * @enum HashAlgorithm
 * @brief Підтримувані алгоритми хешування.
 */
enum class HashAlgorithm {
    SHA256,  ///< SHA-256 (рекомендований).
    SHA512,  ///< SHA-512 (більш безпечний).
    MD5      ///< MD5 (застарілий, для демонстрації).
};

/**
 * @class DigitalSignatureEngine
 * @brief Основний клас для роботи з цифровими підписами.
 *
 * Реалізує спрощену систему цифрових підписів з демонстрацією
 * принципів асимметричного шифрування.
 */
class DigitalSignatureEngine {
public:
    // ========== Конструктор ==========

    /**
     * @brief Конструктор за замовчуванням.
     *
     * Ініціалізує движок з SHA-256 як алгоритмом за замовчуванням.
     */
    DigitalSignatureEngine();

    // ========== Константи ==========

    /// Множник для генерації публічного ключа.
    static const qint64 KEY_MULTIPLIER = 7;

    /// Модуль для генерації публічного ключа.
    static const qint64 KEY_MODULUS = 1000007;

    // ========== Основні методи ==========

    /**
     * @brief Генерація пари ключів з персональних даних.
     *
     * Алгоритм:
     * 1. Об'єднуємо персональні дані.
     * 2. Обчислюємо SHA-256 хеш → приватний ключ.
     * 3. Використовуємо модульну арифметику → публічний ключ.
     *
     * Формула: publicKey = (privateKey * MULTIPLIER) mod MODULUS
     * де MULTIPLIER = 7, MODULUS = 1000007 (просте число).
     *
     * @param firstName Ім'я.
     * @param lastName Прізвище.
     * @param birthDate Дата народження.
     * @param secretWord Секретне слово.
     * @return Пара ключів (приватний + публічний).
     */
    KeyPair generateKeyPair(const QString& firstName,
                            const QString& lastName,
                            const QString& birthDate,
                            const QString& secretWord);

    /**
     * @brief Створення цифрового підпису документа.
     *
     * Алгоритм:
     * 1. Обчислюємо хеш документа (SHA-256/SHA-512/MD5).
     * 2. "Шифруємо" хеш приватним ключем (XOR операція).
     * 3. Повертаємо підпис у hex-форматі.
     *
     * @param fileData Вміст файлу.
     * @param privateKey Приватний ключ.
     * @return Цифровий підпис (hex-рядок).
     */
    QString createSignature(const QByteArray& fileData,
                            const QString& privateKey);

    /**
     * @brief Перевірка цифрового підпису.
     *
     * Алгоритм:
     * 1. Обчислюємо хеш поточного документа.
     * 2. "Розшифровуємо" підпис публічним ключем (XOR операція).
     * 3. Порівнюємо хеші.
     * 4. Повертаємо результат перевірки.
     *
     * @param fileData Вміст файлу.
     * @param signature Цифровий підпис.
     * @param publicKey Публічний ключ.
     * @return Результат перевірки.
     */
    SignatureResult verifySignature(const QByteArray& fileData,
                                    const QString& signature,
                                    const QString& publicKey);

    /**
     * @brief Обчислення хешу файлу.
     *
     * @param fileData Вміст файлу.
     * @return Хеш у hex-форматі.
     */
    QString calculateHash(const QByteArray& fileData);

    /**
     * @brief Встановлення алгоритму хешування.
     *
     * @param algorithm Алгоритм хешування.
     */
    void setHashAlgorithm(HashAlgorithm algorithm);

    /**
     * @brief Отримання поточного алгоритму хешування.
     *
     * @return Поточний алгоритм.
     */
    HashAlgorithm getHashAlgorithm() const;

    /**
     * @brief Отримання назви алгоритму хешування.
     *
     * @param algorithm Алгоритм хешування.
     * @return Назва алгоритму.
     */
    static QString getAlgorithmName(HashAlgorithm algorithm);

    /**
     * @brief Конвертація hex-рядка в число.
     *
     * @param hexString Hex-рядок.
     * @return Числове значення.
     */
    static qint64 hexToNumber(const QString& hexString);

    /**
     * @brief Конвертація числа в hex-рядок.
     *
     * @param number Числове значення.
     * @return Hex-рядок.
     */
    static QString numberToHex(qint64 number);

private:
    // ========== Приватні методи ==========

    /**
     * @brief XOR шифрування/дешифрування.
     *
     * Використовується для "шифрування" хешу приватним ключем
     * та "розшифрування" підпису публічним ключем.
     *
     * @param data Дані для шифрування.
     * @param key Ключ.
     * @return Зашифровані/розшифровані дані.
     */
    QString xorEncrypt(const QString& data, const QString& key);

    /**
     * @brief Обчислення хешу з вибраним алгоритмом.
     *
     * @param data Дані.
     * @param algorithm Алгоритм хешування.
     * @return Хеш у hex-форматі.
     */
    QString computeHash(const QByteArray& data,
                        QCryptographicHash::Algorithm algorithm);

    // ========== Змінні стану ==========

    /// Поточний алгоритм хешування.
    HashAlgorithm currentHashAlgorithm;
};

#endif // DIGITALSIGNATUREENGINE_H
