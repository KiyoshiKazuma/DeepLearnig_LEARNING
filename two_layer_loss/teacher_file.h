#ifndef TEACHER_H
#define TEACHER_H

#define MAX_SIZE_TEACHER (100)
#define DATA_FILE_NAME "teacher.dat"

#include"matrix.h"

int read_teacher_data(int *size_X,int *size_T,S_MATRIX *X,S_MATRIX *T);
int add_teacher_data(int size_X,int size_T,S_MATRIX X,S_MATRIX T);
int get_teacher
#endif