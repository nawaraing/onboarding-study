import java.util.concurrent.CompletableFuture;

public class AsyncNonblocking {
    public static void main(String[] args) {
        System.out.println("=== [Async] [Non-blocking] ===");
        
        Runnable task1 = () -> {
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "독립 작업1 시작"));
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "독립 작업1 완료"));
        };
        Runnable task2 = () -> {
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "독립 작업2 시작"));
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "독립 작업2 완료"));
        };
        Thread thread1 = new Thread(task1);
        Thread thread2 = new Thread(task2);

        thread1.start();
        thread2.start();
        System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "메인 스레드 작업"));
    }    
}
