/*
 * EncryptionEngine.cpp
 *
 * Реалізація алгоритмів шифрування повідомлень.
 */

#include "EncryptionEngine.h"
#include <QDebug>

/*
 * ============================================================================
 * КОНСТРУКТОР
 * ============================================================================
 */

/**
 * @brief Конструктор за замовчуванням.
 */
EncryptionEngine::EncryptionEngine() {
    qDebug() << "EncryptionEngine initialized";
}

/*
 * ============================================================================
 * ГЕНЕРАЦІЯ КЛЮЧА
 * ============================================================================
 */

/**
 * @brief Генерація ключа шифрування з персональних даних.
 */
EncryptionKey EncryptionEngine::generateKey(const QString& email,
                                            const QString& firstName,
                                            const QString& lastName,
                                            const QString& birthYear) {
    EncryptionKey key;

    key.email = email;
    key.personalData = firstName + lastName + birthYear;

    // Генеруємо ключ як SHA-256 хеш від персональних даних.
    key.keyHex = calculateSHA256(key.personalData);

    qDebug() << "Key generated for:" << email;
    qDebug() << "Personal data:" << key.personalData;
    qDebug() << "Key (SHA-256):" << key.keyHex;

    return key;
}

/*
 * ============================================================================
 * ШИФРУВАННЯ/РОЗШИФРУВАННЯ ПОВІДОМЛЕНЬ
 * ============================================================================
 */

/**
 * @brief Шифрування текстового повідомлення.
 */
QString EncryptionEngine::encryptMessage(const QString& message,
                                         const QString& key) {
    qDebug() << "Encrypting message...";
    qDebug() << "Original message:" << message;

    // Конвертуємо повідомлення в байти.
    QByteArray messageBytes = message.toUtf8();

    // Конвертуємо ключ з hex в байти.
    QByteArray keyBytes = QByteArray::fromHex(key.toUtf8());

    // Шифруємо за допомогою XOR.
    QByteArray encrypted = xorCrypt(messageBytes, keyBytes);

    // Конвертуємо в Base64 для зручного відображення.
    QString encryptedBase64 = encrypted.toBase64();

    qDebug() << "Encrypted (Base64):" << encryptedBase64;

    return encryptedBase64;
}

/**
 * @brief Розшифрування текстового повідомлення.
 */
QString EncryptionEngine::decryptMessage(const QString& encryptedMessage,
                                         const QString& key) {
    qDebug() << "Decrypting message...";
    qDebug() << "Encrypted message:" << encryptedMessage;

    // Конвертуємо з Base64 в байти.
    QByteArray encryptedBytes = QByteArray::fromBase64(encryptedMessage.toUtf8());

    // Конвертуємо ключ з hex в байти.
    QByteArray keyBytes = QByteArray::fromHex(key.toUtf8());

    // Розшифровуємо за допомогою XOR.
    QByteArray decrypted = xorCrypt(encryptedBytes, keyBytes);

    // Конвертуємо в текст.
    QString decryptedMessage = QString::fromUtf8(decrypted);

    qDebug() << "Decrypted message:" << decryptedMessage;

    return decryptedMessage;
}

/*
 * ============================================================================
 * ШИФРУВАННЯ/РОЗШИФРУВАННЯ ФАЙЛІВ
 * ============================================================================
 */

/**
 * @brief Шифрування файлу.
 */
QByteArray EncryptionEngine::encryptFile(const QByteArray& fileData,
                                         const QString& key) {
    qDebug() << "Encrypting file (" << fileData.size() << "bytes)...";

    // Конвертуємо ключ з hex в байти.
    QByteArray keyBytes = QByteArray::fromHex(key.toUtf8());

    // Шифруємо за допомогою XOR.
    QByteArray encrypted = xorCrypt(fileData, keyBytes);

    qDebug() << "File encrypted successfully";

    return encrypted;
}

/**
 * @brief Розшифрування файлу.
 */
QByteArray EncryptionEngine::decryptFile(const QByteArray& encryptedData,
                                         const QString& key) {
    qDebug() << "Decrypting file (" << encryptedData.size() << "bytes)...";

    // Конвертуємо ключ з hex в байти.
    QByteArray keyBytes = QByteArray::fromHex(key.toUtf8());

    // Розшифровуємо за допомогою XOR.
    QByteArray decrypted = xorCrypt(encryptedData, keyBytes);

    qDebug() << "File decrypted successfully";

    return decrypted;
}

/*
 * ============================================================================
 * ДОПОМІЖНІ МЕТОДИ
 * ============================================================================
 */

/**
 * @brief Обчислення SHA-256 хешу.
 */
QString EncryptionEngine::calculateSHA256(const QString& data) {
    QByteArray hash = QCryptographicHash::hash(
        data.toUtf8(),
        QCryptographicHash::Sha256
        );

    return QString(hash.toHex()).toUpper();
}

/**
 * @brief XOR шифрування/дешифрування.
 */
QByteArray EncryptionEngine::xorCrypt(const QByteArray& data,
                                      const QByteArray& key) {
    QByteArray result;
    result.resize(data.size());

    // Підготовлюємо ключ до потрібної довжини.
    QByteArray preparedKey = prepareKey(key, data.size());

    // Виконуємо XOR.
    for (int i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ preparedKey[i];
    }

    return result;
}

/**
 * @brief Підготовка ключа до потрібної довжини.
 */
QByteArray EncryptionEngine::prepareKey(const QByteArray& key, int length) {
    QByteArray result;
    result.reserve(length);

    // Повторюємо ключ циклічно до потрібної довжини.
    for (int i = 0; i < length; ++i) {
        result.append(key[i % key.size()]);
    }

    return result;
}
