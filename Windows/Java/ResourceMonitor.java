import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryUsage;
import java.lang.management.OperatingSystemMXBean;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class ResourceMonitor {

    private static double getCpuUsage() {
        OperatingSystemMXBean osBean = ManagementFactory.getOperatingSystemMXBean();
        if (osBean instanceof com.sun.management.OperatingSystemMXBean) {
            return ((com.sun.management.OperatingSystemMXBean) osBean).getProcessCpuLoad() * 100;
        }
        return 0.0;
    }

    private static double getMemoryUsage() {
        MemoryMXBean memoryBean = ManagementFactory.getMemoryMXBean();
        MemoryUsage heapMemoryUsage = memoryBean.getHeapMemoryUsage();
        return ((double) heapMemoryUsage.getUsed() / heapMemoryUsage.getMax()) * 100;
    }

    private static List<Double>[] monitorResources(Thread thread, long interval) throws InterruptedException {
        List<Double> cpuUsage = new ArrayList<>();
        List<Double> memoryUsage = new ArrayList<>();

        while (thread.isAlive()) {
            cpuUsage.add(getCpuUsage());
            memoryUsage.add(getMemoryUsage());
            Thread.sleep(interval);
        }

        return new List[]{cpuUsage, memoryUsage};
    }

    private static int[] quicksortAux(int[] arr) {
        if (arr.length <= 1) {
            return arr;
        }
        int pivot = arr[arr.length / 2];
        int[] left = Arrays.stream(arr).filter(x -> x < pivot).toArray();
        int[] middle = Arrays.stream(arr).filter(x -> x == pivot).toArray();
        int[] right = Arrays.stream(arr).filter(x -> x > pivot).toArray();
        return concatenate(quicksortAux(left), middle, quicksortAux(right));
    }

    private static int[] concatenate(int[] left, int[] middle, int[] right) {
        int[] result = new int[left.length + middle.length + right.length];
        System.arraycopy(left, 0, result, 0, left.length);
        System.arraycopy(middle, 0, result, left.length, middle.length);
        System.arraycopy(right, 0, result, left.length + middle.length, right.length);
        return result;
    }

    private static void quicksort(int[] arr) {
        long init = System.currentTimeMillis();
        quicksortAux(arr);
        System.out.println("Time doing QuickSort: " + (System.currentTimeMillis() - init) + "ms");
    }

    private static void bubbleSort(int[] arr) {
        long init = System.currentTimeMillis();
        int n = arr.length;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        System.out.println("Time doing BubbleSort: " + (System.currentTimeMillis() - init) + "ms");
    }

    private static void noneFunc() throws InterruptedException {
        long init = System.currentTimeMillis();
        Thread.sleep(1000);
        System.out.println("Time waiting 1s: " + (System.currentTimeMillis() - init) + "ms");
    }

    public static void main(String[] args) throws InterruptedException {
        Random random = new Random();
        int[] arr_quick = random.ints(80000, 0, 100000).toArray();
        int[] arr_bubble = random.ints(8000, 0, 100000).toArray();

        // Monitorear y ejecutar nada
        Thread noneThread = new Thread(() -> {
            try {
                noneFunc();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        noneThread.start();
        System.out.println("------------1 sec delay-----------");

        List<Double>[] noneUsage = monitorResources(noneThread, 100);
        List<Double> noneCpuUsage = noneUsage[0];
        List<Double> noneMemoryUsage = noneUsage[1];
        noneThread.join();
        System.out.printf("Mean CPU usage: %.2f%%%n", noneCpuUsage.stream().mapToDouble(Double::doubleValue).average().orElse(0.0));
        System.out.printf("Mean Memory usage: %.2f%%%n", noneMemoryUsage.stream().mapToDouble(Double::doubleValue).average().orElse(0.0));

        // Monitorear y ejecutar Quicksort
        Thread quickThread = new Thread(() -> quicksort(arr_quick));
        System.out.println("------------QuickSort-----------");
        quickThread.start();
        List<Double>[] quickUsage = monitorResources(quickThread, 100);
        List<Double> quickCpuUsage = quickUsage[0];
        List<Double> quickMemoryUsage = quickUsage[1];
        quickThread.join();
        System.out.printf("Mean CPU usage: %.2f%%%n", quickCpuUsage.stream().mapToDouble(Double::doubleValue).average().orElse(0.0));
        System.out.printf("Mean Memory usage: %.2f%%%n", quickMemoryUsage.stream().mapToDouble(Double::doubleValue).average().orElse(0.0));

        // Monitorear y ejecutar Bubble Sort
        Thread bubbleThread = new Thread(() -> bubbleSort(arr_bubble));
        System.out.println("------------BubbleSort-----------");
        bubbleThread.start();
        List<Double>[] bubbleUsage = monitorResources(bubbleThread, 100);
        List<Double> bubbleCpuUsage = bubbleUsage[0];
        List<Double> bubbleMemoryUsage = bubbleUsage[1];
        bubbleThread.join();
        System.out.println("Bubble Sort completed");
        System.out.printf("Mean CPU usage: %.2f%%%n", bubbleCpuUsage.stream().mapToDouble(Double::doubleValue).average().orElse(0.0));
        System.out.printf("Mean Memory usage: %.2f%%%n", bubbleMemoryUsage.stream().mapToDouble(Double::doubleValue).average().orElse(0.0));
    }
}
