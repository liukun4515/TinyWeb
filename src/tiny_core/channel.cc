/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Configer Class---
*We need to upgrade this class
*in order to handle config file error
****************************************
*
*/

#include <tiny_core/channel.h>
#include <tiny_core/eventloop.h>
#include <tiny_base/log.h>

void Channel::m_fUpdate()
{
    m_pEventLoop->updateChannel(this);
}

Channel::Channel(EventLoop *loop, int fd)
{
    m_pEventLoop = loop;
    m_nFd = fd;
    m_nEvent = 0;
    m_nREvent = 0;
    m_nFlag = ChannelFlag_New;
    LOG(Debug) << "class Channel constructor\n";
}

void Channel::handleEvent(Time time)
{
    // std::cout << "Epoller revent:" << m_nREvent << std::endl;
    if (m_nREvent & (EPOLLIN | EPOLLRDHUP))
    {
        //EPOLLRDHUP is the signal of peer close connection;
        if (m_fReadCallback)
            m_fReadCallback(time);
    }
    else if (m_nREvent == EPOLLOUT)
    {
        if (m_fWriteCallback)
            m_fWriteCallback();
    }
    else if (m_nREvent == EPOLLERR)
    {
        if (m_fErrorCallback)
            m_fErrorCallback();
    }
    else if (m_nREvent == EPOLLHUP)
    {
        if (m_fCloseCallback)
            m_fCloseCallback();
    }
}

Channel::~Channel()
{
    LOG(Debug) << "class Channel destructor\n";
}