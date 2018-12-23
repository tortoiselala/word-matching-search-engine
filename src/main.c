#include "../include/library.h"
#include "../include/public.h"

int main(int argc, char *argv[]) {
//    test_get_dataset();
//    test_get_query_chain_mode_script();
//    test_get_query_chain_mode_manual(argc, argv);
//    test_query_all_dataset_all_key();
//    test_core_search();
//    test_exact_search();
//    test_top_k_search();
//    test_top_search();
    praser_result *result = prase(argc, argv);
    if(result->key_head == NULL){
        printf("exit the query due to the last error....\n");
        return -1;
    }
    result_chain *next_result = query_all_dataset_all_key(result->key_head);

    if(result->inquiry_method == coreSearch){
        core_search(next_result);
    }else  if(result->inquiry_method == exactSearch){
        exact_search(next_result);
    }
    else  if(result->inquiry_method == topSearch){
        top_search(next_result);
    }
    else  if(result->inquiry_method == topKSearch){
        top_k_search(next_result);
    }
}