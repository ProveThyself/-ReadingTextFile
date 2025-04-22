#include <stdio.h>
#include <stdlib.h>

// Максимальные размеры поля
#define MAX_WIDTH 100
#define MAX_HEIGHT 100

// Массивы смещений для 4 направлений (вверх, вправо, вниз, влево)
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

// Флаг замкнутости контура (1 - замкнут, 0 - не замкнут)
int contour_closed = 1;

// Функция заливки (заполнения) области
void floodFill(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height, int x, int y) {
    // Если вышли за границы поля - выходим
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    // Если текущая клетка не пустая - выходим
    if (grid[y][x] != ' ') return;

    // Если достигли границы поля - контур не замкнут
    if (x == 0 || x == width-1 || y == 0 || y == height-1) {
        contour_closed = 0;
        return;
    }

    // Заполняем текущую клетку
    grid[y][x] = '*';

    // Рекурсивно вызываем заливку для всех 4 соседних клеток
    for (int dir = 0; dir < 4; dir++) {
        floodFill(grid, width, height, x + dx[dir], y + dy[dir]);
    }
}

// Функция поиска внутренней точки для заливки
int findInnerPoint(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height, int* fx, int* fy) {
    // Проходим по всем клеткам, кроме граничных
    for (int y = 1; y < height-1; ++y)
        for (int x = 1; x < width-1; ++x)
            // Если нашли пустую клетку
            if (grid[y][x] == ' ') {
                int cnt = 0;
                // Проверяем 4 соседние клетки
                for (int d = 0; d < 4; ++d) {
                    int nx = x + dx[d], ny = y + dy[d];
                    // Если соседняя клетка - граница ('#'), увеличиваем счетчик
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && grid[ny][nx] == '#') cnt++;
                }
                // Если у клетки не менее 2 соседей-границ, считаем ее подходящей для заливки
                if (cnt >= 2) {
                    *fx = x; *fy = y; // Сохраняем координаты точки
                    return 1; // Возвращаем успех
                }
            }
    return 0; // Не нашли подходящую точку
}

// Функция копирования сетки
void copyGrid(char dest[MAX_HEIGHT][MAX_WIDTH], char src[MAX_HEIGHT][MAX_WIDTH], int width, int height) {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            dest[y][x] = src[y][x];
}

int main() {
    // Открываем файл с контуром
    FILE *file = fopen("contour.txt", "r");
    if (!file) {
        printf("Ошибка: Не удалось открыть файл.\n");
        return 1;
    }

    char grid[MAX_HEIGHT][MAX_WIDTH]; // Массив для хранения сетки
    int width = 0, height = 0; // Ширина и высота сетки

    // Чтение файла по символам
    int c;
    int cur_x = 0;
    while ((c = fgetc(file)) != EOF && height < MAX_HEIGHT) {
        if (c == '\n' || c == '\r') {
            // Обновляем максимальную ширину
            if (cur_x > width) width = cur_x;
            // Обработка символов перевода строки (CRLF или LF)
            if (c == '\r') {
                int next_c = fgetc(file);
                if (next_c != '\n' && next_c != EOF) ungetc(next_c, file);
            }
            // Заполняем оставшуюся часть строки пробелами
            for (int fill_x = cur_x; fill_x < MAX_WIDTH; ++fill_x)
                grid[height][fill_x] = ' ';
            cur_x = 0;
            height++;
        } else {
            // Записываем символ в сетку
            if (cur_x < MAX_WIDTH)
                grid[height][cur_x++] = (char)c;
        }
    }
    // Обработка последней строки, если файл не заканчивается переводом строки
    if (cur_x > 0 && height < MAX_HEIGHT) {
        if (cur_x > width) width = cur_x;
        for (int fill_x = cur_x; fill_x < MAX_WIDTH; ++fill_x)
            grid[height][fill_x] = ' ';
        height++;
    }
    fclose(file);

    // Вывод исходного контура
    printf("Оригинальный контур:\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) printf("%c", grid[y][x]);
        printf("\n");
    }

    // Поиск точки для заливки
    int fx = -1, fy = -1;
    if (!findInnerPoint(grid, width, height, &fx, &fy)) {
        printf("Внутренняя точка для заполнения не найдена!\n");
        return 1;
    }

    // Проверка замкнутости контура на копии сетки
    char temp_grid[MAX_HEIGHT][MAX_WIDTH];
    copyGrid(temp_grid, grid, width, height);
    contour_closed = 1; // Сбрасываем флаг перед проверкой
    floodFill(temp_grid, width, height, fx, fy);

    if (!contour_closed) {
        printf("\nКонтур НЕ замкнут! Заливка достигла границы. Заливка не выполнена.\n");
    } else {
        // Если контур замкнут - выполняем заливку на оригинальной сетке
        floodFill(grid, width, height, fx, fy);
        printf("\nКонтур замкнут. Заполнение выполнено:\n");
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) printf("%c", grid[y][x]);
            printf("\n");
        }
    }

    return 0;
}