#pragma once
#include <QDialog>
#include <QTextBrowser>
#include <QPushButton>
#include "models.h"

class InvoiceDialog : public QDialog {
    Q_OBJECT

public:
    explicit InvoiceDialog(const Bill& bill, QWidget* parent = nullptr);

private slots:
    void printReceipt();
    void exportPdf();

private:
    Bill bill_;
    QTextBrowser* browser_;

    QString generateHtml() const;
};

