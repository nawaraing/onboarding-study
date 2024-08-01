package com.example.spring_example.util;

public class Util {
    public static void logging(String url) {
        logging(url, "start");
    }
    public static void logging(String url, String message) {
        System.out.println(String.format("[%20s][%9s] %s", Thread.currentThread().getName(), url, message));
    }

    public static void sleepWithLogging(String url, long seconds) {
        for (int i = 0; i < seconds; i++) {
            Util.logging(url, String.format("%d seconds...", i+1));
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    public static void sleep(long millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
