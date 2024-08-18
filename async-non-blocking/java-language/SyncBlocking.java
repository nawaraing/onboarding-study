import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class SyncBlocking {
    public static void main(String[] args) {
        System.out.println("=== [Sync] [Blocking] ===");
        
        String filePath = "a.txt";

        try {
            // line을 읽어야
            System.out.println("선행 작업 시작");
            List<String> lines = Files.readAllLines(Paths.get(filePath));
            System.out.println("선행 작업 완료");
            
            // 출력할 수 있음
            System.out.println("후행 작업 시작");
            for (String line : lines) {
                System.out.println(line);
            }
            System.out.println("후행 작업 완료");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
