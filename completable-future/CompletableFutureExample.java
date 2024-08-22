import java.util.concurrent.Callable;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class CompletableFutureExample {
    public static void main(String[] args) {

        String flowResult = "START";
        CompletableFuture<Void> future;

        Util.loggingWithThread("--- 0. Async Start ---");
            future = CompletableFuture.supplyAsync(() -> {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
                Util.loggingWithThread("--- 1. supplyAsync() ---");
                return flowResult + " -> supplyAsync()";
            }).thenApplyAsync((res) -> {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
                Util.loggingWithThread("--- 2. thenApplyAsync() ---");
                return res + " -> thenApplyAsync()";
            }).thenAcceptAsync((res) -> {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
                Util.loggingWithThread("--- 3. thenAcceptAsync() ---");
            });
        Util.loggingWithThread("--- 4. Async End ---");
        
        // String res = "default";
        try {
            // res = future.get();
            Thread.sleep(1000);
            future.get(); // blocking
            Util.loggingWithThread("--- 5. get() Done ---");
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }


        // Util.loggingWithThread(URL, res);

        // 메인 스레드가 종료되지 않도록 대기
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}