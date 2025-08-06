#include "kv_store.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    if (kv_init("keepdb.data") != 0) {
        printf("Failed to open db file\n");
        return 1;
    }

    kv_set("name", "ryan");
    kv_set("age", "25");

    char* val = kv_get("name");
    if (val) {
        printf("name=%s\n", val);
        free(val);
    }

    kv_delete("age");

    kv_close();
    return 0;
}
