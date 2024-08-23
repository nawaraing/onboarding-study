package com.example.spring_example.sheetjs;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.PostMapping;

import com.example.spring_example.sheetjs.model.ServiceLink;
import java.util.List;

@Controller
public class UploadPageController {
    
    private final String UPLOAD_PAGE_URL = "/upload-page";
    private final String SEND_DATA_URL = "/send-data";

    @GetMapping(UPLOAD_PAGE_URL)
    public String uploadPage() {
        System.out.println("uploadPage");
        return "/sheetjs/uploadSheetjs";
    }

    @PostMapping(SEND_DATA_URL)
    public void sendData(@RequestBody List<ServiceLink> serviceLinkList) {
        System.out.println("sendData");
        for (ServiceLink serviceLink : serviceLinkList) {
            System.out.println("=============================");
            System.out.println("helperNm: " + serviceLink.getHelperNm());
            System.out.println("helperBirthday: " + serviceLink.getHelperBirthday());
            System.out.println("helperGenderNm: " + serviceLink.getHelperGenderNm());
            System.out.println("clientNm: " + serviceLink.getClientNm());
            System.out.println("clientBirthday: " + serviceLink.getClientBirthday());
            System.out.println("clientGenderNm: " + serviceLink.getClientGenderNm());
            System.out.println("svcStartDt: " + serviceLink.getSvcStartDt());
            System.out.println("svcEndDt: " + serviceLink.getSvcEndDt());
        }
    }
}
