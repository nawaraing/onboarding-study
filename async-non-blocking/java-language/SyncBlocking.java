import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class SyncBlocking {
    public static void run() {
        String filePath = "example.txt"; // 읽고자 하는 파일 경로

        try {
            // line을 읽어야
            List<String> lines = Files.readAllLines(Paths.get(filePath));

            // 출력할 수 있음
            for (String line : lines) {
                System.out.println(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
