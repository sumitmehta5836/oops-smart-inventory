#include <QApplication>
#include <QMessageBox>
#include "Theme.h"
#include "LoginDialog.h"
#include "MainWindow.h"
#include "Database.h"
#include "Auth.h"
#include "Inventory.h"
#include "Billing.h"

// OOP ARCHITECTURE: ENTRY POINT & APPLICATION LIFECYCLE
// Demonstrates Abstraction and Encapsulation:
// 1. Abstraction: The GUI entry point instantiates the service layer (Database, Auth, Inventory, Billing)
//    and GUI dialogs (LoginDialog, MainWindow) interacting purely via high-level public interfaces.
// 2. Polymorphism: Dialog lifecycle management (loginDlg.exec()) invokes Qt's virtual event dispatching.
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet(Theme::mainStyleSheet());

    try {
        Database db("inventory.db");
        db.initSchema();

        Auth auth(db);
        Inventory inv(db);
        Billing billing(db);

        // Seed default accounts if fresh database
        if (!auth.hasUsers()) {
            auth.createUser("admin", "admin123", Role::Admin);
            auth.createUser("cashier1", "cash123", Role::Cashier);
        }

        // Application session loop (allows logging out and logging back in)
        while (true) {
            LoginDialog loginDlg(auth);
            if (loginDlg.exec() != QDialog::Accepted) {
                // User closed login dialog
                break;
            }

            auto maybeUser = loginDlg.getAuthenticatedUser();
            if (!maybeUser.has_value()) {
                break;
            }

            MainWindow mainWindow(db, auth, inv, billing, maybeUser.value());
            bool wantsLogout = false;

            QObject::connect(&mainWindow, &MainWindow::logoutRequested, [&]() {
                wantsLogout = true;
                mainWindow.close();
            });

            mainWindow.show();
            app.exec();

            if (!wantsLogout) {
                // Window closed normally (not via logout button)
                break;
            }
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Startup Fatal Error",
            QString("An unhandled error occurred while starting the application:\n\n%1").arg(e.what()));
        return 1;
    }

    return 0;
}

