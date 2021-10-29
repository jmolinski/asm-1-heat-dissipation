#include <stdio.h>
#include <stdlib.h>


void start (int szer, int wys, float *M, float C, float waga) {
    // Przygotowuje symulację, np. inicjuje pomocnicze struktury.
    // Argumentami są: rozmiary matrycy, początkowa zawartość matrycy (temperatury komórek),
    // temperatura chłodnic oraz wspólczynnik proporcjonalności.
}

void place (int ile, int x[], int y[], float temp[]) {
    // Umieszcza grzejniki w podanych miejscach i ustala ich temperatury.
}

void step () {
    // Przeprowadza pojedynczy krok symulacji. Po jej wykonaniu macierz M (przekazana przez parametr
    // procedury start) zawiera nowy stan.
}

void read_matrix(float* matrix, int columns, int rows) {
    for(int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            scanf("%f", &matrix[r * columns + c]); // tutaj poprawka na numerowanie wierszy od gory/dolu s
        }
    }
}

void print_matrix(float* matrix, int columns, int rows) {
    for(int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            printf("%f ", matrix[r * columns + c]); // tutaj poprawka na numerowanie wierszy od gory/dolu s
        }
        printf("\n");
    }
}

int main() {
    int columns, rows;
    float cooler_temperature;
    scanf("%d %d %f", &columns, &rows, &cooler_temperature);

    printf("%d %d %f\n", columns, rows, cooler_temperature); // TODO REMOVE

    float* matrix = malloc(sizeof(float) * (columns * rows));

    read_matrix(matrix, columns, rows);

    print_matrix(matrix, columns, rows);  // TODO remove

    int number_of_heaters;
    scanf("%d", &number_of_heaters);

    int *x_heaters = malloc(sizeof(int) * number_of_heaters);
    int *y_heaters = malloc(sizeof(int) * number_of_heaters);
    float *temp_heaters = malloc(sizeof(float) * number_of_heaters);

    for (int i = 0; i < number_of_heaters; i++) {
        scanf("%d %d %f", &x_heaters[i], &y_heaters[i], &temp_heaters[i]);
    }

    free(x_heaters);
    free(y_heaters);
    free(temp_heaters);
    free(matrix);

    return 0;
}
