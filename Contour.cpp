// Contour.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

// ������� ��� ��������, ������� �� ������
bool isContourClosed(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[y][x] == '#') {
				// ��������� ������� (�����, ������, ������, �����)
				int neighbors = 0;
				if (x > 0 && grid[y][x - 1] == '#') neighbors++;
				if (x < width - 1 && grid[y][x + 1] == '#') neighbors++;
				if (y > 0 && grid[y - 1][x] == '#') neighbors++;
				if (y < height - 1 && grid[y + 1][x] == '#') neighbors++;

				// ���� � ��������� ����� ������ 2 �������, ������ �� �������
				if (neighbors < 2) {
					return false;
				}
			}
		}
	}
	return true;
}

// ����������� ������� (Flood Fill)
void floodFill(char grid[MAX_HEIGHT][MAX_WIDTH], int x, int y, int width, int height) {
	// ��������� ������� � �������� ��������� �������
	if (x < 0 || x >= width || y < 0 || y >= height) return;
	if (grid[y][x] == '#' || grid[y][x] == '*') return;

	// �������� ������� �����
	grid[y][x] = '*';

	// ���������� �������� �������� �����
	floodFill(grid, x + 1, y, width, height);
	floodFill(grid, x - 1, y, width, height);
	floodFill(grid, x, y + 1, width, height);
	floodFill(grid, x, y - 1, width, height);
}

// ������� ��� ������ ��������� ����� ������ �������
bool findStartPoint(char grid[MAX_HEIGHT][MAX_WIDTH], int width, int height, int *startX, int *startY) {
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			if (grid[y][x] == ' ') {
				// ���������, �������� �� ����� �������� � �����-���� �������
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

	// ������ ����� � ������
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

	// ����� ��������� �������
	printf("Original contour:\n");
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%c", grid[y][x]);
		}
		printf("\n");
	}

	// �������� ����������� �������
	if (!isContourClosed(grid, width, height)) {
		printf("Error: Contour is not closed.\n");
		return 1;
	}
	// ����� ��������� ����� ��� �������
	int startX, startY;
	if (!findStartPoint(grid, width, height, &startX, &startY)) {
		printf("Error: Could not find a starting point inside the contour.\n");
		return 1;
	}

	// ������� �������
	floodFill(grid, startX, startY, width, height);

	// ����� ������������ �������
	printf("\nFilled contour:\n");
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%c", grid[y][x]);
		}
		printf("\n");
	}

	return 0;
}