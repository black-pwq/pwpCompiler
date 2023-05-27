// void QuickSort(int arr[], int low, int high)
// {
//     if (low < high)
//     {
//         int i = low;
//         int j = high;
//         int k = arr[low];
        
//         while (i < j)
//         {
//             while(i < j && arr[j] >= k)     // 从右向左找第一个小于k的数
//             {
//                 j--;
//             }
 
//             if(i < j)
//             {
//                 arr[i++] = arr[j];
//             }
 
//             while(i < j && arr[i] < k)      // 从左向右找第一个大于等于k的数
//             {
//                 i++;
//             }
 
//             if(i < j)
//             {
//                 arr[j] = arr[i];
//                 j=j-1;
//             }
//         }
 
//         arr[i] = k;
 
//         // 递归调用
//         QuickSort(arr, low, i - 1);     // 排序k左边
//         QuickSort(arr, i + 1, high);    // 排序k右边
//     }
// }

// void s(int w,int a[10000]){
//   int z = w;
//   // z = a[1];  
//   // putint(s[1]);
//   // int z = w;
//   // return 0;
// }


int main() {
  int arr[10000];
  arr[1] = 1;

  // int size = getint();
  // int ii =  0;
  // int i = 0;
  // int tmp = 0;
  // while(ii<size){
  //   arr[ii] = getint();
  //   ii=ii+1;
  // }
  // while(i < size - 1){
  //   int j = 0;
  //   while(j<size-i-1){
  //     if(arr[j]<arr[j+1]){
  //         tmp = arr[j];
  //         arr[j] = arr[j+1];
  //         arr[j+1] = tmp;
  //     }
  //     j=j+1;
  //   }
  //   i=i+1;
  // }
  // ii = 0;
  // while(ii<size){
  //   putint(arr[size-ii-1]);
  //   ii=ii+1;
  // }
    // for (i = 0; i < size - 1; i++) {  
    //     int j = 0;
    //     for (j = 0; j < size - i - 1; j++) {  
    //         if (arr[j] > arr[j+1]) {  
    //             tmp = arr[j];  
    //             arr[j] = arr[j+1];  
    //             arr[j+1] = tmp;  
    //         }  
    //     }  
    // } 


  
  return 0;
}
