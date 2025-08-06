#ifndef KV_STORE_H
#define KV_STORE_H

// 初始化，讀檔回復
int kv_init(const char* filename);

// 關閉，釋放資源
void kv_close();

// 設定 key-value
int kv_set(const char* key, const char* value);

// 取得 key 對應的 value
// 返回字串為 malloc 出來，需要 caller free
char* kv_get(const char* key);

// 刪除 key
int kv_delete(const char* key);

#endif
