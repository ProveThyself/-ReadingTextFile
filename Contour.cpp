#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

// Направления движения: вверх, вправо, вниз, влево
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

// Функция для проверки замкнутости контура
bool isContourClosed(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height) {
    int startX = -1, startY = -1;

    // Ищем начальную точку на контуре (первая найденная '#')
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == '#') {
                startX = x;
                startY = y;
                break;
            }
        }
        if (startX != -1) break;
    }

    // Если не нашли ни одной точки контура
    if (startX == -1 || startY == -1) {
        return false;
    }

    // Начинаем обход с найденной точки
    int currentX = startX;
    int currentY = startY;
    int direction = 0; // Начальное направление (вверх)

    do {
        // Пробуем повернуть налево
        int leftDirection = (direction + 3) % 4; // Поворот налево
        int nextX = currentX + dx[leftDirection];
        int nextY = currentY + dy[leftDirection];

        if (nextX >= 0 && nextX < width && nextY >= 0 && nextY < height && grid[nextY][nextX] == '#') {
            // Если можем пойти налево, делаем шаг
            currentX = nextX;
            currentY = nextY;
            direction = leftDirection; // Меняем направление на новое
        } else {
            // Иначе пробуем идти прямо
            int straightX = currentX + dx[direction];
            int straightY = currentY + dy[direction];

            if (straightX >= 0 && straightX < width && straightY >= 0 && straightY < height && grid[straightY][straightX] == '#') {
                // Если можем пойти прямо, делаем шаг
                currentX = straightX;
                currentY = straightY;
            } else {
                // Если не можем идти ни налево, ни прямо — поворачиваем направо
                direction = (direction + 1) % 4; // Поворот направо
            }
        }
    } while (currentX != startX || currentY != startY); // Пока не вернёмся в начальную точку

    return true; // Если вышли из цикла, значит контур замкнут
}

// Функция для заливки области внутри контура
void fillContour(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height) {
    // Находим точку внутри контура для начала заливки
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == ' ') { // Предполагаем, что пустое пространство обозначается пробелом
                grid[y][x] = '*'; // Заполняем область звездочками
            }
        }
    }
}

// Основная программа
int main() {
    FILE *file = fopen("contour.txt", "r"); // Открываем файл для чтения
    if (!file) {
        printf("Ошибка: Не удалось открыть файл.\n");
        return 1;
    }

    char grid[MAX_HEIGHT][MAX_WIDTH] = {{0}};
    int width = 0, height = 0;

    char line[MAX_WIDTH];
    while (fgets(line, sizeof(line), file) && height < MAX_HEIGHT) {
        int currentWidth = 0;
        while (line[currentWidth] != '0' && line[currentWidth] != '\n' && currentWidth < MAX_WIDTH) {
            grid[height][currentWidth] = line[currentWidth];
            currentWidth++;
        }
        if (currentWidth > width) {
            width = currentWidth;
        }
        height++;
    }
    fclose(file);

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
        printf("Ошибка: Контур не закрыт.\n");
        return 1;
    }

    printf("Контур замкнут. Заполняем контур...\n");

    // Заполняем контур
    fillContour(grid, width, height);

    // Выводим заполненный контур на экран
    printf("Заполненный контур:\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }

    return 0;
}
