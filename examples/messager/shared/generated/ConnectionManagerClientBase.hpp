//////////////////////////////////////////////////////////////////////////
// Begin generate shared/generated/ConnectionManagerClientBase.hpp file
//////////////////////////////////////////////////////////////////////////
#ifndef  SHARED_GENERATED_CONNECTIONMANAGERCLIENTBASE_HPP
#define  SHARED_GENERATED_CONNECTIONMANAGERCLIENTBASE_HPP
/************************************************************************
 * (c) copyright    2021
 *                  Create by AREG SDK code generator tool from source ConnectionManager.
 * Generated at     23.05.2021  00:18:56 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file            shared/generated/ConnectionManagerClientBase.hpp
 * \ingroup         ConnectionManager Service Interface
 * \brief           This is an automatic generated code of ConnectionManager Service Interface Client base class declaration.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "shared/generated/NEConnectionManager.hpp"
#include "areg/component/ClientBase.hpp"
#include "areg/component/IEProxyListener.hpp"
#include "areg/component/NotificationEvent.hpp"

#include "shared/generated/private/ConnectionManagerProxy.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class Component;
class DispatcherThread;

//////////////////////////////////////////////////////////////////////////
// ConnectionManagerClientBase class declaration.
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       The interface of ConnectionManager Service Interface
 *              Client base object. This object should be inherited
 *              and overrides should be implemented.
 *
 *              The connection manager to register each application with client
 **/
class ConnectionManagerClientBase  : public    IEProxyListener, private ClientBase
{
//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor. Protected
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Initialize ConnectionManager Service Interface client object. 
     *          Specifies used service and owner thread.
     * \param   roleName    The role name assigned to ConnectionManager servicing component object.
     * \param   ownerThread The name of component owner thread to dispatch messages.
     *                      If NULL, all messages are dispatched in current component thread.
     **/
    ConnectionManagerClientBase( const char* roleName, const char * ownerThread = static_cast<const char *>(NULL) );

    /**
     * \brief   Initialize ConnectionManager Service Interface client object.
     *          Specifies used service and owner thread.
     * \param   roleName    The role name assigned to ConnectionManager servicing component object.
     * \param   ownerThread The instance of component owner thread to dispatch messages.
     **/
    ConnectionManagerClientBase( const char* roleName, DispatcherThread & ownerThread );

    /**
     * \brief   Initialize ConnectionManager Service Interface client object.
     *          Specifies used service and owner component.
     * \param   roleName    The role name assigned to ConnectionManager servicing component object.
     * \param   owner       The instance of client owner component. The component object should be already initialized.
     * \note    When this constructor is used, it is important that the Component object is already initialized.
     *          and the component thread is set.
     **/
    ConnectionManagerClientBase( const char* roleName, Component & owner );

    /**
     * \brief   Destructor.
     **/
    virtual ~ConnectionManagerClientBase( void );

//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Clears all notifications, stops receiving notifications from server
     **/
    void clearAllNotifications( void );

    /**
     * \brief   Returns true if the specified certain notification is already assigned.
     *          Otherwise returns false.
     * \param   msgId   The ID of message to check.
     **/
    bool isNotificationAssigned( NEConnectionManager::eMessageIDs msgId ) const;

    /**
     * \brief   Returns true if client object has got connection with servicing component
     **/
    bool isConnected( void ) const;

//////////////////////////////////////////////////////////////////////////
// Start Service Interface operations / attributes and overrides declaration
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ConnectionManager Interface Attributes
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************
 * Attribute ConnectionList functions
 ************************************************************************/
    /**
     * \brief   Returns true if value of ConnectionList attribute is valid.
     *          If Update Notification is disabled, this method will return false.
     **/
    bool isConnectionListValid( void ) const;
    /**
     * \brief   Returns the value of ConnectionList attribute.
     *          To get valid value, the Update Notification should be enabled. 
     *          Attribute ConnectionList description: 
     *          The map of all active connections
     * \param   state   On returns, contains the validation flag of attribute data. 
     *                  Check validation flag before use attribute value.
     * \see     isConnectionListValid, notifyConnectionListUpdate, onConnectionListUpdate
     **/
    const NEConnectionManager::MapConnection & getConnectionList( NEService::eDataStateType & state ) const;
    /**
     * \brief   Call to enable or disable receiving notifications on ConnectionList attribute update.
     *          Once notification is enabled and the data is updated, 
     *          the getConnectionList method will return valid data 
     *          Attribute ConnectionList description:
     *          The map of all active connections
     * \param   notify  If true, notification will be enable. If false, notification is disabled
     * \see     isConnectionListValid, getConnectionList, onConnectionListUpdate
     **/
    void notifyOnConnectionListUpdate( bool notify = true );
    /**
     * \brief   Triggered, when ConnectionList attribute is updated. The function contains
     *          attribute value and validation flag. When notification is enabled,
     *          the method should be overwritten in derived class.
     *          Attributes ConnectionList description: 
     *          The map of all active connections
     * \param   ConnectionList  The value of ConnectionList attribute.
     * \param   state           The data validation flag.
     **/
    virtual void onConnectionListUpdate( const NEConnectionManager::MapConnection & ConnectionList, NEService::eDataStateType state );


//////////////////////////////////////////////////////////////////////////
// ConnectionManager Interface Requests / Responses / Broadcasts
//////////////////////////////////////////////////////////////////////////
public:

/************************************************************************
 * Request Connet
 ************************************************************************/
    /**
     * \brief   Request call.
     *          The request to connect to system
     * \param   nickName    The nick name of connecting client service
     * \param   dateTime    The connecting client connection timestamp in UTC
     * \return  The sequence count number of call
     * \see     responseConnect
     **/
    unsigned int requestConnet( const String & nickName, const DateTime & dateTime );
    /**
     * \brief   Overwrite to handle error of Connet request call.
     * \param   FailureReason   The failure reason value of request call.
     **/
    virtual void requestConnetFailed( NEService::eResultType FailureReason );
    
/************************************************************************
 * Request RegisterConnection
 ************************************************************************/
    /**
     * \brief   Request call.
     *          Request to register direct connection service.
     * \param   nickName        The nick name of connection used in service
     * \param   cookie          The cookie value to set
     * \param   connectCookie   The cookie to use for registration. 
     * \param   dateRegister    The connection registration timestamp
     * \return  The sequence count number of call
     * \see     responseRegisterConnection
     **/
    unsigned int requestRegisterConnection( const String & nickName, unsigned int cookie, unsigned int connectCookie, const DateTime & dateRegister );
    /**
     * \brief   Overwrite to handle error of RegisterConnection request call.
     * \param   FailureReason   The failure reason value of request call.
     **/
    virtual void requestRegisterConnectionFailed( NEService::eResultType FailureReason );
    
/************************************************************************
 * Request Diconnect
 ************************************************************************/
    /**
     * \brief   Request call.
     *          Request to disconnect.
     * \param   nickName    The nick name of client connection service
     * \param   cookie      The cookie given by connection manager. If invalid, it will search by nickName parameter
     * \param   dateTime    Disconnect request date-time
     * \see     Has no response
     **/
    inline void requestDiconnect( const String & nickName, unsigned int cookie, const DateTime & dateTime );
    /**
     * \brief   Overwrite to handle error of Diconnect request call.
     * \param   FailureReason   The failure reason value of request call.
     **/
    virtual void requestDiconnectFailed( NEService::eResultType FailureReason );
    
/************************************************************************
 * Response Connect
 ************************************************************************/
    /**
     * \brief   Response callback.
     *          The result of connection request.
     *          Overwrite, if need to handle Response call of server object. 
     *          This call will be automatically triggered, on every appropriate request call
     * \param   nickName    Client connection data, which contains nick name and connection date-time
     * \param   cookie      Connection assigned cookie for client service
     * \param   dateTime    The client service connection timestamp in UTC
     * \param   result      The connection operation result
     * \see     requestConnet
     **/
    virtual void responseConnect( const String & nickName, unsigned int cookie, const DateTime & dateTime, NEConnectionManager::eConnectionResult result );
    /**
     * \brief   Call to enable or disable receiving notifications on Connect response call.
     *          This function is triggered, when client object is interested only on response result
     *          without triggering request call.
     * \param   notify  If true, notification will be enable. If false, notification is disabled
     **/
    inline void notifyOnResponseConnect( bool notify = true );

/************************************************************************
 * Response RegisterConnection
 ************************************************************************/
    /**
     * \brief   Response callback.
     *          The result of request to register direct connection service.
     *          Overwrite, if need to handle Response call of server object. 
     *          This call will be automatically triggered, on every appropriate request call
     * \param   connection      The connection structure after registration.
     * \param   connectionList  The list of all servicing connections.
     * \param   success         Flag, indicating whether the request to register connection succeeded or not.
     * \see     requestRegisterConnection
     **/
    virtual void responseRegisterConnection( const NEConnectionManager::sConnection & connection, const NEConnectionManager::ListConnection & connectionList, bool success );
    /**
     * \brief   Call to enable or disable receiving notifications on RegisterConnection response call.
     *          This function is triggered, when client object is interested only on response result
     *          without triggering request call.
     * \param   notify  If true, notification will be enable. If false, notification is disabled
     **/
    inline void notifyOnResponseRegisterConnection( bool notify = true );

/************************************************************************
 * Broadcast ConnectionUpdated
 ************************************************************************/
    /**
     * \brief   Server broadcast.
     *          Notifies all client components the connection has been updated.
     *          Overwrite, if need to handle Broadcast call of server object. 
     *          This call will be automatically triggered, on every appropriate request call
     * \param   updatedList The complete list of clients to send.
     **/
    virtual void broadcastConnectionUpdated( const NEConnectionManager::MapConnection & updatedList );
    /**
     * \brief   Call to enable or disable receiving notifications on ConnectionUpdated broadcast call.
     *          This function is triggered, when client object is interested only on response result
     *          without triggering request call.
     * \param   notify  If true, notification will be enable. If false, notification is disabled
     **/
    inline void notifyOnBroadcastConnectionUpdated( bool notify = true );

/************************************************************************
 * Broadcast ClientConnected
 ************************************************************************/
    /**
     * \brief   Server broadcast.
     *          Notifies that a new client has been connected.
     *          Overwrite, if need to handle Broadcast call of server object. 
     *          This call will be automatically triggered, on every appropriate request call
     * \param   clientConnected New client data, which contains nick name and connected date-time
     **/
    virtual void broadcastClientConnected( const NEConnectionManager::sConnection & clientConnected );
    /**
     * \brief   Call to enable or disable receiving notifications on ClientConnected broadcast call.
     *          This function is triggered, when client object is interested only on response result
     *          without triggering request call.
     * \param   notify  If true, notification will be enable. If false, notification is disabled
     **/
    inline void notifyOnBroadcastClientConnected( bool notify = true );

/************************************************************************
 * Broadcast ClientDisconnected
 ************************************************************************/
    /**
     * \brief   Server broadcast.
     *          Notifies a client has been disconnected.
     *          Overwrite, if need to handle Broadcast call of server object. 
     *          This call will be automatically triggered, on every appropriate request call
     * \param   clientLeft  The connection data of disconnected client.
     **/
    virtual void broadcastClientDisconnected( const NEConnectionManager::sConnection & clientLeft );
    /**
     * \brief   Call to enable or disable receiving notifications on ClientDisconnected broadcast call.
     *          This function is triggered, when client object is interested only on response result
     *          without triggering request call.
     * \param   notify  If true, notification will be enable. If false, notification is disabled
     **/
    inline void notifyOnBroadcastClientDisconnected( bool notify = true );

//////////////////////////////////////////////////////////////////////////
// End Service Interface operations / attributes and overrides declaration
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////////
protected:
/************************************************************************/
// IEProxyListener Overrides
/************************************************************************/
    /**
     * \brief   Triggered by Proxy, when gets service connected event.
     *          Make client initializations in this function. No request
     *          will be processed until this function is not called for
     *          client object. Also set listeners here if client is interested
     *          to receive update notifications.
     * \param   isConnected     The flag, indicating whether service is connected
     *                          or disconnected. Make cleanups and stop sending
     *                          requests or assigning for notification if
     *                          this flag is false. No event to Stub target will
     *                          be sent, until no service connected event is
     *                          received.
     * \param   proxy           The Service Interface Proxy object, which is
     *                          notifying service connection.
     * \return  Return true if this service connect notification was relevant to client object,
     *          i.e. if passed Proxy address is equal to the Proxy object that client has.
     *          If Proxy objects are not equal, it should return false;
     **/
    virtual bool serviceConnected( bool isConnected, ProxyBase & proxy );

protected:
/************************************************************************/
// ConnectionManagerClientBase Error Handling overrides
/************************************************************************/

    /**
     * \brief   Overwrite this method if need to make error handling on invalid response
     * \param   InvalidRespId   The ID of invalid response
     **/
    virtual void invalidResponse( NEConnectionManager::eMessageIDs InvalidRespId );

    /**
     * \brief   Overwrite this method if need to make error handling on invalid request
     * \param   InvalidReqId    The ID of invalid request
     **/
    virtual void invalidRequest( NEConnectionManager::eMessageIDs InvalidReqId );
    
    /**
     * \brief   By default, the function calls appropriate request failure function.
     *          Overwrite this method if need to make error handling on request failed.
     * \param   msgId           The ID of either response of request message, which failed. Normally ID of request.
     * \param   FailureReason   The failure reason value of request call.
     **/
    virtual void requestFailed( NEConnectionManager::eMessageIDs FailureMsgId, NEService::eResultType FailureReason );

//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////
protected:

    /**
     * \brief   Returns the current sequence number
     **/
    unsigned int getCurrentSequenceNr( void ) const;

    /**
     * \brief   Call to recreate Proxy for the client. This call will remove and unregister all existing notifications
     *          and the client will not receive pending update or response notifications. 
     *          The client first will receive disconnect message, then again connect.
     *          Reset update notifications manually in connect if need.          
     *          This function call can be used when the client instance should be registered and run in new thread.
     *
     * \return  Returns true if Proxy was created with success.
     **/
     bool recreateProxy( void );

     /**
      * \brief  Returns pointer to client dispatcher thread where the messages are dispatched.
      *         The function can return NULL if Proxy was not instantiated yet.
      **/
     DispatcherThread * getDispatcherThread( void );

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   Connection flag. If true, the client object is connected to server
     **/
    bool                mIsConnected;
    /**
     * \brief   The counter of sequence number
     **/
    unsigned int        mCurrSequenceNr;
    /**
     * \brief   Pointer of Proxy object providing communication
     **/
    ConnectionManagerProxy *   mProxy;

//////////////////////////////////////////////////////////////////////////
// Hidden methods
//////////////////////////////////////////////////////////////////////////
private:
/************************************************************************/
// IENotificationEventConsumer overrides
/************************************************************************/

    /**
     * \brief   Is processing notification event calls.
     * \param   eventElem   Notification Event object to process
     **/
    virtual void processNotificationEvent( NotificationEvent & eventElem );
    
private:
    /**
     * \brief   Enables / Disables notification flags on appropriate message call.
     * \param   msgId   The ID of message to enable / disable notification
     * \param   notify  If true, the notification is enabled. 
     *                  If false, the notification is disabled.
     * \param   always  Flag, indicating whether should notify always or not.
     *                  if 'notify' parameter is true and the notification is already
     *                  assigned, then if parameter 'always' is true, it will trigger
     *                  notification immediately after call. 
     **/
    void notifyOn( NEConnectionManager::eMessageIDs msgId, bool notify, bool always = false );
    /**
     * \brief   Returns reference of ConnectionManagerClientBase object
     **/
    ConnectionManagerClientBase & self( void );

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    ConnectionManagerClientBase( void );
    ConnectionManagerClientBase( const ConnectionManagerClientBase & /* src */ );
    const ConnectionManagerClientBase & operator = ( const ConnectionManagerClientBase & /* src */ );
};

//////////////////////////////////////////////////////////////////////////
// ConnectionManagerClientBase class inline functions implementation
//////////////////////////////////////////////////////////////////////////

inline ConnectionManagerClientBase & ConnectionManagerClientBase::self( void )
{
    return (*this);
}

inline unsigned int ConnectionManagerClientBase::getCurrentSequenceNr( void ) const
{
    return mCurrSequenceNr;
}

inline bool ConnectionManagerClientBase::isConnected( void ) const
{
    return mIsConnected;
}

inline bool ConnectionManagerClientBase::isNotificationAssigned( NEConnectionManager::eMessageIDs msgId ) const
{
    return mProxy->hasNotificationListener(static_cast<unsigned int>(msgId));
}

/************************************************************************
 * Attribute inline functions
 ************************************************************************/

inline bool ConnectionManagerClientBase::isConnectionListValid( void ) const
{
   return mProxy->isConnectionListValid( );
}
inline const NEConnectionManager::MapConnection & ConnectionManagerClientBase::getConnectionList( NEService::eDataStateType & state ) const
{
    return mProxy->getConnectionList( state );
}

inline void ConnectionManagerClientBase::notifyOnConnectionListUpdate( bool notify /* = true */ )
{
    notifyOn( NEConnectionManager::MSG_ID_ConnectionList, notify, false );
}

/************************************************************************
 * Request calls
 ************************************************************************/

inline unsigned int ConnectionManagerClientBase::requestConnet( const String & nickName, const DateTime & dateTime )
{
    return mProxy->requestConnet( static_cast<IENotificationEventConsumer &>(self()), nickName, dateTime );
}

inline unsigned int ConnectionManagerClientBase::requestRegisterConnection( const String & nickName, unsigned int cookie, unsigned int connectCookie, const DateTime & dateRegister )
{
    return mProxy->requestRegisterConnection( static_cast<IENotificationEventConsumer &>(self()), nickName, cookie, connectCookie, dateRegister );
}

inline void ConnectionManagerClientBase::requestDiconnect( const String & nickName, unsigned int cookie, const DateTime & dateTime )
{
    mProxy->requestDiconnect( nickName, cookie, dateTime );
}

/************************************************************************
 * Response notifications
 ************************************************************************/

inline void ConnectionManagerClientBase::notifyOnResponseConnect( bool notify /* = true */ )
{
    notifyOn(NEConnectionManager::MSG_ID_responseConnect, notify, false);
}

inline void ConnectionManagerClientBase::notifyOnResponseRegisterConnection( bool notify /* = true */ )
{
    notifyOn(NEConnectionManager::MSG_ID_responseRegisterConnection, notify, false);
}

/************************************************************************
 * Broadcast notifications
 ************************************************************************/

inline void ConnectionManagerClientBase::notifyOnBroadcastConnectionUpdated( bool notify /* = true */ )
{
    notifyOn(NEConnectionManager::MSG_ID_broadcastConnectionUpdated, notify, false);
}

inline void ConnectionManagerClientBase::notifyOnBroadcastClientConnected( bool notify /* = true */ )
{
    notifyOn(NEConnectionManager::MSG_ID_broadcastClientConnected, notify, false);
}

inline void ConnectionManagerClientBase::notifyOnBroadcastClientDisconnected( bool notify /* = true */ )
{
    notifyOn(NEConnectionManager::MSG_ID_broadcastClientDisconnected, notify, false);
}

#endif   // SHARED_GENERATED_CONNECTIONMANAGERCLIENTBASE_HPP

//////////////////////////////////////////////////////////////////////////
// End generate shared/generated/ConnectionManagerClientBase.hpp file
//////////////////////////////////////////////////////////////////////////