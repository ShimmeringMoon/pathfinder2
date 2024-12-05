#include "pathfinder.h"

// Структура для зберігання поточних координат (від і до вузлів)
typedef struct s_coords {
    int from; // Вузол, з якого починається пошук
    int to;   // Вузол, до якого виконується пошук
} t_coords;

static void find_all_from_to(t_coords cords, t_data *data, t_path_info *path_info, t_path_list *all_paths);

// Макрос для очищення поточного стану шляху
#define RESET_PATH_INFO(info) do { \
    (info)->path_index--; \
    (info)->visited[(info)->path[(info)->path_index]] = 0; \
} while (0)

// Макрос для перевірки умов зупинки пошуку, якщо поточна вага перевищує мінімальну довжину
#define CHECK_WEIGHT_CONDITION(info, paths) \
    ((paths)->min_len != INT_MAX && (info)->weight >= (paths)->min_len)

/**
 * Створює та ініціалізує структуру t_path_info, яка зберігає поточний стан шляху.
 * @param num_vertices Кількість вершин у графі.
 * @return Вказівник на нову структуру t_path_info.
 */
t_path_info *create_path_info(int num_vertices) {
    t_path_info *path_info = (t_path_info *)malloc(sizeof(t_path_info));
    path_info->visited = (int *)calloc(num_vertices, sizeof(int)); // Відвідані вершини
    path_info->path = (int *)malloc(num_vertices * sizeof(int));   // Поточний шлях
    path_info->path_index = 0; // Індекс для поточного положення в шляху
    path_info->weight = 0;     // Поточна вага шляху
    return path_info;
}

/**
 * Звільняє пам'ять, виділену для t_path_info.
 * @param path_info Вказівник на структуру, яку потрібно звільнити.
 */
void free_path_info(t_path_info *path_info) {
    free(path_info->visited);
    free(path_info->path);
    free(path_info);
}

/**
 * Створює та ініціалізує структуру t_path_list для зберігання всіх знайдених шляхів.
 * @return Вказівник на нову структуру t_path_list.
 */
t_path_list *create_paths(void) {
    t_path_list *paths = (t_path_list *)malloc(sizeof(t_path_list));
    paths->paths = NULL;      // Масив шляхів
    paths->count = 0;         // Кількість знайдених шляхів
    paths->min_len = INT_MAX; // Довжина найкоротшого знайденого шляху
    return paths;
}

/**
 * Звільняє пам'ять, виділену для всіх шляхів у t_path_list.
 * @param all_paths Вказівник на структуру t_path_list, яку потрібно звільнити.
 */
void free_paths(t_path_list *all_paths) {
    for (int i = 0; i < all_paths->count; i++) {
        free(all_paths->paths[i].vertices);
    }
    free(all_paths->paths);
    free(all_paths);
}

/**
 * Додає новий шлях до списку всіх шляхів.
 * @param all_paths Вказівник на структуру t_path_list.
 * @param current_path Масив вершин, що представляє поточний шлях.
 * @param length Довжина поточного шляху.
 */
void add_path(t_path_list *all_paths, int *current_path, int length) {
    t_path *new_paths = (t_path *)mx_realloc(all_paths->paths, (all_paths->count + 1) * sizeof(t_path));
    if (!new_paths) {
        fprintf(stderr, "error: memory allocation failed in add_path\n");
        return; // Виходимо, якщо не вдалося виділити пам’ять
    }
    all_paths->paths = new_paths;
    all_paths->paths[all_paths->count].vertices = (int *)malloc(length * sizeof(int));
    all_paths->paths[all_paths->count].length = length;
    for (int i = 0; i < length; i++) {
        all_paths->paths[all_paths->count].vertices[i] = current_path[i];
    }
    all_paths->count++;
}

/**
 * Обробляє випадок, коли досягнуто кінцевої вершини.
 * Якщо знайдений шлях коротший, очищує старі шляхи.
 * Якщо знайдений шлях має таку ж довжину, додає його до списку.
 */
static void reached_destination(t_path_info *path_info, t_path_list *all_paths) {
    if (path_info->weight < all_paths->min_len) {
        // Якщо знайдено новий мінімум, очищаємо всі попередні шляхи
        for (int i = 0; i < all_paths->count; i++) {
            free(all_paths->paths[i].vertices);
        }
        free(all_paths->paths);
        all_paths->paths = NULL;
        all_paths->count = 0;
        all_paths->min_len = path_info->weight;
    }
    // Якщо шлях відповідає мінімальній довжині, додаємо його
    if (path_info->weight == all_paths->min_len) {
        add_path(all_paths, path_info->path, path_info->path_index);
    }
}

/**
 * Досліджує сусідні вершини для поточного вузла.
 * @param cords Поточні координати (from, to).
 * @param data Вказівник на структуру t_data.
 * @param path_info Вказівник на структуру, що містить стан поточного шляху.
 * @param all_paths Вказівник на список усіх шляхів.
 */
static void explore_neighbors(t_coords cords, t_data *data, t_path_info *path_info, t_path_list *all_paths) {
    int from = cords.from;
    int to = cords.to;
    for (int i = 0; i < data->graph->size; i++) {
        int weight = data->graph->matrix[from][i];
        if (weight != INIT && !path_info->visited[i] && path_info->weight + weight <= all_paths->min_len) {
            path_info->weight += weight;
            t_coords new_cords = { .from = i, .to = to };
            find_all_from_to(new_cords, data, path_info, all_paths);
            path_info->weight -= weight; // Відкат ваги
        }
    }
}

/**
 * Рекурсивно знаходить усі можливі шляхи між двома вузлами.
 * @param cords Поточні координати (from, to).
 * @param data Вказівник на структуру t_data.
 * @param path_info Вказівник на структуру, що містить стан поточного шляху.
 * @param all_paths Вказівник на список усіх шляхів.
 */
static void find_all_from_to(t_coords cords, t_data *data, t_path_info *path_info, t_path_list *all_paths) {
    int from = cords.from;
    int to = cords.to;
    path_info->visited[from] = 1; // Позначаємо вузол як відвіданий
    path_info->path[path_info->path_index++] = from; // Додаємо вузол до поточного шляху

    if (from == to) {
        reached_destination(path_info, all_paths); // Додаємо шлях, якщо досягли цілі
    } else {
        explore_neighbors(cords, data, path_info, all_paths); // Продовжуємо пошук
    }

    path_info->path_index--; // Відкат шляху
    path_info->visited[from] = 0; // Знімаємо позначку "відвіданий"
}

/**
 * Знаходить усі шляхи від поточного вузла до всіх інших вузлів.
 * @param start Вказівник на вузол, з якого починається пошук.
 * @param data Вказівник на структуру t_data.
 */
static void find_all_from(t_list *start, t_data *data) {
    for (t_list *copy = start->next; copy; copy = copy->next) {
        t_path_info *path_info = create_path_info(data->graph->size);
        t_path_list *all_paths = create_paths();
        t_coords cords = { .from = ((t_node *)start->data)->id, .to = ((t_node *)copy->data)->id };
        find_all_from_to(cords, data, path_info, all_paths);
        display_paths(all_paths, data); // Виводимо знайдені шляхи
        free_path_info(path_info);
        free_paths(all_paths);
    }
}

/**
 * Основна функція для виконання пошуку шляхів у графі.
 * @param data Вказівник на структуру t_data.
 */
void execute_pathfinding(t_data *data) {
    for (t_list *current = data->nodes; current; current = current->next) {
        find_all_from(current, data);
    }
}
