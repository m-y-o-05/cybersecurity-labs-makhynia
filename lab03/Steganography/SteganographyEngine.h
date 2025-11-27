/*
 * SteganographyEngine.h
 *
 * Заголовочний файл для реалізації алгоритмів стеганографії.
 * Містить класи для приховування та витягування текстової інформації
 * з використанням методу LSB (Least Significant Bit).
 *
 * Основні можливості:
 * - Приховування тексту в зображеннях.
 * - Витягування прихованого тексту.
 * - XOR шифрування/дешифрування.
 * - Аналіз якості стеганографії (PSNR, MSE).
 * - Генерація ключів з персональних даних.
 */

#ifndef STEGANOGRAPHYENGINE_H
#define STEGANOGRAPHYENGINE_H

#include <QImage>
#include <QString>
#include <QMap>
#include <QVector>

/**
 * @struct ImageAnalysisResult
 * @brief Структура для зберігання результатів аналізу зображення.
 *
 * Містить всю інформацію про порівняння оригінального
 * та модифікованого зображення.
 */
struct ImageAnalysisResult {
    // ========== Розміри зображень ==========
    int originalWidth = 0;      ///< Ширина оригінального зображення.
    int originalHeight = 0;     ///< Висота оригінального зображення.
    int modifiedWidth = 0;      ///< Ширина модифікованого зображення.
    int modifiedHeight = 0;     ///< Висота модифікованого зображення.

    // ========== Розміри файлів ==========
    qint64 originalFileSize = 0;  ///< Розмір оригінального файлу в байтах.
    qint64 modifiedFileSize = 0;  ///< Розмір модифікованого файлу в байтах.

    // ========== Статистика повідомлення ==========
    int messageLength = 0;      ///< Довжина прихованого повідомлення в символах.
    int bitsUsed = 0;          ///< Кількість використаних бітів.
    double capacityUsed = 0.0; ///< Відсоток використаної ємності.
    double maxCapacity = 0.0;  ///< Максимальна ємність в символах.

    // ========== Метрики якості ==========
    double psnr = 0.0;  ///< Peak Signal-to-Noise Ratio (дБ).
    double mse = 0.0;   ///< Mean Squared Error.
    int modifiedPixels = 0;  ///< Кількість змінених пікселів.

    // ========== Інформація про повідомлення ==========
    QString message;        ///< Приховане повідомлення.
    bool hasMessage = false;  ///< Чи містить зображення повідомлення.
};

/**
 * @class SteganographyEngine
 * @brief Основний клас для роботи зі стеганографією.
 *
 * Реалізує алгоритм LSB (Least Significant Bit) для приховування
 * текстової інформації в зображеннях.
 */
class SteganographyEngine {
public:
    // ========== Конструктор ==========

    /**
     * @brief Конструктор за замовчуванням.
     *
     * Ініціалізує движок стеганографії з налаштуваннями за замовчуванням.
     */
    SteganographyEngine();

    // ========== Основні методи ==========

    /**
     * @brief Приховування текстового повідомлення в зображенні.
     *
     * Використовує метод LSB для приховування тексту в молодших бітах
     * RGB-каналів зображення.
     *
     * @param image Вхідне зображення (оригінал).
     * @param message Текстове повідомлення для приховування.
     * @param encrypt Чи потрібно шифрувати повідомлення перед приховуванням.
     * @return Модифіковане зображення з прихованим повідомленням.
     */
    QImage hideMessage(const QImage& image, const QString& message, bool encrypt = false);

    /**
     * @brief Витягування прихованого повідомлення з зображення.
     *
     * Зчитує молодші біти пікселів та конвертує їх назад у текст.
     *
     * @param image Зображення з прихованим повідомленням.
     * @param decrypt Чи потрібно розшифрувати повідомлення після витягування.
     * @return Витягнуте текстове повідомлення.
     */
    QString extractMessage(const QImage& image, bool decrypt = false);

    /**
     * @brief Аналіз змін між оригінальним і модифікованим зображенням.
     *
     * Обчислює різні метрики якості стеганографії:
     * - PSNR (Peak Signal-to-Noise Ratio).
     * - MSE (Mean Squared Error).
     * - Кількість змінених пікселів.
     * - Використання ємності.
     *
     * @param original Оригінальне зображення.
     * @param modified Модифіковане зображення.
     * @param message Приховане повідомлення.
     * @return Структура з результатами аналізу.
     */
    ImageAnalysisResult analyzeChanges(const QImage& original,
                                       const QImage& modified,
                                       const QString& message);

    /**
     * @brief Обчислення максимальної ємності зображення.
     *
     * Визначає скільки символів можна приховати в даному зображенні
     * з урахуванням маркерів початку/кінця.
     *
     * @param image Зображення для аналізу.
     * @return Максимальна кількість символів, які можна приховати.
     */
    int calculateMaxCapacity(const QImage& image);

    /**
     * @brief Встановлення користувацького ключа шифрування.
     *
     * Дозволяє встановити ключ вручну (наприклад, отриманий раніше).
     *
     * @param key Ключ шифрування (hex рядок).
     */
    void setKey(const QString& key);

    /**
     * @brief Перевірка наявності прихованого повідомлення.
     *
     * @param image Зображення для перевірки.
     * @return true якщо зображення містить приховане повідомлення.
     */
    bool hasHiddenMessage(const QImage& image);

    // ========== Допоміжні методи ==========

    /**
     * @brief Конвертація тексту в двійковий формат.
     *
     * Перетворює кожен символ Unicode у 16-бітну двійкову послідовність.
     *
     * @param text Текст для конвертації.
     * @return Двійкове представлення (рядок з '0' та '1').
     */
    QString textToBinary(const QString& text);

    /**
     * @brief Конвертація двійкового формату в текст.
     *
     * Перетворює послідовність бітів назад у текст.
     *
     * @param binary Двійкове представлення.
     * @return Відновлений текст.
     */
    QString binaryToText(const QString& binary);

    /**
     * @brief XOR-шифрування/дешифрування.
     *
     * Простий симетричний шифр для базового захисту даних.
     *
     * @param text Текст для шифрування/дешифрування.
     * @param key Ключ шифрування.
     * @return Шифрований/дешифрований текст.
     */
    QString xorCipher(const QString& text, const QString& key);

    /**
     * @brief Генерація ключа шифрування з персональних даних.
     *
     * Використовує SHA-256 для створення унікального ключа
     * на основі особистої інформації користувача.
     *
     * @param firstName Ім'я.
     * @param lastName Прізвище.
     * @param birthDate Дата народження (формат: ddMMyyyy).
     * @param phone Номер телефону.
     * @return Згенерований ключ (16 символів hex).
     */
    QString generateKey(const QString& firstName,
                        const QString& lastName,
                        const QString& birthDate,
                        const QString& phone);

private:
    // ========== Приватні методи ==========

    /**
     * @brief Встановлення молодшого біту в значенні піксела.
     *
     * Замінює останній біт значення кольору на потрібний біт.
     *
     * @param pixel Значення кольору (0-255).
     * @param bit Біт для встановлення (0 або 1).
     * @return Модифіковане значення кольору.
     */
    int setBit(int pixel, int bit);

    /**
     * @brief Отримання молодшого біту з значення піксела.
     *
     * Витягує останній біт значення кольору.
     *
     * @param pixel Значення кольору (0-255).
     * @return Молодший біт (0 або 1).
     */
    int getBit(int pixel);

    /**
     * @brief Обчислення PSNR між двома зображеннями.
     *
     * PSNR (Peak Signal-to-Noise Ratio) - метрика якості зображення.
     * Чим вище значення, тим менша різниця між зображеннями.
     *
     * @param original Оригінальне зображення.
     * @param modified Модифіковане зображення.
     * @return Значення PSNR в децибелах (дБ).
     */
    double calculatePSNR(const QImage& original, const QImage& modified);

    /**
     * @brief Обчислення MSE між двома зображеннями.
     *
     * MSE (Mean Squared Error) - середньоквадратична помилка.
     * Чим менше значення, тим менша різниця між зображеннями.
     *
     * @param original Оригінальне зображення.
     * @param modified Модифіковане зображення.
     * @return Значення MSE.
     */
    double calculateMSE(const QImage& original, const QImage& modified);

    // ========== Константи ==========

    /// Маркер початку повідомлення (для визначення початку даних).
    const QString START_MARKER = "<<START>>";

    /// Маркер кінця повідомлення (для визначення кінця даних).
    const QString END_MARKER = "<<END>>";

    // ========== Змінні стану ==========

    /// Поточний ключ шифрування.
    QString currentKey;
};

#endif // STEGANOGRAPHYENGINE_H
