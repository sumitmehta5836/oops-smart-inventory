#pragma once
#include <QString>

namespace Theme {

inline QString darkStyleSheet() {
    return R"(
        /* Global Window & Font Defaults */
        QWidget {
            background-color: #0f172a;
            color: #f8fafc;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            font-size: 13px;
        }

        /* Top Bar & Cards */
        #HeaderBar {
            background-color: #1e293b;
            border-bottom: 1px solid #334155;
            padding: 10px 16px;
        }
        #Sidebar {
            background-color: #1e293b;
            border-right: 1px solid #334155;
            padding: 12px 8px;
        }
        #ContentCard {
            background-color: #1e293b;
            border: 1px solid #334155;
            border-radius: 8px;
            padding: 16px;
        }
        #StatCard {
            background-color: #1e293b;
            border: 1px solid #334155;
            border-radius: 10px;
            padding: 14px;
        }

        #UserPill {
            background-color: #0f172a;
            border: 1px solid #334155;
            border-radius: 16px;
            padding: 4px 12px;
        }
        #UserNameLabel {
            color: #e2e8f0;
            font-size: 12px;
        }
        #BrandLabel {
            font-size: 16px;
            font-weight: 800;
            color: #f8fafc;
            letter-spacing: 0.5px;
        }

        /* Navigation Sidebar Buttons */
        QPushButton#NavButton {
            background-color: transparent;
            color: #94a3b8;
            font-size: 13px;
            font-weight: 600;
            text-align: left;
            padding: 10px 14px;
            border-radius: 6px;
            border: none;
        }
        QPushButton#NavButton:hover {
            background-color: #334155;
            color: #f8fafc;
        }
        QPushButton#NavButton:checked {
            background-color: #2563eb;
            color: #ffffff;
        }

        /* Standard Buttons */
        QPushButton {
            background-color: #2563eb;
            color: #ffffff;
            font-weight: 600;
            padding: 8px 16px;
            border-radius: 6px;
            border: 1px solid transparent;
        }
        QPushButton:hover {
            background-color: #1d4ed8;
        }
        QPushButton:pressed {
            background-color: #1e40af;
        }
        QPushButton:disabled {
            background-color: #334155;
            color: #64748b;
            border-color: #475569;
        }

        /* Success / Checkout Button */
        QPushButton#SuccessButton {
            background-color: #10b981;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 10px 20px;
            border-radius: 6px;
        }
        QPushButton#SuccessButton:hover {
            background-color: #059669;
        }
        QPushButton#SuccessButton:pressed {
            background-color: #047857;
        }

        /* Danger / Delete Button */
        QPushButton#DangerButton {
            background-color: #ef4444;
            color: #ffffff;
        }
        QPushButton#DangerButton:hover {
            background-color: #dc2626;
        }
        QPushButton#DangerButton:pressed {
            background-color: #b91c1c;
        }

        /* Secondary / Outline Button */
        QPushButton#SecondaryButton {
            background-color: #334155;
            color: #f8fafc;
            border: 1px solid #475569;
        }
        QPushButton#SecondaryButton:hover {
            background-color: #475569;
        }

        /* Theme Toggle Button */
        QPushButton#ThemeToggleBtn {
            background-color: #334155;
            color: #f8fafc;
            border: 1px solid #475569;
            font-size: 12px;
            font-weight: 600;
            padding: 6px 12px;
            border-radius: 6px;
        }
        QPushButton#ThemeToggleBtn:hover {
            background-color: #475569;
            border-color: #64748b;
        }

        /* Input Fields */
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background-color: #0f172a;
            color: #f8fafc;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 7px 10px;
            selection-background-color: #2563eb;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
            border: 1px solid #3b82f6;
            background-color: #131d35;
        }

        /* High-visibility Login Input Fields */
        QLineEdit#LoginInput {
            background-color: #090d16;
            color: #ffffff;
            border: 2px solid #475569;
            border-radius: 8px;
            padding: 0 16px;
            font-size: 15px;
            font-weight: 500;
        }
        QLineEdit#LoginInput:focus {
            border: 2px solid #3b82f6;
            background-color: #0f172a;
        }

        /* ComboBox Dropdown */
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 24px;
            border-left-width: 0px;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QComboBox QAbstractItemView {
            background-color: #1e293b;
            color: #f8fafc;
            selection-background-color: #2563eb;
            border: 1px solid #334155;
        }

        /* Tables */
        QTableWidget, QTableView {
            background-color: #1e293b;
            color: #f8fafc;
            gridline-color: #334155;
            border: 1px solid #334155;
            border-radius: 6px;
            selection-background-color: #2563eb;
            selection-color: #ffffff;
        }
        QHeaderView::section {
            background-color: #0f172a;
            color: #94a3b8;
            font-weight: 600;
            padding: 8px 10px;
            border: none;
            border-bottom: 2px solid #334155;
            border-right: 1px solid #1e293b;
        }
        QTableWidget::item {
            padding: 6px 10px;
            border-bottom: 1px solid #283548;
        }
        QTableWidget::item:selected {
            background-color: #1e3a8a;
            color: #ffffff;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: #0f172a;
            width: 10px;
            margin: 0px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background: #334155;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical:hover {
            background: #475569;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        /* Status Bar */
        QStatusBar {
            background-color: #1e293b;
            color: #94a3b8;
            border-top: 1px solid #334155;
            padding: 4px;
        }

        /* Group Box */
        QGroupBox {
            border: 1px solid #334155;
            border-radius: 8px;
            margin-top: 12px;
            font-weight: bold;
            color: #e2e8f0;
            padding-top: 12px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            padding: 0 4px;
            color: #38bdf8;
        }

        /* Dialogs */
        QDialog {
            background-color: #1e293b;
        }

        /* Labels */
        QLabel {
            color: #f8fafc;
        }
        QLabel#MutedLabel {
            color: #94a3b8;
        }
        QLabel#TitleLabel {
            font-size: 18px;
            font-weight: bold;
            color: #f8fafc;
        }
        QLabel#SubtitleLabel {
            font-size: 13px;
            color: #94a3b8;
        }
        QLabel#SectionHeader {
            font-size: 14px;
            font-weight: bold;
            color: #f8fafc;
            margin-bottom: 4px;
        }
        QLabel#GrandTotalLabel {
            font-size: 18px;
            font-weight: bold;
            color: #f8fafc;
        }
    )";
}

inline QString lightStyleSheet() {
    return R"(
        /* Global Window & Font Defaults - Light Mode */
        QWidget {
            background-color: #f8fafc;
            color: #0f172a;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            font-size: 13px;
        }

        /* Top Bar & Cards */
        #HeaderBar {
            background-color: #ffffff;
            border-bottom: 1px solid #e2e8f0;
            padding: 10px 16px;
        }
        #Sidebar {
            background-color: #ffffff;
            border-right: 1px solid #e2e8f0;
            padding: 12px 8px;
        }
        #ContentCard {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            padding: 16px;
        }
        #StatCard {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 10px;
            padding: 14px;
        }

        #UserPill {
            background-color: #f1f5f9;
            border: 1px solid #e2e8f0;
            border-radius: 16px;
            padding: 4px 12px;
        }
        #UserNameLabel {
            color: #1e293b;
            font-size: 12px;
        }
        #BrandLabel {
            font-size: 16px;
            font-weight: 800;
            color: #0f172a;
            letter-spacing: 0.5px;
        }

        /* Navigation Sidebar Buttons */
        QPushButton#NavButton {
            background-color: transparent;
            color: #64748b;
            font-size: 13px;
            font-weight: 600;
            text-align: left;
            padding: 10px 14px;
            border-radius: 6px;
            border: none;
        }
        QPushButton#NavButton:hover {
            background-color: #f1f5f9;
            color: #0f172a;
        }
        QPushButton#NavButton:checked {
            background-color: #2563eb;
            color: #ffffff;
        }

        /* Standard Buttons */
        QPushButton {
            background-color: #2563eb;
            color: #ffffff;
            font-weight: 600;
            padding: 8px 16px;
            border-radius: 6px;
            border: 1px solid transparent;
        }
        QPushButton:hover {
            background-color: #1d4ed8;
        }
        QPushButton:pressed {
            background-color: #1e40af;
        }
        QPushButton:disabled {
            background-color: #e2e8f0;
            color: #94a3b8;
            border-color: #cbd5e1;
        }

        /* Success / Checkout Button */
        QPushButton#SuccessButton {
            background-color: #10b981;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 10px 20px;
            border-radius: 6px;
        }
        QPushButton#SuccessButton:hover {
            background-color: #059669;
        }
        QPushButton#SuccessButton:pressed {
            background-color: #047857;
        }

        /* Danger / Delete Button */
        QPushButton#DangerButton {
            background-color: #ef4444;
            color: #ffffff;
        }
        QPushButton#DangerButton:hover {
            background-color: #dc2626;
        }
        QPushButton#DangerButton:pressed {
            background-color: #b91c1c;
        }

        /* Secondary / Outline Button */
        QPushButton#SecondaryButton {
            background-color: #f1f5f9;
            color: #1e293b;
            border: 1px solid #cbd5e1;
        }
        QPushButton#SecondaryButton:hover {
            background-color: #e2e8f0;
            color: #0f172a;
        }

        /* Theme Toggle Button */
        QPushButton#ThemeToggleBtn {
            background-color: #f1f5f9;
            color: #1e293b;
            border: 1px solid #cbd5e1;
            font-size: 12px;
            font-weight: 600;
            padding: 6px 12px;
            border-radius: 6px;
        }
        QPushButton#ThemeToggleBtn:hover {
            background-color: #e2e8f0;
            border-color: #94a3b8;
        }

        /* Input Fields */
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background-color: #ffffff;
            color: #0f172a;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            padding: 7px 10px;
            selection-background-color: #2563eb;
            selection-color: #ffffff;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
            border: 1px solid #3b82f6;
            background-color: #ffffff;
        }

        /* High-visibility Login Input Fields */
        QLineEdit#LoginInput {
            background-color: #ffffff;
            color: #0f172a;
            border: 2px solid #cbd5e1;
            border-radius: 8px;
            padding: 0 16px;
            font-size: 15px;
            font-weight: 500;
        }
        QLineEdit#LoginInput:focus {
            border: 2px solid #3b82f6;
            background-color: #ffffff;
        }

        /* ComboBox Dropdown */
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 24px;
            border-left-width: 0px;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            color: #0f172a;
            selection-background-color: #2563eb;
            selection-color: #ffffff;
            border: 1px solid #cbd5e1;
        }

        /* Tables */
        QTableWidget, QTableView {
            background-color: #ffffff;
            color: #0f172a;
            gridline-color: #e2e8f0;
            border: 1px solid #e2e8f0;
            border-radius: 6px;
            selection-background-color: #dbeafe;
            selection-color: #1e40af;
        }
        QHeaderView::section {
            background-color: #f1f5f9;
            color: #475569;
            font-weight: 600;
            padding: 8px 10px;
            border: none;
            border-bottom: 2px solid #cbd5e1;
            border-right: 1px solid #e2e8f0;
        }
        QTableWidget::item {
            padding: 6px 10px;
            border-bottom: 1px solid #f1f5f9;
        }
        QTableWidget::item:selected {
            background-color: #dbeafe;
            color: #1e40af;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: #f8fafc;
            width: 10px;
            margin: 0px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background: #cbd5e1;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical:hover {
            background: #94a3b8;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        /* Status Bar */
        QStatusBar {
            background-color: #ffffff;
            color: #64748b;
            border-top: 1px solid #e2e8f0;
            padding: 4px;
        }

        /* Group Box */
        QGroupBox {
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            margin-top: 12px;
            font-weight: bold;
            color: #1e293b;
            padding-top: 12px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            padding: 0 4px;
            color: #0284c7;
        }

        /* Dialogs */
        QDialog {
            background-color: #ffffff;
        }

        /* Labels */
        QLabel {
            color: #0f172a;
        }
        QLabel#MutedLabel {
            color: #64748b;
        }
        QLabel#TitleLabel {
            font-size: 18px;
            font-weight: bold;
            color: #0f172a;
        }
        QLabel#SubtitleLabel {
            font-size: 13px;
            color: #64748b;
        }
        QLabel#SectionHeader {
            font-size: 14px;
            font-weight: bold;
            color: #0f172a;
            margin-bottom: 4px;
        }
        QLabel#GrandTotalLabel {
            font-size: 18px;
            font-weight: bold;
            color: #0f172a;
        }
    )";
}

inline QString mainStyleSheet() {
    return darkStyleSheet();
}

inline QString getStyleSheet(bool isDark) {
    return isDark ? darkStyleSheet() : lightStyleSheet();
}

} // namespace Theme
