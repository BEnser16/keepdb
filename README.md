# keepdb 
一款手刻的 key-value 資料庫 , 使用 C / Go 語言

## 環境
- gcc 17.0.0
- go 1.24.6


## 使用說明


## 架構
```
+---------------------+     cgo      +---------------------+
|     Logic Layer     | <----------> |    Storage Engine    |
|       (Go)          |              |       (C)            |
|  - CLI/HTTP API     |              |  - HashMap           |
|  - Cache (optional) |              |  - Append-only Log   |
|  - Request handling |              |  - File I/O          |
+---------------------+              +---------------------+

```


