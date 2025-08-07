// import libraries
#include "kv_store.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義最大存儲容量 最大的 char 陣列大小
#define MAX_STORE 1024

// 定義 key-value 結構
typedef struct {
    char* key;
    char* value;
} KVPair;

// 全域變數，存儲 key-value 對
static KVPair store[MAX_STORE];
static int store_count = 0;

// 檔案指標，用於讀寫資料庫檔案
static FILE* db_file = NULL;

// 初始化
int kv_init(const char* filename) {
    // 開啟檔案 a - append mode 會建立檔案 已經存在則接著寫入 , + 讀寫權限
    db_file = fopen(filename, "a+");
    if (!db_file) return -1;

    // rewind 檔案指標，從頭讀
    rewind(db_file);

    char line[1024];
    while (fgets(line, sizeof(line), db_file)) {
        // 去掉換行符
        line[strcspn(line, "\n")] = 0;

        // 找 =
        char* sep = strchr(line, '=');
        if (!sep) continue;

        *sep = '\0';
        char* key = line;
        char* value = sep + 1;

        if (strlen(value) == 0) {
            kv_delete(key);  // 刪除操作
        } else {
            kv_set(key, value);  // 一般 set 操作
        }
    }

    return 0;
}

void kv_close() {
    if (db_file) fclose(db_file);

    // 釋放所有 KVPair 已分配的記憶體
    for (int i = 0; i < store_count; i++) {
        free(store[i].key);
        free(store[i].value);
    }
    store_count = 0;
}

int kv_set(const char* key, const char* value) {
    // 先檢查是否已存在，更新值
    for (int i = 0; i < store_count; i++) {
        // string compare 如果找到key
        if (strcmp(store[i].key, key) == 0) {
            // 先釋放 value
            free(store[i].value);
            // 把 value 複製進去
            store[i].value = strdup(value);
            return 0;
        }
    }

    // 新增
    if (store_count >= MAX_STORE) return -1; // 容量滿

    // 沒找到key 就儲存新的 key-value
    store[store_count].key = strdup(key);
    store[store_count].value = strdup(value);
    store_count++;

    // 寫入檔案 (append)
    if (db_file) {
        fprintf(db_file, "%s=%s\n", key, value);
        // 直接寫入磁碟
        fflush(db_file);
    }

    return 0;
}

char* kv_get(const char* key) {
    for (int i = 0; i < store_count; i++) {
        if (strcmp(store[i].key, key) == 0) {
            return strdup(store[i].value);
        }
    }
    return NULL;
}

int kv_delete(const char* key) {
    for (int i = 0; i < store_count; i++) {
        if (strcmp(store[i].key, key) == 0) {
            free(store[i].key);
            free(store[i].value);
            // 往前移動補洞
            for (int j = i; j < store_count -1; j++) {
                store[j] = store[j+1];
            }
            store_count--;

            // 寫入刪除標記
            if (db_file) {
                fprintf(db_file, "%s=\n", key); // 空值代表刪除
                fflush(db_file);
            }
            return 0;
        }
    }
    return -1; // 找不到
}
