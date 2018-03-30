/*
 * BTree.h
 *
 *  Created on: Mar 10, 2018
 *      Author: sanky
 */

#ifndef BTREE_H_
#define BTREE_H_

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

/* Template leaf, represents an element in Binary tree */
template <typename T>
struct Leaf {
    Leaf(): left(nullptr), right(nullptr), parent(nullptr) {}
    Leaf(T data): left(nullptr), right(nullptr), parent(nullptr), val(data) {}
    ~Leaf() {}
    Leaf<T>* left; // Points to the left child
    Leaf<T>* right; // Points to right child
    Leaf<T>* parent; // Points to the parent
    T val; // Item
};


template <typename T>
class BTree {
private:
    Leaf<T>*         root;
    size_t           size;
public:
    /* @descr:  Default constructor.
     * @state:  root of the tree initialized to NULL and all the size variables are initialized to 0
     * @params: None
     * @return: None
     */
    BTree (): root(nullptr), size(0) {}

    /* @descr: Destructor. Frees all the memory allocated for the tree leaves using post order
               processing
     * @params: None
     * @return: None
     */
    virtual ~BTree ()
    {
        post_order_process(root, [](Leaf<T>* l) {delete l;});
    }

    /* @descr:  Member function to insert an item into the tree using recursion.
     * @state:  root is updated on first insertion, size
     * @params: data - Element to be inserted into the tree
     * @return: None
     */
    void insert (T data)
    {
        recursive_insert(&root, root, data);
        size++;
    }

    /* @descr:  Member function to seacrh for an item in the tree
     * @state:  None
     * @params: data - Element to be searched in the tree
     * @return: Pointer to the leaf whose value is same as data. Null if data is not found in tree
     */
    Leaf<T>* search (T data)
    {
        return recursive_search(root, data);
    }

    /* @descr:  Member function to find minimum leaf in the tree
     * @state:  None
     * @params: None
     * @return: Pointer to the lose whose value is minimum in the tree
     */
    Leaf<T>* find_min (void)
    {
        return recursive_find_min(root);
    }

    /* @descr:  Member function to find maximum leaf in the tree
     * @state:  None
     * @params: None
     * @return: Pointer to the lose whose value is maximum in the tree
     */
    Leaf<T>* find_max (void)
    {
        return recursive_find_max(root);
    }

    /* @descr:  Three cases to handle when deleting a leaf.
     *          a. When leaf has no children, update parent and delete the node
     *          b. When leaf has only one child, make sure it gets a new parent and delete the node.
     *          c  When leaf has two children, find minimum in right sibtree, update leaf value to 
     *             that and then delete the minimum node using one of the above techniques
     * @state:  root is updated if root->val == data, size if updated after each deletion
     * @params: data - Item to be deleted from the tree
     * @return: None
     */
    void pop (T data)
    {
        /* Find the leaf to be deleted (ld) */
        Leaf<T>* ld = recursive_search(root, data);
        Leaf<T>* tmp;

        if (ld == nullptr) return; // Nothing to delete

        size--;
        if (size == 0) {
            delete root;
            root = nullptr;
            return;
        }

        if (ld->right && ld->left) {
            Leaf<T>* md = recursive_find_min(ld->right);
            ld->val = md->val;
            ld = md;
        }

        if (ld->left && !(ld->right)) {
            tmp = ld->left;
            ld->val = tmp->val;
            ld->right = tmp->right;
            ld->left = tmp->left;
            delete tmp;
        } else if (ld->right && !(ld->left)) {
            tmp = ld->right;
            ld->val = tmp->val;
            ld->left = tmp->left;
            ld->right = tmp->right;
            delete tmp;
        } else {
            Leaf<T>* p = ld->parent;
            /* p cant be null here as we already handled the case of single leaf tree */
            if (p->right == ld) p->right = nullptr;
            else p->left = nullptr;
            delete ld;
        }
    }

    /* @descr:  Member function to display all the elements tree using level-order processing
     * @state:  None
     * @params: None
     * @return: None
     */
    void display (void)
    {
        level_order_process([](Leaf<T>* l){std::cout << l->val << "\t";});
        std::cout << std::endl;
        std::cout << "Height = " << get_height_of_tree(root) << std::endl;
    }


    /* @descr:  Member function to balance the tree if needed. Makes sure height = log2(size) + 1
     * @state:  None
     * @params: None
     * @return: None
     */
    void balance_tree (void)
    {
        size_t  vec_len = size;
        size_t  height = get_height_of_tree(root);
        size_t  bal_height = ((size_t) log2(size)) + 1;
        std::vector<T>   sorted_vec;

        auto  vec_push = [](std::vector<T> &v, Leaf<T> *l)
                           {
                              v.push_back(l->val);
                           };

        /* If already balanced, return */
        if (height == bal_height) {
            return;
        }

        /* We know how much space we need */
        sorted_vec.reserve(size);

        /* Add all the elements into the vector in ascending order*/
        auto btree_to_vec = std::bind(vec_push,
                                      std::ref(sorted_vec),
                                      std::placeholders::_1);
        in_order_process(root, btree_to_vec);

        /* delete all the nodes in the binary tree */
        post_order_process(root, [](Leaf<T>* l){delete l;});
        root = nullptr;

        /* Recreate the tree */
        recursive_balance_tree(&root, root, sorted_vec, 0, vec_len);
    }

    /* @descr:  Find the leaf whose value is just less than x. x may or maynot be in the tree
     * @params: x  - Value to compare
     * @state:  None
     * return:  Predecessor leaf. Null if there is no predecessor
     */
    Leaf<T>* find_predecessor (T x)
    {
        Leaf<T>*    l = root;
        Leaf<T>*    pred = nullptr;

        while (l) {
            if (x <= l->val) {
                l = l->left;
            } else {
                pred = l;
                l = l->right;
            }
        }
        return pred;
    }

    /* @descr:  Find the leaf whose value is just greaterthan x. x may or maynot be in the tree
     * @params: x  - Value to compare
     * @state:  None
     * return:  Successor leaf. Null if there is no successor
     */
    Leaf<T>* find_successor (T x)
    {
       Leaf<T>* l = root;
       Leaf<T>* succ = nullptr;

       while (l) {
           if (x < l->val) {
               succ = l;
               l = l->left;
           } else {
               l = l->right;
           }
       }
       return succ;
    }

private:

    /* @descr:  Recursive insertion. Starting from root, find the leaf where the new data belongs.
     *          If current leave value is less then recurse to right sub tree. left sub tree
     *          otherwise. Base case for recursion is reahced when the current leaf ptr is null
     * @params: l - current leaf ptr
     *          p - parent leaf prt
     *          data - item to be inserted into the tree
     * @return: None
     */
    void recursive_insert (Leaf<T> **l, Leaf<T> *p, T data)
    {
        if (*l == nullptr) {
            *l = new Leaf<T>(data);
            (*l)->parent = p;
            return;
        }

        if (data > (*l)->val) {
            recursive_insert(&((*l)->right), *l, data);
        } else {
            recursive_insert(&((*l)->left), *l, data);
        }
    }

    /* @descr:  Recursive insertion. Starting from root, find the leaf where the search data.
     *          belongs. If current leave value is less then recurse to right sub tree. left
     *          subtree otherwise. Base case for recursion is reahced when the current leaf ptr
     *          is null or current ptr value is same as search data
     * @params: b - current leaf ptr
     *          data - item to be searched into the tree
     * @return: pointer to leaf where the data resides
     */
    Leaf<T>* recursive_search (Leaf<T> *b, T data)
    {
        if ((b == nullptr) || (b->val == data)) {
            return b;
        }

        if (data > b->val) {
            return recursive_search(b->right, data);
        } else {
            return recursive_search(b->left, data);
        }
    }

    /* @descr:  Miminum resides in the left most leaf of left sub tree. Base case is same as in
     *          search function
     * params:  b - current leaft ptr
     * @return: Pointer to leaf with minimum value in tree
     */
    Leaf<T>* recursive_find_min (Leaf<T> *b)
    {
        if ((b == nullptr) || (b->left == nullptr)) {
            return b;
        }
        return recursive_find_min(b->left);
    }

    /* @descr:  Maxinum resides in the right most leaf of right sub tree. Base case is same as in
     *          search function
     * params:  b - current leaf ptr
     * @return: Pointer to leaf with maximum value in tree
     */
    Leaf<T>* recursive_find_max (Leaf<T> *b)
    {
        if ((b == nullptr) || (b->right == nullptr)) {
            return b;
        }
        return recursive_find_max(b->right);
    }

    /* @descr:  Add the middle element of sorted_vec. Add the first half of the vector into left
     *          subtree and second half of the vector into right sub tree through recursion 
     * params:  *l    - current leaf ptr
     *          p     - parent to current leaf ptr
     *          sv    - refeerence to a sorted vector, elements of which are to be added to BTree
     *          start - index to start of a  portion of sorted_vec which is to be analysed in this
     *                  round of recursion
     *          end   - index to end of a portion of sorted_vec which is to be analysed in this
     *                  round of the recursion
     * @return: Pointer to leaf with maximum value in tree
     */
    void recursive_balance_tree (Leaf<T> **l, Leaf<T> *p,
                                 std::vector<T> &sv,
                                 size_t start, size_t end)
    {
        if (start == end) return;

        size_t mid = (start + end) / 2;
        *l = new Leaf<T>(sv[mid]);
        (*l)->parent = p;
        recursive_balance_tree(&((*l)->left), *l, sv, start, mid);
        recursive_balance_tree(&((*l)->right), *l, sv, mid + 1, end);
    }

    /* @descr:  recursively find the height of the tree starting from Leaf l
     * @params: None
     * @return: height of the tree starting from l
     */
    size_t get_height_of_tree (Leaf<T> *l)
    {
        if (l == nullptr)  return 0;

        size_t l_h = get_height_of_tree(l->left);
        size_t r_h = get_height_of_tree(l->right);
        return (std::max(l_h, r_h) + 1);
    }

    typedef std::function<void (Leaf<T>*)> process_leaf;

    void in_order_process (Leaf<T>* l, process_leaf process)
    {
        if (l) {
            in_order_process(l->left, process);
            process(l);
            in_order_process(l->right, process);
        }
    }

    void pre_order_process (Leaf<T>* l, process_leaf process)
    {
        if (l) {
            process(l);
            in_order_process(l->left, process);
            in_order_process(l->right, process);
        }
    }

    void post_order_process (Leaf<T> *l, process_leaf process)
    {
        if (l) {
            post_order_process(l->left, process);
            post_order_process(l->right, process);
            process(l);
        }
    }

    void level_order_process (process_leaf process)
    {
        std::queue<Leaf<T>*> q;
        q.push(root);

        while(!q.empty()) {
            Leaf<T>* l = q.front();
            process(l);
            q.pop();
            if (l->left) q.push(l->left);
            if (l->right) q.push(l->right);
        }
    }
};

#endif /* BTREE_H_ */

