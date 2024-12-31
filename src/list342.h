/*
Declaration
*/
#ifndef LIST_342_H_
#define LIST_342_H_
#include <iostream>
#include <fstream>

template<class T> 
class List342
{
  public:

    // Constructor
    List342();

    // Copy Constructor
    List342(const List342& other);

    // Deconstructor - deallocates all allocated memory 
    ~List342();

    // Insertion - inserts object into the list
    bool Insert(T* obj);

    // Remove - removes object from list and assigns to result
    bool Remove(T target, T& result); 

    // Peak - assigns target object from list to result
    bool Peek(T target, T& result) const;

    // Size - returns size of list
    int Size() const;

    // DeleteList - deletes lists
    void DeleteList();

    // Merge - merges two lists
    bool Merge(List342& list1);

    // BuildList - builds a list given a file
    bool BuildList(std::string file_name);

    // Operator +
    List342 operator+(const List342& other) const;

    // Operator +=
    List342& operator+=(const List342& other);

    // Operator =
    List342& operator=(const List342& other);

    // Operator ==
    bool operator==(const List342& other) const;
 
    // Operator !=
    bool operator!=(const List342& other) const;

    // Output Operator
    template<class U>
    friend std::ostream& operator<<(std::ostream& stream, const List342<U>& other);

  private:

    // Struct Node
    struct Node
    {
      T *data;
      Node *next;
    };

    Node* head_; // head pointer
    
    // Private method to check if a given obj exists in the list
    bool exists(const T* object) const;

    // Private helper method for + and += operators
    bool additionHelper(const List342& list1);
};


/*
Implementation
*/

// Default Constructor sets head_ to nullptr
template<class T>
List342<T>::List342() : head_(nullptr)
{

}

// Copy Constructor set head_ to nullptr and uses assignment operator
template<class T>
List342<T>::List342(const List342& other) : head_(nullptr)
{
  *this = other;
}

// Deconstructor calls DeleteList function.
template<class T>
List342<T>::~List342()
{
  DeleteList();
}

// Inserts object into the list in sorted order and checks for duplicates
template<class T>
bool List342<T>::Insert(T* obj)
{
  // Check if valid object
  if(obj == nullptr)
  {
    return false;
  }

  // Check for duplicate. Return false if duplicate
  if(exists(obj))
  {
    return false;
  }

  // Allocate new memory for object. Copy given object to new allocated memory
  T* copy_obj = new T;
  *copy_obj = *obj; 

  // Allocate new memory for node. Insert copied object into new allocated node
  Node* insert_node = new Node();
  insert_node->data = copy_obj;
  insert_node->next = nullptr; 

  // Check if list is empty. Assign head to insert_node
  if(head_ == nullptr)
  {
    head_ = insert_node;
    return true;
  }
  
  // Check if insert_node is to be inserted first in the list
  if(*(insert_node->data) < *(head_->data))
  {
    insert_node->next = head_;
    head_ = insert_node;
    return true;
  }

  // Traverse list for correct position. Return true.
  Node* current = head_;
  while(current->next != nullptr && *(current->next->data) < *(insert_node->data))
  {
    current = current->next;
  }
  insert_node->next = current->next;
  current->next = insert_node;
  return true;
}

// Removes item from list
template<class T>
bool List342<T>::Remove(T target, T& result)
{
  // Check if list is empty. Return false
  if(head_ == nullptr)
  {
    return false;
  }

  // Check if object does not exist in list. Return false
  if(exists(&target) == false)
  {
    return false;
  }

  // Check if object to be removed is the head
  if(*(head_->data) == target)
  {
    result = *(head_->data); 
    Node* remove_node = head_;
    head_ = head_->next;
    
    // Delete allocated memory
    delete remove_node->data; // delete memory in the object
    remove_node->data = nullptr;
    delete remove_node; // delete memory of the node
    remove_node = nullptr;

    return true;
  }

  // Traverse list for correct node
  Node* current = head_;
  while(*(current->next->data) != target) // Iterate until found target. This works because we know that obj exists in the list
  {
    current = current->next;
  }
  result = *(current->next->data);
  Node* remove_node = current->next;
  current->next = current->next->next;

  // Delete allocated memory
  delete remove_node->data;
  remove_node->data = nullptr;
  delete remove_node;
  remove_node = nullptr;
  
  // return true
  return true;;
}

// Returns target object from the list
template<class T>
bool List342<T>::Peek(T target, T& result) const 
{
  // Check if list is empty
  if(head_ == nullptr)
  {
    return false;
  }

  // Check if target exist in list
  if(exists(&target) == false)
  {
    return false;
  }

  // Traverse list for correct node 
  Node* current = head_;
  while(*(current->data) != target) // traverse list until found target because we know object exists in the list
  {
    current = current->next;
  }
    result = *(current->data);
    return true;
}

// Returns size of list
template<class T>
int List342<T>::Size() const
{
  int size = 0;
  Node* current = head_;
  while(current != nullptr)
  {
    size++;
    current = current->next;
  }
  return size;
}

// Deletes all objects and nodes from list.
template<class T>
void List342<T>::DeleteList()
{
  // Check if list empty
  if(head_ != nullptr)
  {
    // Traverse list with head
    while(head_ != nullptr)
    {
      Node* remove_node = head_;
      head_= head_->next;
      
      // Delete allocated memory 
      delete remove_node->data;
      remove_node->data = nullptr;
      delete remove_node;
      remove_node = nullptr;
    }
  }
}

// Merges this list with list1 without new data allocations. 
template<class T>
bool List342<T>::Merge(List342& list1)
{
  // Check if same lists
  if(this == &list1)
  {
    return false;
  }
  // Check if lists with same nodes
  if(*this == list1)
  {
    return false;
  }
  // Check if list to me merged with is empty
  if(list1.head_ == nullptr)
  {
    return false;
  }
  // Check if merging into an empty list
  if(head_ == nullptr)
  {
    head_ = list1.head_;
    list1.head_ = nullptr;
    return true;
  }

  // New pointer to build merged_list
  Node* merged_list = new Node;
  Node* current = merged_list; // Iterator for new pointer

  // Two iterator pointers for the lists
  Node* ptr1 = head_;
  Node* ptr2 = list1.head_;

  // Iterate through the lists and build merged_list pointer in sorted order
  while(ptr1 != nullptr && ptr2 != nullptr)
  {
    // Check if this list element is less
    if(*(ptr1->data) < *(ptr2->data))
    {
      current->next = ptr1;
      ptr1 = ptr1->next;
    }
    // Check if list1 element is less
    else if(*(ptr1->data) > *(ptr2->data))
    {
      current->next = ptr2;
      ptr2 = ptr2->next;
    }
    // else results in duplicate add one pointer to merged_list and delete the other 
    else
    {
      current->next = ptr1;
      ptr1 = ptr1->next;
      
      Node* remove_node = ptr2;
      ptr2 = ptr2->next;
      delete remove_node->data;
      remove_node->data = nullptr;
      delete remove_node;
      remove_node = nullptr;
    } 
    current = current->next;
  }

  // Check remaining
  if(ptr1 != nullptr)
  {
    current->next = ptr1;
  }
  else if(ptr2 != nullptr)
  {
    current->next = ptr2;
  }

  // Assign this list to the new merged_list pointer
  head_ = merged_list->next;
  
  // Delete and assign unused pointers to nullptr
  delete merged_list;
  merged_list = nullptr;
  list1.head_ = nullptr;

  // return true;
  return true;
}

// Builds list with given object attributes from file
template<class T>
bool List342<T>::BuildList(std::string file_name)
{
  std::ifstream in_file;

  in_file.open(file_name);

  // Check if file is closed. Return false if closed
  if(in_file.is_open() == false)
  {
    return false;
  }

  // Iterate through file info. Build objects and place them in new list
  while(!in_file.eof())
  {
    // Create temp memory to hold objects
    T* temp = new T; 
    in_file >> *temp; 

    this->Insert(temp); // Object is copied in insertion
    
    // delete temp memory
    delete temp;
    temp = nullptr;
  }
  in_file.close();
  return true;
}

// Adds two lists and returns a copy list of the summation result
template<class T>
List342<T> List342<T>::operator+(const List342<T>& other) const
{
  // Creates new list.
  List342<T> returnedList;
  returnedList.additionHelper(*this); // Uses helper method to insert data from source to destination
  returnedList.additionHelper(other); // Uses helper method to insert data from source to destination
  return returnedList; // returns new list
}

template<class T>
List342<T>& List342<T>::operator+=(const List342<T>& other)
{
  this->additionHelper(other); // Use helper method to insert data from source to destination
  return *this; // return updated list
}

// Assigns a list to another list
template<class T>
List342<T>& List342<T>::operator=(const List342<T>& other)
{
  // Check if the assignment is the same object
  if(this == &other)
  {
    return *this; // Returns the original object
  }

  // Check if the lists contain the same nodes
  if(*this == other)
  {
    return *this;
  }

  // Empty destination list of elements.
  DeleteList();

  // Iterate through source list and insert the nodes into destination list
  Node* current = other.head_;
  while(current != nullptr)
  {
    // Insert function checks for duplicates, makes a deep copy, and inserts in sorted order
    this->Insert(current->data);
    current = current->next;
  }
  return *this; // Return updated list
}

// Checks if two lists are equal
template<class T>
bool List342<T>::operator==(const List342<T>& other) const
{
  // Checks if same list
  if(&other == this)
  {
    return true;
  }
  // Checks if both lists are empty
  if(head_ == nullptr && other.head_ == nullptr)
  {
    return true;
  }
  // Checks if one of the lists is empty but the other is not
  if(head_ == nullptr || other.head_ == nullptr)
  {
    return false;
  }

  // Check sizes
  if(this->Size() != other.Size())
  {
    return false;
  }

  // If all conditions passed, compares each object in the list
  Node* current = other.head_;
  while(current != nullptr)
  {
    if(exists(current->data) == false)
    {
      return false;
    }
    current = current->next;
  } 
  return true;
}

// Checks if two lists are not equal
template<class T>
bool List342<T>::operator!=(const List342<T>& other) const
{
  return !(*this == other);  
}

// Output stream operator. Outputs list data to console
template<class T>
std::ostream& operator<<(std::ostream& stream, const List342<T>& other)
{
  // Iterates through list and returns object data to stream
  typename List342<T>::Node* current = other.head_;
  while(current != nullptr)
  {
    stream << *(current->data); 
    current = current->next;                      
  }
  return stream;
}

// private helper method to check for duplicates
template<class T>
bool List342<T>::exists(const T* object) const
{
  // Check if valid pointer
  if(object == nullptr)
  {
    return false;
  }

  // Compares the given object against all objects in the list
  Node* current = head_;
  while(current != nullptr)
  {
    if(*(current->data) == *object) 
    {
      return true;
    }
    current = current->next;
  }
  return false;
}

// Private helper method for + and += operators
template<class T>
bool List342<T>::additionHelper(const List342<T>& list1)
{
  // Check if same list
  if(this == &list1)
  {
    return false;
  }

  // Check if objects in both lists are the same
  if(*this == list1)
  {
    return false;
  }

  // Iterates through given list and checks for duplicates before inserting elements to this list.
  Node* current = list1.head_;
  while(current != nullptr)
  {
    if(exists(current->data))
    {
      current = current->next;
    } 
    else
    {
      this->Insert(current->data);
      current = current->next;
    }
  }
  return true;
}

#endif