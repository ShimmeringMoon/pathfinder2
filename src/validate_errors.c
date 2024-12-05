#include "pathfinder.h"

// Окремі функції для перевірки помилок
static bool check_islands_error(t_data *data) {
    if (!data->nodes || mx_list_size(data->nodes) != data->amount) {
        handle_error(ISLANDS, NULL, 0);
        return true;
    }
    return false;
}

static bool check_bridges_error(t_data *data) {
    if (data->error == BRIDGES) {
        handle_error(BRIDGES, NULL, 0);
        return true;
    }
    return false;
}

static bool check_length_error(t_data *data) {
    if (data->error == LENGTH) {
        handle_error(LENGTH, NULL, 0);
        return true;
    }
    return false;
}

/**
 * Перевіряє валідність даних, завантажених у структуру t_data.
 * @param data Вказівник на структуру t_data.
 * @return true, якщо знайдено помилки; false, якщо дані валідні.
 */
bool validate_data(t_data *data) {
    if (!data) {
        fprintf(stderr, "error: data structure is NULL\n");
        return true; // Помилка вже оброблена у функції читання
    }

    if (data->error != NONE) {
        return true; // Помилка вже встановлена
    }

    // Використовуємо окремі функції для перевірки помилок
    if (check_islands_error(data)) return true;
    if (check_bridges_error(data)) return true;
    if (check_length_error(data)) return true;

    return false; // Усі перевірки пройдено успішно
}
