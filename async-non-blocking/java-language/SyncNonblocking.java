import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;

public class SyncNonblocking {
    public static void run() {
        Runnable task = () -> {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(String.format("[%35s] %s", Thread.currentThread().getName(), "Thread run"));
        };
        Thread thread = new Thread(task);
        thread.start();
        System.out.println(String.format("[%35s] %s", Thread.currentThread().getName(), "Main thread work!"));
    }
}
    