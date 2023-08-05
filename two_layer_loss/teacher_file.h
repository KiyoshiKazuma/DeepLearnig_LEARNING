#ifndef TEACHER_H
#define TEACHER_H

#define MAX_SIZE_TEACHER (1000)
#define TEACHER_FILE_NAME "teacher.dat"

#include"matrix.h"

int init_teacher_file(int size_X,int size_T);
int get_teacher_file_info(int * size_teacher,int * size_X,int *size_T);
int read_teacher_data(int *size_X,int *size_T,S_MATRIX *X,S_MATRIX *T);
int add_teacher_data(int size_X,int size_T,S_MATRIX X,S_MATRIX T);
#endif