//
// Created by nsas2020 on 23-12-13.
//此文件记录了路径状态的计算、更新、聚类等函数
#include "staflnet.h"
#include "aflnet.h"

#define MIN3(a,b,c) ((a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c)))
#define MAX(a,b) ((a)<(b)?(b):(a))



//从path_bits中提取路径状态序列
/*u32 **extract_paths(u32 *path_bits, u32 *path_bytes, u32 path_count) {
    u32 **paths = malloc(path_count * sizeof(u32 *));  // 一次性分配足够的内存

    u32 index = 0;

    for (u32 i = 0; i < path_count; i++) {
        if (path_bytes[i]) {
            u32 *path = malloc(path_bytes[i] * sizeof(u32));
            memcpy(path, path_bits + index, path_bytes[i] * sizeof(u32));  // 使用 memcpy 进行内存拷贝

            paths[i] = path;

            index += path_bytes[i];
        } else {
            paths[i] = NULL;  // 处理字节为 0 的情况，将路径设置为 NULL
        }
    }
    return paths;
}*/
u32 **extract_paths(u32 *path_bits, u32 *path_bytes, u32 path_count) {
    u32 **paths = malloc(path_count * sizeof(u32 *));
    if (paths == NULL) {
        // 内存分配失败，进行错误处理
        // 返回适当的错误状态或采取其他适当的操作
        return NULL;
    }
    u32 start = 0;
    for (u32 i = 0; i < path_count; i++) {
        u32 *path = malloc(path_bytes[i] * sizeof(u32));
        // 从 path_bits 的 start 位置复制 path_bytes[i] 个 u32 到 path
        memcpy(path, path_bits + start*sizeof(u32), path_bytes[i] * sizeof(u32));
        paths[i] = path;
        start += path_bytes[i];
    }
    return paths;
}

// 释放动态分配的内存
void free_paths(u32 **paths, u32 path_count) {
    for (u32 i = 0; i < path_count; i++) {
        free(paths[i]);
    }
    free(paths);
}


//获取unsigned int *的长度
u32 path_length(u32 *point) {
    u32 len = 0;
    u32 *p = point;
    while (*p) {
        printf("path[%u]: %c\n", len, *p);
        p++;
        len++;
    }
    return len;
}


//获取两个整数数组之间的编辑距离
extern u32 levenshtein_distance(u32 *point1, u32 *point2) {
    u32 len1 = path_length(point1);
    u32 len2 = path_length(point2);
    u32 res=0;

    u32 dp[len1][len2];

    for (u32 i = 0; i < len1; i++) {
        for (u32 j = 0; j < len2; j++) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else {
                u32 comp = (point1[i - 1] == point2[j - 1]) ? 0 : 1;
                dp[i][j] = MIN3(dp[i - 1][j] + 1,           /* 取三者的最小值 */
                                dp[i][j - 1] + 1,
                                dp[i - 1][j - 1] + comp);
            }
        }
    }
    res = dp[len1 - 1][len2 - 1];
    //是否需要释放dp空间
    return res;
}

extern u32 exist_in_prev_one(path_state_info_t *path_state, u32 *new_path){
    int isExist=0;
    u32 distance= levenshtein_distance(path_state->core, new_path);

    if(distance<=path_state->R){
        isExist=1;
    }
    //将新的路径聚类至当前状态
    //更新该状态的各个变量
    if(isExist){
        //更新变量
        path_state->points_count++;
        path_state->is_covered++;
        path_state->seeds_count++;
        path_state->fuzzs++;
        //重新计算质心
        u32 i=0;
        u32 sum=0;
        u32 max_dis=0;
        while(path_state->all_points[i]){
            u32 dis= levenshtein_distance(path_state->all_points[i], new_path);
            max_dis= MAX(dis,max_dis);
            sum+=dis;
            i++;
        }
        u32 avg_new=sum/i;
        if(avg_new<path_state->avg_distance){
            path_state->core= new_path;
            path_state->R=max_dis;
            path_state->avg_distance=avg_new;
        }
        u32 ** temp = malloc(path_state->points_count * sizeof(u32 *));
        for (int k = 0; k < path_state->points_count-1; k++) {
            temp[k] = path_state->all_points[k];
        }
        temp[path_state->points_count - 1] = new_path;
        free(path_state->all_points);
        path_state->all_points = temp;
    }
    //返回是否聚类至当前状态
    return isExist;
}

/*extern int GetPathStateIdInPrev(state_info_t *state,u32 *ToFindPath){

}*/

/*
void update_path_state_aware_variables(struct queue_entry *q, u8 dry_run) //这个函数应该放在afl-fuzz.c中，因为queue_entry在其中定义
{
    path_state_info_t *path_state;
    unsigned int pstate_count;

    if (!path_bits || !path_state_bytes) return;

    //路径状态序列，二维数组，整数——>整数——>整数
    unsigned int *path_state_sequence=(*extract_path_states)(path_bits, &pstate_count);

    //获取状态码序列
    unsigned int *state_sequence = (*extract_response_codes)(response_buf, response_buf_size, &state_count);

    //获取state_sequence与path_state_sequence之间的映射关系


}*/
void print_hash_table_phms(khash_t(phms) *hash_table) {
    khint_t k;
    for (k = kh_begin(hash_table); k != kh_end(hash_table); ++k) {
        if (kh_exist(hash_table, k)) {
            khint32_t key = kh_key(hash_table, k);
            //khint32_t *value = kh_value(hash_table, k);
            printf("Key: %d\n", key);
            printf("\n");
        }
    }
}

void print_hash_table(khash_t(s2path) *hash_table) {
    khint_t k;
    for (k = kh_begin(hash_table); k != kh_end(hash_table); ++k) {
        if (kh_exist(hash_table, k)) {
            khint32_t key = kh_key(hash_table, k);
            khint32_t *value = kh_value(hash_table, k);
            printf("Key: %d\n", key);
            printf("Value: ");
            for (int i = 0; value[i]; ++i) {
                printf("%d ", value[i]);
            }
            printf("\n");
        }
    }
}