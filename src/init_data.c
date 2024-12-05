#include "pathfinder.h"

// Прототипи функцій
static int **create_matrix(int n);
static int read_islands_amount(int fd, char *filename);
t_data *initialize_data(int fd, char *filename);

// Реалізація функцій
// =============================================================

/**
 * Створює та ініціалізує матрицю суміжності розміром n x n.
 *
 * @param n Розмір матриці.
 * @return Вказівник на створену матрицю або NULL у разі помилки.
 */
static int **create_matrix(int n) {
    int **matrix = malloc(n * sizeof(int *));
    if (!matrix) return NULL; // Перевірка на успішне виділення пам'яті

    for (int i = 0; i < n; ++i) {
        matrix[i] = malloc(n * sizeof(int));
        if (!matrix[i]) { // Перевірка для кожного рядка
            for (int j = 0; j < i; ++j) free(matrix[j]); // Звільнення вже виділених рядків
            free(matrix);
            return NULL;
        }
    }

    // Ініціалізація значеннями INT_MAX
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = INT_MAX;
        }
    }

    return matrix;
}

/**
 * Зчитує кількість островів із першого рядка файлу.
 *
 * @param fd Дескриптор файлу.
 * @param filename Ім'я файлу для повідомлень про помилки.
 * @return Кількість островів або -1 у разі помилки.
 */
static int read_islands_amount(int fd, char *filename) {
    int eof = 0;
    int size = 0;
    char *line = read_line_delim(fd, &size, &eof, '\n');

    // Перевірка на порожній файл
    if (!line && size == 0) {
        return handle_error(EMPTY, filename, 1);
    }

    // Перевірка на невдале зчитування
    if (line == NULL) {
        return -1;
    }

    int result = mx_atoi(line);
    int length = mx_strlen(line);
    free(line);

    // Перевірка на валідність значення
    if (result < 1 || length != mx_numlen(result)) {
        return handle_error(FIRSTLINE, NULL, 1);
    }

    return result;
}

/**
 * Ініціалізує структуру t_data з усіма необхідними даними.
 *
 * @param fd Дескриптор файлу.
 * @param filename Ім'я файлу для повідомлень про помилки.
 * @return Вказівник на структуру t_data або NULL у разі помилки.
 */
t_data *initialize_data(int fd, char *filename) {
    // Виділення пам'яті для основної структури
    t_data *data = malloc(sizeof(t_data));
    if (!data) return NULL;

    data->error = NONE;
    data->nodes = NULL;

    // Зчитування кількості островів
    data->amount = read_islands_amount(fd, filename);
    if (data->amount < 0) {
        free(data);
        return NULL;
    }

    // Створення графа
    data->graph = malloc(sizeof(t_graph));
    if (!data->graph) {
        free(data);
        return NULL;
    }

    data->graph->size = data->amount;
    data->graph->matrix = create_matrix(data->graph->size);

    // Перевірка на помилки під час створення матриці
    if (!data->graph->matrix) {
        free(data->graph);
        free(data);
        return NULL;
    }

    return data;
}
