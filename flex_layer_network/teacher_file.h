#ifndef TEACHER_H
#define TEACHER_H


#include"matrix.h"
#include"configuration.h"

int init_teacher_file(int size_X,int size_T);
int read_teacher_data(S_NETWORK * pnet,S_MATRIX vX[],S_MATRIX vT[]);
int add_teacher_data(S_NETWORK * pnet,S_MATRIX X,S_MATRIX T);
int pick_random_teacher_dataset(S_MATRIX * vX,S_MATRIX * vT,S_MATRIX *vX_out,S_MATRIX * vT_out,int size_teacher,int size_out);
int read_teacher_size(S_NETWORK * pnet);
#endif