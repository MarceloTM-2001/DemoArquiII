import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Random;

public class SortPowerMetrics {
    // Tamaño de los arreglos
    private static final int N_QS = 800000;
    private static final int N_BS = 80000;

    // Contraseña de sudo
    private static final String PASSWORD = "MACho";  // Reemplaza esto con tu contraseña

    public static void main(String[] args) throws Exception {
        int[] arrQS = new int[N_QS];
        int[] arrBS = new int[N_BS];

        Random random = new Random();
        for (int i = 0; i < N_QS; i++) {
            arrQS[i] = random.nextInt(100);
        }
        for (int i = 0; i < N_BS; i++) {
            arrBS[i] = random.nextInt(100);
        }

        // Medir potencia sin ejecución
        System.out.printf("-------NO ALGORITHM RUNNING--------%n");
        long startTime = System.currentTimeMillis();
        double power = ejecutarPowermetrics();
        long endTime = System.currentTimeMillis();
        double timeTaken = (endTime - startTime) / 1000.0;
        System.out.printf("Time Spent: %.5f s%n", timeTaken);
        System.out.printf("Power Spent: %.2f W%n", power);

        // Ejecutar Quick Sort y medir tiempo y potencia
        System.out.printf("------------QUICK SORT-------------%n");
        Thread qsThread = new Thread(() -> quickSort(arrQS, 0, N_QS - 1));
        qsThread.start();
        power = ejecutarPowermetrics();
        qsThread.join();
        System.out.printf("Power Spent: %.2f W%n", power);

        // Ejecutar Bubble Sort y medir tiempo y potencia
        System.out.printf("------------BUBBLE SORT------------%n");
        Thread bsThread = new Thread(() -> bubbleSort(arrBS));
        bsThread.start();
        power = ejecutarPowermetrics();
        bsThread.join();
        System.out.printf("Power Spent: %.2f W%n", power);

        for (int i=0;i<80000-2;i++){
            if(i<8000-2 && arrBS[i]>arrBS[i+1]){
                System.out.printf("Fallo Bubble en %d: [i:%d ,i+1:%d]",i,arrBS[i],arrBS[i+1]);
            }else if(arrQS[i]>arrQS[i+1]){
                System.out.printf("Fallo Quick en %d: [i:%d ,i+1:%d]",i,arrQS[i],arrQS[i+1]);
            }else{
                continue;
            }
        }
    }

    // Implementación de Quick Sort
    private static void quickSort(int[]arr,int low, int high){
        long startTime = System.currentTimeMillis();
        quickSortaux(arr, 0, N_QS - 1);
        long endTime = System.currentTimeMillis();
        double timeTaken = (endTime - startTime) / 1000.0;
        System.out.printf("End QS, time spent: %.5f s%n", timeTaken);

    }

    private static void quickSortaux(int[] arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortaux(arr, low, pi - 1);
            quickSortaux(arr, pi + 1, high);
        }
    }

    private static int partition(int[] arr, int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr, i, j);
            }
        }
        swap(arr, i + 1, high);
        return i + 1;
    }

    private static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    // Implementación de Bubble Sort
    private static void bubbleSort(int[] arr) {
        int n = arr.length;
        long startTime = System.currentTimeMillis();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr, j, j + 1);
                }
            }
        }
        long endTime = System.currentTimeMillis();
        double timeTaken = (endTime - startTime) / 1000.0;
        System.out.printf("End BS, time spent:  %.5f s%n", timeTaken);
    }

    // Ejecutar powermetrics y capturar la salida
    private static double ejecutarPowermetrics() {
        double power = 0.0;
        try {
            ProcessBuilder pb = new ProcessBuilder("sh", "-c", "echo " + PASSWORD + " | sudo -S powermetrics -n 1 --samplers cpu_power");
            Process process = pb.start();
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                 PrintWriter writer = new PrintWriter(new OutputStreamWriter(process.getOutputStream()), true)) {
                writer.println(PASSWORD);
                String line;
                while ((line = reader.readLine()) != null) {
                    if (line.contains("Intel energy model derived package power (CPUs+GT+SA):")) {
                        power = Double.parseDouble(line.split(":")[1].trim().replace("W", ""));
                    }
                }
            }
            process.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return power;
    }
}

