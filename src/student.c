#include "../include/Student.h"

int student_number = 0; //初始化学生数量
Student stu[MAX_STUDENT_NUMBER]; 
int all_course_number = 0; //初始化课程数量
char course_name[MAX_COURSE_NUMBER][20];


/**
 * 创建并打开学生信息数据库
 * 尝试打开名为"Student.db"的SQLite数据库文件。如果数据库不存在，SQLite会自动创建它
 * @return 返回指向打开的数据库的指针；如果无法打开数据库，则返回NULL
 */
sqlite3 *Create_database_student() {
    sqlite3 *db_stu;
    int rc = sqlite3_open("data/Student.db", &db_stu); //打开数据库
    if (rc != SQLITE_OK) {
        printf("--> Can't open database: %s <--\n", sqlite3_errmsg(db_stu));
        sqlite3_close(db_stu);
        return 0;
    }
    printf("--> Opened database Student successfully <--\n");
    return db_stu;
}

/**
 * 创建学生信息表
 * 本函数用于在给定的SQLite数据库中创建一个名为"Student"的表。
 * 表的结构包括学生ID、姓名、性别、年龄以及语文、数学、英语成绩。
 * @param db_stu 学生数据库指针
 */
void Create_table_student(sqlite3 *db_stu) {
    char *sql = "CREATE TABLE IF NOT EXISTS Student(Id varchar(20), Name varchar(20), Sex verchar(10), Age int, Chinese int, Math int, English int);";
    // 创建SQL语句
    char *err_msg = NULL;
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg); // 执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Table created successfully <--\n");
    }
}

/**
 * 查询数据库中指定学生ID的数量
 * @param db_stu 学生数据库指针
 * @param stu_id 要查询的学生ID
 * @return 返回查询到的学生ID数量；如果准备SQL语句或绑定参数失败，返回-1
 */
int Query_student(sqlite3 *db_stu, char *stu_id) {
    const char *sql = "SELECT COUNT(*) FROM Student WHERE Id = ?"; // 创建SQL语句
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db_stu, sql, -1, &stmt, NULL); // 准备SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: Prepare failed <--\n");
        return -1;
    }
    // 将学生ID绑定到SQL语句的参数上，如果失败则打印错误信息，清理资源并返回-1
    rc = sqlite3_bind_text(stmt, 1, stu_id, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        printf("--> SQL error: Bind failed <--\n");
        sqlite3_finalize(stmt);
        return -1;
    }
    rc = sqlite3_step(stmt); // 执行SQL语句
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 0;
    }
    int count = sqlite3_column_int(stmt, 0); // 获取查询结果
    sqlite3_finalize(stmt);
    return count;
}

/**
 * 初始化十个学生信息到数据库中
 * 该函数从文本文件“data/Students.txt”中读取学生数据，并将这些数据插入到数据库的Student表中
 * @param db_stu 学生数据库指针
 */
void Init_student(sqlite3 *db_stu) {
    FILE *fp;
    fp = fopen("data/Students.txt", "r"); // 打开文件
    if (fp == NULL) {
        printf("--> Can't open file <--\n");
        return;
    }
    char id[20], sex[10], name[20],ch;
    int grade[5], age;
    // 读取文件数据
    while(fscanf(fp, "%s%s%s%d%d%d%d", id, name, sex, &age, grade+0, grade+1, grade+2) != EOF){
        //printf("%d\n",Query_student(db_stu, id));
        if (Query_student(db_stu, id) > 0) { // 如果学生ID已存在，则跳过
            continue;
        }
        //printf("Adding student %s %s %s %d %d %d %d\n", id, name, sex, age, grade[0], grade[1], grade[2]);
        char sql[100], *err_msg = NULL;
        sprintf(sql, "INSERT INTO Student VALUES('%s', '%s', '%s', %d, %d, %d, %d);", id, name, sex, age, grade[0], grade[1], grade[2]);
        int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg); // 创建并执行SQL语句
        if (rc != SQLITE_OK) {
            printf("--> SQL error: %s\n <--", err_msg);
            sqlite3_free(err_msg);
        }
    }
    printf("--> Students inited successfully <--\n");
    fclose(fp); // 关闭文件
}

/**
 * 将字符串转换为整数
 * 该函数接受一个字符指针作为输入，该指针指向一个以空字符结尾的字符串
 * 函数的目的是将这个字符串表示的数字转换为整数形式
 * @param str 指向待转换字符串的指针
 * @return 转换后的整数值；如果输入的字符串为NULL，则返回-1
 */
int string_to_int(char *str) {
    if (str == NULL) {
        return -1;
    }
    int num = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

//读取Student.db中的数据
/**
 * 该函数用于解析命令行参数，将学生信息存储到全局数组stu中
 * 参数argc和argv包含学生信息和课程成绩，azColName包含课程名
 * 函数计算学生的总分、平均分、学分加权平均分，并更新学生数量
 */
int get_student_imformation(void *data, int argc, char **argv, char **azColName) {
    strcpy(stu[student_number].id, argv[0]); //获取学生ID
    strcpy(stu[student_number].name, argv[1]); //获取学生姓名
    strcpy(stu[student_number].sex, argv[2]); //获取学生性别
    stu[student_number].age = string_to_int(argv[3]); //获取学生年龄
    stu[student_number].course_number = argc - 4; //获取学生课程数量
    
    //获取课程状态和成绩
    for (int i = 4; i < argc; i++) {
        int score = string_to_int(argv[i]); //获取科目成绩
        if(score == -1) strcpy(stu[student_number].course[i - 4].state, "Deferred"); //缓考
        else if (score < 60) strcpy(stu[student_number].course[i - 4].state, "Failed"); //不及格
        else if (60 <= score && score <= 100) strcpy(stu[student_number].course[i - 4].state, "Passed"); //及格
        stu[student_number].course[i - 4].score = score > 0 ? score : 0; //将成绩储存到结构体中
        strcpy(stu[student_number].course[i - 4].name, azColName[i]); //获取课程名称
    }
    stu[student_number].total_score = 0; //初始化总分
    //计算总分、学分加权总分和总学分
    for (int i = 0; i < stu[student_number].course_number; i++) {
        stu[student_number].total_score += stu[student_number].course[i].score;
        stu[student_number].total_credit_score += stu[student_number].course[i].score * stu[student_number].course[i].credit;
        stu[student_number].total_credit += stu[student_number].course[i].credit;
    }
    //计算平均分
    stu[student_number].average_score = stu[student_number].total_score *1.0 / stu[student_number].course_number;
    //计算学分加权平均分
    stu[student_number].average_credit_score = stu[student_number].total_credit_score *1.0 / stu[student_number].total_credit;
    //增加学生数量
    student_number++;
    return 0;
}
/**
 * 函数用于打印查询结果的一行数据。
 */
int print_row(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "(null)"); // 打印列名和值
    }
    printf("\n");
    return 0;
}
/**
 * 从指定的SQLite数据库中获取学生表中的所有数据
 * 查询结果通过回调函数get_student_imformation处理
 * @param db_stu 学生数据库指针
 */
void Get_student_imformation_from_database(sqlite3 *db_stu) {
    student_number = 0; // 初始化学生数量变量
    char *sql = "SELECT * FROM Student;", *err_msg = NULL;// 定义SQL查询语句
    int rc = sqlite3_exec(db_stu, sql, get_student_imformation, NULL, err_msg); // 执行SQL查询
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Student information getted successfully <--\n");
    }
}

/**
 * 从文件中读取学生信息，并将这些信息插入到学生数据库中
 * 同时为每个学生在用户数据库中创建一个账户，使用默认密码
 * @param db_user 用户数据库指针
 * @param db_stu 学生数据库指针
 * @param filename 文件路径
 */
void Read_from_file(sqlite3 *db_user, sqlite3 *db_stu, char *filename) {
    FILE *fp;
    fp = fopen(filename, "r"); // 打开文件
    if (fp == NULL) {
        printf("--> Error! Could not open file: %s <--\n", strerror(errno));
        return;
    }
    char id[20], sex[10], name[20],ch;
    int grade[5], age;
    //读取文件数据
    while(fscanf(fp, "%s%s%s%d%d%d%d", id, name, sex, &age, grade+0, grade+1, grade+2) != EOF){
        if (Query_student(db_stu, id) > 0) {//如果学生已存在，则跳过
            continue;
        }
        char sql[100], *err_msg = NULL;
        sprintf(sql, "INSERT INTO Student VALUES('%s', '%s', '%s', %d, %d, %d, %d);", id, name, sex, age, grade[0], grade[1], grade[2]);
        int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);//执行SQL语句
        if (rc != SQLITE_OK) {
            printf("--> SQL error: %s <--\n", err_msg);
            sqlite3_free(err_msg);
        }
        Add_user(db_user, id, "123456"); //创建新用户，默认密码为123456
    }
    printf("--> Records created successfully <--\n");
    fclose(fp);//关闭文件
    return;
}

/**
 * 添加学生信息
 * 函数首先检查学生是否已存在，如果存在，则不添加并返回；否则，将新学生信息添加到数据库
 * @param db_stu 学生数据库指针
 * @param stu_id 学生的学号
 * @param stu_name 学生的姓名
 * @param stu_sex 学生的性别
 * @param stu_age 学生的年龄
 */
void Add_student(sqlite3 *db_stu, char *stu_id, char *stu_name, char *stu_sex, int stu_age) {
    if (Query_student(db_stu, stu_id) > 0) {//如果学生已存在，则不添加
        printf("--> Student already exists <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    //创建SQL语句
    sprintf(sql, "INSERT INTO Student (Id, Name, Sex, Age) VALUES ('%s', '%s', '%s', %d)", stu_id, stu_name, stu_sex, stu_age);
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Student added successfully <--\n");
    }
    
}

/**
 * 删除学生信息
 * 函数首先检查学生是否存在，如果存在，则构造SQL语句删除该学生.如果删除操作执行出错，则打印错误信息并释放错误消息内存；如果删除成功，则打印删除成功的提示信息。
 * @param db_stu 学生数据库指针
 * @param stu_id 待删除学生的ID
 */
void Delete_student(sqlite3 *db_stu, char *stu_id) {
    if (Query_student(db_stu, stu_id) == 0) {//如果学生不存在，则不删除
        printf("--> Student does not exist <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "DELETE FROM Student WHERE Id = '%s'", stu_id);//创建SQL语句
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Student deleted successfully <--\n");
    }
}

/**
 * 修改学生姓名
 * 函数首先尝试查询指定ID的学生是否存在，如果存在，则构造并执行一个SQL语句来更新学生的姓名。如果学生不存在，则打印提示信息并返回。如果SQL执行出错，则打印错误信息并释放错误消息内存。如果执行成功，则打印提示信息表示学生姓名修改成功。
 * @param db_stu 学生数据库指针
 * @param stu_id 学生的ID
 * @param stu_name 新的学生姓名
 */
void Modify_name(sqlite3 *db_stu, char *stu_id, char *stu_name) {
    if (Query_student(db_stu, stu_id) == 0) {//如果学生不存在，则不修改
        printf("--> Student does not exist <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "UPDATE Student SET Name = '%s' WHERE Id = '%s'", stu_name, stu_id);//创建SQL语句
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Student name modified successfully <--\n");
    }
}

/**
 * 修改学生性别
 * 此函数首先检查学生是否存在于数据库中。如果学生存在，则构造一个SQL语句来更新学生的性别信息。如果更新操作执行成功，则打印成功消息；如果执行出错，则打印错误消息并释放错误消息内存
 * @param db_stu 学生数据库指针
 * @param stu_id 学生的ID
 * @param stu_sex 新的性别
 */
void Modify_sex(sqlite3 *db_stu, char *stu_id, char *stu_sex) {
    if (Query_student(db_stu, stu_id) == 0) {//如果学生不存在，则不修改
        printf("--> Student does not exist <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "UPDATE Student SET Sex = '%s' WHERE Id = '%s'", stu_sex, stu_id);//创建SQL语句
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Student sex modified successfully <--\n");
    }
}

/**
 * 修改学生年龄函数
 * 该函数首先尝试查询指定ID的学生是否存在，如果存在，则构造一个SQL语句用于更新学生的年龄信息
 * 如果数据库执行SQL语句出错，将打印错误信息并释放错误消息内存；如果执行成功，则提示学生年龄修改成功
 * @param db_stu 学生数据库指针
 * @param stu_id 学生的ID
 * @param stu_age 新的年龄
 */
void Modify_age(sqlite3 *db_stu, char *stu_id, int stu_age) {
    if (Query_student(db_stu, stu_id) == 0) {//如果学生不存在，则不修改
        printf("--> Student does not exist <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "UPDATE Student SET Age = %d WHERE Id ='%s'", stu_age, stu_id);//创建SQL语句
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Student age modified successfully <--\n");
    }
}

/**
 * 修改学生的某科成绩
 * 此函数首先检查学生是否存在，如果存在，则构造一个SQL语句用于更新学生的指定课程成绩；如果更新操作执行成功，则输出成功消息；如果执行失败，则输出错误消息
 * @param db_stu 学生数据库指针
 * @param stu_id 学生的ID
 * @param course_name 需要修改成绩的课程名称
 * @param new_score 新的课程成绩
 * 
 */
void Modify_score(sqlite3 *db_stu, char *stu_id, char *course_name, int new_score) {
    if (Query_student(db_stu, stu_id) == 0) {
        printf("--> Student does not exist <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "UPDATE Student SET %s = %d WHERE Id = '%s'", course_name, new_score, stu_id);
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Student score modified successfully <--\n");
    }
}

//更新课程
void Update_course(sqlite3 *db_stu) {
    all_course_number = 0;
    sqlite3_stmt *stmt;
    int rc;
    char *sql = "PRAGMA table_info(Student);"; 
    rc = sqlite3_prepare_v2(db_stu, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "--> Error preparing query: %s <--\n", sqlite3_errmsg(db_stu));
        sqlite3_finalize(stmt);
        return;
    }
    int i = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        i++;
        char *col_name = sqlite3_column_text(stmt, 1); // 第一列是列名
        if (i < 5) continue;
        if (col_name) {
            strcpy(course_name[all_course_number++], col_name);
        }
    }
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "--> Error executing query: %s <--\n", sqlite3_errmsg(db_stu));
    }
    sqlite3_finalize(stmt);
}

//查询课程
int Query_course(sqlite3 *db_stu, char *course_queried) {
    Update_course(db_stu);
    for (int i = 0; i < all_course_number; i++) {
        if (strcmp(course_queried, course_name[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

//添加课程
void Add_course(sqlite3 *db_stu, char *new_course_name) {
    //printf("%d\n", Query_course(db_stu, new_course_name));
    if (Query_course(db_stu, new_course_name) > 0) {
        printf("--> Course already exists <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "ALTER TABLE Student ADD COLUMN %s INTEGER;", new_course_name);
    //printf("%s", sql);
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);
    if (rc != SQLITE_OK) {
        printf("--> SQL error:%s <--\n", err_msg);
        //printf("%s\n", sqlite3_errcode(db_stu));
        sqlite3_free(err_msg);
    } else {
        printf("--> Course added successfully <--\n");
        Update_course(db_stu);
    }
}

//删除课程
void Delete_course(sqlite3 *db_stu, char *course_deleted) {
    if (Query_course(db_stu, course_deleted) == 0) {
        printf("--> Course does not exist <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "ALTER TABLE Student DROP COLUMN %s;", course_deleted);
    int rc = sqlite3_exec(db_stu, sql, NULL, NULL, err_msg);
    if (rc != SQLITE_OK) {
        printf("--> SQL error:%s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Course deleted successfully <--\n");
        Update_course(db_stu);
    }
}
