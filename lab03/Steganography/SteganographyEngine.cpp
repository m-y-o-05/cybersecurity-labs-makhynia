/*
 * SteganographyEngine.cpp
 *
 * Реалізація алгоритмів стеганографії.
 * Містить методи для приховування та витягування текстової інформації
 * з використанням LSB методу.
 */

#include "SteganographyEngine.h"
#include <QColor>
#include <QDebug>
#include <QtMath>
#include <QCryptographicHash>

/*
 * ============================================================================
 * КОНСТРУКТОР
 * ============================================================================
 */

/**
 * @brief Конструктор за замовчуванням.
 *
 * Ініціалізує движок стеганографії з початковими налаштуваннями.
 */
SteganographyEngine::SteganographyEngine() {
    // Встановлюємо ключ за замовчуванням.
    currentKey = "DEFAULT_KEY";
}

/*
 * ============================================================================
 * ОСНОВНІ МЕТОДИ СТЕГАНОГРАФІЇ
 * ============================================================================
 */

/**
 * @brief Приховування текстового повідомлення в зображенні.
 *
 * Алгоритм:
 * 1. Додаємо маркери початку та кінця до повідомлення.
 * 2. (Опціонально) Шифруємо повідомлення.
 * 3. Конвертуємо текст у двійковий формат.
 * 4. Перевіряємо чи достатньо місця в зображенні.
 * 5. Ховаємо біти в молодших бітах RGB-каналів пікселів.
 *
 * @param image Вхідне зображення (оригінал).
 * @param message Текстове повідомлення.
 * @param encrypt Чи потрібно шифрувати.
 * @return Модифіковане зображення або пусте зображення у разі помилки.
 */
QImage SteganographyEngine::hideMessage(const QImage& image,
                                        const QString& message,
                                        bool encrypt) {
    // Перевірка вхідних даних.
    if (image.isNull() || message.isEmpty()) {
        qWarning() << "Empty image or message!";
        return QImage();
    }

    // Конвертуємо зображення в формат RGB32 для роботи з кольорами.
    QImage resultImage = image.convertToFormat(QImage::Format_RGB32);

    // ========== КРОК 1: Додаємо маркери ==========
    QString fullMessage = START_MARKER + message + END_MARKER;
    qDebug() << "Original message length:" << message.length();
    qDebug() << "Full message with markers length:" << fullMessage.length();

    // ========== КРОК 2: Шифруємо (якщо потрібно) ==========
    if (encrypt && !currentKey.isEmpty()) {
        fullMessage = xorCipher(fullMessage, currentKey);
        qDebug() << "Message encrypted, key:" << currentKey;
    }

    // ========== КРОК 3: Конвертуємо в двійковий формат ==========
    QString binary = textToBinary(fullMessage);
    qDebug() << "Binary length:" << binary.length() << "bits";

    // ========== КРОК 4: Перевіряємо ємність зображення ==========
    // Кожен піксель має 3 канали (R, G, B), отже 3 біти на піксель.
    int maxBits = resultImage.width() * resultImage.height() * 3;
    qDebug() << "Available bits:" << maxBits;

    if (binary.length() > maxBits) {
        qWarning() << "Message too long for this image!";
        qWarning() << "Required:" << binary.length() << "Available:" << maxBits;
        return QImage();
    }

    // ========== КРОК 5: Приховуємо біти в пікселях ==========
    int bitIndex = 0;  // Поточна позиція в бітовій послідовності.
    bool finished = false;

    // Проходимо по всіх пікселях зображення.
    for (int y = 0; y < resultImage.height() && !finished; ++y) {
        for (int x = 0; x < resultImage.width() && !finished; ++x) {
            // Отримуємо колір поточного піксела.
            QColor pixel = resultImage.pixelColor(x, y);

            // Модифікуємо червоний канал.
            if (bitIndex < binary.length()) {
                int r = setBit(pixel.red(), binary[bitIndex].digitValue());
                pixel.setRed(r);
                bitIndex++;
            }

            // Модифікуємо зелений канал.
            if (bitIndex < binary.length()) {
                int g = setBit(pixel.green(), binary[bitIndex].digitValue());
                pixel.setGreen(g);
                bitIndex++;
            }

            // Модифікуємо синій канал.
            if (bitIndex < binary.length()) {
                int b = setBit(pixel.blue(), binary[bitIndex].digitValue());
                pixel.setBlue(b);
                bitIndex++;
            } else {
                // Всі біти приховані.
                finished = true;
            }

            // Зберігаємо модифікований піксель.
            resultImage.setPixelColor(x, y, pixel);
        }
    }

    qDebug() << "Total bits hidden:" << bitIndex;
    qDebug() << "Hiding completed successfully";

    return resultImage;
}

/**
 * @brief Витягування прихованого повідомлення з зображення.
 *
 * Алгоритм:
 * 1. Конвертуємо зображення в потрібний формат.
 * 2. Витягуємо молодші біти з RGB-каналів пікселів.
 * 3. Конвертуємо біти назад у текст.
 * 4. (Опціонально) Дешифруємо повідомлення.
 * 5. Шукаємо маркери та витягуємо повідомлення.
 *
 * @param image Зображення з повідомленням.
 * @param decrypt Чи потрібно дешифрувати.
 * @return Витягнуте повідомлення або повідомлення про помилку.
 */
QString SteganographyEngine::extractMessage(const QImage& image, bool decrypt) {
    // Перевірка вхідних даних.
    if (image.isNull()) {
        qWarning() << "Empty image!";
        return QString();
    }

    // Конвертуємо зображення в формат RGB32.
    QImage workImage = image.convertToFormat(QImage::Format_RGB32);
    QString binary;

    // ========== ОПТИМІЗАЦІЯ: Обмежуємо пошук ==========
    // Щоб уникнути зависання на зображеннях без повідомлення.
    const int MAX_CHARS_TO_SEARCH = 50000;  // Шукаємо максимум 50000 символів.
    const int BITS_PER_CHAR = 16;           // UTF-16: 16 біт на символ.
    const int MAX_BITS = MAX_CHARS_TO_SEARCH * BITS_PER_CHAR;

    int bitsExtracted = 0;
    bool endMarkerFound = false;

    // ========== КРОК 1: Витягуємо біти з пікселів ==========
    for (int y = 0; y < workImage.height() && !endMarkerFound; ++y) {
        for (int x = 0; x < workImage.width() && !endMarkerFound; ++x) {
            QColor pixel = workImage.pixelColor(x, y);

            // Витягуємо молодший біт з червоного каналу.
            binary += QString::number(getBit(pixel.red()));
            bitsExtracted++;
            if (bitsExtracted >= MAX_BITS) break;

            // Витягуємо молодший біт з зеленого каналу.
            binary += QString::number(getBit(pixel.green()));
            bitsExtracted++;
            if (bitsExtracted >= MAX_BITS) break;

            // Витягуємо молодший біт з синього каналу.
            binary += QString::number(getBit(pixel.blue()));
            bitsExtracted++;
            if (bitsExtracted >= MAX_BITS) break;

            // ========== ОПТИМІЗАЦІЯ: Перевіряємо наявність END_MARKER ==========
            // Перевіряємо кожні 320 біт (20 символів).
            if (bitsExtracted % 320 == 0 && bitsExtracted >= 320) {
                QString currentText = binaryToText(binary);

                // Перевіряємо незашифрований текст.
                if (currentText.contains(END_MARKER)) {
                    endMarkerFound = true;
                    qDebug() << "END marker found at" << bitsExtracted
                             << "bits (unencrypted)";
                    break;
                }

                // Якщо потрібно дешифрування, перевіряємо зашифрований текст.
                if (decrypt && !currentKey.isEmpty()) {
                    QString decryptedText = xorCipher(currentText, currentKey);
                    if (decryptedText.contains(END_MARKER)) {
                        endMarkerFound = true;
                        qDebug() << "END marker found at" << bitsExtracted
                                 << "bits (encrypted)";
                        break;
                    }
                }
            }
        }
    }

    qDebug() << "Total bits extracted:" << bitsExtracted;

    // ========== КРОК 2: Конвертуємо біти в текст ==========
    QString extractedText = binaryToText(binary);
    qDebug() << "Text length before decryption:" << extractedText.length();

    // ========== КРОК 3: Дешифруємо (якщо потрібно) ==========
    if (decrypt && !currentKey.isEmpty()) {
        extractedText = xorCipher(extractedText, currentKey);
        qDebug() << "Text decrypted, length:" << extractedText.length();
    }

    // ========== КРОК 4: Шукаємо маркери ==========
    int startPos = extractedText.indexOf(START_MARKER);
    int endPos = extractedText.indexOf(END_MARKER);

    qDebug() << "START marker position:" << startPos;
    qDebug() << "END marker position:" << endPos;

    // Перевіряємо чи знайдені маркери.
    if (startPos == -1 || endPos == -1) {
        qWarning() << "Markers not found!";
        qDebug() << "First 200 chars of extracted text:"
                 << extractedText.left(200);
        return "Повідомлення не знайдено";
    }

    // ========== КРОК 5: Витягуємо повідомлення між маркерами ==========
    QString message = extractedText.mid(startPos + START_MARKER.length(),
                                        endPos - startPos - START_MARKER.length());

    qDebug() << "Message extracted successfully, length:" << message.length();

    return message;
}

/*
 * ============================================================================
 * МЕТОДИ АНАЛІЗУ
 * ============================================================================
 */

/**
 * @brief Аналіз змін між оригінальним і модифікованим зображенням.
 *
 * Обчислює:
 * - Розміри зображень.
 * - Статистику повідомлення.
 * - PSNR (Peak Signal-to-Noise Ratio).
 * - MSE (Mean Squared Error).
 * - Кількість змінених пікселів.
 *
 * @param original Оригінальне зображення.
 * @param modified Модифіковане зображення.
 * @param message Приховане повідомлення.
 * @return Структура з результатами аналізу.
 */
ImageAnalysisResult SteganographyEngine::analyzeChanges(
    const QImage& original,
    const QImage& modified,
    const QString& message) {

    ImageAnalysisResult result;

    // Перевірка вхідних даних.
    if (original.isNull() || modified.isNull()) {
        qWarning() << "Cannot analyze: empty images!";
        return result;
    }

    // ========== Розміри зображень ==========
    result.originalWidth = original.width();
    result.originalHeight = original.height();
    result.modifiedWidth = modified.width();
    result.modifiedHeight = modified.height();

    // ========== Інформація про повідомлення ==========
    result.message = message;
    result.messageLength = message.length();
    result.hasMessage = !message.isEmpty();

    // ========== Обчислюємо використані біти ==========
    QString fullMessage = START_MARKER + message + END_MARKER;
    QString binary = textToBinary(fullMessage);
    result.bitsUsed = binary.length();

    // ========== Обчислюємо ємність ==========
    result.maxCapacity = calculateMaxCapacity(original);
    result.capacityUsed = (result.messageLength / result.maxCapacity) * 100.0;

    // ========== Обчислюємо якісні метрики ==========
    result.mse = calculateMSE(original, modified);
    result.psnr = calculatePSNR(original, modified);

    // ========== Підраховуємо змінені пікселі ==========
    result.modifiedPixels = 0;
    for (int y = 0; y < original.height(); ++y) {
        for (int x = 0; x < original.width(); ++x) {
            QColor origColor = original.pixelColor(x, y);
            QColor modColor = modified.pixelColor(x, y);

            // Перевіряємо чи змінився хоча б один канал.
            if (origColor.red() != modColor.red() ||
                origColor.green() != modColor.green() ||
                origColor.blue() != modColor.blue()) {
                result.modifiedPixels++;
            }
        }
    }

    return result;
}

/**
 * @brief Обчислення максимальної ємності зображення.
 *
 * Формула: (кількість_пікселів × 3 біти - біти_маркерів) / 16 біт_на_символ.
 *
 * @param image Зображення для аналізу.
 * @return Максимальна кількість символів.
 */
int SteganographyEngine::calculateMaxCapacity(const QImage& image) {
    if (image.isNull()) return 0;

    // Загальна кількість бітів, які можна приховати.
    // 3 біти на піксель (по одному біту на канал R, G, B).
    int totalBits = image.width() * image.height() * 3;

    // Віднімаємо біти для маркерів START та END.
    QString markers = START_MARKER + END_MARKER;
    int markerBits = textToBinary(markers).length();

    // Обчислюємо кількість символів.
    // 16 біт на символ (UTF-16).
    int maxChars = (totalBits - markerBits) / 16;

    return maxChars;
}

/**
 * @brief Перевірка наявності прихованого повідомлення.
 *
 * @param image Зображення для перевірки.
 * @return true якщо повідомлення знайдено.
 */
bool SteganographyEngine::hasHiddenMessage(const QImage& image) {
    QString extracted = extractMessage(image, false);
    return !extracted.isEmpty() && extracted != "Повідомлення не знайдено";
}

/*
 * ============================================================================
 * МЕТОДИ КОНВЕРТАЦІЇ
 * ============================================================================
 */

/**
 * @brief Конвертація тексту в двійковий формат.
 *
 * Кожен символ Unicode (16 біт) перетворюється на послідовність з '0' та '1'.
 * Приклад: 'A' (65) → "0000000001000001".
 *
 * @param text Текст для конвертації.
 * @return Двійкове представлення.
 */
QString SteganographyEngine::textToBinary(const QString& text) {
    QString binary;

    // Проходимо по кожному символу.
    for (QChar ch : text) {
        // Отримуємо Unicode код символу (16 біт).
        ushort unicode = ch.unicode();

        // Конвертуємо кожен біт у символ '0' або '1'.
        for (int i = 15; i >= 0; --i) {
            // Зсуваємо біт на позицію i та перевіряємо його значення.
            binary += QString::number((unicode >> i) & 1);
        }
    }

    return binary;
}

/**
 * @brief Конвертація двійкового формату в текст..
 *
 * Групує біти по 16 та конвертує їх назад у символи Unicode.
 * Приклад: "0000000001000001" → 'A' (65).
 *
 * @param binary Двійкове представлення.
 * @return Відновлений текст.
 */
QString SteganographyEngine::binaryToText(const QString& binary) {
    QString text;

    // Обробляємо по 16 біт (один символ Unicode).
    for (int i = 0; i < binary.length() - 15; i += 16) {
        QString chunk = binary.mid(i, 16);  // Беремо 16 біт.
        ushort unicode = 0;

        // Конвертуємо 16-бітний рядок в число.
        for (int j = 0; j < 16; ++j) {
            if (chunk[j] == '1') {
                // Встановлюємо відповідний біт.
                unicode |= (1 << (15 - j));
            }
        }

        // Додаємо символ до результату.
        text += QChar(unicode);
    }

    return text;
}

/*
 * ============================================================================
 * МЕТОДИ ШИФРУВАННЯ
 * ============================================================================
 */

/**
 * @brief XOR-шифрування/дешифрування.
 *
 * Використовує операцію XOR для симетричного шифрування.
 * Той самий метод використовується для шифрування та дешифрування.
 *
 * @param text Текст для шифрування/дешифрування.
 * @param key Ключ шифрування.
 * @return Шифрований/дешифрований текст.
 */
QString SteganographyEngine::xorCipher(const QString& text, const QString& key) {
    // Якщо ключ порожній, повертаємо текст без змін.
    if (key.isEmpty()) return text;

    QString result;
    int keyIndex = 0;

    // Проходимо по кожному символу тексту.
    for (QChar ch : text) {
        // Отримуємо коди символів.
        ushort charCode = ch.unicode();
        ushort keyCode = key[keyIndex % key.length()].unicode();

        // Виконуємо XOR операцію.
        ushort encrypted = charCode ^ keyCode;

        // Додаємо зашифрований символ.
        result += QChar(encrypted);
        keyIndex++;
    }

    return result;
}

/**
 * @brief Генерація ключа шифрування з персональних даних.
 *
 * Використовує SHA-256 хеш-функцію для створення унікального ключа
 * на основі особистої інформації.
 *
 * @param firstName Ім'я.
 * @param lastName Прізвище.
 * @param birthDate Дата народження (формат: ddMMyyyy).
 * @param phone Номер телефону.
 * @return Згенерований ключ (16 hex символів).
 */
QString SteganographyEngine::generateKey(const QString& firstName,
                                         const QString& lastName,
                                         const QString& birthDate,
                                         const QString& phone) {
    // Об'єднуємо всі персональні дані в один рядок.
    QString combined = firstName + lastName + birthDate + phone;

    // Обчислюємо SHA-256 хеш.
    QByteArray hash = QCryptographicHash::hash(
        combined.toUtf8(),
        QCryptographicHash::Sha256
        );

    // Конвертуємо в hex рядок та беремо перші 16 символів.
    QString key = QString(hash.toHex()).left(16).toUpper();

    // Зберігаємо ключ.
    currentKey = key;

    qDebug() << "Generated key:" << key;

    return key;
}

/**
 * @brief Встановлення користувацького ключа шифрування.
 *
 * @param key Ключ шифрування.
 */
void SteganographyEngine::setKey(const QString& key) {
    currentKey = key;
    qDebug() << "Encryption key set to:" << key;
}

/*
 * ============================================================================
 * ПРИВАТНІ МЕТОДИ (РОБОТА З БІТАМИ)
 * ============================================================================
 */

/**
 * @brief Встановлення молодшого біту в значенні піксела.
 *
 * Алгоритм:
 * 1. Очищаємо молодший біт (встановлюємо в 0): pixel & 0xFE.
 * 2. Встановлюємо потрібний біт: result | bit.
 *
 * Приклад: pixel=150 (10010110), bit=1.
 * 1. 10010110 & 11111110 = 10010110.
 * 2. 10010110 | 00000001 = 10010111 (151).
 *
 * @param pixel Значення кольору (0-255).
 * @param bit Біт для встановлення (0 або 1).
 * @return Модифіковане значення кольору.
 */
int SteganographyEngine::setBit(int pixel, int bit) {
    // 0xFE = 11111110 (всі біти 1 крім молодшого).
    return (pixel & 0xFE) | bit;
}

/**
 * @brief Отримання молодшого біту з значення піксела.
 *
 * Використовує маску 0x01 (00000001) для витягування останнього біта.
 *
 * Приклад: pixel=151 (10010111).
 * 10010111 & 00000001 = 00000001 (1).
 *
 * @param pixel Значення кольору (0-255).
 * @return Молодший біт (0 або 1).
 */
int SteganographyEngine::getBit(int pixel) {
    return pixel & 1;
}

/*
 * ============================================================================
 * МЕТОДИ ОБЧИСЛЕННЯ ЯКОСТІ
 * ============================================================================
 */

/**
 * @brief Обчислення PSNR між двома зображеннями.
 *
 * PSNR (Peak Signal-to-Noise Ratio) - міра якості зображення.
 * Формула: PSNR = 10 × log10(MAX² / MSE)
 * де MAX = 255 (максимальне значення піксела).
 *
 * Інтерпретація:
 * - PSNR > 40 дБ: Відмінна якість (зміни непомітні).
 * - PSNR 30-40 дБ: Добра якість (мінімальні зміни).
 * - PSNR < 30 дБ: Низька якість (помітні зміни).
 *
 * @param original Оригінальне зображення.
 * @param modified Модифіковане зображення.
 * @return Значення PSNR в децибелах.
 */
double SteganographyEngine::calculatePSNR(const QImage& original,
                                          const QImage& modified) {
    // Обчислюємо MSE.
    double mse = calculateMSE(original, modified);

    // Якщо MSE = 0, зображення ідентичні.
    if (mse == 0) {
        return 100.0;  // Максимальна якість.
    }

    // Максимальне значення піксела.
    double maxPixelValue = 255.0;

    // Обчислюємо PSNR за формулою.
    double psnr = 10.0 * log10((maxPixelValue * maxPixelValue) / mse);

    return psnr;
}

/**
 * @brief Обчислення MSE між двома зображеннями.
 *
 * MSE (Mean Squared Error) - середньоквадратична помилка.
 * Формула: MSE = (1/N) × Σ(Original - Modified)²
 * де N - загальна кількість пікселів.
 *
 * Інтерпретація:
 * - MSE ≈ 0: Зображення практично ідентичні.
 * - MSE < 1: Дуже малі відмінності.
 * - MSE > 10: Значні відмінності.
 *
 * @param original Оригінальне зображення.
 * @param modified Модифіковане зображення.
 * @return Значення MSE.
 */
double SteganographyEngine::calculateMSE(const QImage& original,
                                         const QImage& modified) {
    // Перевіряємо чи розміри однакові.
    if (original.size() != modified.size()) {
        qWarning() << "Images have different sizes!";
        return -1.0;
    }

    double sum = 0.0;
    int totalPixels = original.width() * original.height();

    // Проходимо по всіх пікселях.
    for (int y = 0; y < original.height(); ++y) {
        for (int x = 0; x < original.width(); ++x) {
            // Отримуємо кольори пікселів.
            QColor origColor = original.pixelColor(x, y);
            QColor modColor = modified.pixelColor(x, y);

            // Обчислюємо різницю для кожного каналу.
            double dr = origColor.red() - modColor.red();
            double dg = origColor.green() - modColor.green();
            double db = origColor.blue() - modColor.blue();

            // Додаємо квадрати різниць.
            // Ділимо на 3 бо усереднюємо по каналах.
            sum += (dr * dr + dg * dg + db * db) / 3.0;
        }
    }

    // Обчислюємо середнє значення.
    return sum / totalPixels;
}
