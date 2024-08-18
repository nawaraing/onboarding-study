import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class FutureExample {
    public static void main(String[] args) {
        // ExecutorService 생성
        ExecutorService executor = Executors.newSingleThreadExecutor();

        // 비동기 작업 정의
        Callable<String> task = () -> {
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                System.out.println("here!");
                e.printStackTrace();
            }
            return "hello world!";
        };

        // 작업을 제출하고 Future 객체를 반환받음
        Future<String> future = executor.submit(task);

        try {
            Thread.sleep(2000);
            future.cancel(true);
            if (future.isCancelled()) {
                System.out.println("Future cancel done!");
            } else {
                String result = future.get(); // 작업 완료까지 블로킹
                System.out.println("Future result: " + result);
            }
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        } finally {
            executor.shutdown();
        }
    }
}