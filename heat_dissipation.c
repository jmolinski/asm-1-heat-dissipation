#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

extern void start(int szer, int wys, float *M, float C, float waga);
extern void place(int ile, int x[], int y[], float temp[]);
extern void step();

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
      printf("%.3f\t", matrix[r * columns + c]);
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
  float *matrix = calloc((columns * rows) * 2, sizeof(float));
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

  free(x_heaters);
  free(y_heaters);
  free(temp_heaters);

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

  free(matrix);

  return 0;
}
