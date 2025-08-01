#ifndef _AzRPC_Controller_H_
#define _AzRPC_Controller_H_

#include <google/protobuf/service.h>
#include <string>

// 描述RPC调用的控制器
// 主要作用是跟踪RPC方法调用的状态、错误信息并提供控制功能
class AzRPC_Controller: public google::protobuf::RpcController {
public:
    AzRPC_Controller();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string& reason);

    // TODO
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);

private:
    bool m_failed;          // RPC方法执行过程中的状态
    std::string m_errText;  // RPC方法执行过程中的错误信息
};

// extern AzRPC_Controller controller; // 改为 extern 声明

#endif