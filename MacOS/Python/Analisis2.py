import subprocess
import re
import random
import time
from threading import Thread


def bubble_sort(arr):
    time_init=time.time()
    n = len(arr)
    for i in range(n):
        # Ultimos i elementos ya estan en su lugar
        for j in range(0, n - i - 1):
            # Intercambiar si el elemento encontrado es mayor que el siguiente elemento
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    time_end=time.time()
    print("-----------BUBBLE SORT------------")
    print("End BS, time spent: ", time_end-time_init,"s")

def partition(arr, low, high):
    i = (low - 1)
    pivot = arr[high]

    for j in range(low, high):
        if arr[j] <= pivot:
            i = i + 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return (i + 1)

def quick_sort_aux(arr, low, high):
    if low < high:
        pi = partition(arr, low, high)
        quick_sort_aux(arr, low, pi - 1)
        quick_sort_aux(arr, pi + 1, high)

def quick_sort(arr):
    time_init=time.time()
    quick_sort_aux(arr,0,len(arr)-1)
    time_end=time.time()
    print("------------QUICK SORT-------------")
    print("End QS, time spent: ", time_end-time_init,"s")

def runWattsMeasurement():
    # Comando powermetrics con opciones para obtener información específica
    comando = "sudo -S powermetrics -n 1 --samplers cpu_power"

    # Solicitar la contraseña al usuario
    contrasena = input("Please input the user password: ")

    #Preparación listas
    arr_QS = [random.randint(1, 100) for _ in range(80000)]
    arr_BS = [random.randint(1, 100) for _ in range(8000)]

    print("-------NO ALGORITHM RUNNING--------")

    # Ejecutar el comando con subprocess y proporcionar la contraseña a través de stdin
    Time_init=time.time()
    proceso = subprocess.Popen(comando, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = proceso.communicate(input=contrasena + '\n')
    Time_end=time.time()
    # Utilizar expresión regular para encontrar la potencia
    valor = re.search(r'(\d+\.\d+)W', stdout)

    if valor:
        valor_numerico = float(valor.group(1))
        print("Power spent: ", valor_numerico,"W")
        print("Time spent : ",  Time_end-Time_init,"s")


    #Ejecucion Quick Sort
    QSThread=Thread(target=quick_sort,args=[arr_QS,])
    QSThread.start()
    proceso = subprocess.Popen(comando, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = proceso.communicate(input=contrasena + '\n')
    proceso2 = subprocess.Popen(comando, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout2, stderr2 = proceso2.communicate(input=contrasena + '\n')

    valor = re.search(r'(\d+\.\d+)W', stdout)
    valor2 = re.search(r'(\d+\.\d+)W', stdout2)

    # Verificar si se encontró el valor y extraerlo
    QSThread.join()
    if valor:
        valor_numerico = float(valor.group(1))
        valor_numerico2 = float(valor2.group(1))

        print("Power Spent: ", (valor_numerico+valor_numerico2)/2,"W")


    # Ejecucion Bubble Sort
    BSThread=Thread(target=bubble_sort,args=[arr_BS,])
    BSThread.start()
    proceso = subprocess.Popen(comando, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = proceso.communicate(input=contrasena + '\n')

    proceso2 = subprocess.Popen(comando, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout2, stderr2 = proceso2.communicate(input=contrasena + '\n')
    #Utilizar expresión regular para encontrar el flotante
    valor = re.search(r'(\d+\.\d+)W', stdout)
    valor2 = re.search(r'(\d+\.\d+)W', stdout2)
    BSThread.join()
    # Verificar si se encontró el valor y extraerlo
    if valor:
        valor_numerico = float(valor.group(1))
        valor_numerico2 = float(valor2.group(1))
        print("Power Spent: ", (valor_numerico+valor_numerico2)/2,"W")


