#pragma once

#include <queue>
#include <vector>
#include <iostream>

template <typename Iter, typename Compare>
void __merge (Iter begin, Iter middle, Iter end, Compare cmp)
{
    Iter it_l = begin;
    Iter it_r = middle;
    std::vector<typename Iter::value_type> v;

    while (it_l != middle && it_r != end) {
        if (cmp(*it_l, *it_r))
            v.push_back(*it_l++);
        else
            v.push_back(*it_r++);
    }

    while (it_l != middle)  v.push_back(*it_l++);
    while (it_r != end) v.push_back(*it_r++);

    std::move(v.begin(), v.end(), begin);
}

template <typename Iter, typename Compare>
void merge_sort (Iter begin, Iter end, Compare cmp)
{
    auto    n      = std::distance(begin , end);
    Iter    middle = std::next(begin, n/2);

    if (n <= 1)  return;

    merge_sort(begin, middle, cmp);
    merge_sort(middle, end, cmp);
    __merge(begin, middle, end, cmp);
}


template <typename Iter, typename Compare>
void heap_sort (Iter begin, Iter end, Compare cmp)
{
    std::priority_queue<typename Iter::value_type,
                        std::vector<typename Iter::value_type>,
                        Compare> q(begin, end);
    Iter it = begin;

    while (!q.empty()) {
        *it++ = q.top();
        q.pop();
    }
}

template <typename Iter, typename Compare>
Iter __partition (Iter low, Iter high, Compare cmp)
{
    Iter wall = low;
    Iter pivot = high - 1;

    for (Iter it = low; it != pivot; ++it) {
        if (cmp(*it, *pivot)) {
            std::iter_swap(it, wall);
            wall++;
        }
    }

    std::iter_swap(wall, pivot);
    return wall;
}

template <typename Iter, typename Compare>
void quick_sort (Iter begin, Iter end, Compare cmp)
{
    auto n = std::distance(begin, end);
    if (n <= 1) return;

    Iter p = __partition(begin, end, cmp);
    quick_sort(begin, p++, cmp);
    quick_sort(p, end, cmp);
}


template <typename Iter, typename Compare>
void selection_sort (Iter begin, Iter end, Compare cmp)
{
    for (Iter it = begin; it != end; ++it) {
        Iter min = it;
        for (Iter jt = std::next(it, 1); jt != end; ++jt) {
            if (cmp(*jt, *min)) min = jt;
        }
        std::iter_swap(it, min);
    }
}


template <typename Iter, typename Compare>
void insertion_sort (Iter begin, Iter end, Compare cmp)
{
    for (Iter it = begin; it != end; ++it) {
        Iter jt = it;
        while ((jt != begin) &&
                cmp(*jt, *(jt - 1))) {
            std::iter_swap(jt, (jt - 1));
            --jt;
        }
    }
}


template <typename Iter, typename Compare>
void bubble_sort (Iter begin, Iter end, Compare cmp)
{
    size_t swaps = 1;
    size_t rounds = 1;

    while (swaps != 0) {
        swaps = 0;
        for (Iter it = begin; it != (end - rounds); it++) {
            if (cmp(*(it + 1), *it)) {
                std::iter_swap(it, it + 1);
                swaps++;
            }
        }
        rounds++;
    }
}


template <typename Iter>
bool bin_search (Iter begin, Iter end, typename Iter::value_type key)
{
    auto size = std::distance(begin, end);

    if (size == 1) {
        if (key == *begin)
            return true;
        else
            return false;
    }

    Iter middle = begin + (size/2);

    if (*middle == key) return true;

    if (*middle < key) {
        return bin_search(middle++, end, key);
    } else {
        return bin_search(begin, middle, key);
    }
}


template <typename Iter>
Iter find_lower_bound (Iter begin, Iter end, typename Iter::value_type key)
{
    auto size = std::distance(begin, end);
    Iter middle;
    Iter low = begin, high = end;

    while (size > 1) {
        middle = low + (size / 2);
        if (key <= *middle) {
            high = middle;
        } else {
            low = middle;
        }
        size = std::distance(low, high);
    }
    if (low == begin)
        return low;
    else
        return low + 1;
}

template <typename Iter>
Iter find_upper_bound (Iter begin, Iter end, typename Iter::value_type key)
{
    auto size = std::distance(begin, end);
    Iter middle;
    Iter low = begin, high = end;

    while (size > 1) {
        middle = low + (size / 2);
        if (key < *middle) {
            high = middle;
        } else {
            low = middle;
        }
        size = std::distance(low, high);
    }
    if (low == begin)
        return low;
    else
        return low + 1;
}
