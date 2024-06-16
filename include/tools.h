#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "../include/student.h"
#include <stdio.h>
#include <stdlib.h>

void Sort_student_average(Student arr[], int n); //按照平均分从大到小排序
void Sort_student_total(Student arr[], int n); //按照总分从大到小排序
void Sort_student_id(Student arr[], int n); //按照学号从小到大排序
void Sort_student_name(Student arr[], int n); //按照姓名从小到大排序
void Sort_student_chinese(Student arr[], int n); //按照语文从大到小排序
void Sort_student_math(Student arr[], int n); //按照数学从大到小排序
void Sort_student_english(Student arr[], int n); //按照英语从大到小排序


#endif // __TOOLS_H__
