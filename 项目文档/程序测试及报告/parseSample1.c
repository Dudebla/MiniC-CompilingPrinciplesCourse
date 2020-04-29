/*语法分析测试用例*/
/*计数排序*/
void countSout(int A[],int B[],int k,int length){
    int C[k];
    int index ;
    index = 0;
    while(index < k){
        C[k] = 0;
        index = index + 1;
    }
    index = 0;
    while(index < k){
        C[index] = [index] + C[index-1]
        index = index +1;
    }
    index = length;
    while(index > 0){
        B[C[A[index]]] = A[j];
        C[A[j]] = C[A[index]] - 1;
    }
    return ;
}