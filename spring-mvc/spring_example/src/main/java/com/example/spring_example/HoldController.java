package com.example.spring_example;

import org.springframework.web.context.request.async.DeferredResult;

import com.example.spring_example.util.Util;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import java.util.concurrent.*;

@RestController
public class HoldController {

    private final String REQUEST_URL = "/hold";

    @GetMapping(REQUEST_URL)
    public String handleAsync() {
        Util.logging(REQUEST_URL);

        long seconds = 10;
        String message = String.format("hold on %d seconds!", seconds);

        Util.logging(REQUEST_URL, message);
        Util.sleepWithLogging(REQUEST_URL, seconds);

        Util.logging(REQUEST_URL, "HoldController 반환");
        return message;
    }
}
