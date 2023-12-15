//
// Created by nsas2020 on 23-12-13.
//此文件记录了路径状态的计算、更新、聚类等函数
#include "staflnet.h"
#include "aflnet.h"

#define MIN3(a,b,c) ((a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c)))



//从path_bits中提取路径状态序列
u8 **extract_paths(u8 *path_bits, u32 *path_bytes, u32 path_count) {
    u8 **paths = NULL;
    u32 index = 0;

    for (u32 i = 0; i < path_count; i++) {
        if (path_bytes[i]) {
            u8 *path = malloc(path_bytes[i] * sizeof(u8));

            for (u32 j = 0; j < path_bytes[i]; j++) {
                path[j] = path_bits[j + index];
            }

            paths = realloc(paths, (i + 1) * sizeof(u8 *));
            paths[i] = path;

            index += path_bytes[i];
        }
    }

    return paths;
}
// 释放动态分配的内存
void free_paths(u8 **paths, u32 path_count) {
    for (u32 i = 0; i < path_count; i++) {
        free(paths[i]);
    }
    free(paths);
}


//获取unsigned int *的长度
u32 path_length(u8 *point) {
    u32 len = 0;
    u8 *p = point;
    while (*p) {
        printf("path[%u]: %c\n", len, *p);
        p++;
        len++;
    }
    return len;
}


//获取两个整数数组之间的编辑距离
extern u32 Levenshtein_distance(u8 *point1, u8 *point2) {
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

extern int Exist_in_prev_one(path_state_info_t *path_state, u8 *new_path){
    u32 j=0;
    int isExist=0;
    //if(path_state->R == new_path) isExist=1;
    int distance= Levenshtein_distance(path_state->core,new_path);

    if(distance)
    if(distance<=path_state->R){
        isExist=1;
    }
    //将新的路径聚类至当前状态
    //更新该状态的各个变量



    //返回是否聚类至当前状态
    return isExist;
}

extern int GetPathStateIdInPrev(state_info_t *state,u8 *ToFindPath){

}

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