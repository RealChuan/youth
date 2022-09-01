# Youth(Linux下基于C++17的网络库) #

### **参考：[muduo](https://github.com/chenshuo/muduo "chenshuo/muduo")**  

## 依赖ZLIB、protobuf、benchmark、GTest、glog ##

## 简介 ##
1. [core](./youth/core)----------基础库封装（时间日期，目录，文件，文件信息，计时器，进程信息，字符串处理，线程）以及单元测试；
2. [net](./youth/net)----------网络库封装（事件循环，主机地址信息，定时器相关，TcpServer，TcpClient）以及单元测试；  
3. [utils](./youth/utils)----------工具类封装（日志，异步日志，单例模板，任务队列模板，线程池）以及单元测试；  
