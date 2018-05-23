#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include "mcache.h"

#define MEMSIZE 32 * 1024 * 1024

#define KEY_NUM 100


int bench_test(void)
{
	int  i;
	int  res;
	char key[64];
	value_t value;

	static const char *key_pre = "key_";

	char         err_buf[1024];

	fprintf(stderr, "Bench Test Start...\n");

	mcache_kv_t *kvs;

	kvs = mcache_kv_init(MEMSIZE, err_buf, sizeof(err_buf));
	if (kvs == NULL) {
		fprintf(stderr, "mcache_kv_init failed: %s\n", err_buf);
		exit(-1);
	}

	struct block_map_entry tmp;
    sprintf(tmp.fingerprint, "abc");

	for (i = 0; i < KEY_NUM; ++i) {
		sprintf(key, "%s%d", key_pre, i);

		tmp.nbd_offset = i * 1000;
		tmp.length = i * 10;
		res = mcache_kv_set(kvs, key, tmp);
		if (res) {
			fprintf(stderr, "mcache_kv_set failed: %s\n", mcache_estr(res));
			exit(-1);
		}

		assert(mcache_kv_count(kvs) == i + 1);
	}

	for (i = 0; i < KEY_NUM; ++i) {
		sprintf(key, "%s%d", key_pre, i);

		res = mcache_kv_get(kvs, key, &value);
		fprintf(stdout, "key - %s value - offset: %lu length: %lu\n", key, value.nbd_offset, value.length);
		if (res) {
			fprintf(stderr, "mcache_kv_get failed: %s\n", mcache_estr(res));
			exit(-1);
		}
	
//		assert(value == i);
	}

	for (i = 0; i < KEY_NUM; ++i) {
		sprintf(key, "%s%d", key_pre, i);

		res = mcache_kv_delete(kvs, key);
		if (res) {
			fprintf(stderr, "mcache_kv_delete failed: %s\n", mcache_estr(res));
			exit(-1);
		}

		assert(mcache_kv_count(kvs) == KEY_NUM - i - 1);
	}

	res = mcache_kv_free(kvs);
	if (res) {
		fprintf(stderr, "mcache_kv_free failed: %s\n", mcache_estr(res));
		exit(-1);
	}

	fprintf(stderr, "Bench Test Passed!\n");

	return 0;
}

typedef struct {
	mcache_kv_t  *kvs;

	int 		  start;
	int 		  end;
} task_t;



int main(int argc, char const *argv[])
{
	// funtional_test();

	bench_test();

	// multi_thread_test();

	return 0;
}
