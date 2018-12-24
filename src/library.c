#include "../include/library.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


char *get_dataset_name(int num) {
    //filename length define by filename_length, default value is 8, ex: "001.txt"
    char *filename = (char *) malloc(sizeof(char) * datasetname_length);
    memset(filename, '0', datasetname_length);
    int i = 2;
    filename[datasetname_length - 5] = '.';
    filename[datasetname_length - 4] = 't';
    filename[datasetname_length - 3] = 'x';
    filename[datasetname_length - 2] = 't';
    filename[datasetname_length - 1] = '\0';
    while (num > 0) {
        filename[i--] = num % 10 + '0';
        num /= 10;
    }
    return filename;
}

dataset *get_dataset() {
    dataset *result_dataset_head = NULL, *result_dataset_tail = NULL;

    bool is_result_dataset_head_first_node = true;

    for (int i = datasetname_base; i <= datasetname_max; ++i) {
        char *datasetname = get_dataset_name(i);
        if (is_result_dataset_head_first_node) {
            result_dataset_head = (dataset *) malloc(sizeof(dataset));
            result_dataset_tail = result_dataset_head;
            result_dataset_tail->next = NULL;
            strcpy(result_dataset_tail->filename, datasetname);
            //get single dataset content
            FILE *file_pointer = fopen(datasetname, "r");
            if (file_pointer == NULL) {
                result_dataset_tail->first_key = NULL;
            } else {
                char buffer_for_dataset_content[tmp_buffer_length * 100];
                memset(buffer_for_dataset_content, '\0', tmp_buffer_length * 100);
                keyword_elem *dataset_content_key_head = NULL, *dataset_content_key_tail = NULL;
                bool is_dataset_content_start_val = false;
                bool is_dataset_content_first_node = true;
                while (fgets(buffer_for_dataset_content, tmp_buffer_length * 100, file_pointer) != NULL) {
                    if (!is_dataset_content_start_val && is_dataset_content_start(buffer_for_dataset_content)) {
                        is_dataset_content_start_val = true;
                    } else if (is_dataset_content_start_val) {
                        if (is_dataset_content_first_node) {
                            keyword_head *tmp_keyword_head = get_keyword_from_str(buffer_for_dataset_content, manual);
                            dataset_content_key_head = tmp_keyword_head->first_keyword;
                            free(tmp_keyword_head);
                            is_dataset_content_first_node = false;
                        } else {
                            keyword_elem *tmp_keyword_elem = dataset_content_key_head;
                            while (tmp_keyword_elem != NULL && tmp_keyword_elem->next_keyword != NULL) {
                                tmp_keyword_elem = tmp_keyword_elem->next_keyword;
                            }
                            keyword_head *tmp_keyword_head = get_keyword_from_str(buffer_for_dataset_content, manual);
                            tmp_keyword_elem->next_keyword = tmp_keyword_head->first_keyword;
                            free(tmp_keyword_head);
                        }
                    }
                }
                result_dataset_tail->first_key = dataset_content_key_head;
                fclose(file_pointer);
            }
            //end get single dataset content
            is_result_dataset_head_first_node = false;
        } else {
            result_dataset_tail->next = (dataset *) malloc(sizeof(dataset));

            result_dataset_tail->next->next = NULL;
            memset(result_dataset_tail->next->filename, '\0', 8);
            strcpy(result_dataset_tail->next->filename, datasetname);
            //load file content
            FILE *file_pointer = fopen(datasetname, "r");
            if (file_pointer == NULL) {
                result_dataset_tail->next->first_key = NULL;
            } else {
                char buffer_for_dataset_content[tmp_buffer_length * 100];
                memset(buffer_for_dataset_content, '\0', tmp_buffer_length * 100);
                keyword_elem *dataset_content_key_head = NULL, *dataset_content_key_tail = NULL;
                bool is_dataset_content_start_val = false;
                bool is_dataset_content_first_node = true;
                while (fgets(buffer_for_dataset_content, tmp_buffer_length * 100, file_pointer) != NULL) {
                    if (!is_dataset_content_start_val && is_dataset_content_start(buffer_for_dataset_content)) {
                        is_dataset_content_start_val = true;
                    } else if (is_dataset_content_start_val) {
                        if (is_dataset_content_first_node) {
                            keyword_head *tmp_keyword_head = get_keyword_from_str(buffer_for_dataset_content, manual);
                            dataset_content_key_head = tmp_keyword_head->first_keyword;
                            free(tmp_keyword_head);
                            is_dataset_content_first_node = false;
                        } else {
                            keyword_elem *tmp_keyword_elem = dataset_content_key_head;
                            while (tmp_keyword_elem != NULL && tmp_keyword_elem->next_keyword != NULL) {
                                tmp_keyword_elem = tmp_keyword_elem->next_keyword;
                            }
                            keyword_head *tmp_keyword_head = get_keyword_from_str(buffer_for_dataset_content, manual);
                            tmp_keyword_elem->next_keyword = tmp_keyword_head->first_keyword;
                            free(tmp_keyword_head);
                        }
                    }
                }
                result_dataset_tail->next->first_key = dataset_content_key_head;
            }
            //end load file content

            result_dataset_tail = result_dataset_tail->next;

        }
    }
    return result_dataset_head;

}

keyword_head *get_keyword_from_str(char *str, mode m) {
    keyword_head *result_keyword_head = (keyword_head *) malloc(sizeof(keyword_head));
    result_keyword_head->inquiry_mode = m;
    result_keyword_head->inquiry_name[0] = '\0';
    result_keyword_head->next_keywords = NULL;
    result_keyword_head->first_keyword = NULL;

    if (str == NULL || !strcmp(str, "")) {
        return result_keyword_head;
    } else {
        int find_space = 0;
        int last_space = 0;

        if (m == script) {
            while (*(str + find_space) != '\0' && *(str + find_space) != '\r' && *(str + find_space) != '\n') {
                if (*(str + find_space) == ' ') {
                    break;
                } else {
                    ++find_space;
                }
            }
            strncpy(result_keyword_head->inquiry_name, str, find_space);
            result_keyword_head->inquiry_name[find_space] = '\0';
            last_space = ++find_space;
        }

        keyword_elem *result_keyword_elem_tail = NULL;
        bool is_first_node = true;
        while (*(str + find_space) != '\0' && *(str + find_space) != '\n' && *(str + find_space) != '\r') {
            if (*(str + find_space) == ' ') {
                if (is_first_node) {
                    result_keyword_head->first_keyword = (keyword_elem *) malloc(sizeof(keyword_elem));
                    result_keyword_elem_tail = result_keyword_head->first_keyword;
                    result_keyword_elem_tail->keyword_value = (char *) malloc(
                            sizeof(char) * (find_space - last_space + 1));
                    memset(result_keyword_elem_tail->keyword_value, '\0', (find_space - last_space + 1));
                    strncpy(result_keyword_elem_tail->keyword_value, (str + last_space), (find_space - last_space));
                    result_keyword_elem_tail->next_keyword = NULL;

                    is_first_node = false;
                    last_space = ++find_space;
                } else {
                    result_keyword_elem_tail->next_keyword = (keyword_elem *) malloc(sizeof(keyword_elem));
                    result_keyword_elem_tail->next_keyword->keyword_value = (char *) malloc(
                            sizeof(char) * (find_space - last_space + 1));
                    memset(result_keyword_elem_tail->next_keyword->keyword_value, '\0', (find_space - last_space + 1));
                    strncpy(result_keyword_elem_tail->next_keyword->keyword_value, (str + last_space),
                            (find_space - last_space));
                    result_keyword_elem_tail->next_keyword->next_keyword = NULL;

                    result_keyword_elem_tail = result_keyword_elem_tail->next_keyword;
                    last_space = ++find_space;
                }
            } else {
                ++find_space;
            }
        }

        if (last_space != find_space) {
            if (is_first_node) {
                result_keyword_head->first_keyword = (keyword_elem *) malloc(sizeof(keyword_elem));
                result_keyword_elem_tail = result_keyword_head->first_keyword;
                result_keyword_elem_tail->keyword_value = (char *) malloc(sizeof(char) * (find_space - last_space + 1));
                memset(result_keyword_elem_tail->keyword_value, '\0', (find_space - last_space + 1));
                strncpy(result_keyword_elem_tail->keyword_value, (str + last_space), (find_space - last_space));
                result_keyword_elem_tail->next_keyword = NULL;

                is_first_node = false;
            } else {
                result_keyword_elem_tail->next_keyword = (keyword_elem *) malloc(sizeof(keyword_elem));
                result_keyword_elem_tail->next_keyword->keyword_value = (char *) malloc(
                        sizeof(char) * (find_space - last_space + 1));
                memset(result_keyword_elem_tail->next_keyword->keyword_value, '\0', (find_space - last_space + 1));
                strncpy(result_keyword_elem_tail->next_keyword->keyword_value, (str + last_space),
                        (find_space - last_space));
                result_keyword_elem_tail->next_keyword->next_keyword = NULL;
            }
        }
    }
    return result_keyword_head;

}

keyword_head *get_query_chain_mode_script(char *path) {
    FILE *file_pointer = fopen(path, "r");
    if (file_pointer == NULL) {
        return NULL;
    }

    keyword_head *result_keyword_head = NULL, *result_keyword_tail = NULL;
    bool is_result_keyword_chain_first = true;
    char buf[tmp_buffer_length * 100];
    memset(buf, '\0', tmp_buffer_length * 100);
    while (fgets(buf, tmp_buffer_length * 100, file_pointer) != NULL) {
        if (is_result_keyword_chain_first) {
            result_keyword_head = get_keyword_from_str(buf, script);
            result_keyword_tail = result_keyword_head;
            is_result_keyword_chain_first = false;
        } else {
            result_keyword_tail->next_keywords = get_keyword_from_str(buf, script);
            result_keyword_tail = result_keyword_tail->next_keywords;
        }
    }
    fclose(file_pointer);
    return result_keyword_head;
}

keyword_head *get_query_chain_mode_manual(int argc, char *argv[]) {
    char result_buffer[tmp_buffer_length * 100];
    memset(result_buffer, '\0', tmp_buffer_length * 100);
    int result_buffer_content_length = 0;
    for (int i = 0; i < argc; ++i) {
        strcat(result_buffer, argv[i]);
        result_buffer_content_length += strlen(argv[i]);
        if (i != argc - 1) {
            result_buffer[result_buffer_content_length++] = ' ';
            result_buffer[result_buffer_content_length + 1] = '\0';
        }
    }
    char *result = NULL;
    if (result_buffer_content_length > 0) {
        result = (char *) malloc(sizeof(char) * (result_buffer_content_length + 1));
        memset(result, '\0', (result_buffer_content_length + 1));
        strncpy(result, result_buffer, result_buffer_content_length);
        return get_keyword_from_str(result, manual);
    } else {
        return NULL;
    }
}

bool is_dataset_content_start(char *string) {
    char *flag = "<<CONTENT>>";
    if (string == NULL || !strcmp(string, "")) {
        return false;
    }
    for (int i = 0; *(string + i) != '\0'; ++i) {
        if (*(string + i) == *flag && strlen(string + i) >= strlen(flag)) {
            int k = i, j = 0;
            for (; *(string + k) != '\0' && *(flag + j); ++k, ++j) {
                if (*(string + k) != *(flag + j)) {
                    break;
                }
            }
            return true;
        }
    }
    return NULL;

}

result_dataset *query_single_dataset_single_key_chain(dataset *data, keyword_head *key) {
    keyword_elem *tmp_data = data->first_key;
    keyword_elem *tmp_key = key->first_keyword;

    result_dataset *result_head = (result_dataset *) malloc(sizeof(result_dataset));
    result_head->first_key = NULL;
    result_head->next = NULL;
    result_head->count = 0;
    strcpy(result_head->filename, data->filename);
    //create key linked list
    result_key *result_key_tail = NULL;
    bool is_first_key = true;
    while (tmp_key != NULL) {
        if (is_first_key) {
            result_head->first_key = (result_key *) malloc(sizeof(result_key));
            result_key_tail = result_head->first_key;

            result_key_tail->count = 0;
            result_key_tail->next = NULL;
            result_key_tail->value = (char *) malloc(sizeof(char) * (strlen(tmp_key->keyword_value) + 1));
            strcpy(result_key_tail->value, tmp_key->keyword_value);
            is_first_key = false;
        } else {
            result_key_tail->next = (result_key *) malloc(sizeof(result_key));
            result_key_tail->next->count = 0;
            result_key_tail->next->next = NULL;
            result_key_tail->next->value = (char *) malloc(sizeof(char) * (strlen(tmp_key->keyword_value) + 1));
            strcpy(result_key_tail->next->value, tmp_key->keyword_value);
            result_key_tail = result_key_tail->next;
        }
        tmp_key = tmp_key->next_keyword;
    }
    //search

    while (tmp_data != NULL) {
        result_key_tail = result_head->first_key;
        while (result_key_tail != NULL) {
            if (!strcmp(result_key_tail->value, tmp_data->keyword_value)) {
                ++result_key_tail->count;
                ++result_head->count;
            }
            result_key_tail = result_key_tail->next;
        }
        tmp_data = tmp_data->next_keyword;
    }

    //if total count == 0  free all space
    if (result_head->count == 0) {
        result_key_tail = result_head->first_key;
        while (result_key_tail != NULL) {
            result_key *tmp = result_key_tail;
            result_key_tail = result_key_tail->next;
            free(tmp->value);
            free(tmp);
        }
        free(result_head);
        return NULL;
    } else {
        return result_head;
    }
}

result_chain *query_all_dataset_single_key_chain(dataset *data, keyword_head *key) {
    dataset *tmp_data = data;
    keyword_head *tmp_key = key;

    result_chain *result_head = (result_chain *) malloc(sizeof(result_chain));
    result_head->next = NULL;
    result_head->m = tmp_key->inquiry_mode;
    if (tmp_key->inquiry_mode == script) {
        result_head->query_name = (char *) malloc(sizeof(char) * (strlen(tmp_key->inquiry_name) + 1));
        memset(result_head->query_name, '\0', strlen(tmp_key->inquiry_name) + 1);
        strcpy(result_head->query_name, tmp_key->inquiry_name);
    } else {
        result_head->query_name = NULL;
    }

    result_head->first_data_result = NULL;

    bool is_first_result_head_data_result = true;

    while (tmp_data != NULL) {
        result_dataset *inquiry_result = query_single_dataset_single_key_chain(tmp_data, tmp_key);
        if (is_first_result_head_data_result && inquiry_result != NULL) {
            result_head->first_data_result = inquiry_result;
            is_first_result_head_data_result = false;
        } else if (inquiry_result != NULL) {
            result_dataset *file_node_it = result_head->first_data_result;
            while (file_node_it != NULL && file_node_it->next != NULL) {
                file_node_it = file_node_it->next;
            }
            file_node_it->next = inquiry_result;
        }
        tmp_data = tmp_data->next;
    }
    if (result_head->first_data_result == NULL) {
        free(result_head->query_name);
        free(result_head);
        return NULL;
    }
    return result_head;
}

result_chain *query_all_dataset_all_key(keyword_head *key) {
    dataset *dataset_head = get_dataset();
    keyword_head *tmp_key_it = key;

    result_chain *result_head = NULL, *result_tail = NULL;
    bool is_result_head_node_first = true;

    while (tmp_key_it != NULL) {
        result_chain *tmp_result = query_all_dataset_single_key_chain(dataset_head, tmp_key_it);
        if (tmp_result != NULL) {
            if (is_result_head_node_first) {
                result_head = tmp_result;
                result_tail = result_head;
                result_tail->next = NULL;
                is_result_head_node_first = false;
            } else {
                result_tail->next = tmp_result;
                result_tail = result_tail->next;
            }
        }
        tmp_key_it = tmp_key_it->next_keywords;
    }
    return result_head;
}

void core_search(mode m, result_chain *result) {
    result_chain *tmp_result = result;
    while (tmp_result != NULL) {
        result_dataset *single_inquiry_result_it = tmp_result->first_data_result;
        while (single_inquiry_result_it != NULL) {
            if(m == manual){
                printf("%s\n", single_inquiry_result_it->filename);
            }else{
                printf("%s %s\n", tmp_result->query_name, single_inquiry_result_it->filename);
            }

            single_inquiry_result_it = single_inquiry_result_it->next;
        }
        tmp_result = tmp_result->next;
    }
}

void exact_search(mode m, result_chain *result) {
    result_chain *tmp_result = result;
    while (tmp_result != NULL) {
        result_dataset *single_inquiry_result_it = tmp_result->first_data_result;
        while (single_inquiry_result_it != NULL) {
            bool is_all_exits = true;
            result_key *key = single_inquiry_result_it->first_key;
            while (key != NULL) {
                if (key->count <= 0) {
                    is_all_exits = false;
                    break;
                }
                key = key->next;
            }
            if (is_all_exits) {
                if(m == manual){
                    printf("%s\n", single_inquiry_result_it->filename);
                }else{
                    printf("%s %s\n", tmp_result->query_name, single_inquiry_result_it->filename);
                }
            }
            single_inquiry_result_it = single_inquiry_result_it->next;
        }
        tmp_result = tmp_result->next;
    }
}

void top_search(mode m, result_chain *result) {
    result_chain *tmp_result = result;

    //a chain to store
    //to lazy fix later
    while (tmp_result != NULL) {
        result_dataset *single_inquiry_result_it = tmp_result->first_data_result;
        int max_count = 0;
        top_search_result_node *result_head = NULL;
        top_search_result_node *result_tail = NULL;
        bool is_first = true;
        while (single_inquiry_result_it != NULL) {
            if (single_inquiry_result_it->count > max_count) {
                max_count = single_inquiry_result_it->count;
                result_tail = result_head;
                while (result_tail != NULL) {
                    top_search_result_node *tmp = result_tail;
                    result_tail = result_tail->next;
                    free(tmp);
                }
                result_head = (top_search_result_node *) malloc(sizeof(top_search_result_node));
                result_head->next = NULL;
                result_tail = result_head;
                result_tail->s = single_inquiry_result_it;
                is_first = false;
            } else if (single_inquiry_result_it->count == max_count) {
                if (is_first) {
                    result_head = (top_search_result_node *) malloc(sizeof(top_search_result_node));
                    result_head->next = NULL;
                    result_tail = result_head;
                    result_tail->s = single_inquiry_result_it;
                    is_first = false;
                } else {
                    result_tail->next = (top_search_result_node *) malloc(sizeof(top_search_result_node));
                    result_tail->next->next = NULL;
                    result_tail->next->s = single_inquiry_result_it;
                    result_tail = result_tail->next;
                }
            }
            single_inquiry_result_it = single_inquiry_result_it->next;
        }
        result_tail = result_head;
        while (result_tail != NULL) {
            if(m == manual){
                printf("%s\n", result_tail->s->filename);
            }else{
                printf("%s %s\n", tmp_result->query_name, result_tail->s->filename);
            }
            top_search_result_node *tmp = result_tail;
            result_tail = result_tail->next;
            free(tmp);
        }
        tmp_result = tmp_result->next;
    }
}

void top_k_search(mode m, result_chain *result) {
    result_chain *tmp_result = result;
    while (tmp_result != NULL) {
        //result_1 is max
        result_dataset *result_1 = NULL;
        int count_1 = 0;
        result_dataset *result_2 = NULL;
        int count_2 = 0;
        result_dataset *result_3 = NULL;
        int count_3 = 0;

        result_dataset *single_inquiry_result_it = tmp_result->first_data_result;
        while (single_inquiry_result_it != NULL) {
            result_dataset *tmp_result = single_inquiry_result_it;
            if (tmp_result != NULL && tmp_result->count > count_1) {
                result_dataset *tmp = tmp_result;

                tmp_result = result_1;
                result_1 = tmp;
                count_1 = result_1->count;
            }
            if (tmp_result != NULL && tmp_result->count > count_2) {
                result_dataset *tmp = tmp_result;
                tmp_result = result_2;
                result_2 = tmp;
                count_2 = result_2->count;
            }
            if (tmp_result != NULL && tmp_result->count > count_3) {
                result_dataset *tmp = tmp_result;
                tmp_result = result_3;
                result_3 = tmp;
                count_3 = result_3->count;
            }
            single_inquiry_result_it = single_inquiry_result_it->next;
        }

        if (result_1 != NULL && count_1 > 0) {
            if(m == manual){
                printf("%s\n", result_1->filename);
            }else{
                printf("%s %s\n", tmp_result->query_name, result_1->filename);
            }
        }
        if (result_2 != NULL && count_2 > 0) {
            if(m == manual){
                printf("%s\n", result_1->filename);
            }else{
                printf("%s %s\n", tmp_result->query_name, result_2->filename);
            }
        }
        if (result_3 != NULL && count_3 > 0) {
            if(m == manual){
                printf("%s\n", result_1->filename);
            }else{
                printf("%s %s\n", tmp_result->query_name, result_3->filename);
            }
        }
        tmp_result = tmp_result->next;
    }
}

praser_result *prase(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("[error] require more arg...\n");
        return NULL;
    }
    praser_result *result = (praser_result *) malloc(sizeof(praser_result));
    result->inquiry_mode = manual;
    result->inquiry_method = coreSearch;
    result->key_head = NULL;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i] + 1, mode_key[0])) {
                result->inquiry_mode = manual;
            } else if (!strcmp(argv[i] + 1, mode_key[1])) {
                result->inquiry_mode = script;
            } else {
                bool tag = true;
                for (int j = 0; j < inquiry_method_num; ++j) {
                    if (!strcmp(argv[i] + 1, method_key[j])) {
                        result->inquiry_method = j;
                        tag = false;
                        break;
                    }
                }
                if (tag) {
                    printf("wrong command!\n");
                }
            }
        } else {
            if (result->inquiry_mode == script) {
                result->key_head = get_query_chain_mode_script(argv[i]);
                break;
            } else {

                result->key_head = get_query_chain_mode_manual(argc - i, argv + i);
                if (result->key_head == NULL) {
                    printf("can't find %s\n", argv[i]);
                }
                break;
            }
        }
    }

    if(result->key_head == NULL){
        printf("[error] command parser false, check your input\n");
        free(result);
        return NULL;
    }
    return result;
}

void usage(){
    printf("Usage:\n");
    printf("   1.query mode:\n");
    printf("         -manual             :  \n");
    printf("         -script             :  \n");
    printf("   2.query method:\n");
    printf("         -coreSearch         :   \n");
    printf("         -exactSearch        :   \n");
    printf("         -topSearch          :   \n");
    printf("         -topKSearch         :   \n");
    printf("   3.query keys or file name\n");
    printf("\n\n");
    printf("         example: -manual -coreSearch school station  \n");
    printf("         example: -script -topSearch input.txt  \n");

}

void test_get_dataset() {
    dataset *dataset_head = get_dataset();
    while (dataset_head != NULL) {
        printf("\n...file name : %s\n", dataset_head->filename);
        keyword_elem *tmp = dataset_head->first_key;
        while (tmp != NULL) {
            printf(" %s", tmp->keyword_value);
            tmp = tmp->next_keyword;
        }
        dataset_head = dataset_head->next;
    }
}

void test_get_query_chain_mode_script() {
    keyword_head *test_head = get_query_chain_mode_script("exactSearchQry.txt");
    keyword_head *test_tail = test_head;

    printf("..............................test function test_get_query_chain_mode_script.......................\n");
    while (test_tail != NULL) {
        printf("   test name : %s, test mode : %s\n",
               test_tail->inquiry_name == NULL ? "null" : test_tail->inquiry_name,
               test_tail->inquiry_mode == script ? "script" : "manual");
        keyword_elem *tmp = test_tail->first_keyword;
        while (tmp != NULL) {
            printf("            %s\n", tmp->keyword_value);
            tmp = tmp->next_keyword;
        }
        test_tail = test_tail->next_keywords;
    }
    printf("..............................test function test_get_query_chain_mode_script end...................\n");
}

void test_get_query_chain_mode_manual(int argc, char *argv[]) {
    keyword_head *test_head = get_query_chain_mode_manual(argc, argv);
    keyword_head *test_tail = test_head;

    printf("..............................test function test_get_query_chain_mode_script.......................\n");
    while (test_tail != NULL) {
        printf("   test name : %s, test mode : %s\n",
               !strcmp(test_tail->inquiry_name, "") ? "null" : test_tail->inquiry_name,
               test_tail->inquiry_mode == script ? "script" : "manual");
        keyword_elem *tmp = test_tail->first_keyword;
        while (tmp != NULL) {
            printf("            %s\n", tmp->keyword_value);
            tmp = tmp->next_keyword;
        }
        test_tail = test_tail->next_keywords;
    }
    printf("..............................test function test_get_query_chain_mode_script end...................\n");
}

void test_query_all_dataset_all_key() {

    keyword_head *key = get_query_chain_mode_script("topKSearchQry.txt");
    result_chain *result = query_all_dataset_all_key(key);
    printf("...............................test function test_query_all_dataset_all_key................\n");


    while (result != NULL) {

        if (result->query_name != NULL) {
            printf("     query name :%s \n", result->query_name);
        }


        result_dataset *tmp_node = result->first_data_result;
        while (tmp_node != NULL) {
            result_key *tmp_key = tmp_node->first_key;
            printf("          file name :%s, total count : %d \n", tmp_node->filename, tmp_node->count);
            while (tmp_key != NULL) {
                printf("             key : %s, count : %d \n", tmp_key->value,
                       tmp_key->count);
                tmp_key = tmp_key->next;
            }
            tmp_node = tmp_node->next;
        }

        result = result->next;
    }
    printf("...............................test function test_query_all_dataset_all_key end............\n");
}

void test_core_search() {
    keyword_head *key = get_query_chain_mode_script("coreSearchQry.txt");
    result_chain *result = query_all_dataset_all_key(key);
    core_search(manual, result);
}

void test_exact_search() {
    keyword_head *key = get_query_chain_mode_script("exactSearchQry.txt");
    result_chain *result = query_all_dataset_all_key(key);
    exact_search(manual, result);
}

void test_top_search() {
    keyword_head *key = get_query_chain_mode_script("topSearchQry.txt");
    result_chain *result = query_all_dataset_all_key(key);
    top_search(manual, result);
}

void test_top_k_search() {
    keyword_head *key = get_query_chain_mode_script("topKSearchQry.txt");
    result_chain *result = query_all_dataset_all_key(key);
    top_k_search(manual, result);
}






