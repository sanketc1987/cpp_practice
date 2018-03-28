/*
 * Llist.h
 *
 *  Created on: Feb 28, 2018
 *      Author: sanket
 */

#ifndef LLIST_H_
#define LLIST_H_

#include <iostream>

/* Implementation of a templated unsorted singly linked list */

/* Template node, represents an element in the linked list */
template <typename T>
struct Node {
    Node() : next(nullptr) {}
    Node(T data) : next(nullptr), val(data) {}
    ~Node() {}
    Node<T>* next;
    T val;
};

template <typename T>
class Llist {
private:
    Node<T>* head; // pointer to head of the list
    Node<T>* tail; // poiinter to tail of the list. Will be usefull in sorted lists to get max

public:
    /* @descr:  Default constructor.
     * @state:  head and tail pointers initialized to NULL
     * @params: None
     * @return: None
     */
    Llist()
    {
        head = nullptr;
        tail = nullptr;
    }

    /* @descr: Destructor. Frees all the memory that was allocated to nodes of the list
     * @params: None
     * @return: None
     */
    virtual
    ~Llist()
    {
        Node<T> *node = head;
        while(node) {
            head = head->next;
            delete node;
            node = head;
        }
        tail = nullptr;
    }


    /* @descr:  Member function to insert an item into the list. Insertion always happens at the
     *          head of the list.
     * @state:  head ptr is updated after each insertion, tail pointer is updated during first
     *          insertion
     * @params: data - Element to be inserted into the list
     * @return: None
     */
    virtual
    void insert_to_list (T data)
    {
        Node<T>* new_node(new Node<T>(data));
        new_node->next = head;
        head = new_node;

        if (!new_node->next) {
            tail = new_node;
        }
    }

    /* @descr:  Member function to delete the node whose value is equal to the input data. First
     *          search the node to be deleted, find predecessor to the node, update predecessor's
     *          next pointer and then finally delete the node
     * @state:  head is updated if node to be deleted is at head, tail is updated if node to be
     *          deleted is at tail
     * @params: data - Element to be deleted from the list
     * @return: None
     */
    virtual
    void delete_from_list (T data)
    {
        Node<T>* pred;
        Node<T>* del_node = search_in_list(data);
        if (del_node) {
            pred = predecessor(data);
            if (!pred) {
                /* If there is no predecessor, node to be deleted is at the head */
                head = del_node->next;
                if (!del_node->next) {
                    tail = head;
                }
            } else {
                pred->next = del_node->next;
                if (tail == del_node) {
                  /* Now tail points to predecessor */
                   tail = pred;
                }
            }
            delete del_node;
        }
    }

    /* @descr:  Member function to search for an element in the list. This function is marked final
     *          because the way you serach in a singly linked list is always the same
     * @state:  None
     * @params: data - element to be deleted in the list
     * @return: search_node - The node whose value is equal to the input param. This will be null if
     *          data is not in the list
     */
    virtual
    Node<T>* search_in_list (T data) final
    {
        Node<T>* search_node = head;

        while(search_node) {
            if (search_node->val == data) {
                break;
            }
            search_node = search_node->next;
        }
        return search_node;
    }

    /* @descr:  Member function to find the node with maximum value in the list
     * @state:  None
     * @params: None
     * @return: max_node - The node whose value is maximum in the list. This will be null if the
     *          list is empty
     */
    virtual
    Node<T>* find_max_in_list (void)
    {
        Node<T>* max_node = head;
        Node<T>* node = head;

        while(node) {
            if (node->val > max_node->val) {
                max_node = node;
            }
            node = node->next;
        }
        return max_node;
    }

    /* @descr:  Member function to find the node with minimum value in the list
     * @state:  None
     * @params: None
     * @return: min_node - The node whose value is minimum in the list. This will be null if the 
     *          list is empty
     */
    virtual
    Node<T>* find_min_in_list (void)
    {
        Node<T>* min_node = head;
        Node<T>* node = head;

        while(node) {
            if (node->val < min_node->val) {
                min_node = node;
            }
            node = node->next;
        }
        return min_node;
    }


    /* @descr:  Member function to display all the elements of the list
     * @state:  None
     * @params: None
     * @return: None
     */
    virtual
    void display_list (void) final
    {
        Node<T>* node = head;

        std::cout << head;
        while(node) {
            std::cout << "--->" << node->val << "|" << node->next;
            node = node->next;
        }
        std::cout << "<---" << tail << std::endl;
    }

private:
    /* @descr:  Fuction to find predecessor node for a given element in the list
     * @state:  None
     * @params: data - Element in the list, whose predecessor is to be found
     * @return: pred_node - Predecessor node to data. This will be NULL if data is not in the list
     *          or data is at the head
     */
    Node<T>* predecessor(T data)
    {
        Node<T>* pred_node = head;

        if (pred_node && (pred_node->val == data)) {
            return nullptr;
        }

        while (pred_node && pred_node->next) {
            if ((pred_node->next)->val == data) {
                return pred_node;
            }
            pred_node = pred_node->next;
        }
        return nullptr;
    }

protected:
    /* Functions to be used by classes inheriting this class to access private data members */
    Node<T>* get_head (void)
    {
        return head;
    }

    void set_head (Node<T>* new_head)
    {
        head = new_head;
    }

    Node<T>* get_tail (void)
    {
        return tail;
    }

    void set_tail (Node<T>* new_tail)
    {
        tail = new_tail;
    }
};

#endif /* LLIST_H_ */

