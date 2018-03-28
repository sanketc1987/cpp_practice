#pragma once
#include <vector>

template <typename T,
          typename Container = std::vector<T>,
          typename Compare = std::less<T>>
class Heap {
    private:
        std::vector<T>  items;
        Compare    cmp;
    public:
        /* Default constructor */
        Heap() {};
        /* Take any container and construct a heap. Assuming value_type for
         * input and items are the same. Not enforcing
         */
        Heap(const Container& input) :
            items(input.begin(), input.end())
        {
            for (size_t s = items.size(); s > 0; s--) {
                bubble_down(s - 1);
            }
        }

        virtual ~Heap() {};

        void insert (T data)
        {
            items.push_back(data);
            bubble_up(items.size() - 1);
        }

        void pop (void)
        {
            size_t   size = items.size();

            if (size == 0) return;

            if (size > 1) {
                items[0] = items[size - 1];
            }

            items.pop_back();

            if (size > 2) {
                bubble_down(0);
            }
        }

        const T& get_min (void)
        {
            /* vector::at() throws exception if size() == 0 */
            return items.at(0);
        }

        void display (void)
        {
            for(const T& d : items) {
                std::cout <<  d << "\t";
            }
            std::cout << std::endl;
        }


    private:
        size_t get_parent (size_t index)
        {
            if (index % 2)
                return (index / 2);
            else
                return (index / 2) - 1;
        }

        size_t get_yonger_child (size_t index)
        {
            return (index * 2) + 1;
        }

        size_t get_elder_child (size_t index)
        {
            return (index * 2) + 2;
        }

        /* Invariant: 0 <= child < items.size() */
        void bubble_up (size_t child)
        {
            if (child == 0) return;

            size_t parent = get_parent(child);

            /* If child is dominating the parent, swap them to restore order */
            if (cmp(items.at(child), items.at(parent))) {
                std::iter_swap(items.begin() + parent,
                               items.begin() + child);
                bubble_up(parent);
            }
        }

        /* Invariant: 0 <= parent < items.size() */
        void bubble_down (size_t parent)
        {
            size_t  yc = get_yonger_child(parent);
            size_t  ec = yc + 1;
            size_t  min_index;

            /* Select the dominating child */
            if (yc >= items.size())  return;
            if (ec >= items.size()) {
                min_index = yc;
            } else {
                min_index = cmp(items[yc], items[ec]) ? yc:ec;
            }

            /* if dominant child is dominating the parent, swap */
            if (cmp(items[min_index], items[parent])) {
                std::iter_swap(items.begin() + min_index,
                               items.begin() + parent);
                bubble_down(min_index);
            }
        }
};
