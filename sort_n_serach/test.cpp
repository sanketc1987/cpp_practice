#include "sort.h"

using namespace std;

int
main (int argc, char *argv[])
{
    vector<int> v = {3, 5, 1, 7, 7, 7, 10, 7, 8};

#if 0
    bubble_sort(v.begin(), v.end(), less<int>());
    insertion_sort(v.begin(), v.end(), less<int>());
    selection_sort(v.begin(), v.end(), greater<int>());
    quick_sort(v.begin(), v.end(), greater<int>());
    heap_sort(v.begin(), v.end(), greater<int>());
    merge_sort(v.begin(), v.end(), greater<int>());

    for (auto x : v) {
        cout << x << "\t";
    }
    cout << endl;

    merge_sort(v.begin(), v.end(), less<int>());
#endif
    quick_sort(v.begin(), v.end(), less<int>());
    for (auto x : v) {
        cout << x << "\t";
    }
    cout << endl;

    cout << boolalpha << bin_search(v.begin(), v.end(), 7) << endl;
    cout << boolalpha << bin_search(v.begin(), v.end(), 10) << endl;


    auto lit = find_lower_bound(v.begin(), v.end(), 11);
    auto uit = find_upper_bound(v.begin(), v.end(), 11);

    auto d = distance(lit, uit);
    cout << *lit << "\t" << *uit << "\t" << d << endl;
}
