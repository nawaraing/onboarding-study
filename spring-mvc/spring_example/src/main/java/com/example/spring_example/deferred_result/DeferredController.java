package com.example.spring_example.deferred_result;

import org.springframework.web.context.request.async.DeferredResult;

import com.example.spring_example.util.Util;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import java.util.concurrent.*;

@RestController
public class DeferredController {

    private final String REQUEST_URL = "/deferred";
    private static final ExecutorService executor = new ThreadPoolExecutor(
        3,
        3,
        0L,
        TimeUnit.MILLISECONDS,
        new ArrayBlockingQueue<>(10) // 큐 용량 10
    );

    @GetMapping(REQUEST_URL)
    public DeferredResult<String> handleAsync() {
        Util.logging(REQUEST_URL);
        DeferredResult<String> deferredResult = new DeferredResult<>();

        // 비동기 작업
        executor.submit(() -> {
            long seconds = 5;
            String resultMessage = String.format("%d초 만에 비동기 작업 완료", seconds);

            Util.sleepWithLogging(REQUEST_URL, seconds);
            Util.logging(REQUEST_URL, resultMessage);
            deferredResult.setResult(resultMessage);
        });

        Util.logging(REQUEST_URL, "DeferredController 반환");
        return deferredResult;
    }
}
