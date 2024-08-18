import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;

public class SyncNonblocking {
    public static void main(String[] args) {
        System.out.println("=== [Sync] [Non-blocking] ===");

        Runnable task1 = () -> {
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "선행 작업 시작"));
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "선행 작업 완료"));
        };
        Runnable task2 = () -> {
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "후행 작업 시작"));
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "후행 작업 완료"));
        };
        Thread thread1 = new Thread(task1);
        Thread thread2 = new Thread(task2);

        thread1.start();

        while (true) {
            if (thread1.getState() != Thread.State.TERMINATED) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                System.out.println(String.format("[%8s] %s", Thread.currentThread().getName(), "선행 작업 종료 확인"));
                continue;
            }

            thread2.start();
            break;
        }

    }

    
}
    