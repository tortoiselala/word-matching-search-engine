//
// Created by tortoiselala on 12/22/18.
//

#ifndef CLION_WORKPLACE_PUBLIC_H
#include <stdio.h>
#include <stdbool.h>

#define manual true
#define script false

#define datasetname_length 8

#define datasetname_base 1
#define datasetname_max 510

#define tmp_buffer_length 20

//inquiry mode
typedef bool mode;

/*
 * keyword Unified interface for all inquiry set
 *      keyword_value    :  the value of key
 *      next_keyword     :  next keyword
 *
 */
typedef struct keyword_chain_elem{
    char *keyword_value;
    struct keyword_chain_elem *next_keyword;
}keyword_elem;
/*
 * keywords Unified interface for inquiry set head
 *      mode           :  inquiry mode
 *      first_keyword  :
 *      inquiry_name   :  such as : "q1"
 *      next_keyword   :  next keyword
 */
typedef struct keyword_chain_head{
    mode inquiry_mode;
    keyword_elem *first_keyword;
    char inquiry_name[8];
    struct keyword_chain_head *next_keywords;
}keyword_head;

/*
 *  dataset the dataset chain node
 *      word
 *
 */
typedef struct dataset_chain_elem{
    char filename[8];
    keyword_elem *first_key;
    struct dataset_chain_elem *next;
}dataset;


//the next lines define the datatype for result
//result datatype is the linked chain
//

typedef struct result_key_node{
    int count;
    char *value;
    struct result_key_node *next;
}result_key;

typedef struct result_dataset_node{
    int count;
    result_key *first_key;
    char filename[8];
    struct result_dataset_node *next;
}result_dataset;

typedef struct result_chain_node{
    mode m;
    char *query_name;
    result_dataset *first_data_result;
    struct result_chain_node *next;
}result_chain;

typedef struct _top_search_result_node{
    result_dataset *s;
    struct _top_search_result_node *next;
}top_search_result_node;

#define CLION_WORKPLACE_PUBLIC_H

#endif //CLION_WORKPLACE_PUBLIC_H
