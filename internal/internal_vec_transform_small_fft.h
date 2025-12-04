#ifndef SMALL_FFT_H
#define SMALL_FFT_H
#ifdef ENA_SMALL_FFT_INDV
//#include "stdio.h"
static inline void riscv_vec_butterfly_f32_small (riscv_vec_cpx_f32_t * FUNC_RESTRICT out,
                                       riscv_vec_cpx_f32_t * FUNC_RESTRICT in,
                                       q31_t * FUNC_RESTRICT factors,
                                       riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
                                       riscv_vec_cpx_f32_t * FUNC_RESTRICT buffer)
{ 
    riscv_vec_cpx_f32_t *p_in_cal = in;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    float32_t TW_81 = 0.70710678;
    fft_size = input_m << 3;
    float32_t tmp_cal_float_r[8];
    float32_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    //input_m = 1;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_f32_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4].i;
        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[5].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[5].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[5].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[5].i;
        tmp_cal_float_r[4] = p_in_cal[2].r + p_in_cal[6].r;
        tmp_cal_float_i[4] = p_in_cal[2].i + p_in_cal[6].i;
        tmp_cal_float_r[5] = p_in_cal[2].r - p_in_cal[6].r;
        tmp_cal_float_i[5] = p_in_cal[2].i - p_in_cal[6].i;
        tmp_cal_float_r[6] = p_in_cal[3].r + p_in_cal[7].r;
        tmp_cal_float_i[6] = p_in_cal[3].i + p_in_cal[7].i;
        tmp_cal_float_r[7] = p_in_cal[3].r - p_in_cal[7].r;
        tmp_cal_float_i[7] = p_in_cal[3].i - p_in_cal[7].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = -tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] + tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] - tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] - tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] + tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;

        out[0].r = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        out[0].i = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        out[1].r = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        out[1].i = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        out[2].r = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        out[2].i = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        out[3].r = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        out[3].i = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
        out[4].r = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        out[4].i = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        out[5].r = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        out[5].i = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        out[6].r = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        out[6].i = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        out[7].r = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        out[7].i = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        out[0].r = tmp_cal_float_r[0] + tmp_cal_float_r[2];
        out[0].i = tmp_cal_float_i[0] + tmp_cal_float_i[2];

        out[1].r = tmp_cal_float_r[1] + tmp_cal_float_i[3];
        out[1].i = tmp_cal_float_i[1] - tmp_cal_float_r[3];

        out[2].r = tmp_cal_float_r[0] - tmp_cal_float_r[2];
        out[2].i = tmp_cal_float_i[0] - tmp_cal_float_i[2];
        out[3].r = tmp_cal_float_r[1] - tmp_cal_float_i[3];
        out[3].i = tmp_cal_float_i[1] + tmp_cal_float_r[3];
    }
    return;
}

// 8pt / 4pt cfft using indivisaul function.
static inline void riscv_vec_inverse_butterfly_f32_small (riscv_vec_cpx_f32_t * FUNC_RESTRICT out,
                                            riscv_vec_cpx_f32_t * FUNC_RESTRICT in,
                                            q31_t * FUNC_RESTRICT factors,
                                            riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
                                            riscv_vec_cpx_f32_t * FUNC_RESTRICT buffer)
{
    riscv_vec_cpx_f32_t *p_in_cal = in;
    float32_t *p_out_cal_r = NULL;
    float32_t *p_out_cal_i = NULL;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    float32_t TW_81 = 0.70710678;
    if(input_m == 0)
    {
        fft_size = 4;    // special case for 4-pts cfft
    }
    else
    {
        fft_size = input_m << 3;
    }
    float32_t tmp_cal_float_r[8];
    float32_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    p_out_cal_r = &out[0].r;
    p_out_cal_i = &out[0].i;
    //input_m = 1;
    float32_t inv_factor = 1.0f / fft_size;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_f32_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4 * input_m].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4 * input_m].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4 * input_m].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4 * input_m].i;
        tmp_cal_float_r[2] = p_in_cal[1 * input_m].r + p_in_cal[5 * input_m].r;
        tmp_cal_float_i[2] = p_in_cal[1 * input_m].i + p_in_cal[5 * input_m].i;
        tmp_cal_float_r[3] = p_in_cal[1 * input_m].r - p_in_cal[5 * input_m].r;
        tmp_cal_float_i[3] = p_in_cal[1 * input_m].i - p_in_cal[5 * input_m].i;
        tmp_cal_float_r[4] = p_in_cal[2 * input_m].r + p_in_cal[6 * input_m].r;
        tmp_cal_float_i[4] = p_in_cal[2 * input_m].i + p_in_cal[6 * input_m].i;
        tmp_cal_float_r[5] = p_in_cal[2 * input_m].r - p_in_cal[6 * input_m].r;
        tmp_cal_float_i[5] = p_in_cal[2 * input_m].i - p_in_cal[6 * input_m].i;
        tmp_cal_float_r[6] = p_in_cal[3 * input_m].r + p_in_cal[7 * input_m].r;
        tmp_cal_float_i[6] = p_in_cal[3 * input_m].i + p_in_cal[7 * input_m].i;
        tmp_cal_float_r[7] = p_in_cal[3 * input_m].r - p_in_cal[7 * input_m].r;
        tmp_cal_float_i[7] = p_in_cal[3 * input_m].i - p_in_cal[7 * input_m].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = -tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] - tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] + tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] + tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] - tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = (tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[2] = (tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[4] = (tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[6] = (tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r) * inv_factor;
        p_out_cal_r[8] = (tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[8] = (tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[10] = (tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[10] = (tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[12] = (tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[12] = (tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[14] = (tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[14] = (tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r) * inv_factor;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        p_out_cal_r[0] = (tmp_cal_float_r[0] + tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_float_i[0] + tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[2] = (tmp_cal_float_r[1] - tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_float_i[1] + tmp_cal_float_r[3]) * inv_factor;

        p_out_cal_r[4] = (tmp_cal_float_r[0] - tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_float_i[0] - tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[6] = (tmp_cal_float_r[1] + tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_float_i[1] - tmp_cal_float_r[3]) * inv_factor;
    }
    return;
}

static inline void riscv_vec_butterfly_f64_small (riscv_vec_cpx_f64_t * FUNC_RESTRICT out,
                                       riscv_vec_cpx_f64_t * FUNC_RESTRICT in,
                                       q31_t * FUNC_RESTRICT factors,
                                       riscv_vec_cpx_f64_t * FUNC_RESTRICT twiddles,
                                       riscv_vec_cpx_f64_t * FUNC_RESTRICT buffer)
{
    riscv_vec_cpx_f64_t *p_in_cal = in;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    float64_t TW_81 = 0.70710678118654752440;
    fft_size = input_m << 3;
    float64_t tmp_cal_float_r[8];
    float64_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    //input_m = 1;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_f64_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4].i;
        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[5].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[5].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[5].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[5].i;
        tmp_cal_float_r[4] = p_in_cal[2].r + p_in_cal[6].r;
        tmp_cal_float_i[4] = p_in_cal[2].i + p_in_cal[6].i;
        tmp_cal_float_r[5] = p_in_cal[2].r - p_in_cal[6].r;
        tmp_cal_float_i[5] = p_in_cal[2].i - p_in_cal[6].i;
        tmp_cal_float_r[6] = p_in_cal[3].r + p_in_cal[7].r;
        tmp_cal_float_i[6] = p_in_cal[3].i + p_in_cal[7].i;
        tmp_cal_float_r[7] = p_in_cal[3].r - p_in_cal[7].r;
        tmp_cal_float_i[7] = p_in_cal[3].i - p_in_cal[7].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = -tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] + tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] - tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] - tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] + tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;

        out[0].r = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        out[0].i = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        out[1].r = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        out[1].i = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        out[2].r = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        out[2].i = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        out[3].r = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        out[3].i = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
        out[4].r = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        out[4].i = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        out[5].r = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        out[5].i = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        out[6].r = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        out[6].i = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        out[7].r = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        out[7].i = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        out[0].r = tmp_cal_float_r[0] + tmp_cal_float_r[2];
        out[0].i = tmp_cal_float_i[0] + tmp_cal_float_i[2];

        out[1].r = tmp_cal_float_r[1] + tmp_cal_float_i[3];
        out[1].i = tmp_cal_float_i[1] - tmp_cal_float_r[3];

        out[2].r = tmp_cal_float_r[0] - tmp_cal_float_r[2];
        out[2].i = tmp_cal_float_i[0] - tmp_cal_float_i[2];
        out[3].r = tmp_cal_float_r[1] - tmp_cal_float_i[3];
        out[3].i = tmp_cal_float_i[1] + tmp_cal_float_r[3];
    }
    return;
}

// 8pt / 4pt cfft using indivisaul function.
static inline void riscv_vec_inverse_butterfly_f64_small (riscv_vec_cpx_f64_t * FUNC_RESTRICT out,
                                            riscv_vec_cpx_f64_t * FUNC_RESTRICT in,
                                            q31_t * FUNC_RESTRICT factors,
                                            riscv_vec_cpx_f64_t * FUNC_RESTRICT twiddles,
                                            riscv_vec_cpx_f64_t * FUNC_RESTRICT buffer)
{
    riscv_vec_cpx_f64_t *p_in_cal = in;
    float64_t *p_out_cal_r = NULL;
    float64_t *p_out_cal_i = NULL;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    float64_t TW_81 = 0.70710678118654752440;
    if(input_m == 0)
    {
        fft_size = 4;    // special case for 4-pts cfft
    }
    else
    {
        fft_size = input_m << 3;
    }
    float64_t tmp_cal_float_r[8];
    float64_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    p_out_cal_r = &out[0].r;
    p_out_cal_i = &out[0].i;
    //input_m = 1;
    float64_t inv_factor = 1.0f / fft_size;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_f64_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4 * input_m].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4 * input_m].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4 * input_m].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4 * input_m].i;
        tmp_cal_float_r[2] = p_in_cal[1 * input_m].r + p_in_cal[5 * input_m].r;
        tmp_cal_float_i[2] = p_in_cal[1 * input_m].i + p_in_cal[5 * input_m].i;
        tmp_cal_float_r[3] = p_in_cal[1 * input_m].r - p_in_cal[5 * input_m].r;
        tmp_cal_float_i[3] = p_in_cal[1 * input_m].i - p_in_cal[5 * input_m].i;
        tmp_cal_float_r[4] = p_in_cal[2 * input_m].r + p_in_cal[6 * input_m].r;
        tmp_cal_float_i[4] = p_in_cal[2 * input_m].i + p_in_cal[6 * input_m].i;
        tmp_cal_float_r[5] = p_in_cal[2 * input_m].r - p_in_cal[6 * input_m].r;
        tmp_cal_float_i[5] = p_in_cal[2 * input_m].i - p_in_cal[6 * input_m].i;
        tmp_cal_float_r[6] = p_in_cal[3 * input_m].r + p_in_cal[7 * input_m].r;
        tmp_cal_float_i[6] = p_in_cal[3 * input_m].i + p_in_cal[7 * input_m].i;
        tmp_cal_float_r[7] = p_in_cal[3 * input_m].r - p_in_cal[7 * input_m].r;
        tmp_cal_float_i[7] = p_in_cal[3 * input_m].i - p_in_cal[7 * input_m].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = -tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] - tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] + tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] + tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] - tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = (tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[2] = (tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[4] = (tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[6] = (tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r) * inv_factor;
        p_out_cal_r[8] = (tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[8] = (tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[10] = (tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[10] = (tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[12] = (tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[12] = (tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[14] = (tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[14] = (tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r) * inv_factor;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        p_out_cal_r[0] = (tmp_cal_float_r[0] + tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_float_i[0] + tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[2] = (tmp_cal_float_r[1] - tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_float_i[1] + tmp_cal_float_r[3]) * inv_factor;

        p_out_cal_r[4] = (tmp_cal_float_r[0] - tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_float_i[0] - tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[6] = (tmp_cal_float_r[1] + tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_float_i[1] - tmp_cal_float_r[3]) * inv_factor;
    }
    return;
}

#if defined (__riscv_zfh)
static inline void riscv_vec_butterfly_f16_small (riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
                                       riscv_vec_cpx_f16_t * FUNC_RESTRICT in,
                                       q31_t * FUNC_RESTRICT factors,
                                       riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
                                       riscv_vec_cpx_f16_t * FUNC_RESTRICT buffer)
{
    riscv_vec_cpx_f16_t *p_in_cal = in;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    float16_t TW_81 = (float16_t) 0.70710678;
    fft_size = input_m << 3;
    float16_t tmp_cal_float_r[8];
    float16_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    //input_m = 1;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_f16_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4].i;
        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[5].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[5].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[5].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[5].i;
        tmp_cal_float_r[4] = p_in_cal[2].r + p_in_cal[6].r;
        tmp_cal_float_i[4] = p_in_cal[2].i + p_in_cal[6].i;
        tmp_cal_float_r[5] = p_in_cal[2].r - p_in_cal[6].r;
        tmp_cal_float_i[5] = p_in_cal[2].i - p_in_cal[6].i;
        tmp_cal_float_r[6] = p_in_cal[3].r + p_in_cal[7].r;
        tmp_cal_float_i[6] = p_in_cal[3].i + p_in_cal[7].i;
        tmp_cal_float_r[7] = p_in_cal[3].r - p_in_cal[7].r;
        tmp_cal_float_i[7] = p_in_cal[3].i - p_in_cal[7].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = -tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] + tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] - tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] - tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] + tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;

        out[0].r = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        out[0].i = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        out[1].r = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        out[1].i = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        out[2].r = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        out[2].i = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        out[3].r = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        out[3].i = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
        out[4].r = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        out[4].i = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        out[5].r = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        out[5].i = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        out[6].r = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        out[6].i = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        out[7].r = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        out[7].i = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        out[0].r = tmp_cal_float_r[0] + tmp_cal_float_r[2];
        out[0].i = tmp_cal_float_i[0] + tmp_cal_float_i[2];

        out[1].r = tmp_cal_float_r[1] + tmp_cal_float_i[3];
        out[1].i = tmp_cal_float_i[1] - tmp_cal_float_r[3];

        out[2].r = tmp_cal_float_r[0] - tmp_cal_float_r[2];
        out[2].i = tmp_cal_float_i[0] - tmp_cal_float_i[2];
        out[3].r = tmp_cal_float_r[1] - tmp_cal_float_i[3];
        out[3].i = tmp_cal_float_i[1] + tmp_cal_float_r[3];
    }
    return;
}

// 8pt / 4pt cfft using indivisaul function.
static inline void riscv_vec_inverse_butterfly_f16_small (riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
                                            riscv_vec_cpx_f16_t * FUNC_RESTRICT in,
                                            q31_t * FUNC_RESTRICT factors,
                                            riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
                                            riscv_vec_cpx_f16_t * FUNC_RESTRICT buffer)
{
    riscv_vec_cpx_f16_t *p_in_cal = in;
    float16_t *p_out_cal_r = NULL;
    float16_t *p_out_cal_i = NULL;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    float16_t TW_81 = (float16_t) 0.70710678;
    if(input_m == 0)
    {
        fft_size = 4;    // special case for 4-pts cfft
    }
    else
    {
        fft_size = input_m << 3;
    }
    float16_t tmp_cal_float_r[8];
    float16_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    p_out_cal_r = &out[0].r;
    p_out_cal_i = &out[0].i;
    //input_m = 1;
    float16_t inv_factor = 1.0f / fft_size;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_f16_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4 * input_m].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4 * input_m].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4 * input_m].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4 * input_m].i;
        tmp_cal_float_r[2] = p_in_cal[1 * input_m].r + p_in_cal[5 * input_m].r;
        tmp_cal_float_i[2] = p_in_cal[1 * input_m].i + p_in_cal[5 * input_m].i;
        tmp_cal_float_r[3] = p_in_cal[1 * input_m].r - p_in_cal[5 * input_m].r;
        tmp_cal_float_i[3] = p_in_cal[1 * input_m].i - p_in_cal[5 * input_m].i;
        tmp_cal_float_r[4] = p_in_cal[2 * input_m].r + p_in_cal[6 * input_m].r;
        tmp_cal_float_i[4] = p_in_cal[2 * input_m].i + p_in_cal[6 * input_m].i;
        tmp_cal_float_r[5] = p_in_cal[2 * input_m].r - p_in_cal[6 * input_m].r;
        tmp_cal_float_i[5] = p_in_cal[2 * input_m].i - p_in_cal[6 * input_m].i;
        tmp_cal_float_r[6] = p_in_cal[3 * input_m].r + p_in_cal[7 * input_m].r;
        tmp_cal_float_i[6] = p_in_cal[3 * input_m].i + p_in_cal[7 * input_m].i;
        tmp_cal_float_r[7] = p_in_cal[3 * input_m].r - p_in_cal[7 * input_m].r;
        tmp_cal_float_i[7] = p_in_cal[3 * input_m].i - p_in_cal[7 * input_m].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = -tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] - tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] + tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] + tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] - tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = (tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[2] = (tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[4] = (tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[6] = (tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r) * inv_factor;
        p_out_cal_r[8] = (tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[8] = (tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[10] = (tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[10] = (tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[12] = (tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[12] = (tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[14] = (tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[14] = (tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r) * inv_factor;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        p_out_cal_r[0] = (tmp_cal_float_r[0] + tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_float_i[0] + tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[2] = (tmp_cal_float_r[1] - tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_float_i[1] + tmp_cal_float_r[3]) * inv_factor;

        p_out_cal_r[4] = (tmp_cal_float_r[0] - tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_float_i[0] - tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[6] = (tmp_cal_float_r[1] + tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_float_i[1] - tmp_cal_float_r[3]) * inv_factor;
    }
    return;
}
#endif // __riscv_zfh

#if defined (__riscv_zfbfmin)
static inline void riscv_vec_butterfly_bf16_small (riscv_vec_cpx_bf16_t * FUNC_RESTRICT out,
                                       riscv_vec_cpx_bf16_t * FUNC_RESTRICT in,
                                       q31_t * FUNC_RESTRICT factors,
                                       riscv_vec_cpx_bf16_t * FUNC_RESTRICT twiddles,
                                       riscv_vec_cpx_bf16_t * FUNC_RESTRICT buffer)
{
    riscv_vec_cpx_bf16_t *p_in_cal = in;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    bf16_t TW_81 = (bf16_t) 0.70710678;
    fft_size = input_m << 3;
    bf16_t tmp_cal_float_r[8];
    bf16_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    //input_m = 1;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_bf16_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4].i;
        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[5].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[5].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[5].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[5].i;
        tmp_cal_float_r[4] = p_in_cal[2].r + p_in_cal[6].r;
        tmp_cal_float_i[4] = p_in_cal[2].i + p_in_cal[6].i;
        tmp_cal_float_r[5] = p_in_cal[2].r - p_in_cal[6].r;
        tmp_cal_float_i[5] = p_in_cal[2].i - p_in_cal[6].i;
        tmp_cal_float_r[6] = p_in_cal[3].r + p_in_cal[7].r;
        tmp_cal_float_i[6] = p_in_cal[3].i + p_in_cal[7].i;
        tmp_cal_float_r[7] = p_in_cal[3].r - p_in_cal[7].r;
        tmp_cal_float_i[7] = p_in_cal[3].i - p_in_cal[7].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = -tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] + tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] - tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] - tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] + tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;

        out[0].r = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        out[0].i = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        out[1].r = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        out[1].i = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        out[2].r = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        out[2].i = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        out[3].r = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        out[3].i = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
        out[4].r = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        out[4].i = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        out[5].r = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        out[5].i = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        out[6].r = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        out[6].i = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        out[7].r = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        out[7].i = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        out[0].r = tmp_cal_float_r[0] + tmp_cal_float_r[2];
        out[0].i = tmp_cal_float_i[0] + tmp_cal_float_i[2];

        out[1].r = tmp_cal_float_r[1] + tmp_cal_float_i[3];
        out[1].i = tmp_cal_float_i[1] - tmp_cal_float_r[3];

        out[2].r = tmp_cal_float_r[0] - tmp_cal_float_r[2];
        out[2].i = tmp_cal_float_i[0] - tmp_cal_float_i[2];
        out[3].r = tmp_cal_float_r[1] - tmp_cal_float_i[3];
        out[3].i = tmp_cal_float_i[1] + tmp_cal_float_r[3];
    }
    return;
}

// 8pt / 4pt cfft using indivisaul function.
static inline void riscv_vec_inverse_butterfly_bf16_small (riscv_vec_cpx_bf16_t * FUNC_RESTRICT out,
                                            riscv_vec_cpx_bf16_t * FUNC_RESTRICT in,
                                            q31_t * FUNC_RESTRICT factors,
                                            riscv_vec_cpx_bf16_t * FUNC_RESTRICT twiddles,
                                            riscv_vec_cpx_bf16_t * FUNC_RESTRICT buffer)
{
    riscv_vec_cpx_bf16_t *p_in_cal = in;
    bf16_t *p_out_cal_r = NULL;
    bf16_t *p_out_cal_i = NULL;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    int32_t fft_size = 0;
    bf16_t TW_81 = (bf16_t) 0.70710678;
    if(input_m == 0)
    {
        fft_size = 4;    // special case for 4-pts cfft
    }
    else
    {
        fft_size = input_m << 3;
    }
    bf16_t tmp_cal_float_r[8];
    bf16_t tmp_cal_float_i[8];
    // for size < 16, calculate the output directly.
    p_in_cal = &in[0];
    p_out_cal_r = &out[0].r;
    p_out_cal_i = &out[0].i;
    //input_m = 1;
    bf16_t inv_factor = 1.0f / fft_size;
    if(fft_size == 8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_bf16_t tmp_cal_cmplx[16];
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[4 * input_m].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[4 * input_m].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[4 * input_m].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[4 * input_m].i;
        tmp_cal_float_r[2] = p_in_cal[1 * input_m].r + p_in_cal[5 * input_m].r;
        tmp_cal_float_i[2] = p_in_cal[1 * input_m].i + p_in_cal[5 * input_m].i;
        tmp_cal_float_r[3] = p_in_cal[1 * input_m].r - p_in_cal[5 * input_m].r;
        tmp_cal_float_i[3] = p_in_cal[1 * input_m].i - p_in_cal[5 * input_m].i;
        tmp_cal_float_r[4] = p_in_cal[2 * input_m].r + p_in_cal[6 * input_m].r;
        tmp_cal_float_i[4] = p_in_cal[2 * input_m].i + p_in_cal[6 * input_m].i;
        tmp_cal_float_r[5] = p_in_cal[2 * input_m].r - p_in_cal[6 * input_m].r;
        tmp_cal_float_i[5] = p_in_cal[2 * input_m].i - p_in_cal[6 * input_m].i;
        tmp_cal_float_r[6] = p_in_cal[3 * input_m].r + p_in_cal[7 * input_m].r;
        tmp_cal_float_i[6] = p_in_cal[3 * input_m].i + p_in_cal[7 * input_m].i;
        tmp_cal_float_r[7] = p_in_cal[3 * input_m].r - p_in_cal[7 * input_m].r;
        tmp_cal_float_i[7] = p_in_cal[3 * input_m].i - p_in_cal[7 * input_m].i;

        tmp_cal_cmplx[0].r = tmp_cal_float_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_float_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_float_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_float_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_float_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_float_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_float_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_float_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_float_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_float_i[6];
        tmp_cal_cmplx[5].r = -tmp_cal_float_i[5];
        tmp_cal_cmplx[5].i = tmp_cal_float_r[5];
        tmp_cal_cmplx[3].r = (tmp_cal_float_r[3] - tmp_cal_float_i[3]) * TW_81;
        tmp_cal_cmplx[3].i = (tmp_cal_float_i[3] + tmp_cal_float_r[3]) * TW_81;
        tmp_cal_cmplx[7].r = (tmp_cal_float_r[7] + tmp_cal_float_i[7]) * TW_81;
        tmp_cal_cmplx[7].i = (tmp_cal_float_i[7] - tmp_cal_float_r[7]) * TW_81;

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = (tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[2] = (tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[4] = (tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[6] = (tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r) * inv_factor;
        p_out_cal_r[8] = (tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r) * inv_factor;
        p_out_cal_i[8] = (tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i) * inv_factor;
        p_out_cal_r[10] = (tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r) * inv_factor;
        p_out_cal_i[10] = (tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i) * inv_factor;
        p_out_cal_r[12] = (tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i) * inv_factor;
        p_out_cal_i[12] = (tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r) * inv_factor;
        p_out_cal_r[14] = (tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i) * inv_factor;
        p_out_cal_i[14] = (tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r) * inv_factor;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        tmp_cal_float_r[0] = p_in_cal[0].r + p_in_cal[2].r;
        tmp_cal_float_i[0] = p_in_cal[0].i + p_in_cal[2].i;
        tmp_cal_float_r[1] = p_in_cal[0].r - p_in_cal[2].r;
        tmp_cal_float_i[1] = p_in_cal[0].i - p_in_cal[2].i;

        tmp_cal_float_r[2] = p_in_cal[1].r + p_in_cal[3].r;
        tmp_cal_float_i[2] = p_in_cal[1].i + p_in_cal[3].i;
        tmp_cal_float_r[3] = p_in_cal[1].r - p_in_cal[3].r;
        tmp_cal_float_i[3] = p_in_cal[1].i - p_in_cal[3].i;

        p_out_cal_r[0] = (tmp_cal_float_r[0] + tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[0] = (tmp_cal_float_i[0] + tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[2] = (tmp_cal_float_r[1] - tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[2] = (tmp_cal_float_i[1] + tmp_cal_float_r[3]) * inv_factor;

        p_out_cal_r[4] = (tmp_cal_float_r[0] - tmp_cal_float_r[2]) * inv_factor;
        p_out_cal_i[4] = (tmp_cal_float_i[0] - tmp_cal_float_i[2]) * inv_factor;

        p_out_cal_r[6] = (tmp_cal_float_r[1] + tmp_cal_float_i[3]) * inv_factor;
        p_out_cal_i[6] = (tmp_cal_float_i[1] - tmp_cal_float_r[3]) * inv_factor;
    }
    return;
}

#endif // __riscv_zfbfmin

static inline void riscv_vec_butterfly_q15_small (riscv_vec_cpx_q15_t * FUNC_RESTRICT out,
                                       riscv_vec_cpx_q15_t * FUNC_RESTRICT in,
                                       q31_t * FUNC_RESTRICT factors,
                                       riscv_vec_cpx_q15_t * FUNC_RESTRICT twiddles,
                                       riscv_vec_cpx_q15_t * FUNC_RESTRICT buffer,
                                       q31_t scaled_flag)
{
    riscv_vec_cpx_q15_t *p_in_cal = &in[0];
    q15_t *p_out_cal_r = &out[0].r;
    q15_t *p_out_cal_i = &out[0].i;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    q15_t TW_81 = 23169;  // q15: sqrt(2)/2 ;
    q15_t tmp_cal_q15_r[8];
    q15_t tmp_cal_q15_i[8];
    uint8_t is_radix8 = (uint8_t) factors[IS_RADIX_8];
    int32_t fft_size = 0;
    uint32_t q_shift_m = 0;
    uint32_t mul_shift = 15;

    if(is_radix8)
    {
        fft_size = 8*input_m;
    }
    else
    {
        fft_size = 4*input_m;
    }


    input_m = 1;
    if(fft_size ==8)
    {
        riscv_vec_cpx_q15_t tmp_cal_cmplx[16];
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        if(scaled_flag)
        {
            q_shift_m = 3;
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q15_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[2] = ((p_in_cal[1*input_m].r)>>q_shift_m) + ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[2] = ((p_in_cal[1*input_m].i)>>q_shift_m) + ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[3] = ((p_in_cal[1*input_m].r)>>q_shift_m) - ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[3] = ((p_in_cal[1*input_m].i)>>q_shift_m) - ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[4] = ((p_in_cal[2*input_m].r)>>q_shift_m) + ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[4] = ((p_in_cal[2*input_m].i)>>q_shift_m) + ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[5] = ((p_in_cal[2*input_m].r)>>q_shift_m) - ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[5] = ((p_in_cal[2*input_m].i)>>q_shift_m) - ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[6] = ((p_in_cal[3*input_m].r)>>q_shift_m) + ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[6] = ((p_in_cal[3*input_m].i)>>q_shift_m) + ((p_in_cal[7*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[7] = ((p_in_cal[3*input_m].r)>>q_shift_m) - ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[7] = ((p_in_cal[3*input_m].i)>>q_shift_m) - ((p_in_cal[7*input_m].i)>>q_shift_m);

        tmp_cal_cmplx[0].r = tmp_cal_q15_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_q15_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_q15_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_q15_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_q15_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_q15_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_q15_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_q15_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_q15_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_q15_i[6];
        tmp_cal_cmplx[5].r = tmp_cal_q15_i[5];
        tmp_cal_cmplx[5].i = -tmp_cal_q15_r[5];
        tmp_cal_cmplx[3].r = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_r[3] + tmp_cal_q15_i[3])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[3].i = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_i[3] - tmp_cal_q15_r[3])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].r = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_r[7] - tmp_cal_q15_i[7])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].i = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_i[7] + tmp_cal_q15_r[7])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        p_out_cal_i[0] = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        p_out_cal_r[2] = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        p_out_cal_i[2] = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        p_out_cal_r[4] = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        p_out_cal_i[4] = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        p_out_cal_r[6] = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        p_out_cal_i[6] = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
        p_out_cal_r[8] = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        p_out_cal_i[8] = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        p_out_cal_r[10] = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        p_out_cal_i[10] = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        p_out_cal_r[12] = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        p_out_cal_i[12] = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        p_out_cal_r[14] = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        p_out_cal_i[14] = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        if(scaled_flag)
        {
            q_shift_m = 2;
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q15_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q15_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[2].i)>>q_shift_m);
        tmp_cal_q15_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q15_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[2].i)>>q_shift_m);

        tmp_cal_q15_r[2] = ((p_in_cal[1].r)>>q_shift_m) + ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q15_i[2] = ((p_in_cal[1].i)>>q_shift_m) + ((p_in_cal[3].i)>>q_shift_m);
        tmp_cal_q15_r[3] = ((p_in_cal[1].r)>>q_shift_m) - ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q15_i[3] = ((p_in_cal[1].i)>>q_shift_m) - ((p_in_cal[3].i)>>q_shift_m);

        p_out_cal_r[0] = tmp_cal_q15_r[0] + tmp_cal_q15_r[2];
        p_out_cal_i[0] = tmp_cal_q15_i[0] + tmp_cal_q15_i[2];

        p_out_cal_r[2] = tmp_cal_q15_r[1] + tmp_cal_q15_i[3];
        p_out_cal_i[2] = tmp_cal_q15_i[1] - tmp_cal_q15_r[3];
        p_out_cal_r[4] = tmp_cal_q15_r[0] - tmp_cal_q15_r[2];
        p_out_cal_i[4] = tmp_cal_q15_i[0] - tmp_cal_q15_i[2];

        p_out_cal_r[6] = tmp_cal_q15_r[1] - tmp_cal_q15_i[3];
        p_out_cal_i[6] = tmp_cal_q15_i[1] + tmp_cal_q15_r[3];
    }
    return ;

}

static inline void riscv_vec_inverse_butterfly_q15_small (riscv_vec_cpx_q15_t * FUNC_RESTRICT Fout,
                                            riscv_vec_cpx_q15_t * FUNC_RESTRICT Fin,
                                            q31_t * FUNC_RESTRICT factors,
                                            riscv_vec_cpx_q15_t * FUNC_RESTRICT twiddles,
                                            riscv_vec_cpx_q15_t * FUNC_RESTRICT buffer,
                                            q31_t scaled_flag)
{
    // for size < 16, calculate the output directly.
    riscv_vec_cpx_q15_t *p_in_cal = &Fin[0];
    q15_t *p_out_cal_r = &Fout[0].r;
    q15_t *p_out_cal_i = &Fout[0].i;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    uint8_t rfft_en = (uint8_t) factors[RFFT_EN];
    q15_t TW_81 = 23169;  // q15: sqrt(2)/2 ;
    q15_t tmp_cal_q15_r[8];
    q15_t tmp_cal_q15_i[8];
    uint8_t is_radix8 = (uint8_t) factors[IS_RADIX_8];
    int32_t fft_size = 0;
    uint32_t q_shift_m = 0;
    uint32_t mul_shift = 15;

    if(is_radix8)
    {
        fft_size = 8*input_m;
    }
    else
    {
        fft_size = 4*input_m;
    }

    input_m = 1;
    p_in_cal = &Fin[0];
    p_out_cal_r = &Fout[0].r;
    p_out_cal_i = &Fout[0].i;
    input_m = 1;
    if(fft_size ==8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_q15_t tmp_cal_cmplx[16];
        if(scaled_flag)
        {
            q_shift_m = 3;
            if(rfft_en)
            {
                q_shift_m = 1;
            }
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q15_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[2] = ((p_in_cal[1*input_m].r)>>q_shift_m) + ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[2] = ((p_in_cal[1*input_m].i)>>q_shift_m) + ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[3] = ((p_in_cal[1*input_m].r)>>q_shift_m) - ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[3] = ((p_in_cal[1*input_m].i)>>q_shift_m) - ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[4] = ((p_in_cal[2*input_m].r)>>q_shift_m) + ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[4] = ((p_in_cal[2*input_m].i)>>q_shift_m) + ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[5] = ((p_in_cal[2*input_m].r)>>q_shift_m) - ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[5] = ((p_in_cal[2*input_m].i)>>q_shift_m) - ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[6] = ((p_in_cal[3*input_m].r)>>q_shift_m) + ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[6] = ((p_in_cal[3*input_m].i)>>q_shift_m) + ((p_in_cal[7*input_m].i)>>q_shift_m);
        tmp_cal_q15_r[7] = ((p_in_cal[3*input_m].r)>>q_shift_m) - ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q15_i[7] = ((p_in_cal[3*input_m].i)>>q_shift_m) - ((p_in_cal[7*input_m].i)>>q_shift_m);

        tmp_cal_cmplx[0].r = tmp_cal_q15_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_q15_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_q15_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_q15_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_q15_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_q15_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_q15_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_q15_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_q15_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_q15_i[6];
        tmp_cal_cmplx[5].r = -tmp_cal_q15_i[5];
        tmp_cal_cmplx[5].i = tmp_cal_q15_r[5];
        tmp_cal_cmplx[3].r = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_r[3] - tmp_cal_q15_i[3])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[3].i = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_i[3] + tmp_cal_q15_r[3])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].r = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_r[7] + tmp_cal_q15_i[7])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].i = (q15_t)( ( (NDSV_F2I16_SAMPPROD)(tmp_cal_q15_i[7] - tmp_cal_q15_r[7])*(NDSV_F2I16_SAMPPROD) TW_81) >> (mul_shift) );

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        p_out_cal_i[0] = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        p_out_cal_r[2] = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        p_out_cal_i[2] = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        p_out_cal_r[4] = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        p_out_cal_i[4] = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        p_out_cal_r[6] = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        p_out_cal_i[6] = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
        p_out_cal_r[8] = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        p_out_cal_i[8] = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        p_out_cal_r[10] = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        p_out_cal_i[10] = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        p_out_cal_r[12] = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        p_out_cal_i[12] = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        p_out_cal_r[14] = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        p_out_cal_i[14] = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        if(scaled_flag)
        {
            q_shift_m = 2;
            if(rfft_en)
            {
                q_shift_m = 0;
            }
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q15_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q15_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[2].i)>>q_shift_m);
        tmp_cal_q15_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q15_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[2].i)>>q_shift_m);

        tmp_cal_q15_r[2] = ((p_in_cal[1].r)>>q_shift_m) + ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q15_i[2] = ((p_in_cal[1].i)>>q_shift_m) + ((p_in_cal[3].i)>>q_shift_m);
        tmp_cal_q15_r[3] = ((p_in_cal[1].r)>>q_shift_m) - ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q15_i[3] = ((p_in_cal[1].i)>>q_shift_m) - ((p_in_cal[3].i)>>q_shift_m);

        p_out_cal_r[0] = tmp_cal_q15_r[0] + tmp_cal_q15_r[2];
        p_out_cal_i[0] = tmp_cal_q15_i[0] + tmp_cal_q15_i[2];

        p_out_cal_r[2] = tmp_cal_q15_r[1] - tmp_cal_q15_i[3];
        p_out_cal_i[2] = tmp_cal_q15_i[1] + tmp_cal_q15_r[3];
        p_out_cal_r[4] = tmp_cal_q15_r[0] - tmp_cal_q15_r[2];
        p_out_cal_i[4] = tmp_cal_q15_i[0] - tmp_cal_q15_i[2];

        p_out_cal_r[6] = tmp_cal_q15_r[1] + tmp_cal_q15_i[3];
        p_out_cal_i[6] = tmp_cal_q15_i[1] - tmp_cal_q15_r[3];
    }
    return;
}

static inline void riscv_vec_butterfly_q31_small (riscv_vec_cpx_q31_t * FUNC_RESTRICT out,
                                       riscv_vec_cpx_q31_t * FUNC_RESTRICT in,
                                       q31_t * FUNC_RESTRICT factors,
                                       riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                       riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
                                       q31_t scaled_flag)
{

    riscv_vec_cpx_q31_t *p_in_cal = &in[0];
    q31_t *p_out_cal_r = &out[0].r;
    q31_t *p_out_cal_i = &out[0].i;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    q31_t TW_81 = 1518500249;  // q31: sqrt(2)/2 ;
    q31_t tmp_cal_q31_r[8];
    q31_t tmp_cal_q31_i[8];
    uint8_t is_radix8 = (uint8_t) factors[IS_RADIX_8];
    int32_t fft_size = 0;
    uint32_t q_shift_m = 0;
    uint32_t mul_shift = 31;

    if(is_radix8)
    {
        fft_size = 8*input_m;
    }
    else
    {
        fft_size = 4*input_m;
    }


    input_m = 1;
    if(fft_size ==8)
    {
        riscv_vec_cpx_q31_t tmp_cal_cmplx[16];
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        if(scaled_flag)
        {
            q_shift_m = 3;
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q31_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[2] = ((p_in_cal[1*input_m].r)>>q_shift_m) + ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[2] = ((p_in_cal[1*input_m].i)>>q_shift_m) + ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[3] = ((p_in_cal[1*input_m].r)>>q_shift_m) - ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[3] = ((p_in_cal[1*input_m].i)>>q_shift_m) - ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[4] = ((p_in_cal[2*input_m].r)>>q_shift_m) + ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[4] = ((p_in_cal[2*input_m].i)>>q_shift_m) + ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[5] = ((p_in_cal[2*input_m].r)>>q_shift_m) - ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[5] = ((p_in_cal[2*input_m].i)>>q_shift_m) - ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[6] = ((p_in_cal[3*input_m].r)>>q_shift_m) + ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[6] = ((p_in_cal[3*input_m].i)>>q_shift_m) + ((p_in_cal[7*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[7] = ((p_in_cal[3*input_m].r)>>q_shift_m) - ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[7] = ((p_in_cal[3*input_m].i)>>q_shift_m) - ((p_in_cal[7*input_m].i)>>q_shift_m);

        tmp_cal_cmplx[0].r = tmp_cal_q31_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_q31_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_q31_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_q31_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_q31_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_q31_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_q31_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_q31_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_q31_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_q31_i[6];
        tmp_cal_cmplx[5].r = tmp_cal_q31_i[5];
        tmp_cal_cmplx[5].i = -tmp_cal_q31_r[5];
        tmp_cal_cmplx[3].r = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_r[3] + tmp_cal_q31_i[3])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[3].i = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_i[3] - tmp_cal_q31_r[3])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].r = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_r[7] - tmp_cal_q31_i[7])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].i = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_i[7] + tmp_cal_q31_r[7])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        p_out_cal_i[0] = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        p_out_cal_r[2] = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        p_out_cal_i[2] = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        p_out_cal_r[4] = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        p_out_cal_i[4] = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        p_out_cal_r[6] = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        p_out_cal_i[6] = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
        p_out_cal_r[8] = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        p_out_cal_i[8] = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        p_out_cal_r[10] = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        p_out_cal_i[10] = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        p_out_cal_r[12] = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        p_out_cal_i[12] = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        p_out_cal_r[14] = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        p_out_cal_i[14] = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        if(scaled_flag)
        {
            q_shift_m = 2;
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q31_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q31_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[2].i)>>q_shift_m);
        tmp_cal_q31_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q31_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[2].i)>>q_shift_m);

        tmp_cal_q31_r[2] = ((p_in_cal[1].r)>>q_shift_m) + ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q31_i[2] = ((p_in_cal[1].i)>>q_shift_m) + ((p_in_cal[3].i)>>q_shift_m);
        tmp_cal_q31_r[3] = ((p_in_cal[1].r)>>q_shift_m) - ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q31_i[3] = ((p_in_cal[1].i)>>q_shift_m) - ((p_in_cal[3].i)>>q_shift_m);

        p_out_cal_r[0] = tmp_cal_q31_r[0] + tmp_cal_q31_r[2];
        p_out_cal_i[0] = tmp_cal_q31_i[0] + tmp_cal_q31_i[2];

        p_out_cal_r[2] = tmp_cal_q31_r[1] + tmp_cal_q31_i[3];
        p_out_cal_i[2] = tmp_cal_q31_i[1] - tmp_cal_q31_r[3];
        p_out_cal_r[4] = tmp_cal_q31_r[0] - tmp_cal_q31_r[2];
        p_out_cal_i[4] = tmp_cal_q31_i[0] - tmp_cal_q31_i[2];

        p_out_cal_r[6] = tmp_cal_q31_r[1] - tmp_cal_q31_i[3];
        p_out_cal_i[6] = tmp_cal_q31_i[1] + tmp_cal_q31_r[3];
    }
    return ;

}

static inline void riscv_vec_inverse_butterfly_q31_small (riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
                                            riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
                                            q31_t * FUNC_RESTRICT factors,
                                            riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                            riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
                                            q31_t scaled_flag)
{
    // for size < 16, calculate the output directly.
    riscv_vec_cpx_q31_t *p_in_cal = &Fin[0];
    q31_t *p_out_cal_r = &Fout[0].r;
    q31_t *p_out_cal_i = &Fout[0].i;
    int32_t input_m = factors[SIZE_FIRST_STAGE];
    uint8_t rfft_en = (uint8_t) factors[RFFT_EN];
    q31_t TW_81 = 1518500249;  // q31: sqrt(2)/2 ;
    q31_t tmp_cal_q31_r[8];
    q31_t tmp_cal_q31_i[8];
    uint8_t is_radix8 = (uint8_t) factors[IS_RADIX_8];
    int32_t fft_size = 0;
    uint32_t q_shift_m = 0;
    uint32_t mul_shift = 31;

    if(is_radix8)
    {
        fft_size = 8*input_m;
    }
    else
    {
        fft_size = 4*input_m;
    }

    input_m = 1;
    p_in_cal = &Fin[0];
    p_out_cal_r = &Fout[0].r;
    p_out_cal_i = &Fout[0].i;
    input_m = 1;
    if(fft_size ==8)
    {
        // for case CFFT 8 or RFFT 16
        // input_m = fft_size /8 ;
        //for(tmp_p =0; tmp_p < input_m; tmp_p++)
        riscv_vec_cpx_q31_t tmp_cal_cmplx[16];
        if(scaled_flag)
        {
            q_shift_m = 3;
            if(rfft_en)
            {
                q_shift_m = 1;
            }
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q31_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[4*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[4*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[2] = ((p_in_cal[1*input_m].r)>>q_shift_m) + ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[2] = ((p_in_cal[1*input_m].i)>>q_shift_m) + ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[3] = ((p_in_cal[1*input_m].r)>>q_shift_m) - ((p_in_cal[5*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[3] = ((p_in_cal[1*input_m].i)>>q_shift_m) - ((p_in_cal[5*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[4] = ((p_in_cal[2*input_m].r)>>q_shift_m) + ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[4] = ((p_in_cal[2*input_m].i)>>q_shift_m) + ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[5] = ((p_in_cal[2*input_m].r)>>q_shift_m) - ((p_in_cal[6*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[5] = ((p_in_cal[2*input_m].i)>>q_shift_m) - ((p_in_cal[6*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[6] = ((p_in_cal[3*input_m].r)>>q_shift_m) + ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[6] = ((p_in_cal[3*input_m].i)>>q_shift_m) + ((p_in_cal[7*input_m].i)>>q_shift_m);
        tmp_cal_q31_r[7] = ((p_in_cal[3*input_m].r)>>q_shift_m) - ((p_in_cal[7*input_m].r)>>q_shift_m);
        tmp_cal_q31_i[7] = ((p_in_cal[3*input_m].i)>>q_shift_m) - ((p_in_cal[7*input_m].i)>>q_shift_m);

        tmp_cal_cmplx[0].r = tmp_cal_q31_r[0];
        tmp_cal_cmplx[0].i = tmp_cal_q31_i[0];
        tmp_cal_cmplx[1].r = tmp_cal_q31_r[1];
        tmp_cal_cmplx[1].i = tmp_cal_q31_i[1];
        tmp_cal_cmplx[2].r = tmp_cal_q31_r[2];
        tmp_cal_cmplx[2].i = tmp_cal_q31_i[2];
        tmp_cal_cmplx[4].r = tmp_cal_q31_r[4];
        tmp_cal_cmplx[4].i = tmp_cal_q31_i[4];
        tmp_cal_cmplx[6].r = tmp_cal_q31_r[6];
        tmp_cal_cmplx[6].i = tmp_cal_q31_i[6];
        tmp_cal_cmplx[5].r = -tmp_cal_q31_i[5];
        tmp_cal_cmplx[5].i = tmp_cal_q31_r[5];
        tmp_cal_cmplx[3].r = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_r[3] - tmp_cal_q31_i[3])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[3].i = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_i[3] + tmp_cal_q31_r[3])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].r = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_r[7] + tmp_cal_q31_i[7])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );
        tmp_cal_cmplx[7].i = (q31_t)( ( (NDSV_F2I32_SAMPPROD)(tmp_cal_q31_i[7] - tmp_cal_q31_r[7])*(NDSV_F2I32_SAMPPROD) TW_81) >> (mul_shift) );

        tmp_cal_cmplx[8].r = tmp_cal_cmplx[0].r + tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[8].i = tmp_cal_cmplx[0].i + tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[9].r = tmp_cal_cmplx[1].r + tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[9].i = tmp_cal_cmplx[1].i + tmp_cal_cmplx[5].i;
        tmp_cal_cmplx[10].r = tmp_cal_cmplx[0].r - tmp_cal_cmplx[4].r;
        tmp_cal_cmplx[10].i = tmp_cal_cmplx[0].i - tmp_cal_cmplx[4].i;
        tmp_cal_cmplx[11].r = tmp_cal_cmplx[1].r - tmp_cal_cmplx[5].r;
        tmp_cal_cmplx[11].i = tmp_cal_cmplx[1].i - tmp_cal_cmplx[5].i;

        tmp_cal_cmplx[12].r = tmp_cal_cmplx[2].r + tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[12].i = tmp_cal_cmplx[2].i + tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[13].r = tmp_cal_cmplx[3].r - tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[13].i = tmp_cal_cmplx[3].i - tmp_cal_cmplx[7].i;
        tmp_cal_cmplx[14].r = tmp_cal_cmplx[2].r - tmp_cal_cmplx[6].r;
        tmp_cal_cmplx[14].i = tmp_cal_cmplx[2].i - tmp_cal_cmplx[6].i;
        tmp_cal_cmplx[15].r = tmp_cal_cmplx[3].r + tmp_cal_cmplx[7].r;
        tmp_cal_cmplx[15].i = tmp_cal_cmplx[3].i + tmp_cal_cmplx[7].i;
        p_out_cal_r[0] = tmp_cal_cmplx[8].r + tmp_cal_cmplx[12].r;
        p_out_cal_i[0] = tmp_cal_cmplx[8].i + tmp_cal_cmplx[12].i;
        p_out_cal_r[2] = tmp_cal_cmplx[9].r + tmp_cal_cmplx[13].r;
        p_out_cal_i[2] = tmp_cal_cmplx[9].i + tmp_cal_cmplx[13].i;
        p_out_cal_r[4] = tmp_cal_cmplx[10].r - tmp_cal_cmplx[14].i;
        p_out_cal_i[4] = tmp_cal_cmplx[10].i + tmp_cal_cmplx[14].r;
        p_out_cal_r[6] = tmp_cal_cmplx[11].r - tmp_cal_cmplx[15].i;
        p_out_cal_i[6] = tmp_cal_cmplx[11].i + tmp_cal_cmplx[15].r;
        p_out_cal_r[8] = tmp_cal_cmplx[8].r - tmp_cal_cmplx[12].r;
        p_out_cal_i[8] = tmp_cal_cmplx[8].i - tmp_cal_cmplx[12].i;
        p_out_cal_r[10] = tmp_cal_cmplx[9].r - tmp_cal_cmplx[13].r;
        p_out_cal_i[10] = tmp_cal_cmplx[9].i - tmp_cal_cmplx[13].i;
        p_out_cal_r[12] = tmp_cal_cmplx[10].r + tmp_cal_cmplx[14].i;
        p_out_cal_i[12] = tmp_cal_cmplx[10].i - tmp_cal_cmplx[14].r;
        p_out_cal_r[14] = tmp_cal_cmplx[11].r + tmp_cal_cmplx[15].i;
        p_out_cal_i[14] = tmp_cal_cmplx[11].i - tmp_cal_cmplx[15].r;
    }// (fft_size ==8)
    else
    {
        // radix-4 for RFFT size = 8 using
        // RFFT size =8 -> CFFT size = 4;
        if(scaled_flag)
        {
            q_shift_m = 2;
            if(rfft_en)
            {
                q_shift_m = 0;
            }
        }
        else
        {
            q_shift_m = 0;
        }
        tmp_cal_q31_r[0] = ((p_in_cal[0].r)>>q_shift_m) + ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q31_i[0] = ((p_in_cal[0].i)>>q_shift_m) + ((p_in_cal[2].i)>>q_shift_m);
        tmp_cal_q31_r[1] = ((p_in_cal[0].r)>>q_shift_m) - ((p_in_cal[2].r)>>q_shift_m);
        tmp_cal_q31_i[1] = ((p_in_cal[0].i)>>q_shift_m) - ((p_in_cal[2].i)>>q_shift_m);

        tmp_cal_q31_r[2] = ((p_in_cal[1].r)>>q_shift_m) + ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q31_i[2] = ((p_in_cal[1].i)>>q_shift_m) + ((p_in_cal[3].i)>>q_shift_m);
        tmp_cal_q31_r[3] = ((p_in_cal[1].r)>>q_shift_m) - ((p_in_cal[3].r)>>q_shift_m);
        tmp_cal_q31_i[3] = ((p_in_cal[1].i)>>q_shift_m) - ((p_in_cal[3].i)>>q_shift_m);

        p_out_cal_r[0] = tmp_cal_q31_r[0] + tmp_cal_q31_r[2];
        p_out_cal_i[0] = tmp_cal_q31_i[0] + tmp_cal_q31_i[2];

        p_out_cal_r[2] = tmp_cal_q31_r[1] - tmp_cal_q31_i[3];
        p_out_cal_i[2] = tmp_cal_q31_i[1] + tmp_cal_q31_r[3];
        p_out_cal_r[4] = tmp_cal_q31_r[0] - tmp_cal_q31_r[2];
        p_out_cal_i[4] = tmp_cal_q31_i[0] - tmp_cal_q31_i[2];

        p_out_cal_r[6] = tmp_cal_q31_r[1] + tmp_cal_q31_i[3];
        p_out_cal_i[6] = tmp_cal_q31_i[1] - tmp_cal_q31_r[3];
    }
    return;
}


#endif // ENA_SMALL_FFT_INDV
#endif // SMALL_FFT_H
