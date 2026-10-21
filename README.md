# Inventory Management System

A modern, fast, and secure desktop inventory management, point-of-sale, and invoicing system built with **C++17**, **Qt (Qt 6 / Qt 5)**, and **SQLite3**.

Designed with **Qt Creator** support (both **CMake** and **qmake**), role-based access control, salted SHA-256 authentication, transactional sales with rollback, and printable/PDF exportable invoice generation.

---

## 📸 Key Features

### 1. 🔐 Authentication & Authorization (RBAC)
- **Salted SHA-256 Hashing**: Passwords are securely hashed with a 16-character random cryptographic salt before persistence.
- **Role-Based Views**:
  - **Administrator (`Admin`)**: Complete control over inventory (add, edit, restock, delete), sales analytics, user account provisioning, POS billing, and invoice history.
  - **Cashier (`Cashier`)**: Streamlined Point of Sale interface, itemized cart, real-time checkout, receipt printing/PDF export, and read-only catalog lookup. Admin sections are strictly hidden and backend-protected.
- **Session Management**: Full logout and account switching support.
- **🌗 Theme Toggling**: Instant 1-click toggle between Dark Slate Mode and Clean Light Mode in the top navigation bar.

### 2. 📦 Product Inventory Management
- **Full CRUD**: Add new products, edit pricing and names, delete deprecated items.
- **Live Search & Filter**: Real-time searching as you type.
- **Stock Threshold Alerts**: One-click filter for low-stock items (`< 5 units`) and visual color-coded badges (`In Stock`, `Low Stock`, `Out of Stock`).
- **Quick Restocking**: Dedicated restock dialog to add quantities with validation.

### 3. 🛒 Point of Sale (POS) & Billing
- **Two-Column Workflow**: Product catalog on the left, interactive live cart on the right.
- **Smart Cart Controls**:
  - Double-click or select & add products.
  - Inline spinbox to adjust quantities directly in the cart table.
  - Real-time line item total and grand total computation.
  - Inventory limit checks (cannot add more than available in stock).
- **ACID Transaction Guarantee**: Billing executes inside SQLite transactions (`BEGIN` -> stock check -> update stock -> create bill -> insert items -> `COMMIT`). If any item is out of stock, the entire cart transaction is automatically rolled back (`ROLLBACK`).

### 4. 🧾 Invoice Generation & Export
- **Printable Invoices**: Clean, branded tax receipt generated instantly upon checkout or viewable anytime from invoice history.
- **🖨️ Physical Print**: Native integration with `QPrinter` and `QPrintDialog`.
- **📄 Vector PDF Export**: Export crisp PDF receipts for customer emailing or record keeping.

### 5. 📊 Sales Analytics & Executive Reports (Admin Only)
- **KPI Summary Cards**: Real-time stats on total revenue, total invoices issued, and active low-stock alerts.
- **Daily Sales Breakdown**: Date, invoice count, and revenue totals.
- **Top-Selling Products**: Ranked by units sold and revenue contribution.

### 6. 👥 User Management (Admin Only)
- Provision new `Cashier` or `Admin` accounts directly from the UI.
- Displays all registered users with their assigned permission level.

---

## 🚀 How to Open and Run in Qt Creator

### Method 1: Using CMake (Recommended for Qt 6 / Qt Creator 6+)
1. Open **Qt Creator**.
2. Click **File -> Open File or Project...** (or press `Cmd + O` / `Ctrl + O`).
3. Navigate to this directory (`Inventory-Management`) and select `CMakeLists.txt`.
4. In the **Configure Project** screen:
   - Select your desired Qt Kit (e.g., **Desktop Qt 6.x.x** or **Desktop Qt 5.x.x** with clang/gcc).
   - Click **Configure Project**.
5. Select the target **`InventoryApp`** from the run configuration selector in the bottom left.
6. Click the green **Run (▶)** button or press `Cmd + R` / `Ctrl + R` to build and launch!

### Method 2: Using qmake (`InventoryApp.pro`)
1. Open **Qt Creator**.
2. Click **File -> Open File or Project...**.
3. Select `InventoryApp.pro`.
4. Choose your Desktop Qt kit and click **Configure Project**.
5. Click the green **Run (▶)** button (`Cmd + R` / `Ctrl + R`).

---

## 🔑 Default Credentials

If the database is new or reset, default accounts are seeded automatically:

| Username | Password | Role | Permissions |
| :--- | :--- | :--- | :--- |
| `admin` | `admin123` | **Admin** | Full system access (Inventory, POS, Invoices, Reports, User Mgmt) |
| `cashier1` | `cash123` | **Cashier** | POS Billing, Invoices lookup, Read-only Catalog |

---

## 📁 Project Structure

```
Inventory-Management/
├── CMakeLists.txt              # Standard Qt CMake build file (Qt 6 & Qt 5 compatible)
├── InventoryApp.pro            # Standard qmake build file for Qt Creator
├── Makefile                    # Makefile for CLI test build
├── README.md                   # Project documentation
├── inventory.db                # SQLite3 database file
└── src/
    ├── models.h                # Data models (Product, User, Bill, CartItem, etc.)
    ├── Database.h / .cpp       # SQLite3 database connection & schema initialization
    ├── Auth.h / .cpp           # Salted SHA-256 authentication & user query logic
    ├── Inventory.h / .cpp      # Product catalog, stock operations, search
    ├── Billing.h / .cpp        # Cart checkout, transactional billing, sales queries
    ├── picosha2.h              # Pure header-only SHA-256 implementation
    ├── main.cpp                # Original CLI runner / test harness
    └── gui/
        ├── main_gui.cpp        # Qt application entry point & session loop
        ├── Theme.h             # Sleek modern dark QSS stylesheet & layout themes
        ├── LoginDialog.h/.cpp  # Login interface with credential validation
        ├── MainWindow.h/.cpp   # Dashboard shell with sidebar navigation & role badges
        ├── InventoryWidget.h/.cpp # Product management catalog with CRUD & search
        ├── BillingWidget.h/.cpp   # Interactive Point of Sale (POS) & cart checkout
        ├── InvoicesWidget.h/.cpp  # Past invoice history & receipt inspection
        ├── ReportsWidget.h/.cpp   # Sales analytics, daily trends, top products
        ├── UsersWidget.h/.cpp     # RBAC user account management
        ├── InvoiceDialog.h/.cpp   # Receipt preview with Print & PDF export
        └── ProductDialogs.h/.cpp  # Add, edit, and restock input dialogs
```

---

## 💻 Running the Headless CLI Version (Optional)
If you wish to test the backend logic directly via terminal without launching Qt:
```bash
make
./app
```

