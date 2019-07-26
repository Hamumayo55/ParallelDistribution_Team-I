from multiprocessing import Pool

import time
import random
import matplotlib.pyplot as plt

def merge_sort(array):
    mid = len(array)
    sorted_list = []
    if mid > 1:
        left = merge_sort(array[:(mid//2)])
        right = merge_sort(array[(mid//2):])
        left_index = 0
        right_index = 0
        while left_index < len(left) and right_index < len(right):
            if left[left_index] <= right[right_index]:
                sorted_list.append(left[left_index])
                left_index += 1
            else:
                sorted_list.append(right[right_index])
                right_index += 1

        if left:
            sorted_list.extend(left[left_index:])
        if right:
            sorted_list.extend(right[right_index:])

        return sorted_list
    elif mid == 1:
        sorted_list.append(array[0])
        return sorted_list

def merge2(left,right):
    array = []
    while len(left) > 0 and len(right) > 0:
        if left[0] > right[0]:
            array.append(right.pop(0))
        else:
            array.append(left.pop(0))
    if len(left) == 0:
        array.extend(right)
    elif len(right) == 0:
        array.extend(left)
    return array

def merge(left, right):
    sorted_list = []
    left_index = 0
    right_index = 0
    while left_index < len(left) and right_index < len(right):
        if left[left_index] <= right[right_index]:
            sorted_list.append(left[left_index])
            left_index += 1
        else:
            sorted_list.append(right[right_index])
            right_index += 1

    if left:
        sorted_list.extend(left[left_index:])
    if right:
        sorted_list.extend(right[right_index:])

    return sorted_list

def wrapper(args):
    return merge(*args)


if __name__ == "__main__":
    p_time_list = []
    n_time_list = []
    value_list = []
    # ランダムな要素の作成
    value = 1000000

    array = [random.randint(1,20) for i in range(value)]
    # 配列の分割
    mid = len(array)
    array_00 = array[:(mid//2)]
    array_01 = array[(mid//2):]
    # array_p = [array_00, array_01]
    mid = len(array_00)
    array_0 = array_00[:(mid//2)]
    array_1 = array_00[(mid//2):]
    array_2 = array_01[:(mid//2)]
    array_3 = array_01[(mid//2):]
    array_p = [array_0, array_1, array_2, array_3]

    # 並列処理
    start = time.time()
    with Pool(processes=4) as p:
        result = p.map(merge_sort, array_p)
    result = [[result[0], result[1]],[result[2], result[3]]]
    with Pool(processes=2) as p:
        result_2 = p.map(wrapper, result)
    merge_3 = merge(result_2[0],result_2[1])
    print("並列処理：",time.time() - start)
    # 逐次処理
    start = time.time()
    merge_t = merge_sort(array)
    print("逐次処理：",time.time() - start)

    # print(merge_3)
    # print(merge_t)
    # start = time.time()
    # with Pool(processes=2) as p:
    #     result = p.map(merge_sort, array_p)
    # answer = merge(result[0], result[1])
    # print("並列処理：",time.time() - start)

    # x = [1, 2, 4]
    # y = [128, 102, 100]
    # plt.plot(x,y)
    # plt.xlabel("Processes")
    # plt.ylabel("Time")

    # plt.show()