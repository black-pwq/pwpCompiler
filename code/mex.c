void QuickSort(int arr[1000], int low, int high)
{
    if (low < high)
    {
        int i = low;
        int j = high;
        int k = arr[low];
        

        // while (i < j)
        // {
        //     while(i < j && arr[j] >= k)     // 从右向左找第一个小于k的数
        //     {
        //         j=j-1;
        //     }
 
        //     if(i < j)
        //     {
        //         arr[i] = arr[j];
        //         i=i+1;
        //     }
 
        //     while(i < j && arr[i] < k)      // 从左向右找第一个大于等于k的数
        //     {
        //         i=i+1;
        //     }
 
        //     if(i < j)
        //     {
        //         arr[j] = arr[i];
        //         j=j-1;
        //     }
        // }
 
        arr[i] = k;
 
        // 递归调用
        QuickSort(arr, low, i - 1);     // 排序k左边
        QuickSort(arr, i + 1, high);    // 排序k右边
    }
}
int main() {
  int array[1000];
  int len = 0;
  int a, b[3],c;
  QuickSort(array,0,len-1);
  return 0;
}
