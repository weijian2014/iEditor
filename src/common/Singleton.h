#ifndef IEDITOR_SRC_COMMON_SINGLETON_H_
#define IEDITOR_SRC_COMMON_SINGLETON_H_

#include <assert.h>
#include <mutex>

namespace common {
/// 该单例模板内部实现为线程安全的，在只声明无定义的类型上会编译报错（类型有Complete
/// type和Incomplete type之分，对于Complete type,
/// 它的大小在编译时是可以确定的，而对于Incomplete type,
/// 它的大小在编译时是不能确定的。所以，上面的代码中，如果C是Incomplete
/// type的话， sizeof就会在编译时报错，从而达到了我们检查C是否是Complete
/// type的目的）。传入的类型需要自己确保线程安全，带有无参构造函数和析构函数（Complete
/// type类型） 只支持无参的T类型构造, 使用进程中只调用一次的std::call_once,
/// 进程结束前使用std::atexit调用T的析构函数
template <typename T>
class Singleton final {
public:
   Singleton() = delete;

   Singleton(const Singleton&) = delete;

   Singleton& operator=(const Singleton&) = delete;

   Singleton(Singleton&&) = delete;

   /// 只在进程第一次调用instance时，
   /// 才使用T类型的无参构造函数及参数列表在堆上构造一个T*指针,
   /// 之后的instance调用返回一个单件指针
   static T* instancePointer() {
      std::call_once(flag_, []() { init(); });

      assert(nullptr != singleton_);
      return singleton_;
   }

private:
   static void init() {
      singleton_ = new T();
      std::atexit(destroy);
   }

   static void destroy() {
      // 当T为不完整类型时编译报错。(当T为不完整类型时，sizeof(T)给出的是0，根据代码规则，-1是不能作为数组的size的，因此，这里相当于强制编译器给出error
      // C++11编译期已经可以检查出来
      // typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
      // T_must_be_complete_type dummy; (void) dummy;

      delete singleton_;
      singleton_ = nullptr;
   }

private:
   static std::once_flag flag_;
   static T* singleton_;
};

template <typename T>
std::once_flag Singleton<T>::flag_;

template <typename T>
T* Singleton<T>::singleton_ = nullptr;

}  // namespace common

#endif  // IEDITOR_SRC_COMMON_SINGLETON_H_
