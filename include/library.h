#ifndef WU_YAOQIN_LIBRARY_H
#define WU_YAOQIN_LIBRARY_H

#include "public.h"
char *get_dataset_name(int num);

dataset *get_dataset();

keyword_head *get_keyword_from_str(char *str, mode);

keyword_head *get_query_chain_mode_script(char *path);

keyword_head *get_query_chain_mode_manual(int argc, char *argv[]);

result_dataset *query_single_dataset_single_key_chain(dataset *data, keyword_head *key);

result_chain *query_all_dataset_single_key_chain(dataset *data, keyword_head *key);

result_chain *query_all_dataset_all_key(keyword_head* key);

void core_search(result_chain *data);

void exact_search(result_chain *data);

void top_search(result_chain *data);

void top_k_search(result_chain *data);

bool is_dataset_content_start(char *flag);

praser_result *prase(int argc, char *argv[]);

//memory manager

//test function
void test_get_dataset();

void test_get_query_chain_mode_script();

void test_get_query_chain_mode_manual(int argc, char *argv[]);

void test_query_all_dataset_all_key();

void test_core_search();

void test_exact_search();

void test_top_search();

void test_top_k_search();


#endif