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

int main() {
  int a[10];
  int k = 1;
  k = 3;

  int name;
  name = k;
  a[2] = 1;
  // a[0]  =1;
  return 0;
}
