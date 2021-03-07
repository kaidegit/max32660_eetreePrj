//
// Created by kai on 2021/3/7.
//

#ifndef MAX32660_I2C_ALGORITHM_H
#define MAX32660_I2C_ALGORITHM_H

#include "stdbool.h"
#include "max30102.h"

#define FS 100
#define BUFFER_SIZE  (FS* 5)
#define HR_FIFO_SIZE 7
#define MA4_SIZE  4 // DO NOT CHANGE
#define HAMMING_SIZE  5// DO NOT CHANGE
#define min(x, y) ((x) < (y) ? (x) : (y))



void maxim_heart_rate_and_oxygen_saturation(
        const uint32_t *pun_ir_buffer, int32_t n_ir_buffer_length,
        const uint32_t *pun_red_buffer,
        int32_t *pn_spo2, int8_t *pch_spo2_valid, int32_t *pn_heart_rate,
        int8_t *pch_hr_valid
);

void maxim_find_peaks(
        int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_size, int32_t n_min_height,
        int32_t n_min_distance, int32_t n_max_num
);

void maxim_peaks_above_min_height(
        int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_size, int32_t n_min_height
);

void maxim_remove_close_peaks(int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_min_distance);

void maxim_sort_ascend(int32_t *pn_x, int32_t n_size);

void maxim_sort_indices_descend(const int32_t *pn_x, int32_t *pn_indx, int32_t n_size);

#endif //MAX32660_I2C_ALGORITHM_H
