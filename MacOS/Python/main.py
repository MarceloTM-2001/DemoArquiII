import time
import psutil
import random
import Analisis2
from threading import Thread


Analisis=input("Which Analysis would you like to do (1/2)?")

# Función para monitorizar el uso de recursos
def monitor_resources(process, interval=0.1):
    cpu_usage = []
    memory_usage = []

    while process.is_alive():
        cpu_usage.append(psutil.cpu_percent(interval=interval))
        memory_usage.append(psutil.virtual_memory().percent)
        time.sleep(interval)

    return cpu_usage, memory_usage

def quicksort_aux(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    return quicksort_aux(left) + middle + quicksort_aux(right)

def quicksort(arr):
    init=time.time()
    quicksort_aux(arr)
    print("Time doing QuickSort: ", time.time() - init, "s")

# Generar una lista de 80000 elementos aleatorios
arr_quick = [random.randint(0, 100000) for _ in range(80000)]
def bubble_sort(arr):
    init = time.time()
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    print("Time doing BubbleSort: ", time.time() - init, "s")


def noneFunc():
    init=time.time()
    time.sleep(1)
    print("Time waiting 1s: ", time.time()-init,"s")
    return None

if(Analisis=="1"):
    # Generar listas de elementos aleatorios
    arr_quick = [random.randint(0, 1000) for _ in range(80000)]
    arr_bubble = [random.randint(0, 1000) for _ in range(8000)]

    # Monitorear y ejecutar nada
    None_thread = Thread(target=lambda: noneFunc())

    None_thread.start()
    print("------------1 sec delay-----------")

    None_cpu_usage, None_memory_usage = monitor_resources(None_thread)
    None_thread.join()
    print(f"Mean CPU usage: {sum(None_cpu_usage) / len(None_cpu_usage):.2f}%")
    print(f"Mean Memory usage: {sum(None_memory_usage) / len(None_memory_usage):.2f}%")


    # Monitorear y ejecutar Quicksort
    quick_thread = Thread(target=lambda: quicksort(arr_quick))
    print("------------QuickSort-----------")
    quick_thread.start()
    quick_cpu_usage, quick_memory_usage = monitor_resources(quick_thread)
    quick_thread.join()
    print(f"Mean CPU usage: {sum(quick_cpu_usage) / len(quick_cpu_usage):.2f}%")
    print(f"Mean Memory usage: {sum(quick_memory_usage) / len(quick_memory_usage):.2f}%")

    # Monitorear y ejecutar Bubble Sort
    print("------------BubbleSort-----------")
    bubble_thread = Thread(target=lambda: bubble_sort(arr_bubble))
    bubble_thread.start()

    bubble_cpu_usage, bubble_memory_usage = monitor_resources(bubble_thread)

    print("Bubble Sort completed")
    bubble_thread.join()
    print(f"Mean CPU usage: {sum(bubble_cpu_usage) / len(bubble_cpu_usage):.2f}%")
    print(f"Mean Memory usage: {sum(bubble_memory_usage) / len(bubble_memory_usage):.2f}%")
elif(Analisis=="2"):
    Analisis2.runWattsMeasurement()
