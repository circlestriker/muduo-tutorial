#include "LongCnn.h"

LongCnn::~LongCnn()
{
  loop_->assertInLoopThread();
  LOG_TRACE << "LongCnn::~LongCnn [" << longCnnName_ << "] destructing";

  TcpConnectionPtr conn(tcpConn_);
  tcpConn_.reset();
  conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void LongCnn::accetp(int sockfd, const InetAddress& peerAddr)
{
  loop_->assertInLoopThread();
  char buf[64];
  snprintf(buf, sizeof buf, "-%s#", ipPort_.c_str());
  string connName = name_ + buf;

  LOG_INFO << "LongCnn::newConnection [" << name_
           << "] - new connection [" << connName
           << "] from " << peerAddr.toIpPort();
  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(new TcpConnection(ioLoop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));
  connections_[connName] = conn;
  conn->setConnectionCallback(connectionCallback_);
  conn->setMessageCallback(messageCallback_);
  conn->setWriteCompleteCallback(writeCompleteCallback_);
  conn->setCloseCallback(
      std::bind(&LongCnn::removeConnection, this, _1)); // FIXME: unsafe
  ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}
