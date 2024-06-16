#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENT_NUMBER 100
#define MAX_COURSE_NUMBER 10

#include <stdio.h>
#include <stdlib.h>
#include "../sqlite/sqlite3.h"
#include "../include/user.h"

typedef struct course_credit { // 课程学分
    char name[20];
    int credit;
} course_credit;

typedef struct course { // 课程
    char name[20];
    int score;
    char state[20];
    int credit;
} course;

typedef struct Student { // 学生结构体
    char id[20];
    char name[20];
    char sex[10];
    int age;
    int course_number;
    course course[MAX_COURSE_NUMBER];
    int total_score, total_credit, total_credit_score;
    float average_score, average_credit_score;
} Student;

/* 全局变量 */
extern int student_number; // 学生人数
extern Student stu[MAX_STUDENT_NUMBER]; // 学生结构体
extern int all_course_number; // 课程总数
extern char course_name[MAX_COURSE_NUMBER][20]; // 课程名结构体

sqlite3 *Create_database_student(); // 创建学生数据库
void Create_table_student(sqlite3 *db_stu); // 创建学生表
// void Init_course_credit();

int Query_student(sqlite3 *db_stu, char *stu_id); // 查询学生是否存在
void Init_student(sqlite3 *db_stu); // 初始化十个学生信息
void Get_student_imformation_from_database(sqlite3 *db_stu); // 更新数据库中学生信息
void Read_from_file(sqlite3 *db_user, sqlite3 *db_stu, char *file_name); // 从文件中导入学生信息
void Add_student(sqlite3 *db_stu, char *stu_id, char *stu_name, char *stu_sex, int stu_age); // 添加学生
void Delete_student(sqlite3 *db_stu, char *stu_id); // 删除学生
void Modify_name(sqlite3 *db_stu, char *stu_id, char *new_name); // 修改学生姓名
void Modify_sex(sqlite3 *db_stu, char *stu_id, char *new_sex); // 修改学生性别
void Modify_age(sqlite3 *db_stu, char *stu_id, int new_age); // 修改学生年龄
void Modify_score(sqlite3 *db_stu, char *stu_id, char *course_name, int new_score); // 修改学生成绩

void Update_course(sqlite3 *db_stu); // 更新课程
int Query_course(sqlite3 *db_stu, char *course_queried); // 查询课程是否存在
void Add_course(sqlite3 *db_stu, char *new_course_name); // 添加课程，课程名称只能包含数字，字母和下划线
void Delete_course(sqlite3 *db_stu, char *course_deleted); // 删除课程




#endif // __STUDENT_H__
