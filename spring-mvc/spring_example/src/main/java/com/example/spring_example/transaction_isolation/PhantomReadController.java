package com.example.spring_example.transaction_isolation;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.concurrent.CompletableFuture;

import javax.sql.DataSource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

import com.example.spring_example.util.DbUtil;
import com.example.spring_example.util.Util;

@RestController
public class PhantomReadController {

    @Autowired
    private DataSource dataSource;

    private final String REQUEST_URL = "/phantom-read/{isolation_level}";

    @GetMapping(REQUEST_URL)
    @Transactional
    public void phantomRead(@PathVariable("isolation_level") String isolationLevel) {
        try (Connection conn1 = dataSource.getConnection();
            Connection conn2 = dataSource.getConnection()) {

            int conn1IsolationLevel = Connection.TRANSACTION_READ_UNCOMMITTED;
            int conn2IsolationLevel = DbUtil.getIsolationLevel(isolationLevel);
            if (conn2IsolationLevel < 0) {
                System.out.println("Error: Wrong isolation_level: " + isolationLevel);
            }
            conn1.setTransactionIsolation(conn1IsolationLevel);
            conn2.setTransactionIsolation(conn2IsolationLevel);
    
            System.out.println("--- [ PHANTOM READ ] " + isolationLevel.toUpperCase() + " ---");
            phantomReadExample(conn1, conn2);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void phantomReadExample(Connection conn1, Connection conn2) throws SQLException {
        conn1.setAutoCommit(false);
        conn2.setAutoCommit(false);

        CompletableFuture<Void> future1 = CompletableFuture.supplyAsync(() -> {
            // 트랜잭션 1 SELECT
            try {
                Util.sleep(1000);

                Statement stmt1 = conn1.createStatement();
                ResultSet rs1 = stmt1.executeQuery("SELECT MAX(id) AS max_id FROM Account");
                if (rs1.next()) {
                    int maxId = rs1.getInt("max_id");
                    System.out.println("conn1: SELECT maxId: " + maxId);
                    return maxId;
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
            return -1;
        }).thenAcceptAsync((maxId) -> {
            // 트랜잭션 1 INSERT
            if (maxId < 0) {
                System.out.println("Error: SELECT maxId");
            }

            try {
                Util.sleep(2000);
                
                Statement stmt1 = conn1.createStatement();
                stmt1.executeUpdate("INSERT INTO Account (id, balance) VALUES (" + ++maxId + ", 300)");
                System.out.println("conn1: INSERT id[" + maxId + "] balance[300]");
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }).thenRunAsync(() -> {
            // 트랜잭션 1 COMMIT
            try {
                // Util.sleep(2000);

                conn1.commit();
                System.out.println("conn1: COMMIT");
            } catch (SQLException e) {
                e.printStackTrace();
            }
        });
        
        CompletableFuture<Void> future2 = CompletableFuture.runAsync(() -> {
            // 트랜잭션 2 SELECT
            try {
                Util.sleep(2000);
                
                Statement stmt2 = conn2.createStatement();
                ResultSet rs2 = stmt2.executeQuery("SELECT id, balance FROM Account");
                while (rs2.next()) {
                    int id = rs2.getInt("id");
                    int balance = rs2.getInt("balance");
                    System.out.println("conn2: SELECT Id[" + id + "] Balance[" + balance + "]");
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }).thenRunAsync(() -> {
            // 트랜잭션 2 SELECT
            try {
                Util.sleep(2000);
                
                Statement stmt2 = conn2.createStatement();
                ResultSet rs2 = stmt2.executeQuery("SELECT id, balance FROM Account");
                while (rs2.next()) {
                    int id = rs2.getInt("id");
                    int balance = rs2.getInt("balance");
                    System.out.println("conn2: SELECT Id[" + id + "] Balance[" + balance + "]");
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }).thenRunAsync(() -> {
            // 트랜잭션 2 COMMIT
            try {
                Util.sleep(2000);
                
                conn2.commit();
                System.out.println("conn2: COMMIT");
            } catch (SQLException e) {
                e.printStackTrace();
            }
        });

        future1.join();
        future2.join();
        System.out.println();
    }
}
