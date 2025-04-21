// Contour.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

// Функция для проверки, замкнут ли контур
bool isContourClosed(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[y][x] == '#') {
				// Проверяем соседей (слева, справа, сверху, снизу)
				int neighbors = 0;
				if (x > 0 && grid[y][x - 1] == '#') neighbors++;
				if (x < width - 1 && grid[y][x + 1] == '#') neighbors++;
				if (y > 0 && grid[y - 1][x] == '#') neighbors++;
				if (y < height - 1 && grid[y + 1][x] == '#') neighbors++;

				// Если у граничной точки меньше 2 соседей, контур не замкнут
				if (neighbors < 2) {
					return false;
				}
			}
		}
	}
	return true;
}

// Рекурсивная заливка (Flood Fill)
void floodFill(char grid[MAX_HEIGHT][MAX_WIDTH], int x, int y, int width, int height) {
	// Проверяем границы и избегаем повторной заливки
	if (x < 0 || x >= width || y < 0 || y >= height) return;
	if (grid[y][x] == '#' || grid[y][x] == '*') return;

	// Заливаем текущую точку
	grid[y][x] = '*';

	// Рекурсивно заливаем соседние точки
	floodFill(grid, x + 1, y, width, height);
	floodFill(grid, x - 1, y, width, height);
	floodFill(grid, x, y + 1, width, height);
	floodFill(grid, x, y - 1, width, height);
}

// Функция для поиска стартовой точки внутри контура
bool findStartPoint(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height, int *startX, int *startY) {
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			if (grid[y][x] == ' ') {
				// Проверяем, окружена ли точка контуром с какой-либо стороны
				if ((grid[y - 1][x] == '#' || grid[y + 1][x] == '#') &&
					(grid[y][x - 1] == '#' || grid[y][x + 1] == '#')) {
					*startX = x;
					*startY = y;
					return true;
				}
			}
		}
	}
	return false;
}

int main() {
	FILE *file = fopen("contour.txt", "r");
	if (!file) {
		printf("Error: Could not open file.\n");
		return 1;
	}

	char grid[MAX_HEIGHT][MAX_WIDTH] = { { 0 } };
	int width = 0, height = 0;

	// Чтение файла в массив
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

	// Вывод исходного рисунка
	printf("Original contour:\n");
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%c", grid[y][x]);
		}
		printf("\n");
	}

	// Проверка замкнутости контура
	if (!isContourClosed(grid, width, height)) {
		printf("Error: Contour is not closed.\n");
		return 1;
	}
	// Поиск стартовой точки для заливки
	int startX, startY;
	if (!findStartPoint(grid, width, height, &startX, &startY)) {
		printf("Error: Could not find a starting point inside the contour.\n");
		return 1;
	}

	// Заливка контура
	floodFill(grid, startX, startY, width, height);

	// Вывод заполненного рисунка
	printf("\nFilled contour:\n");
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%c", grid[y][x]);
		}
		printf("\n");
	}

	return 0;
}