#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define PASSWORD "MACho"  // Reemplaza esto con tu contraseña

// Función de ordenamiento de burbuja
void* bubble_sort(void* arg) {
    int* arr = (int*)arg;
    int n = 80000;
    clock_t time_init=clock();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    clock_t time_end=clock();
    double total_time = time_end-time_init;
    printf("End BS, time spent:  %f s\n",total_time/CLOCKS_PER_SEC);
    return NULL;
}

// Función auxiliar para Quick Sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high-1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// Función auxiliar interna para Quick Sort
void quick_sort_aux(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_aux(arr, low, pi - 1);
        quick_sort_aux(arr, pi + 1, high);
    }
}

// Función de ordenamiento Quick Sort
void* quick_sort(void* arg) {
    int* arr = (int*)arg;
    clock_t time_init = clock();
    quick_sort_aux(arr, 0, 799999);
    clock_t time_end = clock();
    double total_time = time_end-time_init;
    printf("End QS, time spent: %f s\n",total_time/CLOCKS_PER_SEC);
    return NULL;
}

// Función para ejecutar powermetrics con sudo y capturar la salida
double ejecutar_powermetrics() {
    FILE *fp;
    char path[1035];
    char command[150];
    double power = 0.0;

    // Comando para ejecutar powermetrics con sudo
    snprintf(command, sizeof(command), "echo %s | sudo -S powermetrics -n 1 --samplers cpu_power 2>&1", PASSWORD);

    // Abrir el comando para leer
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("No se pudo ejecutar el comando\n");
        exit(1);
    }

    // Leer la salida del comando y buscar la línea con la potencia
    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
        // Buscar la línea que contiene la potencia
        if (strstr(path, "Intel energy model derived package power (CPUs+GT+SA):") != NULL) {
            sscanf(path, "Intel energy model derived package power (CPUs+GT+SA): %lfW", &power);
        }
    }

    // Cerrar el proceso
    pclose(fp);

    return power;
}

int main() {
    double time_taken, power, power2    ;
    int i;

    // Tamaño de los arreglos
    int n_qs = 800000;
    int n_bs = 80000;

    // Arreglos para Quick Sort y Bubble Sort
    int *arr_qs = (int *)malloc(n_qs * sizeof(int));
    int *arr_bs = (int *)malloc(n_bs * sizeof(int));

    // Llenar los arreglos con números aleatorios
    srand(time(NULL));
    for (i = 0; i < n_qs; i++) {
        arr_qs[i] = rand() % 100;
    }
    for (i = 0; i < n_bs; i++) {
        arr_bs[i] = rand() % 100;
    }

    // Obtener la potencia sin ejecución
    printf("-------NO ALGORITHM RUNNING--------\n");
    clock_t time_init = clock();
    power = ejecutar_powermetrics();
    clock_t time_end = clock();
    time_taken = ((double) (time_end - time_init)) / CLOCKS_PER_SEC;
    printf("Time spent: %f s\n", time_taken);
    printf("Power Spent: %lf W\n", power);

    // Ejecutar Quick Sort y medir el tiempo y potencia
    printf("------------QUICK SORT-------------\n");
    pthread_t qs_thread;
    pthread_create(&qs_thread, NULL, quick_sort, arr_qs);
    power = ejecutar_powermetrics();
    power2 =ejecutar_powermetrics();
    pthread_join(qs_thread, NULL);
    printf("Power Spent: %lf W\n", (power+power2)/2);

    // Ejecutar Bubble Sort y medir el tiempo y potencia
    printf("-----------BUBBLE SORT------------\n");
    pthread_t bs_thread;
    pthread_create(&bs_thread, NULL, bubble_sort, arr_bs);
    power = ejecutar_powermetrics();
    power2 = ejecutar_powermetrics();
    pthread_join(bs_thread, NULL);
    printf("Power Spent: %lf W\n", (power+power2)/2);



    // Liberar memoria
    free(arr_qs);
    free(arr_bs);

    return 0;
}
