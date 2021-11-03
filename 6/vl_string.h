/**
 * @brief vl string
 */

#ifndef VLSTRING_H
#define VLSTRING_H

#include <cstddef>
#include <iterator>
#include <ostream>
#include <iostream>
#include "vl_vector.h"
#include "algorithm"
#include <cstring>

/**
 * @brief error exception thrown
 */
#define OUT_OF_RANGE "index out of range"
/**
 * @brief size of default capacity
 */
#define CAP 16
/**
 * @brief endstr
 */
#define END '\0'
/**
 * @brief index zero
 */
#define ZERO 0
/**
 * @brief size used for cap calculations and sizes uses
 */
#define ONE_SIZE 1
/**
 * @brief Stringrep with vector stl
 * @tparam StaticCapacity thegiven capacity or default
 */


template<size_t StaticCapacity = CAP>
class VLString {
 private:
  VLVector<char, StaticCapacity> string_;

 public:
/**
 * @brief constrcutor
 */
  VLString() {
    string_.PushBack(END);

  }
/**
 * @brief copy constructor
 * @param other
 */
  VLString(const VLString &other) {
    string_ = other.string_;

  }
/**
 * @brief constructor
 * @param string
 */
  VLString(const char *string) {
    char character;
    while ((*string) != END) {
      character = *string;
      string_.PushBack(character);
      string++;
    }
    string_.PushBack(END);
  }
/**
 * @brief GET SIZE
 * @return SIZE of str
 */
  size_t Size() {
    if (string_.Size() == ZERO) {
      return ZERO;
    }
    size_t real_size = string_.Size() - ONE_SIZE;
    return real_size;
  }
/**
 * @brief GET CAPACITY
 * @return CAPACITY
 */
  size_t Capacity() {
    return string_.Capacity();
  }
/**
 * @brief IS EMPTY
 * @return IS EMPTY
 */
  bool Empty() {
    return string_.Empty();
  }
/**
 * @brief AT INDEX
 * @param index
 * @return AT INDEX CHAR IN STRING
 */
  char &At(size_t index) {
    if (index >= this->Size()) {
      throw std::out_of_range(OUT_OF_RANGE);
    } else {
      return string_.At(index);
    }
  }
/**
 * @brief AT INDEX
 * @param index
 * @return AT INDEX CHAR IN STRING
 */
  const char &At(size_t index) const {
    if (index >= this->Size()) {
      throw std::out_of_range(OUT_OF_RANGE);
    } else {
      return string_.At(index);
    }

  }
/**
 * @brief push back
 * @param letter to push
 */
  void PushBack(char letter) {
    string_.PushBack(letter);
  }
/**
 * @brief popback
 */
  void PopBack() {
    return string_.PopBack();
  }
/**
 * @brief clear
 */
  void Clear() {
    string_.Clear();
  }
/**
 * @brief get data
 * @return get data
 */
  char *Data() {
    return string_.Data();
  }
  /**
   * @brief get data
   * @return get data
   */
  const char *Data() const noexcept {
    return string_.Data();
  }

  /**
   * @brief typedefs for the iterator
   */
  typedef char *iterator;
  typedef const char *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
/**
 * @brief begin
 * @return begin
 */
  iterator begin() noexcept {
    return string_.begin();
  }
/**
 * @brief end
 * @return end
 */
  iterator end() noexcept {
    return (string_.begin() + string_.Size());
  }
/**
 * @brief cbegin
 * @return cbegin
 */
  const_iterator cbegin() const noexcept {
    return string_.begin();
  }
/**
 * @brief cend
 * @return cend
 */
  const_iterator cend() const noexcept {
    return (string_.cbegin() + string_.Size());
  }
/**
 * @brief begin
 * @return begin
 */
  const_iterator begin() const noexcept {
    return string_.begin();
  }
/**
 * @brief end
 * @return end
 */
  const_iterator end() const noexcept {
    return (string_.cbegin() + string_.Size());
  }
/**
 * @brief rbegin
 * @return rbegin
 */
  reverse_iterator rbegin() noexcept {
    return reverse_iterator(string_.end());
  }
/**
 * @brief rend
 * @return  rend
 */
  reverse_iterator rend() noexcept {
    return reverse_iterator(string_.begin());
  }
/**
 * @brief rbegin
 * @return rbegin
 */
  const_reverse_iterator rbegin() const noexcept {
    return reverse_iterator(string_.end());
  }
/**
 * @brief rend
 * @return rend
 */
  const_reverse_iterator rend() const noexcept {
    return reverse_iterator(string_.begin());
  }
/**
 * @brief erase
 * @param first from wehre to erase
 * @param last not included - until erase
 * @return the rifht elem after erased first
 */
  iterator Erase(const_iterator first, const_iterator last) {
    return string_.Erase(first, last);
  }
  /**
   * @brief erase
   * @param pos
   * @return right elem after erased
   */
  iterator Erase(const_iterator pos) {
    return string_.Erase(pos);
  }
  /**
   * @brief insert
   * @tparam InputIterator
   * @param position
   * @param first
   * @param last
   * @return insert
   */
  template<class InputIterator>
  iterator Insert(const_iterator position, InputIterator first, InputIterator
  last) {
    return string_.Insert(position, first, last);

  }
  /**
   * @brief insert
   * @param position
   * @param newElem
   * @return insert
   */
  iterator Insert(const_iterator position, const char &newElem) {
    return string_.Insert(position, newElem);
  }
  /**
   * @brief operator=
   * @param x
   * @return operator=
   */
  VLString &operator=(const VLString &x) noexcept {
    if (this == &x) {
      return *this;
    }
    string_ = x.string_;
    return *this;
  }
  /**
   * @brief operator[]
   * @param n
   * @return operator[]
   */
  char &operator[](size_t n) noexcept {
    return string_[n];
  }
  /**
   * @brief operator[]
   * @param n
   * @return operator[]
   */
  char operator[](size_t n) const noexcept {
    return string_[n];
  }
  /**
   * @brief operator==
   * @param other
   * @return operator==
   */
  bool operator==(const VLString &other) const {
    return string_ == other.string_;
  }
  /**
   * @brief
   * @param other
   * @return
   */
  bool operator!=(const VLString &other) const {
    return !(*this == other);
  }
  /**
   * @brief operator+=
   * @param letter
   * @return operator+=
   */
  VLString &operator+=(const char letter) {
    std::string str;
    string_.PushBack(letter);
    auto iter = this->begin();
    for (size_t i = 0; i < this->Size(); i++) {
      str += *iter;
      iter++;
    }
    return *this;
  }
  /**
   * @brief operator+=
   * @param string
   * @return operator+=
   */
  VLString &operator+=(const char *string) {
    std::string str;
    string_.PopBack();
    if (Size() != ZERO) {
    }
    int len = strlen(string);
    const_iterator beg = string;
    string_.Insert(this->end(), beg, beg + len + ONE_SIZE);

    return *this;
  }
  /**
   * @brief operator+=
   * @param vector
   * @return operator+=
   */
  VLString &operator+=(VLVector<char> vector) {
    std::string str;
    this->Insert(this->end(), vector.begin(), vector.begin() + vector.Size());
    auto iter = this->begin();
    for (size_t i = 0; i < this->Size(); i++) {
      str += *iter;
      iter++;
    }
    return *this;
  }
  /**
   * @brief operator+
   * @param letter
   * @return operator+
   */
  VLString &operator+(const char letter) {
    VLString str(this);
    str.string_.PushBack(letter);
    return *str;
  }
  /**
   * @brief operator +
   * @param string
   * @return operator+
   */
  VLString &operator+(const char *string) {
    VLString str(this);
    while ((*string) != END) {
      str.PushBack(*string);
      str++;
    }
    str.PushBack(*string);
    return *str;
  }
  /**
   * @brief  operator+
   * @param vector
   * @return operator+
   */
  VLString &operator+(VLVector<char> vector) {
    VLString str(this);
    str.Insert(this->end(), vector.begin(), vector.begin() + vector.Size());
    return *str;
  }
  /**
   * @brief conversion
   * @return  conversion
   */
  operator const char *() const {
    return this->Data();
  }

};

#endif //VLSTRING_H
