#include <iostream>
#include <vector>

void print(std::vector<int>& arr) {
    for (const auto& item : arr) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

int quick_sort_helper(std::vector<int>& arr, int left, int right);

void quick_sort(std::vector<int>& arr, int left, int right) {
    if (left >= right) {
        return;
    }
    auto index = quick_sort_helper(arr, left, right);
    quick_sort(arr, left, index - 1);
    quick_sort(arr, index + 1, right);
}

void swap(std::vector<int>& arr, int i, int j) {
    int tmp = arr[j];
    arr[j] = arr[i];
    arr[i] = tmp;
}

// [5] 3 4 2 6 1 7
//             1 7
//     3 4 2 6
//     3 4 2 1 6 7
//           1
//  1  3 4 2 5 6 7


    // 5
//   3 4 2 6
//           1 7
//   3 4 2 1 6 7
// 1 3 4 2 5 6 7
// return index 4

int quick_sort_helper(std::vector<int>& array, int low, int high) {
    int pivot = array[low];
    while (low < high) {
        print(array);
        while (low < high && array[high] >= pivot) --high;
        array[low] = array[high];
        print(array);
        while (low < high && array[low] <= pivot) ++low;
        array[high] = array[low];
        print(array);
    }
    array[low] = pivot;
    return low;
}

//
// int quick_sort_helper(std::vector<int>& arr, int left, int right) {
//    if (arr.size() <= 1) {
//        return left;
//    }
//    int flag = arr[left];
//    int flag_index = left;
//    left = left + 1;
//
//    while (right != left) {
//        // 升序 先right后left
//        while (arr[right] > flag && right > left) {
//            right--;
//        }
//        while (arr[left] < flag && left < right) {
//            left++;
//        }
//        if (left != right) {
//            swap(arr, left, right);
//        }
//    }
//    if (flag < arr[left]) {
//        return flag_index;
//    }
//    swap(arr, flag_index, left);
//    return left;
//}

// 7, 3, 4, 2, 6, 1, [5]
// 3, 7, 4, 2, 6, 1
// 3, 4, 7, 2, 6, 1
// 3, 4, 2, 7, 6, 1
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {

        for (int x = 0; x < (high - low) + 1; x++) {
            std::cout << arr[x] << " ";
        }
        std::cout << "| i=" << i << " j=" << j << "\n";

        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    for (int x = 0; x < (high - low) + 1; x++) {
        std::cout << arr[x] << " ";
    }
    std::cout << "| i=" << i << "\n";

    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

int main() {
    std::vector<int> arr = {5, 3, 4, 2, 6, 1, 7};
    std::cout << quick_sort_helper(arr, 0, 6) << std::endl;

    for (const auto& item : arr) {
        std::cout << item << " ";
    }
}