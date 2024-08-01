package com.example.spring_example.transaction_isolation;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.concurrent.CompletableFuture;

import javax.sql.DataSource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

import com.example.spring_example.util.DbUtil;
import com.example.spring_example.util.Util;

@RestController
public class DirtyReadController {

    @Autowired
    private DataSource dataSource;

    private final String REQUEST_URL = "/dirty-read/{isolation_level}";

    @GetMapping(REQUEST_URL)
    public void dirtyRead(@PathVariable("isolation_level") String isolationLevel) {
        try (Connection conn1 = dataSource.getConnection();
            Connection conn2 = dataSource.getConnection()) {

            int conn1IsolationLevel = Connection.TRANSACTION_READ_UNCOMMITTED;
            int conn2IsolationLevel = DbUtil.getIsolationLevel(isolationLevel);
            if (conn2IsolationLevel < 0) {
                System.out.println("Error: Wrong isolation_level: " + isolationLevel);
            }
            conn1.setTransactionIsolation(conn1IsolationLevel);
            conn2.setTransactionIsolation(conn2IsolationLevel);
    
            System.out.println("--- [ DIRTY READ ] " + isolationLevel.toUpperCase() + " ---");
            dirtyReadExample(conn1, conn2);
            // lostUpdate(conn1, conn2);
            // nonRepeatableRead(conn1, conn2);
            // phantomRead(conn1, conn2);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void dirtyReadExample(Connection conn1, Connection conn2) throws SQLException {

        conn1.setAutoCommit(false);
        conn2.setAutoCommit(false);

        CompletableFuture<Void> future1 = CompletableFuture.runAsync(() -> {
            // 트랜잭션 1 UPDATE
            try {
                Util.sleep(1000);

                Statement stmt1 = conn1.createStatement();
                stmt1.executeUpdate("UPDATE Account SET balance = 200 WHERE id = 1");
                System.out.println("conn1: UPDATE Balance: 200");
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }).thenRunAsync(() -> {
            // 트랜잭션 1 ROLLBACK
            try {
                Util.sleep(2000);

                conn1.rollback();
                System.out.println("conn1: ROLLBACK");
            } catch (SQLException e) {
                e.printStackTrace();
            }
        });

        CompletableFuture<Void> future2 = CompletableFuture.runAsync(() -> {
            // 트랜잭션 2 SELECT
            try {
                Util.sleep(2000);
                
                Statement stmt2 = conn2.createStatement();
                ResultSet rs2 = stmt2.executeQuery("SELECT balance FROM Account WHERE id = 1");
                if (rs2.next()) {
                    int balance = rs2.getInt("balance");
                    System.out.println("conn2: SELECT Balance: " + balance);
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }).thenRunAsync(() -> {
            // 트랜잭션 2 다시 SELECT
            try {
                Util.sleep(2000);

                Statement stmt2 = conn2.createStatement();
                ResultSet rs2 = stmt2.executeQuery("SELECT balance FROM Account WHERE id = 1");

                if (rs2.next()) {
                    int balance = rs2.getInt("balance");
                    System.out.println("conn2: SELECT Balance: " + balance);
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }).thenRunAsync(() -> {
            // 트랜잭션 2 COMMIT
            try {
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
