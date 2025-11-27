/*
 * DigitalSignatureEngine.cpp
 *
 * Реалізація алгоритмів цифрових підписів.
 * Містить методи для генерації ключів, створення та перевірки підписів.
 */

#include "DigitalSignatureEngine.h"
#include <QCryptographicHash>
#include <QDebug>

/*
 * ============================================================================
 * КОНСТРУКТОР
 * ============================================================================
 */

/**
 * @brief Конструктор за замовчуванням.
 *
 * Встановлює SHA-256 як алгоритм хешування за замовчуванням.
 */
DigitalSignatureEngine::DigitalSignatureEngine()
    : currentHashAlgorithm(HashAlgorithm::SHA256)
{
    qDebug() << "DigitalSignatureEngine initialized with SHA-256";
}

/*
 * ============================================================================
 * ГЕНЕРАЦІЯ КЛЮЧІВ
 * ============================================================================
 */

/**
 * @brief Генерація пари ключів з персональних даних.
 *
 * Демонстрація принципів асимметричного шифрування:
 *
 * 1. ПРИВАТНИЙ КЛЮЧ:
 *    - Генерується з персональних даних + секретного слова.
 *    - Використовується SHA-256 хеш-функція.
 *    - Зберігається в секреті.
 *
 * 2. ПУБЛІЧНИЙ КЛЮЧ:
 *    - Обчислюється з приватного ключа.
 *    - Використовується модульна арифметика.
 *    - Формула: publicKey = (privateKey × 7) mod 1000007.
 *    - Може бути відкритим для всіх.
 *
 * 3. МАТЕМАТИЧНА ОСНОВА:
 *    - 7 - множник (вибрано як просте число).
 *    - 1000007 - модуль (велике просте число для безпеки).
 *    - Зв'язок однонаправлений: легко обчислити публічний з приватного,
 *      але важко відновити приватний з публічного.
 *
 * @param firstName Ім'я користувача.
 * @param lastName Прізвище користувача.
 * @param birthDate Дата народження (формат: ddMMyyyy).
 * @param secretWord Секретне слово (додатковий фактор безпеки).
 * @return Пара ключів.
 */
KeyPair DigitalSignatureEngine::generateKeyPair(const QString& firstName,
                                                const QString& lastName,
                                                const QString& birthDate,
                                                const QString& secretWord) {
    KeyPair keys;

    // ========== КРОК 1: Створення приватного ключа ==========

    // Об'єднуємо всі персональні дані в один рядок.
    QString combined = firstName + lastName + birthDate + secretWord;

    qDebug() << "Generating keys from:" << combined;

    // Обчислюємо SHA-256 хеш - це і буде приватний ключ.
    QByteArray hash = QCryptographicHash::hash(
        combined.toUtf8(),
        QCryptographicHash::Sha256
        );

    // Конвертуємо в hex-рядок.
    keys.privateKey = QString(hash.toHex()).toUpper();

    // Беремо тільки перші 8 hex-символів для числового представлення.
    QString privateKeyShort = keys.privateKey.left(8);
    keys.privateKeyNum = hexToNumber(privateKeyShort);

    qDebug() << "Private key (hex):" << keys.privateKey;
    qDebug() << "Private key (short):" << privateKeyShort;
    qDebug() << "Private key (num):" << keys.privateKeyNum;

    // ========== КРОК 2: Створення публічного ключа ==========

    // Перевіряємо чи privateKeyNum не дорівнює 0.
    if (keys.privateKeyNum == 0) {
        qWarning() << "Private key number is 0! Using fallback value.";
        keys.privateKeyNum = 12345;
    }

    // Використовуємо модульну арифметику для генерації публічного ключа.
    // Формула: publicKey = (privateKey × MULTIPLIER) mod MODULUS.

    keys.publicKeyNum = (keys.privateKeyNum * KEY_MULTIPLIER) % KEY_MODULUS;
    keys.publicKey = numberToHex(keys.publicKeyNum);

    qDebug() << "Public key (hex):" << keys.publicKey;
    qDebug() << "Public key (num):" << keys.publicKeyNum;
    qDebug() << "Formula: (" << keys.privateKeyNum << " * " << KEY_MULTIPLIER
             << ") mod " << KEY_MODULUS << " = " << keys.publicKeyNum;

    return keys;
}

/*
 * ============================================================================
 * СТВОРЕННЯ ПІДПИСУ
 * ============================================================================
 */

/**
 * @brief Створення цифрового підпису документа.
 *
 * Процес створення підпису:
 *
 * 1. ХЕШУВАННЯ ДОКУМЕНТА:
 *    - Обчислюємо криптографічний хеш документа.
 *    - Хеш - це "відбиток" документа фіксованої довжини.
 *    - Будь-яка зміна в документі → інший хеш.
 *
 * 2. "ШИФРУВАННЯ" ХЕШУ:
 *    - Використовуємо приватний ключ для "шифрування" хешу.
 *    - У цій спрощеній реалізації: XOR операція.
 *    - У реальних системах (RSA): модульне піднесення до степеня.
 *
 * 3. РЕЗУЛЬТАТ:
 *    - Зашифрований хеш = цифровий підпис.
 *    - Тільки власник приватного ключа може створити цей підпис.
 *    - Але будь-хто з публічним ключем може перевірити підпис.
 *
 * @param fileData Вміст документа (байти).
 * @param privateKey Приватний ключ (hex-формат).
 * @return Цифровий підпис (hex-формат).
 */
QString DigitalSignatureEngine::createSignature(const QByteArray& fileData,
                                                const QString& privateKey) {
    qDebug() << "Creating signature for file of size:" << fileData.size() << "bytes";

    // ========== КРОК 1: Обчислюємо хеш документа ==========
    QString documentHash = calculateHash(fileData);
    qDebug() << "Document hash:" << documentHash;

    // ========== КРОК 2: "Шифруємо" хеш приватним ключем ==========
    // signature = hash XOR privateKey.
    QString signature = xorEncrypt(documentHash, privateKey);

    qDebug() << "Signature created:" << signature;
    qDebug() << "Algorithm used:" << getAlgorithmName(currentHashAlgorithm);

    return signature;
}

/*
 * ============================================================================
 * ПЕРЕВІРКА ПІДПИСУ
 * ============================================================================
 */

/**
 * @brief Перевірка цифрового підпису.
 *
 * Процес перевірки підпису:
 *
 * 1. ОБЧИСЛЕННЯ ХЕШУ ПОТОЧНОГО ДОКУМЕНТА:
 *    - Хешуємо документ, який потрібно перевірити.
 *    - Якщо документ змінено → хеш буде інший.
 *
 * 2. "РОЗШИФРУВАННЯ" ПІДПИСУ:
 *    - Використовуємо публічний ключ для розшифрування підпису.
 *    - Отримуємо оригінальний хеш, який був підписаний.
 *    - У реальній RSA: hash = signature^e mod n.
 *    - У нашій версії: hash = signature XOR publicKey.
 *
 * 3. ПОРІВНЯННЯ ХЕШІВ:
 *    - Поточний хеш == розшифрований хеш → підпис ДІЙСНИЙ.
 *    - Поточний хеш != розшифрований хеш → підпис ПІДРОБЛЕНИЙ.
 *
 * 4. БЕЗПЕКА:
 *    - Тільки власник приватного ключа міг створити дійсний підпис.
 *    - Будь-яка зміна документа → підпис стає недійсним.
 *    - Публічний ключ дозволяє ПЕРЕВІРИТИ, але не СТВОРИТИ підпис.
 *
 * @param fileData Вміст документа.
 * @param signature Цифровий підпис.
 * @param publicKey Публічний ключ.
 * @return Результат перевірки.
 */
SignatureResult DigitalSignatureEngine::verifySignature(
    const QByteArray& fileData,
    const QString& signature,
    const QString& publicKey) {

    SignatureResult result;

    qDebug() << "Verifying signature...";
    qDebug() << "File size:" << fileData.size() << "bytes";
    qDebug() << "Signature:" << signature;
    qDebug() << "Public key:" << publicKey;

    // ========== КРОК 1: Обчислюємо хеш поточного документа ==========
    result.currentHash = calculateHash(fileData);
    qDebug() << "Current document hash:" << result.currentHash;

    // ========== КРОК 2: "Розшифровуємо" підпис публічним ключем ==========
    // originalHash = signature XOR publicKey
    result.decryptedHash = xorEncrypt(signature, publicKey);
    qDebug() << "Decrypted hash from signature:" << result.decryptedHash;

    // ========== КРОК 3: Порівнюємо хеші ==========
    result.isValid = (result.currentHash == result.decryptedHash);
    result.originalHash = result.decryptedHash;

    // ========== КРОК 4: Формуємо повідомлення про результат ==========
    if (result.isValid) {
        result.message = "ПІДПИС ДІЙСНИЙ\n\n"
                         "Документ не був змінений після підписання.\n"
                         "Підпис створений власником приватного ключа.";
        qDebug() << "Verification SUCCESSFUL";
    } else {
        result.message = "ПІДПИС ПІДРОБЛЕНИЙ\n\n"
                         "Документ був змінений після підписання,\n"
                         "або підпис не відповідає публічному ключу.\n\n"
                         "ПОПЕРЕДЖЕННЯ: Цей документ не є автентичним!";
        qDebug() << "Verification FAILED";
        qDebug() << "Expected hash:" << result.currentHash;
        qDebug() << "Got hash:" << result.decryptedHash;
    }

    return result;
}

/*
 * ============================================================================
 * ХЕШУВАННЯ
 * ============================================================================
 */

/**
 * @brief Обчислення хешу файлу з поточним алгоритмом.
 *
 * @param fileData Вміст файлу.
 * @return Хеш у hex-форматі.
 */
QString DigitalSignatureEngine::calculateHash(const QByteArray& fileData) {
    QCryptographicHash::Algorithm algorithm;

    // Вибираємо алгоритм хешування.
    switch (currentHashAlgorithm) {
    case HashAlgorithm::SHA256:
        algorithm = QCryptographicHash::Sha256;
        break;
    case HashAlgorithm::SHA512:
        algorithm = QCryptographicHash::Sha512;
        break;
    case HashAlgorithm::MD5:
        algorithm = QCryptographicHash::Md5;
        break;
    default:
        algorithm = QCryptographicHash::Sha256;
    }

    return computeHash(fileData, algorithm);
}

/**
 * @brief Обчислення хешу з конкретним алгоритмом.
 *
 * @param data Дані.
 * @param algorithm Алгоритм Qt.
 * @return Хеш у hex-форматі.
 */
QString DigitalSignatureEngine::computeHash(
    const QByteArray& data,
    QCryptographicHash::Algorithm algorithm) {

    QByteArray hash = QCryptographicHash::hash(data, algorithm);
    return QString(hash.toHex()).toUpper();
}

/*
 * ============================================================================
 * НАЛАШТУВАННЯ АЛГОРИТМУ
 * ============================================================================
 */

/**
 * @brief Встановлення алгоритму хешування.
 *
 * @param algorithm Новий алгоритм.
 */
void DigitalSignatureEngine::setHashAlgorithm(HashAlgorithm algorithm) {
    currentHashAlgorithm = algorithm;
    qDebug() << "Hash algorithm changed to:" << getAlgorithmName(algorithm);
}

/**
 * @brief Отримання поточного алгоритму.
 *
 * @return Поточний алгоритм хешування.
 */
HashAlgorithm DigitalSignatureEngine::getHashAlgorithm() const {
    return currentHashAlgorithm;
}

/**
 * @brief Отримання назви алгоритму хешування.
 *
 * @param algorithm Алгоритм.
 * @return Назва алгоритму.
 */
QString DigitalSignatureEngine::getAlgorithmName(HashAlgorithm algorithm) {
    switch (algorithm) {
    case HashAlgorithm::SHA256:
        return "SHA-256";
    case HashAlgorithm::SHA512:
        return "SHA-512";
    case HashAlgorithm::MD5:
        return "MD5";
    default:
        return "Unknown";
    }
}

/*
 * ============================================================================
 * ДОПОМІЖНІ МЕТОДИ
 * ============================================================================
 */

/**
 * @brief XOR шифрування/дешифрування.
 *
 * XOR (виключне АБО) - це симетрична операція:
 * - data XOR key = encrypted.
 * - encrypted XOR key = data.
 *
 * У реальних системах RSA використовується модульне піднесення до степеня,
 * але для навчальних цілей XOR наочно демонструє принцип.
 *
 * @param data Дані (hex-рядок).
 * @param key Ключ (hex-рядок).
 * @return Результат XOR (hex-рядок).
 */
QString DigitalSignatureEngine::xorEncrypt(const QString& data,
                                           const QString& key) {
    // Конвертуємо hex-рядки в байти.
    QByteArray dataBytes = QByteArray::fromHex(data.toUtf8());
    QByteArray keyBytes = QByteArray::fromHex(key.toUtf8());

    // Перевірка на пусті дані.
    if (dataBytes.isEmpty() || keyBytes.isEmpty()) {
        qWarning() << "XOR: Empty data or key!";
        return data;
    }

    // Результуючий масив.
    QByteArray result;
    result.resize(dataBytes.size());

    // Виконуємо XOR для кожного байта.
    for (int i = 0; i < dataBytes.size(); ++i) {
        // Використовуємо ключ циклічно (якщо він коротший за дані).
        result[i] = dataBytes[i] ^ keyBytes[i % keyBytes.size()];
    }

    // Конвертуємо назад в hex.
    return QString(result.toHex()).toUpper();
}

/**
 * @brief Конвертація hex-рядка в число.
 *
 * Приклад: "1A2B" → 6699.
 *
 * @param hexString Hex-рядок.
 * @return Числове значення.
 */
qint64 DigitalSignatureEngine::hexToNumber(const QString& hexString) {
    // Обмежуємо довжину до 16 символів (64 біти).
    QString limited = hexString.left(16);

    bool ok;
    qint64 number = limited.toLongLong(&ok, 16);

    if (!ok) {
        qWarning() << "Failed to convert hex to number:" << limited;
        return 1;
    }

    // Переконуємось що число позитивне.
    if (number <= 0) {
        number = qAbs(number);
        if (number == 0) number = 1;
    }

    return number;
}

/**
 * @brief Конвертація числа в hex-рядок.
 *
 * Приклад: 6699 → "1A2B".
 *
 * @param number Числове значення.
 * @return Hex-рядок.
 */
QString DigitalSignatureEngine::numberToHex(qint64 number) {
    // Переконуємось що число позитивне.
    if (number < 0) {
        number = qAbs(number);
    }

    return QString("%1").arg(number, 0, 16).toUpper();
}
