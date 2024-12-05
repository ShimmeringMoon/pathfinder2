#include "pathfinder.h"

/**
 * Звільняє пам'ять, виділену для списку островів.
 *
 * @param islands Вказівник на список вузлів (островів).
 */
static void free_island_list(t_list **islands) {
    if (!islands || !*islands) return; // Перевірка на NULL
    t_list *current = *islands;

    while (current) {
        t_list *next = current->next;
        if (((t_node *)current->data)->name) {
            free(((t_node *)current->data)->name); // Звільнення імені острова
        }
        free(current->data); // Звільнення структури вузла
        free(current);       // Звільнення елемента списку
        current = next;
    }

    *islands = NULL; // Указівник на список стає NULL
}

/**
 * Звільняє пам'ять, виділену для матриці суміжності.
 *
 * @param matrix Матриця суміжності.
 * @param n Кількість рядків у матриці.
 */
static void free_matrix(int **matrix, int n) {
    if (!matrix) return; // Перевірка на NULL
    for (int i = 0; i < n; ++i) {
        free(matrix[i]); // Звільнення кожного рядка
    }
    free(matrix); // Звільнення матриці
}

/**
 * Звільняє всі ресурси, пов'язані з даними графа.
 *
 * @param data Вказівник на структуру t_data, яка містить всі дані графа.
 */
void free_resources(t_data *data) {
    if (!data) return; // Перевірка на NULL
    free_island_list(&data->nodes); // Звільнення списку вузлів

    if (data->graph) { // Якщо граф існує
        free_matrix(data->graph->matrix, data->graph->size); // Звільнення матриці
        free(data->graph); // Звільнення структури графа
    }

    free(data); // Звільнення основної структури даних
}
