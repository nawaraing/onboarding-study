public class SharedVariableExample {
    private static int sharedValue = 0;

    public static void main(String[] args) {
        Runnable task = () -> {
            for (int i = 0; i < 5; i++) {
                sharedValue++;
                System.out.println(Thread.currentThread().getName() + ": " + sharedValue);
            }
        };

        Thread thread1 = new Thread(task, "Thread-1");
        Thread thread2 = new Thread(task, "Thread-2");

        thread1.start();
        thread2.start();
    }
}
