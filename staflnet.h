//
// Created by nsas2020 on 23-12-12.
//

#ifndef AFLNETSTATUSNEU_STAFLNET_H
#define AFLNETSTATUSNEU_STAFLNET_H

#endif //AFLNETSTATUSNEU_STAFLNET_H

#include <stdio.h>
#include "klist.h"
#include "khash.h"
#include "types.h"
#include "aflnet.h"

/*描述路径状态的结构体*/
typedef struct {
    u32 id;                     /* state id */
    u32 R;                      /*聚类半径*/
    u32 avg_distance;           /*当前核心点至其他点的中心*/
    u32 *core;                   /*以一个整数数组表示该状态的核心点*/
    u8 is_covered;              /* has this state been covered */
    //u32 paths;                  /* total number of paths exercising this state */
    //u32 paths_discovered;       /* total number of new paths that have been discovered when this state is targeted/selected */
    u32 selected_times;         /* total number of times this state has been targeted/selected */
    u32 fuzzs;                  /* Total number of fuzzs (i.e., inputs generated) */
    u32 score;                  /* current score of the state */
    u32 selected_seed_index;    /* the recently selected seed index */
    struct queue_entry **seeds;               /* keeps all seeds reaching this state -- can be casted to struct queue_entry* */
    u32 seeds_count;            /* total number of seeds, it must be equal the size of the seeds array */
    u32 points_count;           /*记录点的总数*/
    u32 **all_points;  /*记录当前路径状态中的所有核心点*/
} path_state_info_t;

//初始化一个哈希表，key为int型，value为path_state_info_t类型
//路径状态库
KHASH_INIT(phms,khint32_t,path_state_info_t *, 1, kh_int_hash_func, kh_int_hash_equal)

//初始化一个哈希表，存储所有的路径状态id,SET只有键，这个后面看用SET还是MAP
KHASH_SET_INIT_INT(phs32)

//初始化一个哈希表，状态码——路径状态id的list
KHASH_INIT(s2path,khint32_t,khint32_t *, 1, kh_int_hash_func, kh_int_hash_equal)

//获取unsigned int *的长度
u32 path_length(u32 *point);

/*从path_bits中提取路径状态序列，path_count即为message_sent
  u8 *path_bits 记录基本块编号序列的缓冲区
  path_bytes，一个记录每个路径大小的数组
  path_count，路径的个数
  path_bits={path_count * path_bytes[i]}*/
extern u32 **extract_paths(u32 *path_bits, u32 *path_bytes, u32 path_count);
// 释放paths动态分配的内存
extern void free_paths(u32 **paths, u32 path_count);

//获取两个整数数组之间的编辑距离
extern u32 Levenshtein_distance(u32 *point1, u32 *point2);

extern int Exist_in_prev_one(path_state_info_t *path_state, u32 *new_path);

//extern int GetPathStateIdInPrev(state_info_t *state,u32 *ToFindPath);

//新建一个路径状态，插入ippsm，并更新各种变量
void Create_path_state(state_info_t *state,unsigned int *new_path);

void print_hash_table(khash_t(s2path) *hash_table);

