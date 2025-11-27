/*
 * SteganographyWindow_Slots.cpp
 *
 * Файл з обробниками подій (slots) головного вікна.
 * Містить всю логіку обробки користувацьких дій.
 */

#include "SteganographyWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QClipboard>
#include <QApplication>

/*
 * ============================================================================
 * ОБРОБНИКИ ВКЛАДКИ ПРИХОВУВАННЯ
 * ============================================================================
 */

/**
 * @brief Вибір вхідного зображення для приховування.
 *
 * Відкриває діалог вибору файлу, завантажує зображення
 * та відображає інформацію про його ємність.
 */
void SteganographyWindow::selectInputImage() {
    // Відкриваємо діалог вибору файлу.
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Виберіть зображення",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp)"
        );

    // Якщо користувач нічого не вибрав.
    if (fileName.isEmpty()) {
        return;
    }

    // Завантажуємо зображення.
    originalImage.load(fileName);

    if (!originalImage.isNull()) {
        // Зберігаємо шлях.
        currentImagePath = fileName;
        hide_imagePathEdit->setText(fileName);

        // ========== Відображаємо попередній перегляд ==========
        QPixmap pixmap = QPixmap::fromImage(originalImage);
        hide_imageLabel->setPixmap(
            pixmap.scaled(hide_imageLabel->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );

        // Оновлюємо зображення у вкладці аналізу.
        analysis_originalImageLabel->setPixmap(
            pixmap.scaled(analysis_originalImageLabel->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );

        // ========== Обчислюємо та показуємо ємність ==========
        int maxCapacity = stegoEngine->calculateMaxCapacity(originalImage);

        QMessageBox::information(
            this,
            "Інформація про зображення",
            QString("Зображення завантажено!\n\n"
                    "Розмір: %1 x %2 пікселів\n"
                    "Максимальна ємність: %3 символів")
                .arg(originalImage.width())
                .arg(originalImage.height())
                .arg(maxCapacity)
            );
    } else {
        QMessageBox::warning(
            this,
            "Помилка",
            "Не вдалося завантажити зображення!"
            );
    }
}

/**
 * @brief Приховування повідомлення в зображенні.
 *
 * Алгоритм:
 * 1. Перевірка наявності зображення та тексту.
 * 2. Перевірка ємності зображення.
 * 3. Генерація ключа (якщо потрібне шифрування).
 * 4. Приховування повідомлення.
 * 5. Відображення результату.
 */
void SteganographyWindow::hideMessage() {
    // ========== КРОК 1: Перевірка наявності зображення ==========
    if (originalImage.isNull()) {
        QMessageBox::warning(
            this,
            "Помилка",
            "Спочатку виберіть зображення!"
            );
        return;
    }

    // ========== КРОК 2: Перевірка наявності тексту ==========
    QString message = hide_messageEdit->toPlainText();
    if (message.isEmpty()) {
        QMessageBox::warning(
            this,
            "Помилка",
            "Введіть текст для приховування!"
            );
        return;
    }

    // ========== КРОК 3: Перевірка ємності ==========
    int maxCapacity = stegoEngine->calculateMaxCapacity(originalImage);
    if (message.length() > maxCapacity) {
        QMessageBox::warning(
            this,
            "Помилка",
            QString("Повідомлення занадто довге!\n\n"
                    "Максимальна довжина: %1 символів\n"
                    "Ваше повідомлення: %2 символів")
                .arg(maxCapacity)
                .arg(message.length())
            );
        return;
    }

    // ========== КРОК 4: Показуємо прогрес ==========
    hide_progressBar->setVisible(true);
    hide_progressBar->setValue(30);

    // ========== КРОК 5: Генеруємо ключ (якщо потрібно) ==========
    bool encrypt = hide_encryptCheckBox->isChecked();
    QString generatedKey;

    if (encrypt) {
        // Отримуємо персональні дані.
        QString firstName = hide_firstNameEdit->text();
        QString lastName = hide_lastNameEdit->text();
        QString birthDate = hide_birthDateEdit->date().toString("ddMMyyyy");
        QString phone = hide_phoneEdit->text();

        // Перевіряємо чи заповнені дані.
        if (firstName.isEmpty() || lastName.isEmpty()) {
            QMessageBox::warning(
                this,
                "Помилка",
                "Для шифрування заповніть персональні дані!"
                );
            hide_progressBar->setVisible(false);
            return;
        }

        // Генеруємо ключ.
        generatedKey = stegoEngine->generateKey(
            firstName,
            lastName,
            birthDate,
            phone
            );
        hide_progressBar->setValue(50);

        // Відображаємо згенерований ключ.
        hide_generatedKeyEdit->setText(generatedKey);
    } else {
        // Очищаємо поле ключа якщо шифрування не використовується.
        hide_generatedKeyEdit->clear();
    }

    // ========== КРОК 6: Приховуємо повідомлення ==========
    modifiedImage = stegoEngine->hideMessage(originalImage, message, encrypt);
    hide_progressBar->setValue(80);

    // ========== КРОК 7: Обробка результату ==========
    if (!modifiedImage.isNull()) {
        // Відображаємо результат.
        QPixmap pixmap = QPixmap::fromImage(modifiedImage);
        hide_resultImageLabel->setPixmap(
            pixmap.scaled(hide_resultImageLabel->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );

        // Оновлюємо вкладку аналізу.
        analysis_modifiedImageLabel->setPixmap(
            pixmap.scaled(analysis_modifiedImageLabel->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );

        hide_progressBar->setValue(100);

        // Формуємо повідомлення про успіх.
        QString successMsg = "Повідомлення успішно приховано!\n\n";

        if (encrypt && !generatedKey.isEmpty()) {
            successMsg += QString(
                              "Ваш ключ шифрування: %1\n\n"
                              "⚠️ ВАЖЛИВО: Збережіть цей ключ!\n"
                              "Він знадобиться для розшифрування повідомлення.\n\n"
                              ).arg(generatedKey);
        }

        successMsg += "Перейдіть до вкладки 'Аналіз зображення' "
                      "для детального аналізу.";

        QMessageBox::information(this, "Успіх", successMsg);
    } else {
        QMessageBox::critical(
            this,
            "Помилка",
            "Не вдалося приховати повідомлення!"
            );
    }

    // Приховуємо прогрес-бар.
    hide_progressBar->setVisible(false);
}

/**
 * @brief Очищення всіх полів вкладки приховування.
 */
void SteganographyWindow::clearHideTab() {
    // Очищуємо текстові поля.
    hide_firstNameEdit->clear();
    hide_lastNameEdit->clear();
    hide_birthDateEdit->setDate(QDate(2000, 1, 1));
    hide_phoneEdit->clear();
    hide_messageEdit->clear();
    hide_imagePathEdit->clear();
    hide_generatedKeyEdit->clear();

    // Очищуємо мітки зображень.
    hide_imageLabel->clear();
    hide_imageLabel->setText("Натисніть кнопку\nдля вибору зображення");
    hide_resultImageLabel->clear();
    hide_resultImageLabel->setText("Результат з'явиться\nпісля приховування");

    // Очищуємо зображення.
    originalImage = QImage();
    modifiedImage = QImage();
    currentImagePath.clear();
}

/*
 * ============================================================================
 * ОБРОБНИКИ ВКЛАДКИ ВИТЯГУВАННЯ
 * ============================================================================
 */

/**
 * @brief Вибір зображення для витягування повідомлення.
 *
 * Відкриває діалог вибору файлу та завантажує зображення.
 */
void SteganographyWindow::selectExtractImage() {
    // Відкриваємо діалог вибору файлу.
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Виберіть зображення",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    // Завантажуємо зображення.
    QImage image;
    image.load(fileName);

    if (!image.isNull()) {
        extractImagePath = fileName;
        extract_imagePathEdit->setText(fileName);

        // Відображаємо попередній перегляд.
        QPixmap pixmap = QPixmap::fromImage(image);
        extract_imageLabel->setPixmap(
            pixmap.scaled(extract_imageLabel->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );
    } else {
        QMessageBox::warning(
            this,
            "Помилка",
            "Не вдалося завантажити зображення!"
            );
    }
}

/**
 * @brief Витягування прихованого повідомлення.
 *
 * Алгоритм:
 * 1. Перевірка наявності зображення.
 * 2. Встановлення ключа дешифрування (якщо потрібно).
 * 3. Витягування повідомлення.
 * 4. Відображення результату.
 */
void SteganographyWindow::extractMessage() {
    // ========== КРОК 1: Перевірка наявності зображення ==========
    if (extractImagePath.isEmpty()) {
        QMessageBox::warning(
            this,
            "Помилка",
            "Спочатку виберіть зображення!"
            );
        return;
    }

    // Завантажуємо зображення.
    QImage image;
    image.load(extractImagePath);

    if (image.isNull()) {
        QMessageBox::warning(
            this,
            "Помилка",
            "Не вдалося завантажити зображення!"
            );
        return;
    }

    // ========== КРОК 2: Показуємо прогрес ==========
    extract_progressBar->setVisible(true);
    extract_progressBar->setValue(30);

    bool decrypt = extract_decryptCheckBox->isChecked();

    // ========== КРОК 3: Встановлюємо ключ (якщо потрібно) ==========
    if (decrypt) {
        // Перевіряємо чи введений ключ вручну.
        QString manualKey = extract_keyEdit->text().trimmed();

        if (!manualKey.isEmpty()) {
            // Використовуємо введений ключ.
            stegoEngine->setKey(manualKey);
        } else {
            // Пробуємо згенерувати ключ з персональних даних.
            QString firstName = hide_firstNameEdit->text();
            QString lastName = hide_lastNameEdit->text();
            QString birthDate = hide_birthDateEdit->date().toString("ddMMyyyy");
            QString phone = hide_phoneEdit->text();

            if (firstName.isEmpty() || lastName.isEmpty()) {
                // Питаємо чи продовжити без розшифрування.
                int result = QMessageBox::question(
                    this,
                    "Попередження",
                    "Ключ не введений і персональні дані не заповнені!\n"
                    "Продовжити без розшифрування?",
                    QMessageBox::Yes | QMessageBox::No
                    );

                if (result == QMessageBox::No) {
                    extract_progressBar->setVisible(false);
                    return;
                }

                decrypt = false;
            } else {
                // Генеруємо ключ.
                QString generatedKey = stegoEngine->generateKey(
                    firstName,
                    lastName,
                    birthDate,
                    phone
                    );

                QMessageBox::information(
                    this,
                    "Згенерований ключ",
                    QString("Використовується ключ: %1").arg(generatedKey)
                    );
            }
        }
    }

    extract_progressBar->setValue(60);

    // ========== КРОК 4: Витягуємо повідомлення ==========
    QString message = stegoEngine->extractMessage(image, decrypt);
    extract_progressBar->setValue(100);

    // ========== КРОК 5: Відображаємо результат ==========
    extract_messageEdit->setPlainText(message);

    if (message != "Повідомлення не знайдено") {
        QMessageBox::information(
            this,
            "Успіх",
            "Повідомлення успішно витягнуто!"
            );
    } else {
        QMessageBox::warning(
            this,
            "Попередження",
            "Повідомлення не знайдено або зображення не містить прихованих даних.\n"
            "Якщо повідомлення було зашифроване, перевірте правильність ключа."
            );
    }

    extract_progressBar->setVisible(false);
}

/**
 * @brief Очищення всіх полів вкладки витягування.
 */
void SteganographyWindow::clearExtractTab() {
    extract_imagePathEdit->clear();
    extract_imageLabel->clear();
    extract_imageLabel->setText("Натисніть кнопку\nдля вибору зображення");
    extract_messageEdit->clear();
    extract_keyEdit->clear();
    extractImagePath.clear();
}

/*
 * ============================================================================
 * ОБРОБНИКИ ВКЛАДКИ АНАЛІЗУ
 * ============================================================================
 */

/**
 * @brief Проведення аналізу зображення.
 *
 * Порівнює оригінальне та модифіковане зображення,
 * обчислює метрики якості та відображає результати.
 */
void SteganographyWindow::analyzeImage() {
    // Перевірка наявності зображень.
    if (originalImage.isNull() || modifiedImage.isNull()) {
        QMessageBox::warning(
            this,
            "Помилка",
            "Спочатку приховайте повідомлення у вкладці 'Приховування'!"
            );
        return;
    }

    // Отримуємо повідомлення.
    QString message = hide_messageEdit->toPlainText();

    // Виконуємо аналіз.
    ImageAnalysisResult result = stegoEngine->analyzeChanges(
        originalImage,
        modifiedImage,
        message
        );

    // Відображаємо результати.
    displayAnalysisResults(result);
}

/*
 * ============================================================================
 * ОБРОБНИКИ ВКЛАДКИ АЛГОРИТМУ
 * ============================================================================
 */

/**
 * @brief Заповнення форми прикладом персональних даних.
 *
 * Автоматично заповнює всі поля для демонстрації роботи програми.
 */
void SteganographyWindow::fillPersonalDataExample() {
    hide_firstNameEdit->setText("Іван");
    hide_lastNameEdit->setText("Петренко");
    hide_birthDateEdit->setDate(QDate(1995, 3, 15));
    hide_phoneEdit->setText("+380123456789");
    hide_messageEdit->setPlainText(
        "Це моє секретне повідомлення, яке я хочу приховати в зображенні. "
        "Стеганографія дозволяє зберігати конфіденційність даних!"
        );
}
