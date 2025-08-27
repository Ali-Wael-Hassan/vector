#pragma once
#include <stdexcept>
#include <algorithm>
#include <memory>

namespace my {
  template <class T>
  class vector {
    private:
      size_t capacity_, curSize_;
      T* arr;
      void makeNew(int sze) {
        T* newArr = static_cast<T*>(::operator new(sze * sizeof(T)));
        for(size_t i = 0; i < this->curSize_; ++i) {
          new(&newArr[i]) T(std::move(arr[i]));
          arr[i].~T();
        }
        ::operator delete(arr);
        this->arr = newArr, this->capacity_ = sze;
      }
    public:
      // Constractors
      vector():capacity_(0),curSize_(0), arr(nullptr) {}
      vector(int sze):capacity_(2*sze),curSize_(sze) {
        arr = static_cast<T*>(::operator new(this->capacity_ * sizeof(T)));
        for(size_t i = 0; i < this->curSize_; ++i) new(&arr[i]) T();
      }
      vector(int sze, const T& elem):capacity_(2*sze),curSize_(sze) {
        arr = static_cast<T*>(::operator new(this->capacity_ * sizeof(T)));
        for(size_t i = 0; i < this->curSize_; ++i) new(&arr[i]) T(elem);
      }

      // Copy Constractor
      vector(const vector& other):capacity_(other.capacity_),curSize_(other.curSize_) {
        arr = static_cast<T*>(::operator new(this->capacity_ * sizeof(T)));
        for(size_t i = 0; i < this->curSize_; ++i) new(&arr[i]) T(other.arr[i]);
      }
      vector& operator=(const vector& other) {
        if(&other == this) return *this;
        vector tmp(other);
        swap(tmp);
        return *this;
      }
      
      // Move Constractor
      vector(vector&& other) noexcept :capacity_(other.capacity_),curSize_(other.curSize_), arr(other.arr) {
        other.arr = nullptr, other.curSize_ = 0, other.capacity_ = 0;
      }
      vector& operator=(vector&& other) noexcept {
        if(&other == this) return *this;
        for (size_t i = 0; i < curSize_; ++i) arr[i].~T();
        ::operator delete(arr);
        this->arr = other.arr;
        capacity_ = other.capacity_;
        curSize_ = other.curSize_;
        other.arr = nullptr;
        other.curSize_ = 0;
        other.capacity_ = 0;
        return *this;
      }
      // access operators
      T& operator[](size_t i) {
        return arr[i];
      }
      const T& operator[](size_t i) const{
        return arr[i];
      }
      // operations
      void clear() {
        for (size_t i = 0; i < curSize_; ++i) arr[i].~T();
        this->curSize_ = 0;
      }
      void push_back(const T& elem) {
        if(this->curSize_ == this->capacity_) {
          makeNew((this->capacity_ == 0? 1: 2*this->capacity_));
        }
        new(&this->arr[this->curSize_++]) T(elem);
      }

      void push_back(T&& elem) {
        if(this->curSize_ == this->capacity_) {
          makeNew((this->capacity_ == 0? 1: 2*this->capacity_));
        }
        new(&this->arr[this->curSize_++]) T(std::move(elem));
      }
      void pop_back() {
        if(this->curSize_ == 0) throw std::runtime_error("vector is Empty");
        --this->curSize_;
        arr[this->curSize_].~T();
      }
      void swap(vector& other) noexcept {
        std::swap(arr,other.arr);
        std::swap(curSize_,other.curSize_);
        std::swap(capacity_,other.capacity_);
      }
      // setters
      void assign(int sze, const T& elem) {
        if(sze < 0) throw std::invalid_argument("assign with negative size");
        if(this->capacity_ < sze) makeNew(std::max(2*this->capacity_, static_cast<size_t>(sze)));
        size_t minSize = std::min(curSize_, static_cast<size_t>(sze));
        for (size_t i = 0; i < minSize; ++i) arr[i] = elem;
        for (size_t i = sze; i < curSize_; ++i) arr[i].~T();
        for (size_t i = curSize_; i < sze; ++i) new(&arr[i]) T(elem);
        this->curSize_ = sze;
      }
      void reserve(int sze) {
        if(this->capacity_ < sze) {
          makeNew(sze);
        }
      }
      void resize(int sze, const T& val = T()) {
        if (sze < 0) throw std::invalid_argument("resize with negative size");
        if (sze < curSize_) {
          for (size_t i = sze; i < curSize_; ++i) arr[i].~T();
          curSize_ = sze;
        } else {
          if (sze > capacity_) makeNew(std::max(2*capacity_, static_cast<size_t>(sze)));
          for (size_t i = curSize_; i < sze; ++i) new(&arr[i]) T(val);
          curSize_ = sze;
        }
      }
      // getters
      bool empty() const {
        return this->curSize_ == 0;
      }
      int size() const {
        return this->curSize_;
      }
      int capacity() const {
        return this->capacity_;
      }
      T* begin() {
        return this->arr;
      }
      T* end() {
        return this->arr + this->curSize_;
      }
      const T* begin() const {
        return this->arr;
      }
      const T* end() const {
        return this->arr + this->curSize_;
      }
      T& front() {
        if(this->curSize_ == 0) throw std::runtime_error("vector is empty");
        return this->arr[0];
      }
      T& back() {
        if(this->curSize_ == 0) throw std::runtime_error("vector is empty");
        return this->arr[this->curSize_ - 1];
      }
      const T& front() const {
        if(this->curSize_ == 0) throw std::runtime_error("vector is empty");
        return this->arr[0];
      }
      const T& back() const {
        if(this->curSize_ == 0) throw std::runtime_error("vector is empty");
        return this->arr[this->curSize_ - 1];
      }
      //logical operator
      bool operator==(const vector& other) const {
        if(this->curSize_ != other.curSize_) return false;
        for(size_t i = 0; i < this->curSize_; ++i) {
          if(this->arr[i] != other.arr[i]) {
            return false;
          }
        }
        return true;
      }
      bool operator>(const vector& other) const {
        if(*this == other) return false;
        for(size_t i = 0; i < std::min(other.curSize_,this->curSize_); ++i) {
          if(this->arr[i] > other.arr[i]) return true;
          else if(this->arr[i] < other.arr[i]) return false;
        }
        return this->curSize_ > other.curSize_;
      }
      bool operator<(const vector& other) const {
        if(*this == other) return false;
        for(size_t i = 0; i < std::min(other.curSize_,this->curSize_); ++i) {
          if(this->arr[i] < other.arr[i]) return true;
          else if(this->arr[i] > other.arr[i]) return false;
        }
        return this->curSize_ < other.curSize_;
      }
      bool operator!=(const vector& other) const {return !(*this == other);}
      bool operator>=(const vector& other) const {return *this > other||*this == other;}
      bool operator<=(const vector& other) const {return *this < other||*this == other;}
      // destructor
      ~vector() {
        for (size_t i = 0; i < curSize_; ++i) arr[i].~T();
        ::operator delete(arr);
      }
  };
} 