package com.example.spring_example.util;

import java.sql.Connection;

public class DbUtil {
    public static int getIsolationLevel(String isolationLevel) {
        isolationLevel = isolationLevel.toUpperCase();

        switch (isolationLevel) {
            case "READ-UNCOMMITTED": return Connection.TRANSACTION_READ_UNCOMMITTED;
            case "READ-COMMITTED": return Connection.TRANSACTION_READ_COMMITTED;
            case "REPEATABLE-READ": return Connection.TRANSACTION_REPEATABLE_READ;
            case "SERIALIZABLE": return Connection.TRANSACTION_SERIALIZABLE;
            default: return -1;
        }
    }
}
