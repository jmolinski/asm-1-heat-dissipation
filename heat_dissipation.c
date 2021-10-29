#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

float *temp_matrix, *diff_matrix;
int width, height;
float coolant_temp, coeff;


void start(int szer, int wys, float *M, float C, float waga) {
    temp_matrix = M;
    diff_matrix = &M[szer * wys];
    width = szer;
    height = wys;
    coolant_temp = C;
    coeff = waga;

    for (int i = 0; i < width * height; i++) {
        diff_matrix[i] = 0;
    }
}

void place(int ile, int x[], int y[], float temp[]) {
    for (int i = 0; i < ile; i++) {
        int c = x[i];
        int r = y[i];

        temp_matrix[r * width + c] = temp[i];
        float a = nanf("1");
        diff_matrix[r * width + c] = a;
    }
}

float get_cell_temp(int r, int c) {
    if (r < 0 || c < 0 || r >= height || c >= width) {
        return coolant_temp;
    }

    return temp_matrix[r * width + c];
}

void calculate_diff_for_field(int r, int c) {
    float cell = diff_matrix[r * width + c];
    if (isnan(cell)) {
        return;
    }

    cell = temp_matrix[r * width + c];
    float diff = 0;
    diff += get_cell_temp(r - 1, c);
    diff -= cell;
    diff += get_cell_temp(r + 1, c);
    diff -= cell;
    diff += get_cell_temp(r, c - 1);
    diff -= cell;
    diff += get_cell_temp(r, c + 1);
    diff -= cell;

    diff = diff * coeff;
    diff_matrix[r * width + c] = diff;
}

void step() {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            calculate_diff_for_field(r, c);
        }
    }
    for (int i = 0; i < width * height; i++) {
        float cell_diff = diff_matrix[i];
        if (!isnan(cell_diff)) {
            temp_matrix[i] += cell_diff;
        }
    }
}

void read_matrix(FILE *fp, float *matrix, int columns, int rows) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            fscanf(fp, "%f", &matrix[r * columns + c]);
        }
    }
}

void print_matrix(float *matrix, int columns, int rows) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            printf("%f ", matrix[r * columns + c]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    assert(argc == 4);

    FILE *fp = fopen(argv[1], "r");
    float coeff = strtof(argv[2], NULL);
    long steps = strtol(argv[3], NULL, 10);

    int columns, rows;
    float cooler_temperature;
    fscanf(fp, "%d", &columns);
    fscanf(fp, "%d", &rows);
    fscanf(fp, "%f", &cooler_temperature);
    float *matrix = malloc(sizeof(float) * (columns * rows) * 2);
    read_matrix(fp, matrix, columns, rows);

    int number_of_heaters;
    fscanf(fp, "%d", &number_of_heaters);
    int *x_heaters = malloc(sizeof(int) * number_of_heaters);
    int *y_heaters = malloc(sizeof(int) * number_of_heaters);
    float *temp_heaters = malloc(sizeof(float) * number_of_heaters);
    for (int i = 0; i < number_of_heaters; i++) {
        fscanf(fp, "%d %d %f", &x_heaters[i], &y_heaters[i], &temp_heaters[i]);
    }

    fclose(fp);

    start(columns, rows, matrix, cooler_temperature, coeff);
    place(number_of_heaters, x_heaters, y_heaters, temp_heaters);

    for (int i = 0; i <= steps; i++) {
        printf("Liczba wykonanych krokow: %d\nStan macierzy:\n", i);
        print_matrix(matrix, columns, rows);
        printf("\n");
        step();

        if (i < steps) {
            printf("Nacisnij ENTER aby kontynuowac\n");
            char c;
            do {
                c = getchar();
            } while (c != '\n');
        }
    }

    free(x_heaters);
    free(y_heaters);
    free(temp_heaters);
    free(matrix);

    return 0;
}
