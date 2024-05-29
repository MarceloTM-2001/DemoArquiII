#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>

typedef struct {
    double *cpu_usage;
    double *memory_usage;
    int size;
    HANDLE hProcess;
    int interval;
    int done;
} ResourceUsage;

DWORD WINAPI monitor_resources(LPVOID arg) {
    ResourceUsage *usage = (ResourceUsage*)arg;
    int index = 0;

    while (!usage->done) {
        FILETIME ftCreation, ftExit, ftKernel, ftUser;
        ULARGE_INTEGER ulKernel, ulUser;
        GetProcessTimes(usage->hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser);
        ulKernel.LowPart = ftKernel.dwLowDateTime;
        ulKernel.HighPart = ftKernel.dwHighDateTime;
        ulUser.LowPart = ftUser.dwLowDateTime;
        ulUser.HighPart = ftUser.dwHighDateTime;

        usage->cpu_usage[index] = (double)(ulKernel.QuadPart + ulUser.QuadPart) / 10000000.0;

        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        usage->memory_usage[index] = (double)(statex.ullTotalPhys - statex.ullAvailPhys) / statex.ullTotalPhys * 100;

        index++;
        if (index >= usage->size) break;
        Sleep(usage->interval);
    }

    return 0;
}

void quicksort_aux(int *arr, int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    if (left < j)
        quicksort_aux(arr, left, j);
    if (i < right)
        quicksort_aux(arr, i, right);
}

DWORD WINAPI quicksort(LPVOID arg) {
    int *arr = (int*)arg;
    clock_t start = clock();
    quicksort_aux(arr, 0, 79999);
    clock_t end = clock();
    printf("Time doing QuickSort: %f s\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}

DWORD WINAPI bubble_sort(LPVOID arg) {
    int *arr = (int*)arg;
    clock_t start = clock();
    int n = 8000;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    clock_t end = clock();
    printf("Time doing BubbleSort: %f s\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}

DWORD WINAPI none_func(LPVOID arg) {
    clock_t start = clock();
    Sleep(1000);
    clock_t end = clock();
    printf("Time waiting 1s: %f s\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}

int main() {
    srand((unsigned int)time(NULL));
    int n_quick = 80000;
    int n_bubble = 8000;
    int *arr_quick = (int *) malloc(n_quick * sizeof(int));
    int *arr_bubble = (int *) malloc(n_bubble * sizeof(int));

    for (int i = 0; i < n_quick; i++) {
        arr_quick[i] = rand() % 100000;
    }
    for (int i = 0; i < n_bubble; i++) {
        arr_bubble[i] = rand() % 100000;
    }

    HANDLE hProcess = GetCurrentProcess();

    int size = 100;
    ResourceUsage usage_1;
    usage_1.cpu_usage = (double *) malloc(size * sizeof(double));
    usage_1.memory_usage = (double *) malloc(size * sizeof(double));
    usage_1.size = size;
    usage_1.hProcess = hProcess;
    usage_1.interval = 100;
    usage_1.done = 0;

    // Monitorear y ejecutar nada
    printf("------------1 sec delay-----------\n");
    HANDLE none_thread = CreateThread(NULL, 0, none_func, NULL, 0, NULL);
    HANDLE monitor_thread = CreateThread(NULL, 0, monitor_resources, &usage_1, 0, NULL);
    WaitForSingleObject(none_thread, INFINITE);
    usage_1.done = 1;
    WaitForSingleObject(monitor_thread, INFINITE);

    double mean_cpu_usage_1 = 0;
    double mean_memory_usage_1 = 0;
    for (int i = 0; i < size; i++) {
        mean_cpu_usage_1 += usage_1.cpu_usage[i];
        mean_memory_usage_1 += usage_1.memory_usage[i];
    }
    mean_cpu_usage_1 /= size;
    mean_memory_usage_1 /= size;

    printf("Mean CPU usage: %.2f%%\n", mean_cpu_usage_1);
    printf("Mean Memory usage: %.2f%%\n", mean_memory_usage_1);

    usage_1.done = 0;
    free(usage_1.cpu_usage);
    free(usage_1.memory_usage);

    // Monitorear y ejecutar Quicksort

    printf("------------QuickSort-----------\n");

    ResourceUsage usage_2;
    usage_2.cpu_usage = (double *) malloc(size * sizeof(double));
    usage_2.memory_usage = (double *) malloc(size * sizeof(double));
    usage_2.size = size;
    usage_2.hProcess = hProcess;
    usage_2.interval = 100;
    usage_2.done = 0;


    HANDLE quick_thread = CreateThread(NULL, 0, quicksort, arr_quick, 0, NULL);
    monitor_thread = CreateThread(NULL, 0, monitor_resources, &usage_2, 0, NULL);
    WaitForSingleObject(quick_thread, INFINITE);
    usage_2.done = 1;
    WaitForSingleObject(monitor_thread, INFINITE);

    double mean_cpu_usage_2 = 0;
    double mean_memory_usage_2 = 0;
    for (int i = 0; i < size; i++) {
        mean_cpu_usage_2 += usage_2.cpu_usage[i];
        mean_memory_usage_2 += usage_2.memory_usage[i];
    }
    mean_cpu_usage_2 /= size;
    mean_memory_usage_2 /= size;

    printf("Mean CPU usage: %.2f%%\n", mean_cpu_usage_2);
    printf("Mean Memory usage: %.2f%%\n", mean_memory_usage_2);

    usage_2.done = 0;
    free(usage_2.cpu_usage);
    free(usage_2.memory_usage);

    // Monitorear y ejecutar Bubble Sort

    ResourceUsage usage_3;
    usage_3.cpu_usage = (double *) malloc(size * sizeof(double));
    usage_3.memory_usage = (double *) malloc(size * sizeof(double));
    usage_3.size = size;
    usage_3.hProcess = hProcess;
    usage_3.interval = 100;
    usage_3.done = 0;

    printf("------------BubbleSort-----------\n");
    HANDLE bubble_thread = CreateThread(NULL, 0, bubble_sort, arr_bubble, 0, NULL);
    monitor_thread = CreateThread(NULL, 0, monitor_resources, &usage_3, 0, NULL);
    WaitForSingleObject(bubble_thread, INFINITE);
    usage_3.done = 1;
    WaitForSingleObject(monitor_thread, INFINITE);

    double mean_cpu_usage_3 = 0;
    double mean_memory_usage_3 = 0;
    for (int i = 0; i < size; i++) {
        mean_cpu_usage_3 += usage_3.cpu_usage[i];
        mean_memory_usage_3 += usage_3.memory_usage[i];
    }
    mean_cpu_usage_3 /= size;
    mean_memory_usage_3 /= size;

    printf("Bubble Sort completed\n");
    printf("Mean CPU usage: %.2f%%\n", mean_cpu_usage_3);
    printf("Mean Memory usage: %.2f%%\n", mean_memory_usage_3);

    free(usage_3.cpu_usage);
    free(usage_3.memory_usage);
    free(arr_quick);
    free(arr_bubble);

    return 0;
}
