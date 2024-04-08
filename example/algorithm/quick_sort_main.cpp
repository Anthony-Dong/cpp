#include <iostream>
#include <vector>

template <typename T, template <typename...> class Array>
void swap(Array<T>& arr, typename Array<T>::size_type i, typename Array<T>::size_type j) {
    int tmp = arr[j];
    arr[j] = arr[i];
    arr[i] = tmp;
}

template <typename T, template <typename...> class Array>
int quick_sort_helper(Array<T>& arr, typename Array<T>::size_type left, typename Array<T>::size_type right);

template <typename T, template <typename...> class Array>
void quick_sort(Array<T>& arr, typename Array<T>::size_type left, typename Array<T>::size_type right) {
    if (left >= right) {
        return;
    }
    auto index = quick_sort_helper(arr, left, right);
    quick_sort(arr, left, index - 1);
    quick_sort(arr, index + 1, right);
}

template <typename T, template <typename...> class Array>
int quick_sort_helper(Array<T>& arr, typename Array<T>::size_type left, typename Array<T>::size_type right) {
    if (arr.size() <= 1) {
        return left;
    }
    int flag = arr[left];
    int flag_index = left;
    left = left + 1;

    while (right != left) {
        // 升序 先右后左
        while (arr[right] > flag && right > left) {
            right--;
        }
        while (arr[left] < flag && left < right) {
            left++;
        }
        if (left != right) {
            swap(arr, left, right);
        }
    }
    if (flag < arr[left]) {
        return flag_index;
    }
    swap(arr, flag_index, left);
    return left;
}

int main() {
    std::vector<int> arr = {5, 3, 4, 2, 1, 6, 7, 11, -1, -5, 20, 1};
    quick_sort(arr, 0, std::vector<int>::size_type(arr.size() - 1));
    //    std::cout << quick_sort_helper(arr, 0, arr.size() - 1) << std::endl;
}