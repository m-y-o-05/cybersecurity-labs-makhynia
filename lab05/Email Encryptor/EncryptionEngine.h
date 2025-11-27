/*
 * EncryptionEngine.h
 *
 * Заголовочний файл шифрування повідомлень.
 * Містить алгоритми симетричного шифрування.
 */

#ifndef ENCRYPTIONENGINE_H
#define ENCRYPTIONENGINE_H

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

/**
 * @struct EncryptionKey
 * @brief Структура для зберігання ключа шифрування.
 */
struct EncryptionKey {
    /// Hex-представлення ключа.
    QString keyHex;

    /// Email користувача.
    QString email;

    /// Персональні дані для генерації ключа.
    QString personalData;

    /// Конструктор за замовчуванням.
    EncryptionKey() : keyHex(""), email(""), personalData("") {}
};

/**
 * @class EncryptionEngine
 * @brief Движок симетричного шифрування повідомлень.
 *
 * Реалізує алгоритм симетричного шифрування для захисту
 * електронних повідомлень та файлів.
 */
class EncryptionEngine {
public:
    // ========== Конструктор ==========

    /**
     * @brief Конструктор за замовчуванням.
     */
    EncryptionEngine();

    // ========== Основні методи ==========

    /**
     * @brief Генерація ключа шифрування з персональних даних.
     *
     * @param email Email користувача.
     * @param firstName Ім'я.
     * @param lastName Прізвище.
     * @param birthYear Рік народження.
     * @return Ключ шифрування.
     */
    EncryptionKey generateKey(const QString& email,
                              const QString& firstName,
                              const QString& lastName,
                              const QString& birthYear);

    /**
     * @brief Шифрування текстового повідомлення.
     *
     * @param message Текст повідомлення.
     * @param key Ключ шифрування.
     * @return Зашифрований текст (Base64).
     */
    QString encryptMessage(const QString& message, const QString& key);

    /**
     * @brief Розшифрування текстового повідомлення.
     *
     * @param encryptedMessage Зашифроване повідомлення.
     * @param key Ключ розшифрування.
     * @return Оригінальний текст.
     */
    QString decryptMessage(const QString& encryptedMessage, const QString& key);

    /**
     * @brief Шифрування файлу.
     *
     * @param fileData Вміст файлу.
     * @param key Ключ шифрування.
     * @return Зашифровані дані.
     */
    QByteArray encryptFile(const QByteArray& fileData, const QString& key);

    /**
     * @brief Розшифрування файлу.
     *
     * @param encryptedData Зашифровані дані.
     * @param key Ключ розшифрування.
     * @return Оригінальні дані файлу.
     */
    QByteArray decryptFile(const QByteArray& encryptedData, const QString& key);

    /**
     * @brief Обчислення SHA-256 хешу.
     *
     * @param data Дані для хешування.
     * @return Hex-представлення хешу.
     */
    static QString calculateSHA256(const QString& data);

private:
    // ========== Приватні методи ==========

    /**
     * @brief XOR шифрування/дешифрування.
     *
     * @param data Дані.
     * @param key Ключ.
     * @return Результат XOR.
     */
    QByteArray xorCrypt(const QByteArray& data, const QByteArray& key);

    /**
     * @brief Підготовка ключа до потрібної довжини.
     *
     * @param key Оригінальний ключ.
     * @param length Потрібна довжина.
     * @return Підготовлений ключ.
     */
    QByteArray prepareKey(const QByteArray& key, int length);
};

#endif // ENCRYPTIONENGINE_H
