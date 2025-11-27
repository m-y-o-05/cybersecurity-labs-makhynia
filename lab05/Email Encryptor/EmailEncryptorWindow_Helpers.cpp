/*
 * EmailEncryptorWindow_Helpers.cpp
 *
 * Файл з допоміжними методами головного вікна.
 * Містить функції оформлення та теми.
 */

#include "EmailEncryptorWindow.h"

/*
 * ============================================================================
 * ЗАСТОСУВАННЯ ТЕМНОЇ ТЕМИ
 * ============================================================================
 */

/**
 * @brief Застосування темної теми з бірюзовими акцентамию
 *
 * Встановлює CSS-стилі для всіх елементів інтерфейсу:
 * - Темний фон (#1a1a1a)ю
 * - Бірюзові акценти (#00CED1)ю
 * - Градієнти для кнопокю
 * - Стилізація таблиць, вкладок, полів введенняю
 */
void EmailEncryptorWindow::applyDarkTheme() {
    QString darkTheme = R"(
        /* ========== Головне вікно ========== */
        QMainWindow {
            background: #1a1a1a;
        }

        QWidget {
            background: #1a1a1a;
            color: #e0e0e0;
        }

        /* ========== Вкладки ========== */
        QTabWidget::pane {
            border: 2px solid #00CED1;
            border-radius: 10px;
            background: #1a1a1a;
            padding: 10px;
        }

        QTabBar::tab {
            background: #2a2a2a;
            color: #e0e0e0;
            padding: 12px 25px;
            margin-right: 5px;
            border: 1px solid #3a3a3a;
            border-bottom: none;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            font-size: 13px;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00CED1, stop:1 #008B8B);
            color: #000000;
            font-weight: bold;
        }

        QTabBar::tab:hover:!selected {
            background: #3a3a3a;
            color: #00E5E8;
        }

        /* ========== Групи (QGroupBox) ========== */
        QGroupBox {
            border: 2px solid #00CED1;
            border-radius: 10px;
            margin-top: 15px;
            padding-top: 15px;
            background: #1a1a1a;
            font-size: 13px;
            font-weight: bold;
            color: #00E5E8;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 15px;
            padding: 5px 10px;
            background: #1a1a1a;
            color: #00CED1;
        }

        /* ========== Поля введення ========== */
        QLineEdit, QTextEdit, QDateEdit {
            background: #2a2a2a;
            border: 2px solid #3a3a3a;
            border-radius: 8px;
            padding: 8px;
            color: #e0e0e0;
            font-size: 12px;
        }

        QLineEdit:focus, QTextEdit:focus, QDateEdit:focus {
            border-color: #00CED1;
            background: #2f2f2f;
        }

        QLineEdit:read-only {
            background: #252525;
            color: #999;
        }

        /* ========== QDateEdit специфічні стилі ========== */
        QDateEdit::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 20px;
            border-left: 1px solid #3a3a3a;
            border-top-right-radius: 8px;
            border-bottom-right-radius: 8px;
            background: #3a3a3a;
        }

        QDateEdit::drop-down:hover {
            background: #4a4a4a;
        }

        QDateEdit::down-arrow {
            image: none;
            border: 2px solid #e0e0e0;
            width: 6px;
            height: 6px;
            border-top: none;
            border-right: none;
            transform: rotate(-45deg);
            margin-top: -2px;
        }

        QDateEdit::up-button, QDateEdit::down-button {
            background: transparent;
            border: none;
        }

        /* ========== Календар ========== */
        QCalendarWidget {
            background: #2a2a2a;
            color: #e0e0e0;
        }

        QCalendarWidget QWidget {
            alternate-background-color: #2a2a2a;
        }

        QCalendarWidget QAbstractItemView {
            background: #2a2a2a;
            selection-background-color: #00CED1;
            selection-color: #000000;
        }

        QCalendarWidget QToolButton {
            background: #3a3a3a;
            color: #e0e0e0;
            border: 1px solid #4a4a4a;
            border-radius: 4px;
            padding: 5px;
        }

        QCalendarWidget QToolButton:hover {
            background: #4a4a4a;
            border-color: #00CED1;
        }

        QCalendarWidget QMenu {
            background: #2a2a2a;
            color: #e0e0e0;
        }

        QCalendarWidget QSpinBox {
            background: #2a2a2a;
            color: #e0e0e0;
            border: 1px solid #3a3a3a;
            selection-background-color: #00CED1;
            selection-color: #000000;
        }

        QCalendarWidget QToolButton::menu-indicator {
            image: none;
        }

        /* ========== Кнопки ========== */
        QPushButton {
            background: #3a3a3a;
            border: 2px solid #4a4a4a;
            border-radius: 8px;
            color: #e0e0e0;
            padding: 10px;
            font-size: 13px;
        }

        QPushButton:hover {
            background: #4a4a4a;
            border-color: #00CED1;
            color: #00E5E8;
        }

        QPushButton:pressed {
            background: #2a2a2a;
        }

        QPushButton:disabled {
            background: #2a2a2a;
            color: #666666;
            border-color: #333333;
        }

        /* ========== Scrollbar ========== */
        QScrollBar:vertical {
            border: none;
            background: #2a2a2a;
            width: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background: #00CED1;
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background: #00E5E8;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            border: none;
            background: #2a2a2a;
            height: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal {
            background: #00CED1;
            border-radius: 6px;
            min-width: 20px;
        }

        QScrollBar::handle:horizontal:hover {
            background: #00E5E8;
        }

        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        /* ========== Tooltip ========== */
        QToolTip {
            background: #2a2a2a;
            color: #00E5E8;
            border: 2px solid #00CED1;
            border-radius: 5px;
            padding: 5px;
            font-size: 12px;
        }

        /* ========== MessageBox ========== */
        QMessageBox {
            background: #1a1a1a;
        }

        QMessageBox QLabel {
            color: #e0e0e0;
        }

        QMessageBox QPushButton {
            min-width: 80px;
            max-height: 15px;
        }
    )";

    setStyleSheet(darkTheme);
}
