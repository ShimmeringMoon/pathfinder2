#include "pathfinder.h"

// Функції обробки помилок
// =============================================================

/**
 * Обробляє помилку неправильного використання програми.
 */
static void handle_args_error(void) {
    mx_printerr("usage: ./pathfinder [filename]\n");
}

/**
 * Обробляє помилку, якщо файл не існує.
 *
 * @param filename Ім'я файлу, який не знайдено.
 */
static void handle_filename_error(char *filename) {
    mx_printerr("error: file ");
    mx_printerr(filename);
    mx_printerr(" does not exist\n");
}

/**
 * Обробляє помилку, якщо файл порожній.
 *
 * @param filename Ім'я порожнього файлу.
 */
static void handle_empty_file_error(char *filename) {
    mx_printerr("error: file ");
    mx_printerr(filename);
    mx_printerr(" is empty\n");
}

/**
 * Обробляє помилку, якщо перший рядок у файлі є невірним.
 */
static void handle_firstline_error(void) {
    mx_printerr("error: line 1 is not valid\n");
}

/**
 * Обробляє помилку у конкретному рядку файлу.
 *
 * @param line Номер рядка з помилкою.
 */
static void handle_line_error(int line) {
    mx_printerr("error: line ");
    char *linestr = mx_itoa(line);
    mx_printerr(linestr);
    free(linestr);
    mx_printerr(" is not valid\n");
}

/**
 * Обробляє помилку невідповідності кількості островів.
 */
static void handle_islands_error(void) {
    mx_printerr("error: invalid number of islands\n");
}

/**
 * Обробляє помилку дубльованих мостів.
 */
static void handle_bridges_error(void) {
    mx_printerr("error: duplicate bridges\n");
}

/**
 * Обробляє помилку перевищення максимальної суми довжин мостів.
 */
static void handle_length_error(void) {
    mx_printerr("error: sum of bridges lengths is too big\n");
}

/**
 * Вибирає відповідну функцію для обробки помилки.
 *
 * @param error Код помилки.
 * @param filename Ім'я файлу (для помилок, пов'язаних із файлом).
 * @param line Номер рядка (для помилок, пов'язаних із рядками).
 * @return Код завершення (-1 у разі помилки).
 */
int handle_error(t_errors error, char *filename, int line) {
    switch (error) {
        case NONE:
            return 0;
        case ARGS:
            handle_args_error();
            break;
        case NAME:
            handle_filename_error(filename);
            break;
        case EMPTY:
            handle_empty_file_error(filename);
            break;
        case FIRSTLINE:
            handle_firstline_error();
            break;
        case LINE:
            handle_line_error(line);
            break;
        case ISLANDS:
            handle_islands_error();
            break;
        case BRIDGES:
            handle_bridges_error();
            break;
        case LENGTH:
            handle_length_error();
            break;
        default:
            mx_printerr("error: unknown error occurred\n");
    }
    return -1;
}
