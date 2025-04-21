#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_WIDTH 100   
#define MAX_HEIGHT 100  

// Функция проверяет, замкнут ли контур
bool isContourClosed(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height) {
    // Проходим по всем ячейкам карты
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Если текущая ячейка содержит символ '#'
            if (grid[y][x] == '#') {
                int neighbors = 0; // Количество соседей (символов '#')

                // Проверяем соседей сверху, снизу, слева и справа
                if (x > 0 && grid[y][x - 1] == '#') neighbors++;         // Слева
                if (x < width - 1 && grid[y][x + 1] == '#') neighbors++; // Справа
                if (y > 0 && grid[y - 1][x] == '#') neighbors++;         // Сверху
                if (y < height - 1 && grid[y + 1][x] == '#') neighbors++;// Снизу

                // Если у текущей точки меньше двух соседей, контур не замкнут
                if (neighbors < 2) {
                    return false; // Контур не замкнут
                }
            }
        }
    }
    return true; // Контур замкнут
}

// Рекурсивная функция для заливки области внутри контура
void floodFill(char grid[MAX_HEIGHT][MAX_WIDTH], int x, int y, int width, int height) {
    // Условие выхода из рекурсии:
    // Если координаты выходят за границы массива — возвращаемся
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    // Если текущая ячейка уже заполнена ('*') или это стена ('#'), выходим
    if (grid[y][x] == '#' || grid[y][x] == '*') return;

    // Заполняем текущую ячейку символом '*'
    grid[y][x] = '*';

    // Рекурсивно вызываем функцию для соседних ячеек: справа, слева, сверху и снизу
    floodFill(grid, x + 1, y, width, height); // Справа
    floodFill(grid, x - 1, y, width, height); // Слева
    floodFill(grid, x, y + 1, width, height); // Снизу
    floodFill(grid, x, y - 1, width, height); // Сверху
}

// Функция ищет начальную точку для заливки внутри контура
bool findStartPoint(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height, int *startX, int *startY) {
    // Проходим по всем ячейкам карты, кроме границ
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Если текущая ячейка пустая (' ')
            if (grid[y][x] == ' ') {
                // Проверяем, есть ли рядом стена ('#')
                if ((grid[y - 1][x] == '#' || grid[y + 1][x] == '#') &&   // Сверху или снизу
                    (grid[y][x - 1] == '#' || grid[y][x + 1] == '#')) {   // Слева или справа
                    *startX = x; // Сохраняем координаты начальной точки
                    *startY = y;
                    return true; // Точка найдена
                }
            }
        }
    }
    return false; // Начальная точка не найдена
}

// Основная программа
int main() {
    FILE *file = fopen("contour.txt", "r"); // Открываем файл для чтения
    if (!file) {       // Если файл не удалось открыть
        printf("Ошибка: Не удалось открыть файл.\n");
        return 1;       // Завершаем программу с ошибкой
    }

    char grid[MAX_HEIGHT][MAX_WIDTH] = {{0}}; // Двумерный массив для хранения карты
    int width = 0, height = 0;       // Размеры карты

    char line[MAX_WIDTH];    // Строка для чтения из файла
    while (fgets(line, sizeof(line), file) && height < MAX_HEIGHT) { 
        int currentWidth = 0;   // Текущая ширина строки
                while (line[currentWidth] != '0' && line[currentWidth] != '\n' && currentWidth < MAX_WIDTH) {
            grid[height][currentWidth] = line[currentWidth]; // Записываем символ в массив
            currentWidth++;
        }
        if (currentWidth > width) {    // Обновляем максимальную ширину карты
            width = currentWidth;
        }
        height++;      // Увеличиваем высоту карты на одну строку
    }
    fclose(file);    // Закрываем файл

    // Выводим исходный контур на экран
    printf("Исходный контур:\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }

    // Проверяем, замкнут ли контур
    if (!isContourClosed(grid, width, height)) {
        printf("Ошибка: Контур не замкнут.\n"); 
        return 1; 
    }

    int startX, startY;  // Координаты начальной точки для заливки

    // Ищем начальную точку внутри контура
    if (!findStartPoint(grid, width, height, &startX, &startY)) {
        printf("Ошибка: Не удалось найти начальную точку внутри контура..\n");
        return 1;
    }

    // Выполняем заливку области внутри контура
    floodFill(grid, startX, startY, width, height);

    // Выводим заполненный контур на экран
    printf("\nЗаполненный контур:\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }

    return 0;
}
