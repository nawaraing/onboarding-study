package com.example.spring_example.sheetjs;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;


@Controller
public class UploadPageController {
    
    private final String REQUEST_URL = "/upload_page";

    @GetMapping(REQUEST_URL)
    public String uploadPage() {
        System.out.println("uploadPage");
        return "/sheetjs/uploadSheetjs";
    }
}
