// 显然，shape_wrapper有很多缺陷，我们要对他加以改进

#include <iostream>
#include <memory>

class shared_count {
public:
  shared_count() : count_(1) {}
  void add_count() { count_++; }
  long reduce_count() {
    count_--;
    return count_;
  }
  long get_count() const { return count_; }

private:
  long count_;
};

template <typename T> class smart_ptr {
public:
  explicit smart_ptr(T *ptr = nullptr) : ptr_(ptr) {
    //构造时初始化shared_count_对象
    if (ptr) {
      shared_count_ = new shared_count();
    }
  }
  ~smart_ptr() {
    if (ptr_ != nullptr && !shared_count_->reduce_count()) {
      delete ptr_;
      delete shared_count_;
    }
  }
  T *get() const { return ptr_; }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  operator bool() const { return ptr_; }
  //   T *release() {
  //     T *ptr = ptr_;
  //     ptr_ = nullptr;
  //     return ptr;
  //   }
  void swap(smart_ptr &rhs) {
    std::swap(ptr_, rhs.ptr_);
    std::swap(shared_count_, rhs.shared_count_);
  }
  //拷贝构造函数
  template <typename U> friend class smart_ptr;
  template <typename U> smart_ptr(smart_ptr<U> &other) {
    ptr_ = other.ptr_;
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }
  //移动构造函数
  template <typename U> smart_ptr(smart_ptr<U> &&other) {
    ptr_ = other.ptr_;
    if (ptr_) {
      shared_count_ = other.shared_count_;
      other.shared_count_ = nullptr;
    }
  }

  smart_ptr &operator=(smart_ptr rhs) {
    rhs.swap(*this);
    return *this;
  }

  long use_count() const {
    if (ptr_)
      return shared_count_->get_count();
    else
      return 0;
  }

private:
  T *ptr_;
  shared_count *shared_count_;
};

class shape {
public:
  virtual ~shape() {}
};
class circle : public shape {
public:
  ~circle() { std::cout << "delete circle" << std::endl; }
};
// class triangle : public shape {};
// class rectangle : public shape {};
// class trapezoid : public shape {};
int main() {
  smart_ptr<circle> ptr1(new circle());
  printf("use count of ptr1 is %ld\n", ptr1.use_count());
  smart_ptr<shape> ptr2;
  printf("use count of ptr2 was %ld\n", ptr2.use_count());
  ptr2 = ptr1;
  printf("use count of ptr2 is now %ld\n", ptr2.use_count());
  if (ptr1) {
    puts("ptr1 is not empty");
  }
}