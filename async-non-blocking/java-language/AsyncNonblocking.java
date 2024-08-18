import java.util.concurrent.CompletableFuture;

public class AsyncNonblocking {
    public static void run() {
        CompletableFuture<Void> future;
        
        future = CompletableFuture.runAsync(() -> {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(String.format("[%35s] %s", Thread.currentThread().getName(), "runAsync()"));
        });

        System.out.println(String.format("[%35s] %s", Thread.currentThread().getName(), "Work main thread"));
        future.join();
    }
}
