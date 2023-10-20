#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// input parameters

#define H_inputs 16
#define W_inputs 16
#define C_inputs channels

// filter parameters

#define H_filter 5
#define W_filter 5
#define C_filter channels
#define FILT_HALF_x (H_filter / 2)
#define FILT_HALF_y (W_filter / 2)

// output parameters

#define N_outputs batch_size
#define H_outputs (((H_inputs + 2 * padding - H_filter) / stride) + 1)
#define W_outputs (((W_inputs + 2 * padding - W_filter) / stride) + 1)
#define channels_outputs N_filters

// im2col parameters

#define H_im2col (H_filter * W_filter * C_filter)
#define W_im2col (H_outputs * W_outputs)

// general parameters

#define stride 1
#define padding 1
#define N_filters 1
#define channels 1
#define batch_size 1

// filter 4d 1 channel
static int32_t filter[N_filters][C_filter][H_filter][W_filter] =
    {
        {{{1, 2, 3, 4, 5},
          {4, 5, 6, 7, 8},
          {7, 8, 9, 1, 2},
          {10, 11, 12, 13, 14},
          {13, 14, 15, 16, 17}}}};

// 4d input 1 channel
static int32_t inputs[batch_size][C_inputs][H_inputs][W_inputs] =
    {{{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}

    }}};

// inputs 4d 3 channels
// static int32_t inputs[batch_size][C_inputs][H_inputs][W_inputs] =
//{
//{
//    {{1, 2, 3, 4, 5, 6},
//     {2, 3, 4, 5, 6, 7},
//     {3, 4, 5, 6, 7, 8},
//     {4, 5, 6, 7, 8, 9},
//     {5, 6, 7, 8, 9, 10},
//     {6, 7, 8, 9, 10, 11}},
//     {
//      {1, 2, 3, 4, 5, 6},
//     {2, 3, 4, 5, 6, 7},
//     {3, 4, 5, 6, 7, 8},
//     {4, 5, 6, 7, 8, 9},
//     {5, 6, 7, 8, 9, 10},
//     {6, 7, 8, 9, 10, 11}},
//     {
//      {1, 2, 3, 4, 5, 6},
//     {2, 3, 4, 5, 6, 7},
//     {3, 4, 5, 6, 7, 8},
//     {4, 5, 6, 7, 8, 9},
//     {5, 6, 7, 8, 9, 10},
//     {6, 7, 8, 9, 10, 11}
//     }
//
//
//
//}
//};
static int32_t in2col_mat[batch_size][H_im2col][W_im2col];
static int32_t in2col_vett[N_filters][H_filter * W_filter * C_filter];

// filter 4d 3 channels
// static int32_t filter[N_filters][C_filter][H_filter][W_filter]=
//
//{
//
//   {
//    {
//     {1, 2, 3 ,4, 5},
//      {4, 5, 6 ,7, 8},
//      {7, 8, 9 , 1, 2},
//      {10, 11, 12, 13, 14},
//        {13, 14, 15, 16, 17}
//     },
//     {
//      {2, 3, 4, 5, 6},
//      {5, 6, 7, 8, 9},
//      {8, 9, 1, 2, 3},
//      {11, 12, 13, 14, 15},
//      {14, 15, 16, 17, 18}
//     },
//     {
//      {3, 4, 5, 6, 7},
//      {6, 7, 8, 9, 1},
//      {9, 1,2, 3, 4},
//      {12, 13, 14, 15, 16},
//      {15, 16, 17, 18, 19}
//      }
//      }
//
//   };

int main()
{

  int i, j, k, l, c, m, n, o, p, q, r, s, t, u, v, w;





  for (l = 0; l < batch_size; l++)
  {
    for (k = 0; k < N_filters; k++)
    {
      for (m = 0; m < H_outputs; m++)
      {
        for (n = 0; n < W_outputs; n++)
        {

          for (w = 0; w < C_filter; w++)
          {
            for (i = -FILT_HALF_x; i <= FILT_HALF_x; i++)
            {
              for (j = -FILT_HALF_y; j <= FILT_HALF_y; j++)
              {

                int dato_selezionato = inputs[l][w][m + i + FILT_HALF_x][n + j + FILT_HALF_y];

                int posto_input = (W_filter) * (i + 2) + (j + 2) + w * (H_filter * W_filter);
                in2col_vett[k][(W_filter) * (i + 2) + (j + 2) + w * (H_filter * W_filter)] = filter[k][w][i + FILT_HALF_x][j + FILT_HALF_y];
                in2col_mat[l][(W_filter) * (i + 2) + (j + 2) + w * (H_filter * W_filter)][n + (H_outputs * m)] = inputs[l][w][m + i + FILT_HALF_x][n + j + FILT_HALF_y];
              }
            }
          }
        }
      }
    }
  }

  return 0;
}