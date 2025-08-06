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
    db_file = fopen(filename, "a+");
    if (!db_file) return -1;

    // TODO: 讀取檔案並恢復資料，這裡先不做

    return 0;
}

void kv_close() {
    if (db_file) fclose(db_file);

    for (int i = 0; i < store_count; i++) {
        free(store[i].key);
        free(store[i].value);
    }
    store_count = 0;
}

int kv_set(const char* key, const char* value) {
    // 先檢查是否已存在，更新值
    for (int i = 0; i < store_count; i++) {
        if (strcmp(store[i].key, key) == 0) {
            free(store[i].value);
            store[i].value = strdup(value);
            return 0;
        }
    }

    // 新增
    if (store_count >= MAX_STORE) return -1; // 容量滿

    store[store_count].key = strdup(key);
    store[store_count].value = strdup(value);
    store_count++;

    // 寫入檔案 (append)
    if (db_file) {
        fprintf(db_file, "%s=%s\n", key, value);
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
