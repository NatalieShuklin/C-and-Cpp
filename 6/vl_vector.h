/**
 * @brief vl_vector represents generic vector container, which
 * makes efficient use of memory

 */

#ifndef VL_VECTOR_H_
#define VL_VECTOR_H_

#include <cstddef>
#include <iterator>
#include <ostream>
#include <iostream>
#include "algorithm"
/**
 * @brief index zero
 */
#define ZERO 0
/**
 * @brief size of vector initialization
 */
#define INITIAL_SIZE 0
/**
 * @brief error exception thrown
 */
#define OUT_OF_RANGE "index out of range"
/**
 * @brief error bad alloc
 */
#define BAD_ALLOC "Bad alloc error"
/**
 * @brief size of default capacity
 */
#define CAP 16
/**
 * @brief size used for cap calculations
 */
#define THREE_SIZE 3
/**
 * @brief size used for cap calculations
 */
#define TWO_SIZE 2
/**
 * @brief size used for cap calculations and sizes uses
 */
#define ONE_SIZE 1

/**
 * @brief Data Structure VLVector generic container
 * @tparam T template structure
 * @tparam StaticCapacity - a static capacity of the container
 */
template<class T, size_t StaticCapacity = CAP>
class VLVector {
 private:
  /*
   * capacity of the container
   */
  size_t capacity_;
  /*
   * size of the container
   */
  size_t size_;
  /*
   * the heap memory of container
   */
  T *dynamic_array_;
  /*
   * the stack memory of container
   */
  T static_array_[StaticCapacity];
  /*
   * is current container using the stack memory
   */
  bool is_static_{};
  /*
   * is current container using the heap memory
   */
  bool is_dynamic_{};

 public:
  /*
   * iterator on the container
   */
  typedef T *iterator;
  /*
   * const iterator on the container
   */
  typedef const T *const_iterator;
  /*
   * reverse iterator
   */
  typedef std::reverse_iterator<iterator> reverse_iterator;
  /*
   * const reverse iterator
   */
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  /**
   * @brief begin const iterator
   * @return begin const iterator to current data of the container set to the
   * start
   * of it -
   */
  const_iterator begin() const noexcept {
    return &(Data()[ZERO]);
  }
  /**
   * @brief end iterator
   * @return end iterator
   */
  const_iterator end() const noexcept {
    return (this->Size() + this->cbegin());
  }
  /**
   * @brief const cbegin iterator
   * @return const cbegin iterator
   */
  const_iterator cbegin() const noexcept {
    return &(Data()[ZERO]);
  }
  /**
   * @brief CONST END CEND ITERATOR
   * @return CONST END CEND ITERATOR
   */
  const_iterator cend() const noexcept {
    return (this->Size() + this->cbegin());
  }
  /**
   * @brief begin iterator
   * @return begin iterator to current data of the container set to the start
   * of it
   */
  iterator begin() noexcept {
    return &(Data()[ZERO]);
  }
  /**
   * @brief END ITERATOR
   * @return END ITERATOR
   */
  iterator end() noexcept {
    return (this->begin() + this->Size());
  }
  /**
   * @brief const reverse iterator begin
   * @return const reverse iterator begin
   */
  const_reverse_iterator rbegin() const noexcept {
    return reverse_iterator(this->end());
  }
  /**
   * @brief const reverse iterator end
   * @return const reverse iterator end
   */
  const_reverse_iterator rend() const noexcept {
    return reverse_iterator(this->begin());
  }
  /**
   * @brief reverse iterator begin
   * @return reverse iterator begin
   */
  reverse_iterator rbegin() noexcept {
    return reverse_iterator(this->end());
  }
  /**
   * @brief reverse iterator end
   * @return reverse iterator end
   */
  reverse_iterator rend() noexcept {
    return reverse_iterator(this->begin());
  }

  /**
   * @brief default constructor initializes empty VLVector
   */
  VLVector()
  try : capacity_(StaticCapacity), size_(INITIAL_SIZE), dynamic_array_(new T
  [StaticCapacity]) {
    is_static_ = true;
    is_dynamic_ = false;
  }
  catch (std::bad_alloc &ba) {
    std::cerr << BAD_ALLOC;
  }

  /**
   * @brief copy constructor of VLVector
   * @param other vector to copy from
   */
  VLVector(const VLVector &other) {
    try {
      capacity_ = other.capacity_;
      size_ = other.size_;
      if (other.is_dynamic_) {
        is_dynamic_ = true;
        is_static_ = false;
        dynamic_array_ = new T[other.capacity_];
        for (size_t i = 0; i < other.size_; i++) {
          dynamic_array_[i] = other.dynamic_array_[i];
        }
      } else if (other.is_static_) {
        dynamic_array_ = new T[StaticCapacity];
        is_dynamic_ = false;
        is_static_ = true;
        for (unsigned int i = 0; i < other.size_; i++) {
          static_array_[i] = other.static_array_[i];
        }
      }
    }
    catch (std::bad_alloc &ba) {
      std::cerr << "bad_alloc caught";
    }
  }

  /**
   * @brief Sequence based constructor, constrcuts vector from given range input
   * @tparam InputIterator input iterator
   * @param first iterator points to first elem
   * @param last iterator points to last elem
   */
  template<class InputIterator>
  VLVector(InputIterator first, InputIterator last)
  try :capacity_(StaticCapacity),
       size_(INITIAL_SIZE),
       dynamic_array_(new T[StaticCapacity]) {
    size_t count = INITIAL_SIZE;
    for (InputIterator it = first; it != last; it++) {
      count++;
    }
    capacity_ = CalculateCapC(count);
    is_static_ = true;
    for (InputIterator iter = first; iter != last; iter++) {
      this->PushBack(*iter);
    }
  }
  catch (std::bad_alloc &ba) {
    std::cerr << BAD_ALLOC;
  }

  /**
   * @brief calculates the capacity for the VLVector
   * @return CURRENT VECTOR CAPACITY
   */
  size_t CalculateCapC(int k) {
    size_t size = size_;
    if (size + k <= StaticCapacity) {
      return StaticCapacity;
    } else {
      return (THREE_SIZE * (size + k)) / TWO_SIZE;
    }
  }

  /**
   * @brief destructor of vector
   */
  ~VLVector() {
    delete[] dynamic_array_;
  }

  /**
   * @brief copying from one dynamic array to another and enlarging size
   * @param k given amount of added elems
   */
  void CopyDynamicArrays(int k) {
    try {
      size_t cap_c = CalculateCapC(k);
      capacity_ = cap_c;
      T *new_arr_dyn = new T[capacity_];
      for (size_t i = 0; i < size_; i++) {
        new_arr_dyn[i] = dynamic_array_[i];
      }
      delete[] dynamic_array_;
      dynamic_array_ = new_arr_dyn;
      is_dynamic_ = true;
      is_static_ = false;
    }
    catch (std::bad_alloc &ba) {
      std::cerr << BAD_ALLOC;
    }
  }

  /**
   * @brief copies static array into dynamic new array of new size
   * @param k  given added elems now
   */
  void CopyStaticToDynamic(int k) {
    try {
      size_t cap_c = CalculateCapC(k);
      capacity_ = cap_c;
      delete[] dynamic_array_;
      dynamic_array_ = new T[cap_c];
      for (size_t i = 0; i < size_; i++) {
        dynamic_array_[i] = static_array_[i];
      }
      is_static_ = false;
      is_dynamic_ = true;
    }
    catch (std::bad_alloc &ba) {
      std::cerr << BAD_ALLOC;
    }
  }

  /**
   * @brief erase assist function - copies dynamic to static
   */
  void EraseCopyDynamicToStatic() {
    try {
      capacity_ = StaticCapacity;
      for (size_t i = 0; i < size_; i++) {
        static_array_[i] = dynamic_array_[i];
      }
      delete[] dynamic_array_;
      dynamic_array_ = new T[StaticCapacity];
      is_static_ = true;
      is_dynamic_ = false;
    }
    catch (std::bad_alloc &ba) {
      std::cerr << BAD_ALLOC;
    }
  }

  /**
   * @brief returns number of elements in the VLVector
   * @return number of elements in the VLVector
   */
  size_t Size() const noexcept {
    return size_;
  }

  /**
   * @brief gets the capacity of the VLVector defined by the capC function
   * @return the capacity of the VLVector currently
   */
  size_t Capacity() const noexcept {
    return capacity_;
  }

  /**
   * @brief returns true if the VLVector has no elements inside, else false
   * @return  true if the VLVector has no elements inside, else false
   */
  bool Empty() const noexcept {
    return size_ == ZERO;
  }

  /**
   * @brief returns the reference to a value inside VLVector that sits at given
   * index
   * @param index GIVEN INDEX TO LOOK FOR
   * @return reference to a value inside VLVector that sits at given
   * index
   */
  T &At(size_t index) {
    if (index >= size_) {
      throw std::out_of_range(OUT_OF_RANGE);
    }
    if (is_dynamic_) {
      return dynamic_array_[index];
    }
    return static_array_[index];
  }

  /**
   * @brief returns the const reference to a value inside VLVector that sits at
   * given index
   * @param index GIVEN INDEX
   * @return const reference to a value inside VLVector that sits at given index
   */
  const T &At(size_t index) const {
    if (index >= size_) {
      throw std::out_of_range(OUT_OF_RANGE);
    } else {
      if (is_dynamic_) {
        return dynamic_array_[index];
      }
      if (is_static_) {
        return static_array_[index];
      }
    }
  }

  /**
   * @brief insets new value at the end of the vector
   * @param new_elem the elem to be pushed
   */
  void PushBack(T new_elem) {
    if (is_static_) {
      if (size_ + ONE_SIZE > capacity_) {
        CopyStaticToDynamic(ONE_SIZE);
        dynamic_array_[size_] = new_elem;
      } else {
        static_array_[size_] = new_elem;
      }
    } else if (is_dynamic_) {
      if (size_ + ONE_SIZE > capacity_) {
        CopyDynamicArrays(ONE_SIZE);
        dynamic_array_[size_] = new_elem;
      } else {
        dynamic_array_[size_] = new_elem;
      }
    }
    size_++;
  }

  /**
   * @brief get index array of the position iterator pointed to
   * @param position -iterator that points to elem
   * @return the index in the array where positin points to
   */
  size_t GetIndex(const_iterator position) {
    size_t index = ZERO;
    if (is_static_) {
      for (const_iterator it = (*this).begin(); it != position; it++) {
        index++;
      }
    }
    if (is_dynamic_) {
      for (const_iterator it = (*this).begin(); it != position; it++) {
        index++;
      }
    }
    return index;
  }

  /**
   * @brief insert (1) second func - copies all elements and adds new_elem in
   * correct place
   * @param index the index where to insert
   * @param new_elem the elem to insert
   */
  void InsertStaticArray(size_t index,
                         const T &new_elem) {
    T array[StaticCapacity];
    size_t new_index = ZERO;
    for (size_t i = index; i < size_; i++) {
      array[new_index] = static_array_[i];
      new_index++;
    }
    static_array_[index] = new_elem;
    size_++;
    new_index = ZERO;
    for (size_t j = index + ONE_SIZE; j < size_; j++) {
      static_array_[j] = array[new_index];
      new_index++;
    }
  }

  /**
   * @brief insert (1) second function - inserts new elem in right place,
   * and copies array
   * @param index given index
   * @param new_elem elem to add
   */
  void InsertDynamicArray(size_t index,
                          const T &new_elem) {
    try {
      T *n_arr = new T[capacity_];
      size_t index_new = ZERO;
      for (size_t i = index; i < size_; i++) {
        n_arr[index_new] = dynamic_array_[i];
        index_new++;
      }
      index_new = ZERO;
      dynamic_array_[index] = new_elem;
      size_++;
      for (size_t j = index + ONE_SIZE; j < size_; j++) {
        dynamic_array_[j] = n_arr[index_new];
        index_new++;
      }
      delete[] n_arr;
    }
    catch (std::bad_alloc &ba) {
      std::cerr << BAD_ALLOC;
    }
  }

  /**
   * @brief insert 1 INSERTS ELEMENT IN THE POINTED POSITION
   * @param position given position
   * @param new_elem elem to insert
   * @return iterator to the new inserted element
   */
  iterator Insert(const_iterator position, const T &new_elem) {
    iterator it;
    size_t index = GetIndex(position);
    if (is_static_) {
      if (size_ + ONE_SIZE > capacity_) {
        CopyStaticToDynamic(ONE_SIZE);
        InsertDynamicArray(index, new_elem);
      } else {
        InsertStaticArray(index, new_elem);
      }
    } else if (is_dynamic_) {
      if (size_ + ONE_SIZE > capacity_) {
        CopyDynamicArrays(ONE_SIZE);
        InsertDynamicArray(index, new_elem);
      } else {
        InsertDynamicArray(index, new_elem);
      }
    }
    it = this->begin();
    for (size_t i = 0; i < index; i++) {
      it++;
    }
    return it;
  }

  /**
   * @brief count the number of element in the given range
   * @tparam InputIterator
   * @param first range start
   * @param last range end
   * @return number of elems in range, not including the last
   */
  template<class InputIterator>
  size_t GetRangeCount(InputIterator first, InputIterator last) {
    size_t range = ZERO;
    for (auto it = first; it != last; it++) {
      range++;
    }
    return range;
  }

  /**
   * @brief insert (2) - second func - inserts new values from range and
   * copies arrays
   * @tparam InputIterator input iterator
   * @param index given index
   * @param first fist pointed to
   * @param last last pointed to
   */
  template<class InputIterator>
  void InsertStaticRangeArrays(size_t index,
                               InputIterator first,
                               InputIterator last) {
    T array[StaticCapacity];
    for (size_t i = 0; i < capacity_; i++) {
      array[i] = static_array_[i];
    }
    size_t copy_index = index;
    for (auto it = first; it != last; it++) {
      static_array_[index] = *it;
      index++;
    }
    for (size_t j = index; j < capacity_; j++) {
      static_array_[j] = array[copy_index];
      copy_index++;
    }
  }

  /**
   * @brief insert(2) second func - insert elemnts from range and copies arrays
   * @tparam InputIterator iterator input
   * @param index index where to insert
   * @param first first position range
   * @param last last position range
   */
  template<class InputIterator>
  void InserDynamicRangeArrays(size_t index,
                               InputIterator first,
                               InputIterator last) {
    try {
      T *n_arr = new T[capacity_];
      for (size_t i = 0; i < capacity_; i++) {
        n_arr[i] = dynamic_array_[i];
      }
      size_t copy_index = index;
      for (auto it = first; it != last; it++) {
        dynamic_array_[index] = *it;
        index++;
      }
      for (size_t j = index; j < capacity_; j++) {
        dynamic_array_[j] = n_arr[copy_index];
        copy_index++;
      }
      delete[] n_arr;
    }
    catch (std::bad_alloc &ba) {
      std::cerr << BAD_ALLOC;
    }
  }

  /**
   * @brief insert (2) - inserts iterator range input elements before position
   * place in VLVector
   * @tparam InputIterator input iterator
   * @param position position where to insert
   * @param first pointed to first in range
   * @param last pointed to last in range
   * @return iterator that points to first elem that was added currently
   */
  template<class InputIterator>
  iterator Insert(const_iterator position, InputIterator first, InputIterator
  last) {
    iterator it;
    size_t index = GetIndex(position);
    size_t range_count = GetRangeCount(first, last);
    if (is_static_) {
      if (size_ + range_count > capacity_) {
        CopyStaticToDynamic(range_count);
        InserDynamicRangeArrays(index, first, last);
      } else {
        InsertStaticRangeArrays(index, first, last);
      }
    } else if (is_dynamic_) {
      if (size_ + range_count > capacity_) {
        CopyDynamicArrays(range_count);
        InserDynamicRangeArrays(index, first, last);
      } else {
        InserDynamicRangeArrays(index, first, last);
      }
    }
    size_ += range_count;
    it = this->begin();
    for (size_t i = 0; i < index; i++) {
      it++;
    }
    return it;
  }

  /**
   * @brief removes last element from VLVector
   */
  void PopBack() {
    if (size_ == ZERO) {
      return;
    }
    iterator iter = this->begin();
    for (size_t i = 0; i < size_; i++) {
      iter++;
    }
    this->Erase(iter);
  }

  /**
   * @brief erase (1) 2nd func - erasing in a static array;
   * @param pos where to erase
   */
  void EraseStaticArr(const_iterator pos) {
    T array[StaticCapacity];
    size_t index = GetIndex(pos);
    for (size_t i = index; i < size_; i++) {
      array[i] = static_array_[i];
    }
    size_t old_size = size_;
    size_--;
    size_t new_index = index;
    new_index++;
    for (unsigned int j = index; j < old_size; j++) {
      static_array_[j] = array[new_index];
      new_index++;
    }
  }

  /**
   * @brief erase (1) 2nd func - erasing in a dynamic array;
   * @param pos the position where to erase
   */
  void EraseDynamicArr(const_iterator pos) {
    try {
      T *new_array = new T[capacity_];
      size_t index = GetIndex(pos);
      for (size_t i = index; i < capacity_; i++) {
        new_array[i] = dynamic_array_[i];
      }
      size_t old_size = size_;
      size_--;
      size_t new_index = index;
      new_index++;
      for (size_t j = index; j < old_size; j++) {
        dynamic_array_[j] = new_array[new_index];
        new_index++;
      }
      delete[] new_array;
    }
    catch (std::bad_alloc &ba) {
      std::cerr << BAD_ALLOC;
    }
  }

  /**
   * @brief erasing element in pos place from VLVector, adjusting arrays
   * correctly, if dynamic
   * array is
   * in the right size for StaticCapacity - change it to static array
   * @param pos posiiton to erase
   * @return iterator to next elem from the right of the deleted one
   */
  iterator Erase(const_iterator pos) {
    iterator iter;
    size_t index = GetIndex(pos);
    if (is_static_) {
      EraseStaticArr(pos);
    } else if (is_dynamic_) {
      EraseDynamicArr(pos);
      if (size_ <= StaticCapacity) {
        EraseCopyDynamicToStatic();
      }
    }
    iter = (*this).begin();
    for (size_t i = 0; i < index; i++) {
      iter++;
    }
    return iter;
  }

  /**
   * @brief erase (2) 2nd func - erasing in a static array
   * @param first position to first elem in range
   * @param last position to last elem in range
   */
  void EraseStaticArrRange(const_iterator first, const_iterator last) {
    T new_array[capacity_];
    size_t index = GetIndex(first);
    iterator iter;
    size_t index2 = GetIndex(last);
    size_t range = index2 - index;
    for (size_t i = 0; i < size_; i++) {
      new_array[i] = static_array_[i];
    }
    size_ -= range;
    for (size_t j = index; j <= size_; j++) {
      static_array_[j] = new_array[index2];
      index2++;
    }
  }

  /**
   * @brief erase(2) 2nd func - erasing in a static array
   * @param first position to first elem in range
   * @param last position to last elem in range
   */
  void EraseDynamicArrRange(const_iterator first, const_iterator last) {
    try {
      T *array = new T[capacity_];
      size_t index = GetIndex(first);
      size_t index2 = GetIndex(last);
      size_t range = index2 - index;
      for (unsigned int i = 0; i < size_; i++) {
        array[i] = dynamic_array_[i];
      }
      size_ -= range;
      for (unsigned int j = index; j <= size_; j++) {
        dynamic_array_[j] = array[index2];
        index2++;
      }
      delete[] array;
    }
    catch (std::bad_alloc &ba) {
      std::cerr << BAD_ALLOC;
    }
  }

  /**
   * @brief erases given range of elements in VLVector
   * @param first position to first elem in range
   * @param last position to last elem in range
   * @return iterator to the elem from the right of the deleted elems
   */
  iterator Erase(const_iterator first, const_iterator last) {
    size_t index = GetIndex(first);
    iterator iter;
    if (is_static_) {
      EraseStaticArrRange(first, last);
    }
    if (is_dynamic_) {
      EraseDynamicArrRange(first, last);
      if (size_ <= StaticCapacity) {
        EraseCopyDynamicToStatic();
      }
    }
    iter = (*this).begin();
    for (size_t i = 0; i < index; i++) {
      iter++;
    }
    return iter;
  }

  /**
   * @brief erases all elements from the VLVector
   */
  void Clear() {
    if (is_static_) {
      size_ = ZERO;
    }
    if (is_dynamic_) {
      size_ = ZERO;
      delete[] dynamic_array_;
      dynamic_array_ = new T[StaticCapacity];
      is_static_ = true;
      is_dynamic_ = false;
      capacity_ = StaticCapacity;
    }
  }

  /**
   * @brief returns pointer to first place of our data structure
   * @return  pointer to first place of our data structure
   */
  T *Data() noexcept {
    if (is_static_) {
      return static_array_;
    }
    return dynamic_array_;
  }

  /**
   * @brief returns const pointer to first place of our data structure
   * @return const pointer to first place of our data structure
   */
  const T *Data() const noexcept {
    if (is_static_) {
      return static_array_;
    } else if (is_dynamic_) {
      return dynamic_array_;
    }
  }

  /**
   * @brief returns reference to element in vector at place n
   * @param n INDEX
   * @return reference to element in vector at place n
   */
  T &operator[](size_t n) noexcept {
    if (is_static_) {
      return static_array_[n];
    }
    return dynamic_array_[n];
  }

  /**
   * @brief returns element value in vector at place n
   * @param n INDEX
   * @return element value in vector at place n
   */
  T operator[](size_t n) const noexcept {
    if (is_static_) {
      return static_array_[n];
    }
    if (is_dynamic_) {
      return dynamic_array_[n];
    }
  }

  /**
   * @brief assignment operator for vectors
   * @param x
   * @return returns reference to vector after assignment
   */
  VLVector &operator=(const VLVector &x) noexcept {
    if (this == &x) {
      return *this;
    }
    size_ = x.size_;
    capacity_ = x.capacity_;
    is_dynamic_ = x.is_dynamic_;
    is_static_ = x.is_static_;
    if (is_static_) {
      for (size_t i = 0; i < x.size_; i++) {
        static_array_[i] = x.static_array_[i];
      }
    } else if (is_dynamic_) {
      delete[] dynamic_array_;
      try {
        dynamic_array_ = new T[capacity_];
        for (size_t i = 0; i < x.size_; i++) {
          dynamic_array_[i] = x.dynamic_array_[i];
        }
      }
      catch (std::bad_alloc &ba) {
        std::cerr << BAD_ALLOC;
      }
    }
    return *this;
  }

  /**
   * @brief returns true if both vectors  are the same element wise
   * @param other other vector
   * @return true if equal else false
   */
  bool operator==(const VLVector &other) const {
    if (size_ != other.size_) {
      return false;
    }
    if (is_static_) {
      for (unsigned int i = 0; i < size_; i++) {
        if (static_array_[i] != other.static_array_[i]) {
          return false;
        }
      }
    }
    if (is_dynamic_) {
      for (unsigned int i = 0; i < size_; i++) {
        if (dynamic_array_[i] != other.dynamic_array_[i]) {
          return false;
        }
      }
    }
    return true;
  }

  /**
   * @brief returns true if both vectors  are not the same element wise
   * @param other vector to comapre to
   * @return true if not equal, else false
   */
  bool operator!=(const VLVector &other) const {
    return !(*this == other);
  }

};

#endif //VL_VECTOR_H_
