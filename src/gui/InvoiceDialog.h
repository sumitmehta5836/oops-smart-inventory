#pragma once
#include <QDialog>
#include <QTextBrowser>
#include <QPushButton>
#include "models.h"

// OOP CONCEPT: INHERITANCE & ABSTRACTION
// 1. Inheritance: InvoiceDialog inherits from QDialog to deliver modal receipt viewing and printing dialogs.
//    Base Class: QDialog | Derived Class: InvoiceDialog
// 2. Abstraction: The caller simply passes a Bill object. The intricate generation of formatted HTML invoices,
//    CSS tables, QPrinter integration, and PDF document rasterization are abstracted away.
//
// OOP CONCEPT: ENCAPSULATION
// The Bill data model and internal QTextBrowser view are encapsulated privately within the dialog.
class InvoiceDialog : public QDialog {
    Q_OBJECT

public:
    explicit InvoiceDialog(const Bill& bill, QWidget* parent = nullptr);

private slots:
    void printReceipt();
    void exportPdf();

private:
    // OOP CONCEPT: ENCAPSULATION (DATA HIDING)
    Bill bill_;
    QTextBrowser* browser_;

    QString generateHtml() const;
};

