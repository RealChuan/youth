# Youth( Linux下C++库) #

### **参考：[muduo](https://github.com/chenshuo/muduo "chenshuo/muduo")**  

1. [examples](./examples)----------例子;  
2. [youth](./youth)----------库;  
   1. [core](./youth/core)
      1. [CountDownLatch](./youth/core/CountDownLatch.h)-----------倒计时器封装；  
      2. [CurrentThread](./youth/core/CurrentThread.h)-----------当前线程ID信息；  
      3. [DateTime](./youth/core/DateTime.hpp)-----------时间日期处理；
      4. [Dir](./youth/core/Dir.h)-----------目录；  
      5. [ElapsedTimer](./youth/core/ElapsedTimer.h)-----------计时器；
      6. [File](./youth/core/File.h)-----------文件；
      7. [FileInfo](./youth/core/FileInfo.h)-----------文件详情；
      8. [Object](./youth/core/Object.h)-----------拷贝<!--值语义-->和不可拷贝<!--对象语义-->类，用于继承；  
      9. [Process](./youth/core/Process.h)-----------进程信息类； 
      10. [StringFunction](./youth/core/StringFunction.h)-----------字符串处理；
      11. [Thread](./youth/core/Thread.hpp)-----------线程/互斥量/条件变量封装类；  
      12. [WeakCallback](./youth/core/WeakCallback.h)-----------在类生命周期内调用回调函数；  
      13. [tests](./youth/core/tests)-----------测试;  
          1. [CountDownLatch_unittext](./youth/core/tests/CountDownLatch_unittext.cc)-----------倒计时类测试；
          2. [DateTime_unittext](./youth/core/tests/DateTime_unittext.cc)-----------时间日期类测试；
          3. [Dir_unittest](./youth/core/tests/Dir_unittest.cc)-----------目录类测试；
          4. [ElapsedTimer_unittext](./youth/core/tests/ElapsedTimer_unittext.cc)-----------计时器类测试；
          5. [File_unittext](./youth/core/tests/File_unittext.cc)-----------文件类测试；  
          6. [FileInfo_unittext](./youth/core/tests/FileInfo_unittext.cc)-----------文件详情类测试；
          7. [Process_unittext](./youth/core/tests/Process_unittext.cc)-----------进程信息类测试；
          8. [StringFunction_unittext](./youth/core/tests/StringFunction_unittext.cc)-----------字符串处理类测试；
          9.  [Thread_unittest](./youth/core/tests/Thread_unittest.cc)-----------线程类测试；
   2. [net](./youth/net)  
      1. [Acceptor](./youth/net/Acceptor.h)-----------TCP服务器请求接受处理类；
      2. [Buffer](./youth/net/Buffer.h)-----------IO缓冲区类；  
      3. [Callback](./youth/net/Callback.h)-----------各种回调函数别名；  
      4. [Channel](./youth/net/Channel.h)-----------文件描述符fd事件处理相关控制类；  
      5. [Connector](./youth/net/Connector.h)-----------TCP连接类；  
      6. [Epoll](./youth/net/Epoll.h)-----------Epoll封装；  
      7. [EventLoop](./youth/net/EventLoop.h)-----------事件循环类；  
      8. [EventLoopThread](./youth/net/EventLoopThread.h)-----------绑定事件循环的线程；  
      9. [EventLoopThreadPool](./youth/net/EventLoopThreadPool.h)-----------绑定事件循环的线程池；  
      10. [Socket](./youth/net/Socket.h)-----------TCP服务建立类；  
      11. [SocketFunc](./youth/net/SocketFunc.h)-----------TCP相关函数封装；  
      12. [TcpAddressInfo](./youth/net/TcpAddressInfo.h)-----------TCP地址相关信息类；  
      13. [TcpClient](./youth/net/TcpClient.h)-----------TCP客户端类；  
      14. [TcpConnection](./youth/net/TcpConnection.h)-----------TCP连接处理类；  
      15. [TcpServer](./youth/net/TcpServer.h)-----------TCP服务端类；  
      16. [Timer](./youth/net/Timer.h)-----------定时器类；  
      17. [TimerId](./youth/net/TimerId.h)-----------定时器ID类；  
      18. [TimerQueue](./youth/net/TimerQueue.h)-----------定时器队列；  
      19. [tests](./youth/net/tests)-----------测试;  
          1. [EventLoopTest](./youth/net/tests/EventLoopTest.cpp)------------事件循环测试；  
          2. [Resolve_unittest](./youth/net/tests/Resolve_unittest.cc)------------根据域名获取IP地址信息；
          3. [TcpClientTest](./youth/net/tests/TcpClientTest.cpp)-----------TCP客户端测试；  
          4. [TcpServerTest](./youth/net/tests/TcpServerTest.cpp)-----------TCP服务端测试（reactor）；  
          5. [TimerTest](./youth/net/tests/TimerTest.cpp)-----------定时器测试；  
   3. [utils](./youth/utils)  
      1. [LogAsync](./youth/utils/LogAsync.h)-----------异步日志类；  
      2. [LogBuffer](./youth/utils/LogBuffer.h)-----------日志缓冲区类；  
      3. [LogFile](./youth/utils/LogFile.h)-----------日志文件类；  
      4. [Logging](./youth/utils/Logging.h)-----------日志外部接口；  
      5. [LogOut](./youth/utils/LogOut.h)-----------日志组装类；  
      6. [LogStream](./youth/utils/LogStream.h)-----------日志流式输出类；  
      7. [Singleton](./youth/utils/Singleton.h)-----------饿汉单例模板；  
      8. [TaskQueue](./youth/utils/TaskQueue.h)-----------线程安全队列模板；
      9. [ThreadPool](./youth/utils/ThreadPool.h)-----------线程池封装类；  
      10. [tests](./youth/utils/tests)-----------测试;
          1. [Log_benchmark](./youth/utils/tests/Log_benchmark.cc)-----------性能测试；
          2. [LogAsyncTest](./youth/utils/tests/LogAsyncTest.cpp)-----------异步日志测试；
          3. [LogMulti_unittest](./youth/utils/tests/LogMulti_unittest.cc)-----------多线程日志测试；
          4. [LogSpeed_unitTest](./youth/utils/tests/LogSpeed_unitTest.cc)-----------日志速度测试；
          5. [LogTest](./youth/utils/tests/LogTest.cpp)-----------日志测试；
          6. [SingletonTest](./youth/utils/tests/SingletonTest.cpp)-----------单例模板测试；  
          7. [ThreadPoolTest](./youth/utils/tests/ThreadPoolTest.cpp)-----------线程池测试；  
