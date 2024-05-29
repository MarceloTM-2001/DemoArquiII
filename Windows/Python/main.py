import time
import psutil
import random
from threading import Thread

# Función para monitorizar el uso de recursos
def monitor_resources(process, interval=0.1):
    cpu_usage = []
    memory_usage = []

    while process.is_alive():
        cpu_usage.append(psutil.cpu_percent(interval=interval))
        memory_usage.append(psutil.virtual_memory().percent)
        time.sleep(interval)

    return cpu_usage, memory_usage

def quicksort(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    return quicksort(left) + middle + quicksort(right)

# Generar una lista de 80000 elementos aleatorios
arr_quick = [random.randint(0, 100000) for _ in range(80000)]
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]

def noneFunc():
    print("Nothing")
    time.sleep(1)
    return None

# Generar listas de elementos aleatorios
arr_quick = [random.randint(0, 1000) for _ in range(80000)]
arr_bubble = [random.randint(0, 1000) for _ in range(8000)]

# Monitorear y ejecutar nada
None_thread = Thread(target=lambda: noneFunc())
None_thread.start()

None_cpu_usage, None_memory_usage = monitor_resources(None_thread)
None_thread.join()
print(f"Uso promedio de CPU: {sum(None_cpu_usage) / len(None_cpu_usage):.2f}%")
print(f"Uso promedio de Memoria: {sum(None_memory_usage) / len(None_memory_usage):.2f}%")


# Monitorear y ejecutar Quicksort
quick_thread = Thread(target=lambda: quicksort(arr_quick))
quick_thread.start()

quick_cpu_usage, quick_memory_usage = monitor_resources(quick_thread)
quick_thread.join()

print("Quicksort completado")
print(f"Uso promedio de CPU: {sum(quick_cpu_usage) / len(quick_cpu_usage):.2f}%")
print(f"Uso promedio de Memoria: {sum(quick_memory_usage) / len(quick_memory_usage):.2f}%")

# Monitorear y ejecutar Bubble Sort
bubble_thread = Thread(target=lambda: bubble_sort(arr_bubble))
bubble_thread.start()

bubble_cpu_usage, bubble_memory_usage = monitor_resources(bubble_thread)
bubble_thread.join()

print("Bubble Sort completado")
print(f"Uso promedio de CPU: {sum(bubble_cpu_usage) / len(bubble_cpu_usage):.2f}%")
print(f"Uso promedio de Memoria: {sum(bubble_memory_usage) / len(bubble_memory_usage):.2f}%")


