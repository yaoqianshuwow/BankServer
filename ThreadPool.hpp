

#define THREADPOOL_MAX_NUM 12


#include<mutex>
#include<condition_variable>
#include<atomic>
#include<future>//future package_task
#include<thread>
#include<functional>

#include<iostream>
#include<queue>
#include<vector>
using namespace std;


#include <sys/syscall.h> // 定义 SYS_gettid
#include <unistd.h>      // 定义 syscall

// 获取当前线程的 TID（整数类型）
// pid_t get_tid() {
//     return static_cast<pid_t>(syscall(SYS_gettid));
// }
class ThreadPool{

private:
using Task=function<void()>;
mutex mt;
condition_variable cond;
string threadtype_;
atomic<bool>run={true};
atomic<int>idthr={0};

vector<thread>thvec;
queue<Task>taskq;

public:
inline ThreadPool(unsigned short size ,string threadtype)
{  //stop 、empty 和条件变量可以阻塞线程
    threadtype_=threadtype;
    for (; thvec.size() < THREADPOOL_MAX_NUM && size > 0; --size)
   {
   
    thvec.emplace_back([this]{
    std::cout<<threadtype_<< "Thread id is created,thread D (TID): " <<static_cast<pid_t>(syscall(SYS_gettid))<< std::endl;
   while (run)
   {
    
    Task task;

   
   {
   unique_lock<mutex>lok(mt);
   cond.wait(lok, [this] {  // 正确格式：[捕获列表] { 函数体 }
    return (taskq.empty()==false || !run);
   });


   if(taskq.empty()==true&&run==false)return;
    //lambelambe 返回false会阻塞  ！empty=true等价于empty==false
    // 队列有东西或者run是true
    
    
   
   
    task=taskq.front();
    taskq.pop();//放在unique_lock的生命周期里面可以实现互斥
    
}
   idthr--;
    task();
    idthr++;
    
   }

    });
  
    
   }
}
~ThreadPool(){

    run = false;                  // 停止标志
    cond.notify_all();         // 唤醒所有线程
    for (thread& thread : thvec) {
        if (thread.joinable())
            thread.join();         // 等待所有线程结束
    }

}
template<class F,class ...Args>
inline auto addtask(F&& f, Args&&... args) ->future<decltype(f(args...))>
{
    if (!run)    // stoped ??
    throw runtime_error("addtask on ThreadPool is stopped.");

   using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
 
//    auto task = make_shared<packaged_task<RetType()>>(
//     bind(forward<F>(f), forward<Args>(args)...)
//      );//变量在右边？？？
 // 将任务包装为 packaged_task
 auto task = std::make_shared<std::packaged_task<RetType()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
);
   
  {
    

  lock_guard<mutex> lock{mt };
   
  taskq.emplace([task]() {  (*task)();   });

  }

  cond.notify_one();
  return task->get_future();
  
    
}

inline int idnum(){
return thvec.size();

}
inline int idth(){

return idthr;
}
};