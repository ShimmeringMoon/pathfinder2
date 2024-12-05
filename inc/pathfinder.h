#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <limits.h>
#include "libmx.h"

#define INIT INT_MAX // Значення, що позначає початкову вагу ребра у графі

/**
 * Перелік можливих помилок, що можуть виникнути в програмі.
 */
typedef enum e_errors {
    NONE,         // Помилок немає
    ARGS,         // Помилка з аргументами командного рядка
    NAME,         // Файл не існує
    EMPTY,        // Файл порожній
    FIRSTLINE,    // Невалідний перший рядок у файлі
    LINE,         // Невалідний формат рядка у файлі
    ISLANDS,      // Невідповідність кількості островів
    BRIDGES,      // Дубльовані мости
    LENGTH        // Занадто велика сума довжин мостів
} t_errors;

typedef struct s_node t_node;
typedef struct s_path_info t_path_info;
typedef struct s_path t_path;
typedef struct s_path_list t_path_list;
typedef struct s_graph t_graph;
typedef struct s_data t_data;
// Декларації функцій
// =============================================================

/**
 * Читає рядок з файлу до вказаного символу.
 * @param fd Дескриптор файлу.
 * @param size Розмір прочитаного рядка.
 * @param eof Ознака кінця файлу.
 * @param delim Символ роздільник.
 * @return Прочитаний рядок.
 */
char *read_line_delim(int fd, int *size, int *eof, char delim);

/**
 * Ініціалізує структуру t_data.
 * @param fd Дескриптор файлу.
 * @param filename Ім'я файлу.
 * @return Вказівник на ініціалізовану структуру t_data.
 */
t_data *initialize_data(int fd, char *filename);

/**
 * Розбирає файл з вхідними даними.
 * @param filename Ім'я файлу.
 * @return Вказівник на структуру t_data, що містить розібрані дані.
 */
t_data *parse_input_file(char *filename);

/**
 * Перевіряє дані на наявність помилок.
 * @param data Вказівник на структуру t_data.
 * @return true, якщо помилки знайдено; false, якщо помилок немає.
 */
bool validate_data(t_data *data);

/**
 * Виконує пошук усіх можливих шляхів у графі.
 * @param data Вказівник на структуру t_data.
 */
void execute_pathfinding(t_data *data);

/**
 * Виводить усі знайдені шляхи.
 * @param all_paths Вказівник на список шляхів.
 * @param data Вказівник на структуру t_data.
 */
void display_paths(t_path_list *all_paths, t_data *data);

/**
 * Звільняє ресурси, пов'язані зі структурою t_data.
 * @param data Вказівник на структуру t_data.
 */
void free_resources(t_data *data);

/**
 * Функція для обробки помилок.
 * @param error Тип помилки.
 * @param filename Ім'я файлу, пов'язане з помилкою.
 * @param line Номер рядка, у якому виникла помилка.
 * @return Код завершення роботи функції.
 */
int handle_error(t_errors error, char *filename, int line);

// Декларації структур
// =============================================================

/**
 * Структура, що представляє вузол графа (острів).
 */
typedef struct s_node {
    int id;        // Унікальний ідентифікатор вузла
    char *name;    // Назва вузла
} t_node;

/**
 * Інформація про поточний шлях під час виконання пошуку.
 */
typedef struct s_path_info {
    int *visited;      // Відвідані вершини
    int *path;         // Поточний шлях
    int path_index;    // Індекс поточного вузла в шляху
    int weight;        // Поточна вага шляху
} t_path_info;

/**
 * Структура, що представляє шлях у графі.
 */
typedef struct s_path {
    int *vertices; // Масив вершин, які входять у шлях
    int length;    // Загальна довжина шляху
} t_path;

/**
 * Структура, що представляє список шляхів.
 */
typedef struct s_path_list {
    t_path *paths; // Масив усіх можливих шляхів
    int count;     // Кількість шляхів у списку
    int min_len;   // Довжина найкоротшого шляху
} t_path_list;

/**
 * Граф у вигляді матриці суміжності.
 */
typedef struct s_graph {
    int size;          // Розмір графа (кількість вузлів)
    int **matrix;      // Матриця суміжності
} t_graph;

/**
 * Основна структура, що зберігає дані програми.
 */
typedef struct s_data {
    int amount;        // Кількість островів
    t_list *nodes;     // Список вузлів графа
    t_graph *graph;    // Граф
    t_errors error;    // Код помилки
} t_data;

#endif // PATHFINDER_H

