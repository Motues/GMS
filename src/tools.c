#include "../include/tools.h"

// 交换两个学生元素
void swapStudents(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 将指定节点及其子节点调整为最大堆形态。
 * 此函数用于维护一个最大堆的属性。最大堆是一个二叉树，其中每个父节点的值都大于或等于其子节点的值。
 * 它的核心逻辑是确保当前节点是其子节点中的最大值，如果不是，则与最大子节点交换，然后递归地对交换后的节点进行调整。
 * @param arr 学生数组
 * @param i 当前正在调整的节点索引
 * @param n 学生数组的长度
 */
void maxHeapify_average(Student arr[], int i, int n) {
    int largest = i; //初始化当前节点为最大值
    int left = 2 * i + 1; //计算当前节点的左子节点索引
    int right = 2 * i + 2; //计算当前节点的右子节点索引

    //如果左子节点存在且成绩大于当前最大值，则更新最大值索引
    if (left < n && arr[left].average_score < arr[largest].average_score) {
        largest = left;
    }
    //如果右子节点存在且成绩大于当前最大值，则更新最大值索引
    if (right < n && arr[right].average_score < arr[largest].average_score) {
        largest = right;
    }

    //如果最大值不是当前节点，则交换当前节点和最大值节点，并递归调整被交换节点
    if (largest != i) {
        swapStudents(&arr[i], &arr[largest]);
        maxHeapify_average(arr, largest, n);
    }
}
/**
 * 构建最大堆
 * 该函数通过遍历数组，从最后一个非叶子节点开始，向上调整每个节点，以确保满足最大堆的性质。
 * 最大堆是一个完全二叉树，其中每个父节点的值都大于或等于其子节点的值。
 * @param arr 学生数组，存储了学生的相关信息
 * @param n 学生数组中学生的数量
 */
void buildMaxHeap_average(Student arr[], int n) {
    //从最后一个非叶子节点开始调整
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify_average(arr, i, n);//调整当前节点
    }
}
/**
 * 对学生数组按照平均分进行降序排序
 * 该函数首先构建一个最大堆，然后通过交换堆顶和堆底元素，并重新调整堆，来实现排序。
 * 时间复杂度O(nlogn)
 * @param arr 学生数组，包含n个学生
 * @param n 学生数组的长度
 */
void Sort_student_average(Student arr[], int n) {
    buildMaxHeap_average(arr, n); // 构建最大堆
    
    // 从堆底到堆顶依次取出元素，完成排序
    for (int i = n - 1; i > 0; --i) {
        swapStudents(&arr[0], &arr[i]); // 交换堆顶（即最大元素）和堆底元素
        maxHeapify_average(arr, 0, i); // 重新调整堆，保持堆的性质
    }
}

/**
 * 将指定节点及其子节点调整为最大堆形态。
 * 此函数用于维护一个最大堆的属性。最大堆是一个二叉树，其中每个父节点的值都大于或等于其子节点的值。
 * 它的核心逻辑是确保当前节点是其子节点中的最大值，如果不是，则与最大子节点交换，然后递归地对交换后的节点进行调整。
 * @param arr 学生数组
 * @param i 当前正在调整的节点索引
 * @param n 学生数组的长度
 */
void maxHeapify_total(Student arr[], int i, int n) {
    int largest = i; //初始化当前节点为最大值
    int left = 2 * i + 1; //计算当前节点的左子节点索引
    int right = 2 * i + 2; //计算当前节点的右子节点索引

    //如果左子节点存在且成绩大于当前最大值，则更新最大值索引
    if (left < n && arr[left].total_score > arr[largest].total_score) {
        largest = left;
    }
    //如果右子节点存在且成绩大于当前最大值，则更新最大值索引
    if (right < n && arr[right].total_score > arr[largest].total_score) {
        largest = right;
    }

    //如果最大值不是当前节点，则交换当前节点和最大值节点，并递归调整被交换节点
    if (largest != i) {
        swapStudents(&arr[i], &arr[largest]);
        maxHeapify_total(arr, largest, n);
    }
}
/**
 * 构建最大堆
 * 该函数通过遍历数组，从最后一个非叶子节点开始，向上调整每个节点，以确保满足最大堆的性质。
 * 最大堆是一个完全二叉树，其中每个父节点的值都大于或等于其子节点的值。
 * @param arr 学生数组，存储了学生的相关信息
 * @param n 学生数组中学生的数量
 */
void buildMaxHeap_total(Student arr[], int n) {
    //从最后一个非叶子节点开始调整
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify_total(arr, i, n);//调整当前节点
    }
}
/**
 * 对学生数组按照总分进行降序排序
 * 该函数首先构建一个最大堆，然后通过交换堆顶和堆底元素，并重新调整堆，来实现排序。
 * 时间复杂度O(nlogn)
 * @param arr 学生数组，包含n个学生
 * @param n 学生数组的长度
 */
void Sort_student_total(Student arr[], int n) {
    buildMaxHeap_total(arr, n); // 构建最大堆
    
    // 从堆底到堆顶依次取出元素，完成排序
    for (int i = n - 1; i > 0; --i) {
        swapStudents(&arr[0], &arr[i]); // 交换堆顶（即最大元素）和堆底元素
        maxHeapify_total(arr, 0, i); // 重新调整堆，保持堆的性质
    }
}

//按照学号排序
void maxHeapify_id(Student arr[], int i, int n) {
    int largest = i; 
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && strcmp(arr[left].id, arr[largest].id) > 0) {
        largest = left;
    }
    if (right < n && strcmp(arr[right].id, arr[largest].id) > 0) {
        largest = right;
    }
    if (largest != i) {
        swapStudents(&arr[i], &arr[largest]);
        maxHeapify_id(arr, largest, n);
    }
}
void buildMaxHeap_id(Student arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify_id(arr, i, n);
    }
}
void Sort_student_id(Student arr[], int n) {
    buildMaxHeap_id(arr, n); // 构建最大堆
    
    // 从堆底到堆顶依次取出元素，完成排序
    for (int i = n - 1; i > 0; --i) {
        swapStudents(&arr[0], &arr[i]); // 交换堆顶（即最大元素）和堆底元素
        maxHeapify_id(arr, 0, i); // 重新调整堆，保持堆的性质
    }
}

//按照姓名排序
void maxHeapify_name(Student arr[], int i, int n) {
    int largest = i; 
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && strcmp(arr[left].name, arr[largest].name) > 0) {
        largest = left;
    }
    if (right < n && strcmp(arr[right].name, arr[largest].name) > 0) {
        largest = right;
    }
    if (largest != i) {
        swapStudents(&arr[i], &arr[largest]);
        maxHeapify_name(arr, largest, n);
    }
}
void buildMaxHeap_name(Student arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify_name(arr, i, n);
    }
}
void Sort_student_name(Student arr[], int n) {
    buildMaxHeap_name(arr, n); // 构建最大堆
    
    // 从堆底到堆顶依次取出元素，完成排序
    for (int i = n - 1; i > 0; --i) {
        swapStudents(&arr[0], &arr[i]); // 交换堆顶（即最大元素）和堆底元素
        maxHeapify_name(arr, 0, i); // 重新调整堆，保持堆的性质
    }
}

//按照语文排序
void maxHeapify_chinese(Student arr[], int i, int n) {
    int largest = i; 
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left].course[0].score < arr[largest].course[0].score) {
        largest = left;
    }
    if (right < n && arr[right].course[0].score < arr[largest].course[0].score) {
        largest = right;
    }
    if (largest != i) {
        swapStudents(&arr[i], &arr[largest]);
        maxHeapify_chinese(arr, largest, n);
    }
}
void buildMaxHeap_chinese(Student arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify_chinese(arr, i, n);
    }
}
void Sort_student_chinese(Student arr[], int n) {
    buildMaxHeap_chinese(arr, n); // 构建最大堆
    
    // 从堆底到堆顶依次取出元素，完成排序
    for (int i = n - 1; i > 0; --i) {
        swapStudents(&arr[0], &arr[i]); // 交换堆顶（即最大元素）和堆底元素
        maxHeapify_chinese(arr, 0, i); // 重新调整堆，保持堆的性质
    }
}

//按照数学排序
void maxHeapify_math(Student arr[], int i, int n) {
    int largest = i; 
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left].course[1].score < arr[largest].course[1].score) {
        largest = left;
    }
    if (right < n && arr[right].course[1].score < arr[largest].course[1].score) {
        largest = right;
    }
    if (largest != i) {
        swapStudents(&arr[i], &arr[largest]);
        maxHeapify_math(arr, largest, n);
    }
}
void buildMaxHeap_math(Student arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify_math(arr, i, n);
    }
}
void Sort_student_math(Student arr[], int n) {
    buildMaxHeap_math(arr, n); // 构建最大堆
    
    // 从堆底到堆顶依次取出元素，完成排序
    for (int i = n - 1; i > 0; --i) {
        swapStudents(&arr[0], &arr[i]); // 交换堆顶（即最大元素）和堆底元素
        maxHeapify_math(arr, 0, i); // 重新调整堆，保持堆的性质
    }
}

//按照英语排序
void maxHeapify_english(Student arr[], int i, int n) {
    int largest = i; 
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left].course[2].score < arr[largest].course[2].score) {
        largest = left;
    }
    if (right < n && arr[right].course[2].score < arr[largest].course[2].score) {
        largest = right;
    }
    if (largest != i) {
        swapStudents(&arr[i], &arr[largest]);
        maxHeapify_english(arr, largest, n);
    }
}
void buildMaxHeap_english(Student arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify_english(arr, i, n);
    }
}
void Sort_student_english(Student arr[], int n) {
    buildMaxHeap_english(arr, n); // 构建最大堆
    
    // 从堆底到堆顶依次取出元素，完成排序
    for (int i = n - 1; i > 0; --i) {
        swapStudents(&arr[0], &arr[i]); // 交换堆顶（即最大元素）和堆底元素
        maxHeapify_english(arr, 0, i); // 重新调整堆，保持堆的性质
    }
}
