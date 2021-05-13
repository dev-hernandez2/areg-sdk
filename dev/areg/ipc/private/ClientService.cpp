/************************************************************************
 * \file        areg/ipc/private/ClientService.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Service Connection client implementation
 ************************************************************************/
#include "areg/ipc/ClientService.hpp"

#include "areg/ipc/ConnectionConfiguration.hpp"
#include "areg/ipc/IERemoteServiceConsumer.hpp"
#include "areg/ipc/private/NEConnection.hpp"
#include "areg/ipc/private/RemoteServiceEvent.hpp"
#include "areg/component/DispatcherThread.hpp"
#include "areg/component/StreamableEvent.hpp"
#include "areg/component/RemoteEventFactory.hpp"
#include "areg/component/RequestEvents.hpp"
#include "areg/component/ResponseEvents.hpp"
#include "areg/component/NEService.hpp"
#include "areg/base/Process.hpp"
#include "areg/base/File.hpp"
#include "areg/trace/GETrace.h"

DEF_TRACE_SCOPE(areg_ipc_private_ClientService_StartRemotingService);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_StopRemotingService);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_ProcessEvent);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_startConnection);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_stopConnection);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_FailedSendMessage);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_FailedReceiveMessage);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_FailedProcessMessage);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_ProcessReceivedMessage);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_ProcessRemoteRequestEvent);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_ProcessRemoteNotifyRequestEvent);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_ProcessRemoteResponseEvent);
DEF_TRACE_SCOPE(areg_ipc_private_ClientService_RunDispatcher);

const NERemoteService::eServiceConnection   ClientService::CONNECT_TYPE   = NERemoteService::ConnectionTcpip;

ClientService::ClientService( IERemoteServiceConsumer & serviceConsumer )
    : IERemoteService              ( )
    , DispatcherThread           ( NEConnection::CLIENT_DISPATCH_MESSAGE_THREAD )
    , IERemoteServiceHandler       ( )
    , IEClientServiceEventConsumer ( )
    , IERemoteEventConsumer        ( )
    , IETimerConsumer              ( )

    , mClientConnection ( )
    , mServiceConsumer  ( serviceConsumer )
    , mTimerConnect     ( static_cast<IETimerConsumer &>(self()), NEConnection::CLIENT_CONNECT_TIMER_NAME)
    , mThreadReceive    ( static_cast<IERemoteServiceHandler &>(self()), mClientConnection )
    , mThreadSend       ( static_cast<IERemoteServiceHandler &>(self()), mClientConnection )
    , mIsServiceEnabled ( NEConnection::DEFAULT_REMOVE_SERVICE_ENABLED )    // TODO: by default, should be false and read out from configuration file.
    , mConfigFile       ( "" )
    , mChannel          ( )
    , mConnectionState  ( ConnectionStopped )
    , mLock             ( )
{
    ; // do nothing
}

ClientService::~ClientService(void)
{
    ; // do nothing
}

bool ClientService::configureRemoteServicing( const char * configFile )
{
    Lock lock( mLock );
    ConnectionConfiguration configConnect;
    if ( configConnect.loadConfiguration( configFile ) )
    {
        mConfigFile             = configConnect.getConfigFileName( );
        mIsServiceEnabled       = configConnect.getConnectionEnableFlag( CONNECT_TYPE );
        String hostName       = configConnect.getConnectionHost( CONNECT_TYPE );
        unsigned short hostPort = configConnect.getConnectionPort( CONNECT_TYPE );

        return mClientConnection.setAddress( hostName, hostPort );
    }
    else
    {
        return mClientConnection.setAddress( NEConnection::DEFAULT_REMOTE_SERVICE_HOST, NEConnection::DEFAULT_REMOTE_SERVICE_PORT );
    }
}

void ClientService::setRemoteServiceAddress( const char * hostName, unsigned short portNr )
{
    Lock lock( mLock );
    mClientConnection.setAddress( hostName, portNr );
}

bool ClientService::startRemoteServicing(void)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_StartRemotingService);
    Lock lock( mLock );
    bool result = true;
    if ( mClientConnection.isValid() == false && isRunning() == false )
    {
        if ( isRemoteServicingEnabled() )
        {
            lock.unlock();

            TRACE_DBG("The remote servicing is enabled, going to start remote servicing");
            result = createThread(Thread::WAIT_INFINITE) && waitForDispatcherStart(IESynchObject::WAIT_INFINITE);

            lock.lock();
        }
        else
        {
            TRACE_DBG("The remote servicing is disabled, nothing to start");
            result = true;
        }
    }
    else
    {
        TRACE_DBG("Ignoring to start remote servicing. The client connection is [ %s ], the servicing thread is [ %s ]"
                    , mClientConnection.isValid() ? "VALID" : "INVALID"
                    , isRunning() ? "RUNNING" : "NOT RUNNING");
        ASSERT(isRunning());
    }

    return result;
}

void ClientService::stopRemoteServicing(void)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_StopRemotingService);
    TRACE_DBG( "Stopping remote servicing client connection, current state is [ %s ]", isRunning() ? "RUNNING" : "NOT RUNNING" );
    if ( isRunning() )
    {
        ClientServiceEvent::sendEvent( ClientServiceEventData(ClientServiceEventData::CMD_StopService), static_cast<IEClientServiceEventConsumer &>(self()), static_cast<DispatcherThread &>(self()) );
        DispatcherThread::triggerExitEvent();
        completionWait(Thread::WAIT_INFINITE);
        destroyThread( Thread::DO_NOT_WAIT );
    }
}

bool ClientService::isRemoteServicingStarted(void) const
{
    Lock lock( mLock );
    return isStarted();
}

bool ClientService::isRemoteServicingConfigured( void ) const
{
    Lock lock(mLock);
    return mClientConnection.getAddress().isValid();
}

bool ClientService::isRemoteServicingEnabled(void) const
{
    Lock lock(mLock);
    return mIsServiceEnabled;
}

void ClientService::enableRemoteServicing( bool enable )
{
    Lock lock( mLock );
    if ( isRunning() )
    {
        if ( enable == false )
            stopRemoteServicing();
    }
    mIsServiceEnabled = enable;
}

bool ClientService::registerService( const StubAddress & stubService )
{
    Lock lock( mLock );
    bool result = false;
    if ( isStarted() )
    {
        result = SendMessageEvent::sendEvent(  SendMessageEventData(NEConnection::createBrokerRegisterService(stubService, mClientConnection.getCookie()))
                                               , static_cast<IESendMessageEventConsumer &>(mThreadSend)
                                               , static_cast<DispatcherThread &>(mThreadSend) );
    }
    return result;
}

void ClientService::unregisterService(const StubAddress & stubService)
{
    Lock lock( mLock );
    if ( isStarted() )
    {
        SendMessageEvent::sendEvent(  SendMessageEventData(NEConnection::createBrokerUnregisterService(stubService, mClientConnection.getCookie()))
                                        , static_cast<IESendMessageEventConsumer &>(mThreadSend)
                                        , static_cast<DispatcherThread &>(mThreadSend) );
    }
}

bool ClientService::registerServiceClient(const ProxyAddress & proxyService)
{
    Lock lock( mLock );
    bool result = false;
    if ( isStarted() )
    {
        result = SendMessageEvent::sendEvent( SendMessageEventData(NEConnection::createBrokerRegisterClient(proxyService, mClientConnection.getCookie()))
                                                , static_cast<IESendMessageEventConsumer &>(mThreadSend)
                                                , static_cast<DispatcherThread &>(mThreadSend) );
    }
    return result;
}

void ClientService::unregisterServiceClient(const ProxyAddress & proxyService)
{
    Lock lock( mLock );
    if ( isStarted() )
    {
        SendMessageEvent::sendEvent(  SendMessageEventData(NEConnection::createBrokerUnregisterClient(proxyService, mClientConnection.getCookie()))
                                        , static_cast<IESendMessageEventConsumer &>(mThreadSend)
                                        , static_cast<DispatcherThread &>(mThreadSend) );
    }
}

void ClientService::processTimer(Timer & timer)
{
    if ( &timer == &mTimerConnect )
        startConnection();
}

void ClientService::processEvent( const ClientServiceEventData & data )
{
    TRACE_SCOPE(areg_ipc_private_ClientService_ProcessEvent);
    
    ClientServiceEventData::eClientServiceCommands cmdService = data.getCommand();
    TRACE_DBG("Client service is executing command [ %s ]", ClientServiceEventData::getString(cmdService));

    switch ( cmdService )
    {
    case ClientServiceEventData::CMD_StartService:
        {
            TRACE_DBG("Starting remove servicing");;
            mChannel.setCookie( NEService::COOKIE_LOCAL );
            mChannel.setSource( NEService::SOUR_UNKNOWN );
            mChannel.setTarget( NEService::TARGET_UNKNOWN );
            startConnection();
        }
        break;

    case ClientServiceEventData::CMD_StopService:
        {
            TRACE_DBG("Stopping remote servicing");
            setConnectionState(ClientService::ConnectionStopping);
            Channel channel = mChannel;
            mChannel.setCookie( NEService::COOKIE_UNKNOWN );
            mChannel.setSource( NEService::SOUR_UNKNOWN );
            mChannel.setTarget( NEService::TARGET_UNKNOWN );
            stopConnection();
            mThreadReceive.completionWait( Thread::WAIT_INFINITE );
            mThreadSend.completionWait( Thread::WAIT_INFINITE );
            mServiceConsumer.remoteServiceStopped( channel );

            mThreadReceive.destroyThread( Thread::DO_NOT_WAIT );
            mThreadSend.destroyThread( Thread::DO_NOT_WAIT );
        }
        break;

    case ClientServiceEventData::CMD_ServiceStarted:
        {
            ASSERT(isStarted());
            if ( mClientConnection.getCookie() != NEService::COOKIE_LOCAL )
            {
                TRACE_DBG("Client service succeeded to start, client cookie is [ %p ]", mClientConnection.getCookie());
                setConnectionState(ClientService::ConnectionStarted);
                mChannel.setCookie( mClientConnection.getCookie() );
                mChannel.setSource( getCurrentThreadId() );
                mChannel.setTarget( NEService::TARGET_LOCAL );
                mServiceConsumer.remoteServiceStarted(mChannel);
            }
        }
        break;

    case ClientServiceEventData::CMD_ServiceStopped:
        {
            TRACE_DBG("Client service is stopped. Resetting cookie");
            setConnectionState(ClientService::ConnectionStopping);
            Channel channel = mChannel;
            mChannel.setCookie( NEService::COOKIE_UNKNOWN );
            mChannel.setSource( NEService::SOUR_UNKNOWN );
            mChannel.setTarget( NEService::TARGET_UNKNOWN );
            stopConnection();

            mThreadReceive.completionWait( Thread::WAIT_INFINITE );
            mThreadSend.completionWait( Thread::WAIT_INFINITE );
            mServiceConsumer.remoteServiceStopped( channel );
        }
        break;

    case ClientServiceEventData::CMD_ServiceLost:
    {
        TRACE_WARN("Client service is lost connection. Resetting cookie and trying to restart");
        Channel channel = mChannel;
        mChannel.setCookie( NEService::COOKIE_UNKNOWN );
        mChannel.setSource( NEService::SOUR_UNKNOWN );
        mChannel.setTarget( NEService::TARGET_UNKNOWN );
        stopConnection();

        mThreadReceive.completionWait( Thread::WAIT_INFINITE );
        mThreadSend.completionWait( Thread::WAIT_INFINITE );
        mServiceConsumer.remoteServiceStopped( channel );

        if (getConnectionState() == ClientService::ConnectionStarted)
        {
            TRACE_DBG("Restarting lost connection with remote service");
            setConnectionState(ClientService::ConnectionStarting);
            startConnection();
        }
    }
    break;

    default:
        ASSERT(false);
        break;
    }
}

inline bool ClientService::isStarted(void) const
{
    ITEM_ID cookie = mClientConnection.getCookie();
    return (mClientConnection.isValid() && (cookie != NEService::COOKIE_LOCAL) && (cookie != NEService::COOKIE_UNKNOWN));
}

inline void ClientService::setConnectionState(const ClientService::eConnectionState newState)
{
    mConnectionState    = newState;
}

inline ClientService::eConnectionState ClientService::getConnectionState( void ) const
{
    return mConnectionState;
}

inline bool ClientService::startConnection( void )
{
    TRACE_SCOPE(areg_ipc_private_ClientService_startConnection);

    ASSERT(mClientConnection.getAddress().isValid());
    ASSERT(mClientConnection.isValid() == false);
    ASSERT(mThreadReceive.isRunning() == false);
    ASSERT(mThreadSend.isRunning() == false);

    bool result = false;
    mTimerConnect.stopTimer();

    if ( mClientConnection.createSocket() )
    {
        if ( mThreadReceive.createThread( Thread::WAIT_INFINITE ) && mThreadSend.createThread( Thread::WAIT_INFINITE ) )
        {
            VERIFY( mThreadReceive.waitForDispatcherStart( IESynchObject::WAIT_INFINITE ) );
            VERIFY( mThreadSend.waitForDispatcherStart( IESynchObject::WAIT_INFINITE ) );
            TRACE_DBG("Client service starting connection to broker service.");
            result = mClientConnection.requestConnectServer();
        }
    }
    if ( result == false )
    {
        TRACE_WARN("Client service failed to start connection, going to repeat connection in [ %u ] ms", NEConnection::DEFAULT_RETRY_CONNECT_TIMEOUT);
        mThreadSend.destroyThread( Thread::DO_NOT_WAIT );
        mThreadReceive.destroyThread( Thread::DO_NOT_WAIT );
        mClientConnection.closeSocket();
        mTimerConnect.startTimer( NEConnection::DEFAULT_RETRY_CONNECT_TIMEOUT, static_cast<DispatcherThread &>(self()), 1);
    }

    return result;
}

inline void ClientService::stopConnection(void)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_stopConnection);
    TRACE_WARN("Stopping client service connection");
    mTimerConnect.stopTimer();

    mClientConnection.requestDisconnectServer();
    mClientConnection.disableReceive();
    mClientConnection.setCookie( NEService::COOKIE_UNKNOWN );

    mThreadReceive.triggerExitEvent();
    mThreadSend.triggerExitEvent();

    mThreadReceive.destroyThread( Thread::DO_NOT_WAIT );
    mThreadSend.destroyThread( Thread::DO_NOT_WAIT );
}

void ClientService::failedSendMessage(const RemoteMessage & msgFailed)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_FailedSendMessage);
    TRACE_WARN("Failed to send message [ %p ] to target [ %p ], source is [ %p ]", msgFailed.getMessageId(), msgFailed.getTarget(), msgFailed.getSource());

    StreamableEvent * eventError = RemoteEventFactory::createRequestFailedEvent(msgFailed, mChannel);
    if ( eventError != NULL )
        eventError->sendEvent();
}

void ClientService::failedReceiveMessage(SOCKETHANDLE whichSource)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_FailedReceiveMessage);
    TRACE_WARN("Failed to receive message from socket [ %u ], current client socket is [ %u ]. Going to stop service",  whichSource, mClientConnection.getSocketHandle());
    if ( whichSource == mClientConnection.getSocketHandle() )
    {
        ClientServiceEvent::sendEvent( ClientServiceEventData(ClientServiceEventData::CMD_ServiceLost), static_cast<IEClientServiceEventConsumer &>(self()), static_cast<DispatcherThread &>(self()) );
        // DispatcherThread::triggerExitEvent();
    }
}

void ClientService::failedProcessMessage( const RemoteMessage & msgUnprocessed )
{
    TRACE_SCOPE(areg_ipc_private_ClientService_FailedProcessMessage);
    TRACE_DBG("The message [ %p ] for target [ %p ] and from source [ %p ] is unprocessed, going to create failed event", msgUnprocessed.getMessageId(), msgUnprocessed.getTarget(), msgUnprocessed.getSource());
    StreamableEvent * eventError = RemoteEventFactory::createRequestFailedEvent(msgUnprocessed, mChannel);
    if ( eventError != NULL )
    {
        RemoteMessage data;
        if ( RemoteEventFactory::createStreamFromEvent( data, *eventError, mChannel) )
            SendMessageEvent::sendEvent( SendMessageEventData(data), static_cast<IESendMessageEventConsumer &>(mThreadSend), static_cast<DispatcherThread &>(mThreadSend) );
    }
}

void ClientService::processReceivedMessage( const RemoteMessage & msgReceived, const NESocket::InterlockedValue & addrHost, SOCKETHANDLE whichSource )
{
    TRACE_SCOPE(areg_ipc_private_ClientService_ProcessReceivedMessage);
    if ( msgReceived.isValid() && whichSource != NESocket::InvalidSocketHandle )
    {
        NEService::eFuncIdRange msgId = static_cast<NEService::eFuncIdRange>( msgReceived.getMessageId());
        NEMemory::eMessageResult result = static_cast<NEMemory::eMessageResult>(msgReceived.getResult());
        TRACE_DBG("Processing received message [ %s ] ( ID = %p ), received message is valid, message result [ %s ]", NEService::getString( msgId), msgId, NEMemory::getString(result));
        switch ( msgId )
        {
        case NEService::SI_ROUTER_NOTIFY:
            {
                NEService::eServiceConnection connection = NEService::ServiceConnectionUnknown;
                ITEM_ID cookie = NEService::COOKIE_UNKNOWN;
                msgReceived >> connection;
                msgReceived >> cookie;
                TRACE_DBG("Broker connection notification. Connection status [ %s ], cookie [ %p ]", NEService::getString(connection), cookie);

                if ( (msgReceived.getResult() == static_cast<unsigned int>(NEMemory::ResultSucceed)) && (connection == NEService::ServiceConnected))
                {
                    Lock lock(mLock);
                    ASSERT(cookie == msgReceived.getTarget());
                    mClientConnection.setCookie(cookie);
                    ClientServiceEvent::sendEvent( ClientServiceEventData(ClientServiceEventData::CMD_ServiceStarted), static_cast<IEClientServiceEventConsumer &>(self()), static_cast<DispatcherThread &>(self()) );
                }
                else if ( (static_cast<unsigned int>(connection) & static_cast<unsigned int>(NEService::ServiceConnected)) == 0 )
                {
                    ClientServiceEvent::sendEvent( ClientServiceEventData(ClientServiceEventData::CMD_ServiceStopped), static_cast<IEClientServiceEventConsumer &>(self()), static_cast<DispatcherThread &>(self()) );
                }
                else
                {
                    ; // ignore
                }
            }
            break;

        case NEService::SI_ROUTER_REGISTER_NOTIFY:
            {
                ASSERT( mClientConnection.getCookie() == msgReceived.getTarget() );
                NEService::eServiceRequestType reqType;
                msgReceived >> reqType;
                TRACE_DBG("Service Broker registration notification of type [ %s ]", NEService::getString(reqType));

                switch ( reqType )
                {
                case NEService::SERVICE_REQUEST_REGISTER_CLIENT:
                    {
                        ProxyAddress proxy(msgReceived);
                        proxy.setSource( mChannel.getSource() );
                        if ( result == NEMemory::ResultSucceed )
                            mServiceConsumer.registerRemoteProxy(proxy);
                        else
                            mServiceConsumer.unregisterRemoteProxy(proxy);
                    }
                    break;

                case NEService::SERVICE_REQUEST_REGISTER_STUB:
                    {
                        StubAddress stub(msgReceived);
                        stub.setSource( mChannel.getSource() );
                        if ( result == NEMemory::ResultSucceed )
                            mServiceConsumer.registerRemoteStub(stub);
                        else
                            mServiceConsumer.unregisterRemoteStub(stub);
                    }
                    break;

                case NEService::SERVICE_REQUEST_UNREGISTER_CLIENT:
                    {
                        ProxyAddress proxy(msgReceived);
                        proxy.setSource( mChannel.getSource() );
                        mServiceConsumer.unregisterRemoteProxy(proxy);
                    }
                    break;

                case NEService::SERVICE_REQUEST_UNREGISTER_STUB:
                    {
                        StubAddress stub(msgReceived);
                        stub.setSource( mChannel.getSource() );
                        mServiceConsumer.unregisterRemoteStub(stub);
                    }
                    break;

                default:
                    ASSERT(false);
                    break;
                }
            }
            break;

        default:
            {
                if ( NEService::isExecutableId(msgId) )
                {
                    TRACE_DBG("Processing executable remote message with ID [ %p ]", msgId);
                    StreamableEvent * eventRemote = RemoteEventFactory::createEventFromStream(msgReceived, mChannel);
                    if ( eventRemote != NULL )
                        eventRemote->sendEvent();
                    else
                        failedProcessMessage(msgReceived);
                }
                else
                {
                    TRACE_WARN("The message [ %s ] ( ID = %p )was not processed on client service side", NEService::getString(msgId), msgId);
                }
            }
            break;
        }
    }
    else
    {
        TRACE_WARN("Invalid message from host [ %s : %u ], ignore processing", addrHost.getHostAddress().getString(), addrHost.getHostPort());
    }
}

void ClientService::processRemoteEvent( RemoteRequestEvent & requestEvent)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_ProcessRemoteRequestEvent);
    TRACE_DBG("Processing request event [ %s ] with message ID [ %p ] of runtime object [ %s ], target stub [ %s ], source proxy [ %s ], request type [ %s ]"
                , Event::getString( requestEvent.getEventType() )
                , requestEvent.getRequestId()
                , requestEvent.getRuntimeClassName()
                , StubAddress::convAddressToPath(requestEvent.getTargetStub()).getString()
                , ProxyAddress::convAddressToPath(requestEvent.getEventSource()).getString()
                , NEService::getString(requestEvent.getRequestType()) );

    if ( requestEvent.isRemote() )
    {
        RemoteMessage data;
        if ( RemoteEventFactory::createStreamFromEvent( data, requestEvent, mChannel) )
        {
            TRACE_DBG("Send request, remote message of ID [ %p ] ( %s ) from source [ %u ] to target [ %u ]"
                            , data.getMessageId()
                            , NEService::getString( static_cast<NEService::eFuncIdRange>(data.getMessageId()))
                            , data.getSource()
                            , data.getTarget());

            SendMessageEvent::sendEvent( SendMessageEventData(data), static_cast<IESendMessageEventConsumer &>(mThreadSend), static_cast<DispatcherThread &>(mThreadSend) );
        }
        else
        {
            TRACE_ERR("Failed to create remote request message data with ID [ %p ]", requestEvent.getRequestId() );
        }
    }
    else
    {
        TRACE_WARN("Request event with ID [ %p ] is not remote, ignoring sending event", requestEvent.getRequestId());
    }
}

void ClientService::processRemoteEvent( RemoteNotifyRequestEvent & requestNotifyEvent )
{
    TRACE_SCOPE(areg_ipc_private_ClientService_ProcessRemoteNotifyRequestEvent);
    TRACE_DBG("Processing notify request event [ %s ] with message ID [ %p ] of runtime object [ %s ], target stub [ %s ], source proxy [ %s ], request type [ %s ]"
                , Event::getString( requestNotifyEvent.getEventType() )
                , requestNotifyEvent.getRequestId()
                , requestNotifyEvent.getRuntimeClassName()
                , StubAddress::convAddressToPath(requestNotifyEvent.getTargetStub()).getString()
                , ProxyAddress::convAddressToPath(requestNotifyEvent.getEventSource()).getString()
                , NEService::getString(requestNotifyEvent.getRequestType()) );


    if ( requestNotifyEvent.isRemote() )
    {
        RemoteMessage data;
        if ( RemoteEventFactory::createStreamFromEvent( data, requestNotifyEvent, mChannel) )
        {
            TRACE_DBG("Send notify request remote message of ID [ %p ] ( %s ) from source [ %p ] to target [ %p ]"
                            , data.getMessageId()
                            , NEService::getString( static_cast<NEService::eFuncIdRange>(data.getMessageId()))
                            , data.getSource()
                            , data.getTarget());

            SendMessageEvent::sendEvent( SendMessageEventData(data), static_cast<IESendMessageEventConsumer &>(mThreadSend), static_cast<DispatcherThread &>(mThreadSend) );
        }
        else
        {
            TRACE_ERR("Failed to create remote notify request message data with ID [ %p ]", requestNotifyEvent.getRequestId() );
        }
    }
    else
    {
        TRACE_WARN("Notify request event with ID [ %p ] is not remote, ignoring sending event", requestNotifyEvent.getRequestId());
    }
}


void ClientService::processRemoteEvent(RemoteResponseEvent & responseEvent)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_ProcessRemoteResponseEvent);
    TRACE_DBG("Processing response event [ %s ] with message ID [ %p ] of runtime object [ %s ], target proxy [ %s ], data type [ %s ]"
                , Event::getString( responseEvent.getEventType() )
                , responseEvent.getResponseId()
                , responseEvent.getRuntimeClassName()
                , ProxyAddress::convAddressToPath(responseEvent.getTargetProxy()).getString()
                , NEService::getString(responseEvent.getDataType()) );

    if ( responseEvent.isRemote() )
    {
        RemoteMessage data;
        if ( RemoteEventFactory::createStreamFromEvent( data, responseEvent, mChannel) )
        {
            TRACE_DBG("Send response remote message of ID [ %p ] ( %s ) from source [ %p ] to target [ %p ]"
                            , data.getMessageId()
                            , NEService::getString( static_cast<NEService::eFuncIdRange>(data.getMessageId()))
                            , data.getSource()
                            , data.getTarget());

            SendMessageEvent::sendEvent( SendMessageEventData(data), static_cast<IESendMessageEventConsumer &>(mThreadSend), static_cast<DispatcherThread &>(mThreadSend) );
        }
        else
        {
            TRACE_ERR("Failed to create remote response message data with ID [ %p ]", responseEvent.getResponseId() );
        }
    }
    else
    {
        TRACE_WARN("Response event with ID [ %p ] is not remote, ignoring sending event", responseEvent.getResponseId());
    }
}

bool ClientService::runDispatcher(void)
{
    TRACE_SCOPE(areg_ipc_private_ClientService_RunDispatcher);
    ClientServiceEvent::addListener( static_cast<IEClientServiceEventConsumer &>(self()), static_cast<DispatcherThread &>(self()) );
    ClientServiceEvent::sendEvent( ClientServiceEventData(ClientServiceEventData::CMD_StartService), static_cast<IEClientServiceEventConsumer &>(self()), static_cast<DispatcherThread &>(self()) );

    setConnectionState(ClientService::ConnectionStarting);
    bool result = DispatcherThread::runDispatcher();
    setConnectionState(ClientService::ConnectionStopped);

    ClientServiceEvent::removeListener( static_cast<IEClientServiceEventConsumer &>(self()), static_cast<DispatcherThread &>(self()) );

    return result;
}

bool ClientService::postEvent(Event & eventElem)
{
    if ( eventElem.isRemote() )
        eventElem.setEventConsumer( static_cast<IERemoteEventConsumer *>(this) );
    return EventDispatcher::postEvent(eventElem);
}
