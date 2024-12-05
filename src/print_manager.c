#include "pathfinder.h"

// Макрос для друку розділювальної лінії
#define SEPARATOR "========================================\n"

// Структура для зберігання контексту друку шляху
typedef struct s_path_context {
    t_path_list *all_paths; // Список шляхів
    t_data *data;           // Основні дані графа
    int path_ind;           // Індекс поточного шляху
} t_path_context;

// Утиліти
// =============================================================

/**
 * Повертає назву вузла за його ідентифікатором.
 *
 * @param islands Список вузлів (островів).
 * @param id Ідентифікатор вузла.
 * @return Назва вузла або NULL, якщо вузол не знайдено.
 */
static char *name_by_id(t_list *islands, int id) {
    for (t_list *copy = islands; copy; copy = copy->next) {
        if (id == ((t_node *)copy->data)->id) {
            return ((t_node *)copy->data)->name;
        }
    }
    return NULL;
}

// Логіка друку
// =============================================================

/**
 * Друкує розділювальну лінію.
 */
static void print_separator(void) {
    mx_printstr(SEPARATOR);
}

/**
 * Друкує всі деталі шляху (Path, Route, Distance).
 *
 * @param ctx Контекст шляху.
 */
static void print_path_details(t_path_context *ctx) {
    t_path_list *all_paths = ctx->all_paths;
    t_data *data = ctx->data;
    int path_ind = ctx->path_ind;

    // Друк "Path"
    mx_printstr("Path: ");
    mx_printstr(name_by_id(data->nodes, all_paths->paths[path_ind].vertices[0]));
    mx_printstr(" -> ");
    mx_printstr(name_by_id(data->nodes,
                           all_paths->paths[path_ind].vertices[
                               all_paths->paths[path_ind].length - 1]));

    // Друк "Route"
    mx_printstr("\nRoute: ");
    for (int j = 0; j < all_paths->paths[path_ind].length - 1; j++) {
        mx_printstr(name_by_id(data->nodes, all_paths->paths[path_ind].vertices[j]));
        mx_printstr(" -> ");
    }
    mx_printstr(name_by_id(data->nodes,
                           all_paths->paths[path_ind].vertices[
                               all_paths->paths[path_ind].length - 1]));

    // Друк "Distance"
    mx_printstr("\nDistance: ");
    if (all_paths->paths[path_ind].length == 2) {
        mx_printint(all_paths->min_len);
    } else {
        int total = 0;
        for (int j = 0; j < all_paths->paths[path_ind].length - 1; j++) {
            int x = all_paths->paths[path_ind].vertices[j];
            int y = all_paths->paths[path_ind].vertices[j + 1];
            int weight = data->graph->matrix[x][y];
            total += weight;
            mx_printint(weight);
            if (j < all_paths->paths[path_ind].length - 2) mx_printstr(" + ");
        }
        mx_printstr(" = ");
        mx_printint(total);
    }
    mx_printstr("\n");
}

// Основна функція
// =============================================================

/**
 * Виводить усі знайдені шляхи графа.
 *
 * @param all_paths Список шляхів.
 * @param data Основні дані графа.
 */
void display_paths(t_path_list *all_paths, t_data *data) {
    for (int i = 0; i < all_paths->count; i++) {
        print_separator();
        t_path_context ctx = { all_paths, data, i };
        print_path_details(&ctx);
        print_separator();
    }
}
