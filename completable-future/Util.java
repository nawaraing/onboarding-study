public class Util {

    private final static int THREAD_FORMAT_SIZE = 32;

    private final static String DEFAULT_BODY = "%s";

    public static void loggingWithSleep(String url, long seconds) {
        for (int i = 0; i < seconds; i++) {
            Util.logging(url, String.format("%d seconds...", i+1));
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    public static void loggingWithThread(String message) {

        String header = generateHeadersFormat(THREAD_FORMAT_SIZE);
        String body = DEFAULT_BODY;

        logging(generateMessageFormat(header, body), Thread.currentThread().getName(), message);
    }

    public static void logging(String formatStr, String... messages) {
        System.out.println(String.format(formatStr, messages));
    }

    public static String generateMessageFormat(String header, String body) {
        return header + " " + body;
    }

    // [header1][header2][header3] body
    private static String generateHeadersFormat(int... widths) {
        String format = "";

        for (int width : widths) {
            format += "[%" + width + "s]";
        }
        return format;
    }
}
