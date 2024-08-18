import java.util.concurrent.Callable;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class SyncAsyncBlockingNonblockingExample {
    public static void main(String[] args) {

        // SyncBlocking.run();
        // AsyncBlocking.run();
        SyncNonblocking.run();
        // AsyncNonblocking.run(); // CompletableFuture
    }
}