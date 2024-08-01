package com.example.spring_example;

import org.springframework.web.context.request.async.DeferredResult;

import com.example.spring_example.util.Util;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import java.util.concurrent.*;

@RestController
public class DeferredExampleController {

    private final String REQUEST_URL = "/deferred-example";

    @GetMapping(REQUEST_URL)
    public DeferredResult<String> handleAsync() {
        Util.logging(REQUEST_URL);

        int corePoolSize = 3;
        int maximumPoolSize = 3;
        long keepAliveTime = 0L;
        TimeUnit unit = TimeUnit.MILLISECONDS;
        BlockingQueue<Runnable> workQueue = new ArrayBlockingQueue<>(10); // 큐 용량 10

        ExecutorService executor = new ThreadPoolExecutor(
            corePoolSize,
            maximumPoolSize,
            keepAliveTime,
            unit,
            workQueue
        );

        DeferredResult<String> deferredResult = new DeferredResult<>();

        // 비동기 작업
        executor.submit(() -> {
            long seconds = 5;
            String resultMessage = String.format("%d초 만에 비동기 작업 완료", seconds);

            Util.sleepWithLogging(REQUEST_URL, seconds);
            Util.logging(REQUEST_URL, resultMessage);
            deferredResult.setResult(resultMessage);
        });

        return deferredResult;
    }
}
