//////////////////////////////////////////////////////////////////////////
// Begin generate shared/generated/private/CentralMessagerClientBase.cpp file
//////////////////////////////////////////////////////////////////////////
/************************************************************************
 * (c) copyright    2021
 *                  Create by AREG SDK code generator tool from source CentralMessager.
 * Generated at     12.05.2021  16:41:20 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file            shared/generated/private/CentralMessagerClientBase.cpp
 * \ingroup         CentralMessager Service Interface
 * \brief           This is an automatic generated code of CentralMessager Service Interface Client base class implementation.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "shared/generated/CentralMessagerClientBase.hpp"
#include "areg/component/ComponentThread.hpp"
#include "areg/component/Component.hpp"
#include "areg/trace/GETrace.h"

/************************************************************************
 * Request error handling initialization
 ************************************************************************/
namespace NECentralMessager
{
    /**
     * \brief   Initialize request failure function pointers to make error handling
     **/
    typedef void (CentralMessagerClientBase::* FuncRequestFailure) ( NEService::eResultType );
    static FuncRequestFailure failureFunctions[] = 
    {
          &CentralMessagerClientBase::requestSendMessageFailed
        , &CentralMessagerClientBase::requestKeyTypingFailed
    };
}

//////////////////////////////////////////////////////////////////////////
// CentralMessagerClientBase class implementation
//////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Constructor / Destructor
 ************************************************************************/

CentralMessagerClientBase::CentralMessagerClientBase( const char* roleName, const char * ownerThread /*= static_cast<const char *>(NULL)*/ )
    : IEProxyListener   ( )
    , ClientBase        ( )

    , mIsConnected      ( false )
    , mCurrSequenceNr   ( 0 )
    , mProxy            ( CentralMessagerProxy::createProxy(roleName, static_cast<IEProxyListener &>(self()), ownerThread) )
{
    ; // do nothing
}

CentralMessagerClientBase::CentralMessagerClientBase( const char* roleName, DispatcherThread & ownerThread )
    : IEProxyListener   ( )
    , ClientBase        ( )

    , mIsConnected      ( false )
    , mCurrSequenceNr   ( 0 )
    , mProxy            ( CentralMessagerProxy::createProxy(roleName, static_cast<IEProxyListener &>(self()), ownerThread) )
{
    ; // do nothing
}

CentralMessagerClientBase::CentralMessagerClientBase( const char* roleName, Component & owner )
    : IEProxyListener   ( )
    , ClientBase        ( )

    , mIsConnected      ( false )
    , mCurrSequenceNr   ( 0 )
    , mProxy            ( CentralMessagerProxy::createProxy(roleName, static_cast<IEProxyListener &>(self()), owner.getMasterThread()) )
{
    ; // do nothing
}

CentralMessagerClientBase::~CentralMessagerClientBase( void )
{
    if (mProxy != NULL)
    {
        mProxy->clearAllNotifications( static_cast<IENotificationEventConsumer &>(self()) );
        mProxy->freeProxy( static_cast<IEProxyListener &>(self()) );
        mProxy  = NULL;
    }
    
    mIsConnected= false;
}

/************************************************************************
 * Operations
 ************************************************************************/

bool CentralMessagerClientBase::recreateProxy( void )
{
    bool result         = false;
    String roleName   = mProxy != NULL ? mProxy->getProxyAddress().getRoleName() : "";
    String threadName = mProxy != NULL ? mProxy->getProxyAddress().getThread()   : "";
    if ( roleName.isEmpty() == false )
    {
        CentralMessagerProxy * newProxy = CentralMessagerProxy::createProxy(roleName.getString(), static_cast<IEProxyListener &>(self()), threadName.getString());
        if (newProxy != NULL)
        {
            mProxy->clearAllNotifications( static_cast<IENotificationEventConsumer &>(self()) );
            mProxy->freeProxy (static_cast<IEProxyListener &>(self()) );
            mProxy = newProxy;
            result = true;
        }
        else
        {
            ; // do nothing
        }
    }
    else
    {
        ; // do nothing, no role name is assigned
    }
    
    return result;
}

DispatcherThread * CentralMessagerClientBase::getDispatcherThread( void )
{
    return ( mProxy != static_cast<CentralMessagerProxy *>(NULL) ? &(mProxy->getProxyDispatcherThread()) : static_cast<DispatcherThread *>(NULL) );
}

void CentralMessagerClientBase::clearAllNotifications( void )
{
    mProxy->clearAllNotifications(static_cast<IENotificationEventConsumer &>(self()));
}

DEF_TRACE_SCOPE(shared_generated_CentralMessagerClientBase_ServiceConnected);
bool CentralMessagerClientBase::serviceConnected( bool isConnected, ProxyBase & proxy )
{
    TRACE_SCOPE(shared_generated_CentralMessagerClientBase_ServiceConnected);
    
    bool result = false;
    if(mProxy == &proxy)
    {
        TRACE_DBG("The Service [ %s ] with Role Name [ %s ] is [ %s ]"
                 , proxy.getProxyAddress().getServiceName().getString()
                 , proxy.getProxyAddress().getRoleName().getString()
                 , isConnected ? "CONNECTED" : "DISCONNECTED");
        
        mIsConnected= isConnected;
        result      = true;
    }
    
    return result;
}

void CentralMessagerClientBase::notifyOn( NECentralMessager::eMessageIDs msgId, bool notify, bool always /* = false */ )
{
    if (notify)
        mProxy->setNotification(msgId, static_cast<IENotificationEventConsumer &>(self()), always);
    else
        mProxy->clearNotification(msgId, static_cast<IENotificationEventConsumer &>(self()));
}

/************************************************************************
 * Event processing
 ************************************************************************/

DEF_TRACE_SCOPE(shared_generated_CentralMessagerClientBase_processNotificationEvent);
void CentralMessagerClientBase::processNotificationEvent( NotificationEvent & eventElem )
{
    const NotificationEventData & data  = static_cast<const NotificationEvent &>(eventElem).getData();
    NEService::eResultType result       = data.getNotifyType();
    NECentralMessager::eMessageIDs msgId   = static_cast<NECentralMessager::eMessageIDs>(data.getNotifyId());
    mCurrSequenceNr = data.getSequenceNr();

    switch (result)
    {
    case NEService::RESULT_REQUEST_BUSY:
    case NEService::RESULT_REQUEST_ERROR:
    case NEService::RESULT_REQUEST_CANCELED:
        {
        /************************************************************************
         * Trigger request error handling if request failed.
         ************************************************************************/
            requestFailed(msgId, result);
        }
        break;

    case NEService::RESULT_INVALID:
        {
        /************************************************************************
         * Trigger invalid response / broadcast handling. May happen when remove notification 
         ************************************************************************/
            invalidResponse(msgId);
        }
        break;

    case NEService::RESULT_DATA_OK:
    case NEService::RESULT_DATA_INVALID:
        break;

    case NEService::RESULT_OK:
        {
            switch (msgId)
            {
        /************************************************************************
         * Trigger response processing
         ************************************************************************/
        /************************************************************************
         * Trigger broadcast processing
         ************************************************************************/
            case NECentralMessager::MSG_ID_broadcastSendMessage:
                {
                    const String & nickName = mProxy->getParamnickName();
                    unsigned int cookie = mProxy->getParamcookie();
                    const String & newMessage = mProxy->getParamnewMessage();
                    const DateTime & dateTime = mProxy->getParamdateTime();
                    broadcastSendMessage( nickName, cookie, newMessage, dateTime );
                }
                break;

            case NECentralMessager::MSG_ID_broadcastKeyTyping:
                {
                    const String & nickName = mProxy->getParamnickName();
                    unsigned int cookie = mProxy->getParamcookie();
                    const String & newMessage = mProxy->getParamnewMessage();
                    broadcastKeyTyping( nickName, cookie, newMessage );
                }
                break;

            case NECentralMessager::MSG_ID_broadcastBroadcastMessage:
                {
                    const String & serverMessage = mProxy->getParamserverMessage();
                    const DateTime & dateTime = mProxy->getParamdateTime();
                    broadcastBroadcastMessage( serverMessage, dateTime );
                }
                break;

            default:
                {
                    TRACE_SCOPE(shared_generated_CentralMessagerClientBase_processNotificationEvent);
                    TRACE_ERR("Client object CentralMessagerClientBase of proxy [ %s ] received unexpected Response message ID [ %d ]."
                                , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString()
                                , msgId);
                    ASSERT(false);
                }
                break;
            }
        }        
        break;

    default:
        {
            TRACE_SCOPE(shared_generated_CentralMessagerClientBase_processNotificationEvent);
            TRACE_ERR("Client object CentralMessagerClientBase of proxy [ %s ] received unexpected Response result type [ %d ]."
                        , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString()
                        , static_cast<int>(result));
            ASSERT(false);
        }
        break;
    }
}

/************************************************************************
 * Error handling calls
 ************************************************************************/
DEF_TRACE_SCOPE(shared_generated_CentralMessagerClientBase_invalidResponse);
void CentralMessagerClientBase::invalidResponse( NECentralMessager::eMessageIDs InvalidRespId )
{
    TRACE_SCOPE(shared_generated_CentralMessagerClientBase_invalidResponse);
    TRACE_WARN(">>> There is an invalid response [ %s ] (value = [ %d ]) in client CentralMessagerClientBase with path [ %s ], which cannot be processed! Make error handling! <<<"
                    , NECentralMessager::getString(InvalidRespId)
                    , static_cast<unsigned int>(InvalidRespId)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString() );
                    
    ClientBase::responseInvalidNotImpelemnted("CentralMessagerClientBase", static_cast<unsigned int>(InvalidRespId));
}

DEF_TRACE_SCOPE(shared_generated_CentralMessagerClientBase_invalidRequest);
void CentralMessagerClientBase::invalidRequest( NECentralMessager::eMessageIDs InvalidReqId )
{
    TRACE_SCOPE(shared_generated_CentralMessagerClientBase_invalidRequest);
    TRACE_WARN(">>> There is an invalid request [ %s ] (value = [ %d ]) in client CentralMessagerClientBase with path [ %s ], which was not able to process! Make error handling! <<<"
                    , NECentralMessager::getString(InvalidReqId)
                    , static_cast<unsigned int>(InvalidReqId)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString() );
                    
    ASSERT(false);
}

DEF_TRACE_SCOPE(shared_generated_CentralMessagerClientBase_requestFailed);
void CentralMessagerClientBase::requestFailed( NECentralMessager::eMessageIDs FailureMsgId, NEService::eResultType FailureReason )
{
    TRACE_SCOPE(shared_generated_CentralMessagerClientBase_requestFailed);
    TRACE_WARN(">>> The request [ %s ] (value = [ %d ]) in Proxy [ %s ] of CentralMessagerClientBase failed with reason [ %s ]! Triggering appropriate request failed function! <<<"
                    , NECentralMessager::getString(FailureMsgId)
                    , static_cast<unsigned int>(FailureMsgId)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString()
                    , NEService::getString(FailureReason) );
                    
    unsigned int index = static_cast<unsigned int>(NECentralMessager::MSG_ID_INVALID);
    index = static_cast<unsigned int>( NEService::isResponseId(static_cast<unsigned int>(FailureMsgId)) ? NECentralMessager::getRequestId(FailureMsgId) : FailureMsgId);
    index = NEService::isRequestId(index)  ? GET_REQ_INDEX(index) : static_cast<unsigned int>(NECentralMessager::MSG_ID_INVALID);
    if ( index != static_cast<unsigned int>(NECentralMessager::MSG_ID_INVALID) )
    {
        (this->*NECentralMessager::failureFunctions[index])( FailureReason );
    }
    else
    {
        invalidRequest( FailureMsgId );
    }
}

/************************************************************************
 * Attribute notifications
 ************************************************************************/

/************************************************************************
 * Request failure / Response and Broadcast notifications
 ************************************************************************/
 
void CentralMessagerClientBase::requestSendMessageFailed( NEService::eResultType /* FailureReason */ )
{
    ClientBase::requestFailedNotImplemented( "CentralMessagerClientBase", static_cast<unsigned int>(NECentralMessager::MSG_ID_requestSendMessage) );
}

void CentralMessagerClientBase::requestKeyTypingFailed( NEService::eResultType /* FailureReason */ )
{
    ClientBase::requestFailedNotImplemented( "CentralMessagerClientBase", static_cast<unsigned int>(NECentralMessager::MSG_ID_requestKeyTyping) );
}

void CentralMessagerClientBase::broadcastSendMessage( const String & /* nickName */, unsigned int /* cookie */, const String & /* newMessage */, const DateTime & /* dateTime */ )
{
    ClientBase::broadcastNotImplemented( "CentralMessagerClientBase", static_cast<unsigned int>(NECentralMessager::MSG_ID_broadcastSendMessage) );
}

void CentralMessagerClientBase::broadcastKeyTyping( const String & /* nickName */, unsigned int /* cookie */, const String & /* newMessage */ )
{
    ClientBase::broadcastNotImplemented( "CentralMessagerClientBase", static_cast<unsigned int>(NECentralMessager::MSG_ID_broadcastKeyTyping) );
}

void CentralMessagerClientBase::broadcastBroadcastMessage( const String & /* serverMessage */, const DateTime & /* dateTime */ )
{
    ClientBase::broadcastNotImplemented( "CentralMessagerClientBase", static_cast<unsigned int>(NECentralMessager::MSG_ID_broadcastBroadcastMessage) );
}


//////////////////////////////////////////////////////////////////////////
// End generate shared/generated/private/CentralMessagerClientBase.cpp file
//////////////////////////////////////////////////////////////////////////
