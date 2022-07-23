#include "muduo/base/AsyncLogging.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpConnection.h"

using namespace muduo;
using namespace muduo::net;

class LongCnn{
private:
  TcpConnection tcpConn_;
  int heartBeatIntv_; // in seconds
  EventLoop* loop_;
  const string longCnnName_;
  
  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };

public:
  LongCnn();
  ~LongCnn();
  
  LongCnn(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop),
      server_(loop, listenAddr, "EchoServer")
  {
  }
}
