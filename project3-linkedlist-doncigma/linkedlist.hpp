/// @file linkedlist.hpp
/// @brief A simple linked list implementation
/// @details This file contains the implementation of a simple linked list data structure.
/// Each element of the list is represented by a node that contains a value and a pointer to the next node.
/// The list is implemented as a template class, so it can hold elements of any type.
/// The list is implemented as a singly linked list, so it can only be traversed in one direction.
#pragma once

#include <stdexcept>

/// @brief Exception class for linked list errors.  Allows us to catch known errors for our implementation.
class LinkedListException : public std::exception
{
private:
    const char *message;

public:
    LinkedListException(const char *msg) : message(msg) {}
    const char *what() const noexcept override
    {
        return message;
    }
};

/// @brief A basic linked list implementation
template <typename T>
class LinkedList
{
public:
    /// @brief Constructor - sets the initial state to be empty and self-consistent.
    LinkedList()
    {
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    }

    /// @brief Destructor - cleans up all memory allocated by this class
    ~LinkedList()
    {
        Clear();
    }

    /// @brief Function to add a new element to the end of the list
    /// @param value The value to be added
    void Append(const T &value)
    {
        Node *newNode = new Node(value);

        if (_size > 0) {
            _tail->next = newNode;
            _tail = newNode;

            _size++;
        }
        else if (_head == nullptr) {
            _head = newNode;
            _tail = newNode;
            _tail->next = nullptr;

            _size++;
        }
        else throw LinkedListException("Invalid index, Append()");
    }

    /// @brief Function to add a new element to the beginning of the list
    /// @param value The value to be added
    void Prepend(const T &value)
    {
        Node *newNode = new Node(value);

        if (_size > 0) {
            newNode->next = _head;
            _head = newNode;

            _size++;
        }
        else if (_head == nullptr) {
            _head = newNode;
            _tail = newNode;
            _tail->next = nullptr;

            _size++;
        } 
        else throw LinkedListException("Invalid index, Prepend()");
    }

    /// @brief Function to insert a new element at a specific position
    /// @param value The value to be inserted
    /// @param position The position to insert the value at
    /// @throws LinkedListException if the position is invalid
    void InsertAt(const T &value, int position)
    {
        if (_size > 0) {
            if (position == 0) {
                Prepend(value);
            }
            else if (position == _size) {
                Append(value);
            }
            else if (position > 0 && position < _size && position > -1) {
                Node *ptr = _head;
                Node *prevNode = nullptr;
                Node *nodeToInsert = new Node(value);

                for (int i = 0; i < position; i++) {
                    prevNode = ptr;
                    ptr = ptr->next;
                }
                nodeToInsert->next = ptr;
                prevNode->next = nodeToInsert;
                
                _size++;
            }
            else throw LinkedListException("Invalid index, InsertAt()");
        }
        else throw LinkedListException("InsertAt() cannot be called on an empty list");
    }

    /// @brief Function to remove an element at a specific position
    /// @param position The position of the element to remove
    /// @throws LinkedListException if the position is invalid
    void RemoveAt(int position)
    {
        if (_size > 0) {
            if (position == 0) {
                Node *oldHead = _head;
                Node *newHead = _head->next;
                delete oldHead;
                _head = newHead;

                _size--;
            }
            else if (position == _size - 1) {
                Node *ptr = _head;
                Node *oldTail = _tail;
                Node *newTail = nullptr;
                
                for (int i = 0; i < position; i++) {
                    newTail = ptr;
                    ptr = ptr->next;
                }
                delete oldTail;
                _tail = newTail;
                _tail->next = nullptr;

                _size--;
            }
            else if (position > 0 && position < _size && position > -1) {
                Node *ptr = _head;
                Node *prevNode = nullptr;
                
                for (int i = 0; i < position; i++) {
                    prevNode = ptr;
                    ptr = ptr->next;
                }
                prevNode->next = ptr->next;
                delete ptr;
                
                _size--;
            }
            else throw LinkedListException("Invalid index, RemoveAt()");
        }
        else throw LinkedListException("RemoveAt() cannot be called on an empty list");
    }

    /// @brief Function to get the size of the linked list
    /// @return The size of the linked list
    int Size() const
    {
        if (_size > -1) {
            return _size;
        }
        else return 0;
    }

    /// @brief Function to check if the linked list is empty
    /// @return True if the linked list is empty, false otherwise
    bool Empty() const
    {
        if (_size == 0) {
            return true;
        }
        else if (_size > 0) {
            return false;
        }
        else return 0;
    }

    /// @brief Function to clear the linked list
    void Clear()
    { 
        if (_size > 0) {
            Node *ptr = _head;
            Node *nodeToDel = nullptr;

            while (ptr) {
                nodeToDel = ptr;
                ptr = ptr->next;
                delete nodeToDel;
            }
            _head = nullptr;
            _tail = nullptr;
            _size = 0;
        }
        else throw LinkedListException("List already empty, Clear()");
    }

    /// @brief Function to get the element at a specific position
    /// @param position The position of the element to get
    /// @return The element at the specified position
    /// @throws LinkedListException if the position is invalid
    T Get(int position) const
    {
        if (_size > 0 && position > -1 && position < _size) {
            Node *ptr = _head;
            int i = 0;
            
            while (i != position) {
                i++;
                ptr = ptr->next;
            }
            return ptr->data;
        } 
        else throw LinkedListException("Invalid index, Get()");
        return 0;
    }

    /// @brief Function to get the element at a specific position
    /// @param position The position of the element to get
    /// @return The element at the specified position
    /// @throws LinkedListException if the position is invalid
    T operator[](int position) const
    {
        Get(position);
    }

    /// @brief Function to find an element that satisfies a predicate
    /// @tparam Predicate The predicate function should take a const reference to the data type stored in the list and return a bool
    /// @param pred The predicate to apply to each element in the list.  Hint: pred(value) will apply the predicate to the value and return a bool.
    /// @return The first element that satisfies the predicate
    /// @throws LinkedListException if no element satisfies the predicate
    template <typename Predicate>
    T Find(Predicate pred) const
    {
        Node *ptr = _head;

        while (ptr) {
            if (pred(ptr->data)) {
                return ptr->data;
            }
            ptr = ptr->next;
        }
        throw LinkedListException("Invalid index, Find()");
        return 0;
    }

    /// @brief Finds the index of the first element in the list that satisfies the given predicate.
    /// @tparam Predicate The predicate function should take a const reference to the data type stored in the list and return a bool
    /// @param pred The predicate to apply to each element in the list.  Hint: pred(value) will apply the predicate to the value and return a bool.
    /// @return The index of the first element in the list that satisfies the predicate.
    /// @throws LinkedListException if no element in the list satisfies the predicate.
    template <typename Predicate>
    int FindIndex(Predicate pred) const
    {
        Node *ptr = _head;
        int position = 0;

        while (ptr) {
            if (pred(ptr->data)) {
                return position;
            }
            position++;
            ptr = ptr->next;
        }
        throw LinkedListException("Invalid index, FindIndex()");
        return 0;
    }

    /// @brief Applies a function to each element of the linked list.
    /// @tparam Function The function should take a const reference to the data type stored in the list and return void.
    /// @param func The function to apply.  Hint func(value) will apply the function to the value.
    template <typename Function>
    void ForEach(Function func) const
    {
        Node *ptr = _head;

        while (ptr) {
            func(ptr->data);
            ptr = ptr->next;
        }
    }

private:
    /// @brief Node class
    class Node
    {
    public:
        T data;     ///< The data stored in the node
        Node *next; ///< Pointer to the next node

        /// @brief Constructor that copies the value into the node.  Template type must support copy constructor.
        /// @param value The value to be copied into the node
        Node(const T &value) : data(value), next(nullptr) {}
    };

    Node *_head; ///< Pointer to the first node
    Node *_tail; ///< Pointer to the last node
    int _size;   ///< The number of elements in the list
};
