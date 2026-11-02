
#!/bin/bash

# Smart Inventory Management System
# Automatic macOS Build and Launcher

set -e

cd "$(dirname "$0")"

echo "======================================"
echo "  SMART INVENTORY MANAGEMENT SYSTEM"
echo "======================================"

# 1. Check CMake
if ! command -v cmake >/dev/null 2>&1; then
    echo "ERROR: CMake is not installed."
    echo "Install it using: brew install cmake"
    read -p "Press Enter to exit..."
    exit 1
fi

# 2. Locate Qt 6
QT_CONFIG=$(find "$HOME/Qt" \
    -path "*/lib/cmake/Qt6/Qt6Config.cmake" \
    -print -quit 2>/dev/null || true)

if [ -z "$QT_CONFIG" ]; then
    echo "ERROR: Qt 6 installation not found."
    echo "Install Qt 6 with the Qt Widgets and PrintSupport components."
    read -p "Press Enter to exit..."
    exit 1
fi

QT_PREFIX="${QT_CONFIG%/lib/cmake/Qt6/Qt6Config.cmake}"

echo "Qt found: $QT_PREFIX"

# 3. Configure project
echo ""
echo "Configuring project..."

cmake -S . -B build_mac \
    -DCMAKE_PREFIX_PATH="$QT_PREFIX" \
    -DCMAKE_BUILD_TYPE=Release

# 4. Compile application
echo ""
echo "Building application..."

cmake --build build_mac --parallel 4

# 5. Check executable
APP="./build_mac/InventoryApp"

if [ ! -f "$APP" ]; then
    echo "ERROR: Application executable was not generated."
    read -p "Press Enter to exit..."
    exit 1
fi

# 6. Launch application
echo ""
echo "Build successful!"
echo "Launching Smart Inventory Management System..."

chmod +x "$APP"
"$APP"

