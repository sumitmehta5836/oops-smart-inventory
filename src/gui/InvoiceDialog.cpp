#include "InvoiceDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>

InvoiceDialog::InvoiceDialog(const Bill& bill, QWidget* parent)
    : QDialog(parent), bill_(bill) {
    setWindowTitle(QString("Invoice #%1").arg(bill_.id));
    resize(520, 640);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(14);

    browser_ = new QTextBrowser(this);
    browser_->setHtml(generateHtml());
    browser_->setStyleSheet("background-color: #ffffff; color: #1e293b; border-radius: 8px; padding: 12px;");
    mainLayout->addWidget(browser_);

    QHBoxLayout* actionsLayout = new QHBoxLayout();

    QPushButton* printBtn = new QPushButton("🖨️ Print Receipt", this);
    printBtn->setObjectName("SecondaryButton");
    connect(printBtn, &QPushButton::clicked, this, &InvoiceDialog::printReceipt);
    actionsLayout->addWidget(printBtn);

    QPushButton* pdfBtn = new QPushButton("📄 Export to PDF", this);
    pdfBtn->setObjectName("SecondaryButton");
    connect(pdfBtn, &QPushButton::clicked, this, &InvoiceDialog::exportPdf);
    actionsLayout->addWidget(pdfBtn);

    actionsLayout->addStretch();

    QPushButton* closeBtn = new QPushButton("Close", this);
    closeBtn->setObjectName("SuccessButton");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    actionsLayout->addWidget(closeBtn);

    mainLayout->addLayout(actionsLayout);
}

QString InvoiceDialog::generateHtml() const {
    QString html = R"(
    <!DOCTYPE html>
    <html>
    <head>
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Arial, sans-serif; margin: 16px; color: #1e293b; font-size: 13px; }
        .header { text-align: center; border-bottom: 2px dashed #94a3b8; padding-bottom: 12px; margin-bottom: 16px; }
        .store-name { font-size: 20px; font-weight: bold; color: #0f172a; margin: 0; }
        .subtitle { font-size: 12px; color: #64748b; margin-top: 4px; }
        .meta-table { width: 100%; margin-bottom: 16px; font-size: 12px; }
        .meta-table td { padding: 3px 0; }
        .items-table { width: 100%; border-collapse: collapse; margin-top: 10px; margin-bottom: 16px; }
        .items-table th { background-color: #f1f5f9; color: #334155; font-weight: bold; border-bottom: 2px solid #cbd5e1; text-align: left; padding: 6px 8px; font-size: 12px; }
        .items-table td { padding: 6px 8px; border-bottom: 1px solid #e2e8f0; font-size: 12px; }
        .text-right { text-align: right; }
        .total-box { margin-top: 12px; border-top: 2px dashed #94a3b8; padding-top: 8px; }
        .total-row { font-size: 16px; font-weight: bold; color: #0f172a; }
        .footer { text-align: center; margin-top: 24px; font-size: 11px; color: #64748b; border-top: 1px solid #e2e8f0; padding-top: 12px; }
    </style>
    </head>
    <body>
        <div class="header">
            <div class="store-name">📦 INVENTORY MANAGEMENT</div>
            <div class="subtitle">Tax Invoice / Cash Receipt</div>
        </div>
        <table class="meta-table">
            <tr>
                <td><strong>Invoice #:</strong> )" + QString::number(bill_.id) + R"(</td>
                <td class="text-right"><strong>Date:</strong> )" + QString::fromStdString(bill_.createdAt) + R"(</td>
            </tr>
            <tr>
                <td><strong>Cashier:</strong> )" + QString::fromStdString(bill_.cashier) + R"(</td>
                <td class="text-right"><strong>Status:</strong> Paid (Completed)</td>
            </tr>
        </table>
        <table class="items-table">
            <thead>
                <tr>
                    <th style="width: 45%;">Item Description</th>
                    <th class="text-right">Rate</th>
                    <th class="text-right">Qty</th>
                    <th class="text-right">Amount</th>
                </tr>
            </thead>
            <tbody>
    )";

    for (const auto& item : bill_.items) {
        html += QString(R"(
            <tr>
                <td>%1</td>
                <td class="text-right">Rs %2</td>
                <td class="text-right">%3</td>
                <td class="text-right">Rs %4</td>
            </tr>
        )")
        .arg(QString::fromStdString(item.name))
        .arg(QString::number(item.unitPrice, 'f', 2))
        .arg(item.quantity)
        .arg(QString::number(item.lineTotal, 'f', 2));
    }

    html += QString(R"(
            </tbody>
        </table>
        <div class="total-box">
            <table style="width: 100%;">
                <tr class="total-row">
                    <td>GRAND TOTAL:</td>
                    <td class="text-right">Rs %1</td>
                </tr>
            </table>
        </div>
        <div class="footer">
            Thank you for your purchase!<br>
            Please keep this invoice for your records & returns.
        </div>
    </body>
    </html>
    )").arg(QString::number(bill_.total, 'f', 2));

    return html;
}

void InvoiceDialog::printReceipt() {
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        browser_->document()->print(&printer);
    }
}

void InvoiceDialog::exportPdf() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Invoice PDF",
        QString("Invoice_%1.pdf").arg(bill_.id), "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    browser_->document()->print(&printer);

    QMessageBox::information(this, "Exported", QString("Invoice exported successfully to:\n%1").arg(fileName));
}

