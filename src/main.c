#include "pathfinder.h"

/**
 * Основна функція програми Pathfinder.
 * @param argc Кількість аргументів командного рядка.
 * @param argv Массив аргументів командного рядка.
 * @return Код завершення програми.
 */
int main(int argc, char *argv[]) {
    // Перевірка кількості аргументів
    if (argc != 2) {
        handle_error(ARGS, NULL, 0);
        return -1; // Код помилки, якщо аргументів недостатньо
    }

    // Ініціалізація даних з вхідного файлу
    t_data *data = parse_input_file(argv[1]);
    if (!data) {
        return -1; // Повертаємо код помилки, якщо дані не вдалося ініціалізувати
    }

    // Перевірка даних на валідність
    if (validate_data(data)) {
        free_resources(data); // Звільнення ресурсів у разі помилки
        return -1;
    }

    // Виконання пошуку шляхів у графі
    execute_pathfinding(data);

    // Звільнення ресурсів перед завершенням програми
    free_resources(data);

    return 0; // Успішне завершення програми
}
