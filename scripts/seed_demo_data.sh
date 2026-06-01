#!/usr/bin/env bash
# Заполняет базу данных демо-данными (сотрудники, компоненты, клиенты, продажи, позиции).
# Использование: ./scripts/seed_demo_data.sh /путь/к/app.db
# Требуется: sqlite3 (пакет sqlite3 в Debian/Ubuntu)

set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SQL_FILE="${SCRIPT_DIR}/seed_demo_data.sql"

if [ $# -lt 1 ]; then
    echo "Использование: $0 <путь_к_файлу_базы.db>" >&2
    echo "Пример: $0 ./app.db" >&2
    exit 1
fi
DB_PATH="$1"

if ! command -v sqlite3 &>/dev/null; then
    echo "Ошибка: sqlite3 не найден. Установите пакет sqlite3." >&2
    exit 1
fi

if [ ! -f "$SQL_FILE" ]; then
    echo "Ошибка: не найден файл $SQL_FILE" >&2
    exit 1
fi

if [ ! -f "$DB_PATH" ]; then
    echo "Ошибка: файл базы не найден: $DB_PATH" >&2
    echo "Создайте базу в приложении (Новая база данных) или укажите существующий файл." >&2
    exit 1
fi

echo "Загрузка демо-данных в $DB_PATH ..."
sqlite3 "$DB_PATH" < "$SQL_FILE"
echo "Готово."
