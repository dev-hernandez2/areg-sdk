/************************************************************************
 * \file        areg/ipc/private/NERemoteService.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform Remote service namespace
 ************************************************************************/

#include "areg/ipc/NERemoteService.hpp"

/************************************************************************
 * \file        areg/ipc/private/NERemoteService.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform Remote service namespace
 ************************************************************************/

#include "areg/ipc/NERemoteService.hpp"
#include "areg/base/String.hpp"

const char * NERemoteService::getServiceConnectionTypeString(NERemoteService::eServiceConnection connectionType)
{
    switch (connectionType)
    {
    case NERemoteService::ConnectionTcpip:
        return NERemoteService::STR_CONNECTION_TYPE_TCPIP;
    default:
        return String::EmptyString;
    }
}

NERemoteService::eServiceConnection NERemoteService::getServiceConnectionType(const char * connectionType, bool caseSensitive /* = true */ )
{
    if ( NEString::compareStrings<char, char>(NERemoteService::STR_CONNECTION_TYPE_TCPIP, connectionType, NEString::CountAll, caseSensitive ) == 0)
        return NERemoteService::ConnectionTcpip;
    else
        return NERemoteService::ConnectionUndefined;
}