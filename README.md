### 环境准备

#### 操作系统·

##### Ubuntu 24.04.2 LTS

#### 安装基础工具

```shell
sudo apt-get install -y wget cmake build-essential unzipshell
```

#### 安装muduo库

##### 安装必要工具

```shell
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y git cmake g++ libboost-all-dev libssl-dev
```

##### 下载muduo库

```shell
git clone https://github.com/chenshuo/muduo.git
```

##### 进入muduo库并编译安装

```shell
cd muduo
mkdir build
cd build
cmake ..
make
sudo make install
```

##### 编写C++代码验证安装成功

```cpp
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <boost/bind/bind.hpp>
#include <muduo/net/EventLoop.h>
using namespace boost::placeholders;

// 使用muduo开发回显服务器
class EchoServer {
public:
    EchoServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr);
    void start(); 

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

    muduo::net::TcpServer server_;
};

EchoServer::EchoServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr): server_(loop, listenAddr, "EchoServer") {
    server_.setConnectionCallback(boost::bind(&EchoServer::onConnection, this, _1));
    server_.setMessageCallback(boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start() {
    server_.start();
}

void EchoServer::onConnection(const muduo::net::TcpConnectionPtr& conn) {
    LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " is " << (conn->connected() ? "UP" : "DOWN");
}

void EchoServer::onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time){
    // 接收到所有的消息，然后回显
    muduo::string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, " << "data received at " << time.toString();
    conn->send(msg);  
}


int main(int argc, char const* argv[]){
    LOG_INFO << "pid = " << getpid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(8888);
    EchoServer server(&loop, listenAddr);
    server.start();
    loop.loop();

    return 0;
}
```

##### 配置CMakeLists.txt（MyMuduoApp可替换）

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyMuduoApp)

set(CMAKE_CXX_STANDARD 11)

find_package(muduo REQUIRED)

add_executable(MyMuduoApp main.cpp)
target_link_libraries(MyMuduoApp muduo_net muduo_base pthread)
```

##### 使用g++进行编译

```powershell
g++ main.cpp -lmuduo_net -lmuduo_base -lpthread -std=c++11
./a.out
```

##### 打开新终端验证监听

```shell
nc 127.0.0.1 8888
hello world
hello world
hello muduo
hello muduo
```

#### 安装Zookeeper

```shell
sudo apt install zookeeperd
```

#### 安装Zookeeper开发库

```shell
sudo apt install libzookeeper-mt-dev
```

#### 安装Protobuf 3.21.12版本

```shell
sudo apt-get install protobuf-compiler libprotobuf-dev
```

#### 安装boost库

```shell
sudo apt-get install -y libboost-all-dev
```

#### 安装Glog日志库

```shell
sudo apt-get install libgoogle-glog-dev libgflags-dev
```

### 编译指令

#### 进入example文件夹下生成user.pb.h和user.pb.cc

```shell
cd example
protoc --cpp_out=. user.proto
```

#### 进入src文件夹下生成AzRPC_Header.pb.h和AzRPC_Header.pb.cc

```shell
cd ../src
protoc --cpp_out=. AzRPC_Header.proto
mv AzRPC_Header.pb.h include/
```

#### 生成项目可执行文件

```shell
mkdir build
cd build
cmake ..
make -j${nproc}
```

#### 进入bin文件夹下分别运行./server和./client

##### 进入bin文件夹

```cd
cd bin
```

##### server

```shell
./server -i ./test.conf
```

##### client

```shell
./client -i ./test.conf
```
