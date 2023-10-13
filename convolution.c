#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// general parameters

#define stride 1
#define padding 0
#define N_filters 1
#define channels 1
#define batch_size 1

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
#define H_outputs ((H_inputs + 2 * padding - H_filter) / stride) + 1
#define W_outputs ((W_inputs + 2 * padding - W_filter) / stride) + 1
#define channels_outputs N_filters


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

// filter 4d 1 channel
static int32_t filter[N_filters][C_filter][H_filter][W_filter] =
    {
        {{{1, 2, 3, 4, 5},
          {4, 5, 6, 7, 8},
          {7, 8, 9, 1, 2},
          {10, 11, 12, 13, 14},
          {13, 14, 15, 16, 17}}}};

// input 4D 3 channel
// static int32_t inputs[batch_size][C_inputs][H_inputs][W_inputs] =
//{
//{
//    {{1, 2, 3, 4, 5, 6},
//     {2, 3, 4, 5, 6, 7},
//     {3, 4, 5, 6, 7, 8},
//     {4, 5, 6, 7, 8, 9},
//     {5, 6, 7, 8, 9, 10},
//     {6, 7, 8, 9, 10, 11}},
//
//     {{1, 2, 3, 4, 5, 6},
//     {2, 3, 4, 5, 6, 7},
//     {3, 4, 5, 6, 7, 8},
//     {4, 5, 6, 7, 8, 9},
//     {5, 6, 7, 8, 9, 10},
//     {6, 7, 8, 9, 10, 11}},
//
//     {{1, 2, 3, 4, 5, 6},
//     {2, 3, 4, 5, 6, 7},
//     {3, 4, 5, 6, 7, 8},
//     {4, 5, 6, 7, 8, 9},
//     {5, 6, 7, 8, 9, 10},
//     {6, 7, 8, 9, 10, 11}
//     }
//}
//};

// filter 4D 3 channel
// static int32_t filter[N_filters][C_filter][H_filter][W_filter]= {
//
//  {
//   {
//    {1, 2, 3 ,4, 5},
//     {4, 5, 6 ,7, 8},
//     {7, 8, 9 , 1, 2},
//     {10, 11, 12, 13, 14},
//       {13, 14, 15, 16, 17}
//    },
//    {
//     {2, 3, 4, 5, 6},
//     {5, 6, 7, 8, 9},
//     {8, 9, 1, 2, 3},
//     {11, 12, 13, 14, 15},
//     {14, 15, 16, 17, 18}
//    },
//    {
//     {3, 4, 5, 6, 7},
//     {6, 7, 8, 9, 1},
//     {9, 1,2, 3, 4},
//     {12, 13, 14, 15, 16},
//     {15, 16, 17, 18, 19}
//     }
//     }
//
//  };

// 4d input conv_Davide example
// static int32_t inputs[batch_size][C_inputs][H_inputs][W_inputs] =
//{{
//    {
//        {1, 1, 1},
//        {2, 2, 2},
//        {3, 3, 3},
//        {4, 4, 4},
//        {5, 5, 5},
//        {6, 6, 6},
//        {7, 7, 7},
//        {8, 8, 8},
//        {9, 9, 9}
//    }
//}};

// filter conv Davide example
// static int32_t filter[N_filters][C_filter][H_filter][W_filter]=
//{
//  {
//    { {2, 2, 2}, {2, 2, 2}, {2, 2, 2} }
//  }
//};

static int32_t outputs[N_outputs][channels_outputs][H_outputs][W_outputs];

int main()
{
  int32_t l, r, c, k, i, j, w, t;
  int32_t S;
  int32_t coeff;
  int32_t data;
  for (l = 0; l < N_outputs; l++)
  {
    for (k = 0; k < N_filters; k++)
    {
      for (r = 0; r < H_outputs; r++)
      {
        for (c = 0; c < W_outputs; c++)
        {
          S = 0;
          for (w = 0; w < C_filter; w++)
          {
            for (i = -FILT_HALF_x; i <= FILT_HALF_x; i++)
            {
              for (j = -FILT_HALF_y; j <= FILT_HALF_y; j++)
              {
                coeff = filter[k][w][i + FILT_HALF_x][j + FILT_HALF_y];

                data = inputs[l][w][r + i + FILT_HALF_x][c + j + FILT_HALF_y];

                S += coeff * data;
              }
            }
          }
          outputs[l][k][r][c] = S;
        }
      }
    }
  }

  return 0;
}
