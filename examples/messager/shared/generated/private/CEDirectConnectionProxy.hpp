//////////////////////////////////////////////////////////////////////////
// Begin generate shared/generated/private/CEDirectConnectionProxy.hpp file
//////////////////////////////////////////////////////////////////////////
#ifndef  SHARED_GENERATED_PRIVATE_CEDIRECTCONNECTIONPROXY_HPP
#define  SHARED_GENERATED_PRIVATE_CEDIRECTCONNECTIONPROXY_HPP
/************************************************************************
 * (c) copyright    2019
 *                  Create by AREGtech code generator tool from source DirectConnection.
 * Generated at     03.09.2019  02:48:09 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file            shared/generated/private/CEDirectConnectionProxy.hpp
 * \ingroup         DirectConnection Service Interface
 * \brief           This is an automatic generated code of DirectConnection Service Interface Proxy class declaration.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "shared/generated/NEDirectConnection.hpp"
#include "areg/component/CEProxyBase.hpp"
 
/************************************************************************
 * Dependencies
 ************************************************************************/
class CEDirectConnectionResponseEvent;
 
//////////////////////////////////////////////////////////////////////////
// CEDirectConnectionProxy class declaration Begin
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       DirectConnection Service Interface proxy class.
 *              Automatically instantiated per thread at client startup.
 *              Used to communicate with stub object and sending notifications
 *              to each client running in the thread instance.
 *
 *              Direct Connection service interface. It is used to send connection messages 
 *              to distributed applications. As soon as the distributed applications are connected, 
 *              each application has one instance of implemented service where 
 *              the role name is a nick name of a registered application.
 **/
class CEDirectConnectionProxy   : public CEProxyBase
{

private:
    //////////////////////////////////////////////////////////////////////////
    // CEDirectConnectionProxy::CEDirectConnectionServiceAvailableEvent class declaration
    //////////////////////////////////////////////////////////////////////////
    /**
     * \brief       The event is used to immediately notify new client
     *              about service availability and escape duplicating 
     *              notification for other, already connected clients.
     *  
     **/
    class CEDirectConnectionServiceAvailableEvent : public CEProxyBase::CEServiceAvailableEvent
    {
    //////////////////////////////////////////////////////////////////////////
    // Runtime internals
    //////////////////////////////////////////////////////////////////////////
        DECLARE_RUNTIME_EVENT(CEDirectConnectionProxy::CEDirectConnectionServiceAvailableEvent);

    //////////////////////////////////////////////////////////////////////////
    // Constructor/ Destructor
    //////////////////////////////////////////////////////////////////////////
    public:
        /**
         * \brief   Sets event consumer object to deliver notification.
         **/
        CEDirectConnectionServiceAvailableEvent( IENotificationEventConsumer & consumer );
        /**
         * \brief   Destructor
         **/
        virtual ~CEDirectConnectionServiceAvailableEvent( void );
    //////////////////////////////////////////////////////////////////////////
    // Forbidden calls
    //////////////////////////////////////////////////////////////////////////
    private:
        CEDirectConnectionServiceAvailableEvent( void );
        CEDirectConnectionServiceAvailableEvent( const CEDirectConnectionServiceAvailableEvent & /*src*/ );
        const CEDirectConnectionServiceAvailableEvent & operator = ( const CEDirectConnectionServiceAvailableEvent & /*src*/ );
    };
//////////////////////////////////////////////////////////////////////////
// CEDirectConnectionProxy predefined constants and static members.
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Returns existing or creates new proxy object if it is not existing
     *          in the current thread scope.
     * \param   roleName        The role name of stub component object
     * \param   connectListener The reference to connection listener
     * \param   ownerThread     The instance of thread to dispatch messages.
     * \return  Returns pointer to instantiated proxy object.
     **/
    static CEDirectConnectionProxy * CreateProxy( const char * const roleName, IEProxyListener & connectListener, CEDispatcherThread & ownerThread );

    /**
     * \brief   Returns existing or creates new proxy object if it is not existing
     *          in the current thread scope.
     * \param   roleName        The role name of stub component object
     * \param   connectListener The reference to connection listener
     * \param   ownerThread     The name of thread instance to dispatch messages.
     *                          If NULL, current dispatching thread is used to dispatch messages.
     * \return  Returns pointer to instantiated proxy object.
     **/
    static CEDirectConnectionProxy * CreateProxy( const char * const roleName, IEProxyListener & connectListener, const char * ownerThread = static_cast<const char *>(NULL) );

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor. Protected
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Constructor. Creates Proxy object by given role name. 
     *          Do not create proxy object directly, instead use CreateProxy function.
     * \param   roleName    The role name of proxy, i.e. role name of server component object.
     * \param   ownerThread The instance of thread to dispatch messages.
     *                      If NULL, current dispatching thread is used to dispatch messages.
     **/
    CEDirectConnectionProxy( const char * const roleName, CEDispatcherThread * ownerThread = static_cast<CEDispatcherThread *>(NULL) );
    /**
     * \brief   Destructor
     **/
    virtual ~CEDirectConnectionProxy( void );

//////////////////////////////////////////////////////////////////////////
// Attributes and operations.
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************
 * Attributes
 ************************************************************************/
    /**
     * \brief   Returns true if value of InitiatedConnections attribute is valid.
     *          If Update Notification is disabled, this method will return false.
     **/
    inline bool IsInitiatedConnectionsValid( void ) const;
    /**
     * \brief   Returns the value of InitiatedConnections attribute.
     *          To get valid value, the Update Notification should be enabled. 
     *          Attribute InitiatedConnections description: 
     *          The map of initiated connections and list of participants.
     * \param   state   On returns, contains the validation flag of attribute data. 
     *                  Check validation flag before use attribute value.
     **/
    inline const NEDirectConnection::MapParticipants & GetInitiatedConnections( NEService::eDataStateType & state ) const;

/************************************************************************
 * Parameters
 ************************************************************************/
    /**
     * \brief   Returns value of succeeded of response call function.
     *          The parameter is validated and set when Proxy is getting response call.
     *          Parameter succeeded description: 
     *          Flag, indicating whether the connection setup succeeded or not.
     **/
    inline const bool & GetParamsucceeded( void ) const;

    /**
     * \brief   Returns value of target of response call function.
     *          The parameter is validated and set when Proxy is getting response call.
     *          Parameter target description: 
     *          The targeted participant to include in chat-room
     **/
    inline const NEDirectConnection::sParticipant & GetParamtarget( void ) const;

    /**
     * \brief   Returns value of initiator of response call function.
     *          The parameter is validated and set when Proxy is getting response call.
     *          Parameter initiator description: 
     *          The chat-room initiator
     **/
    inline const NEDirectConnection::sInitiator & GetParaminitiator( void ) const;

    /**
     * \brief   Returns value of listParticipants of response call function.
     *          The parameter is validated and set when Proxy is getting response call.
     *          Parameter listParticipants description: 
     *          List of chat-room participants
     **/
    inline const NEDirectConnection::ListParticipants & GetParamlistParticipants( void ) const;

/************************************************************************
 * Requests
 ************************************************************************/
    /**
     * \brief   Request call.
     *          Request to setup the direct connection to chat room initiator.
     * \param   caller              The reference of caller object to get response.
     * \param   initiator           The initiator to setup connection
     * \param   listParticipants    The list of chat-room participants
     * \return  The sequence count number of call
     * \see     ResponseConnectoinSetup
     **/
    unsigned int RequestConnectoinSetup( IENotificationEventConsumer & caller, const NEDirectConnection::sInitiator & initiator, const NEDirectConnection::ListParticipants & listParticipants );
    
    /**
     * \brief   Request call.
     *          The request to add a participant in the direct chat-room.
     * \param   caller              The reference of caller object to get response.
     * \param   initiator           The initiator to add to chat-room
     * \param   listParticipants    List of participants
     * \return  The sequence count number of call
     * \see     ResponseAddParticipant
     **/
    unsigned int RequestAddParticipant( IENotificationEventConsumer & caller, const NEDirectConnection::sInitiator & initiator, const NEDirectConnection::ListParticipants & listParticipants );
    
    /**
     * \brief   Request call.
     *          Request to remove initiator from chat-room
     * \param   caller              The reference of caller object to get response.
     * \param   initiator           The initiator to remove from chat-room
     * \param   listParticipants    List of chat-room participants
     * \return  The sequence count number of call
     * \see     ResponseRemoveParticipant
     **/
    unsigned int RequestRemoveParticipant( IENotificationEventConsumer & caller, const NEDirectConnection::sInitiator & initiator, const NEDirectConnection::ListParticipants & listParticipants );
    
    /**
     * \brief   Request call.
     *          Request to close chat-room. There is no response expected.
     * \param   initiator   The initiator to close chat-room.
     * \see     Has no Response.
     **/
    void RequestCloseConnection( const NEDirectConnection::sInitiator & initiator );

//////////////////////////////////////////////////////////////////////////
// Operations.
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Checks whether there is already listener of Notification Event
     *          exists. If does not exit, adds new listener entry in the listener
     *          list. If need, send appropriate message to Stub to start 
     *          sending attribute update messages. If already listener
     *          exists in listener list and notifyAlways flag is true, 
     *          sends immediate update notification based on existing update data status.
     * \param   msgId           The Notification Message ID
     * \param   consumer        The pointer of Notification Event consumer
     * \param   notifyAlways    The flag, indicating whether the notification event
     *                          should be send to client immediately if the notification
     *                          already exists. By default it is false. 
     **/
    inline void SetNotification( const NEDirectConnection::eMessageIDs notifyId, IENotificationEventConsumer & listener, bool notifyAlways = false );
    /**
     * \brief   Clears listener entries of specified Notification Event consumer
     * \param   msgId       The Notification Message ID
     * \param   consumer    The pointer of Notification Event Consumer.
     * \return  
     **/
    inline void ClearNotification( const NEDirectConnection::eMessageIDs notifyId, IENotificationEventConsumer & listener );
    /**
     * \brief   Clears all notification for specified listener and unregisters it
     * \param   listener    The notification consumer object, which should be unregistered.
     **/
    inline void ClearAllNotifications( IENotificationEventConsumer & listener );

//////////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////////
protected:
/************************************************************************/
// IEProxyEventConsumer interface overrides.
/************************************************************************/
    /**
     * \brief   Method derived from IEProxyEventConsumer interface.
     *          Triggered when on the request to execute function
     *          on server side, Proxy have got response message.
     * \param   eventElem   The Service Response event object.
     *                      Contains response message and information
     *                      sent by Stub
     **/
    virtual void ProcessResponseEvent( CEServiceResponseEvent & eventElem );
    /**
     * \brief   Method derived from IEProxyEventConsumer interface.
     *          Triggered when on server side a certain Attribute 
     *          value has been updated.
     * \param   eventElem   The Service Response event object.
     *                      Contains new updated value of Attribute
     *                      and validation flag.
     **/
    virtual void ProcessAttributeEvent( CEServiceResponseEvent & eventElem );

/************************************************************************/
// CEProxyBase interface overrides.
/************************************************************************/

    /**
     * \brief   Creates notification event to send to client objects. 
     *          All Notification Events should be internal events and 
     *          should be instances of CENotificationEvent class.
     * \param   data    The Notification Event data object containing
     *                  notification information.
     * \return  Returns new created notification event object.
     **/
    virtual CENotificationEvent * CreateNotificationEvent( const CENotificationEventData & data ) const;

    /**
     * \brief   Create Request event to send to Stub object. 
     *          Request events are triggering request calls on Stub side.
     * \param   args    The buffer containing serialized arguments of request call.
     * \param   reqId   The ID of request call.
     * \return  Return pointer of valid Request event.
     **/
    virtual CEServiceRequestEvent * CreateRequestEvent( const CEEventDataStream & args, const unsigned int reqId );

    /**
     * \brief   Creates event requesting to receive update notification events.
     *          The caller should be address of current proxy and the target 
     *          should address of appropriate sub address.
     * \param   msgId       The message ID to send. Should be either attribute or response (info)
     * \param   reqType     The type of request.
     * \return  Returns valid pointer of created service request event object.
     **/
    virtual CEServiceRequestEvent * CreateNotificationRequestEvent( const unsigned int msgId, const NEService::eRequestType reqType );

    /**
     * \brief   Overwrite method to create response event from streaming object for 
     *          further dispatching by proxy.
     * \param   stream  Streaming object, which contains event data.
     * \return  If operation succeeds, returns valid pointer to Service Response event object.
     *          Otherwise, it returns NULL.
     **/
    virtual CERemoteResponseEvent * CreateRemoteResponseEvent( const IEInStream & stream ) const;

    /**
     * \brief   Overwrite method to create error remote response event.
     *          Function is triggered when remote request sent by proxy failed and did not reach target.
     * \param   addrProxy   Address of Proxy object, which sent request event
     * \param   msgId       Message ID, which was requested to process
     * \param   reason      Failure reason set by system
     * \param   seqNr       The sequence number of processing message.
     **/
    virtual CERemoteResponseEvent * CreateRemoteRequestFailedEvent( const CEProxyAddress & addrProxy, unsigned int msgId, const NEService::eResultType reason, unsigned int seqNr ) const;

    /**
     * \brief   Overwrite this method to create service available event to new instantiated clients.
     * \param   consumer    The instance of consumer, which receives service available event.
     * \return  If succeeds, returns valid pointer to service available event object.
     **/
    virtual CEProxyBase::CEServiceAvailableEvent * CreateServiceAvailableEvent( IENotificationEventConsumer & consumer );

    /**
     * \brief   Called to register all servicing listeners. It is called when proxy is instantiated.
     *          Overwrite method to add service event listeners.
     **/
    virtual void RegisterServiceListeners( void );

    /**
     * \brief   Called to unregister all servicing listeners. It is called when proxy is freed.
     *          Overwrite method to remove service event listeners.
     **/
    virtual void UnregisterServiceListeners( void );

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
/************************************************************************
 * Attribute members
 ************************************************************************/
    /**
     * \brief   InitiatedConnections attribute value.
     *          The map of initiated connections and list of participants.
     **/
    NEDirectConnection::MapParticipants     mInitiatedConnections;

/************************************************************************
 * Parameter members
 ************************************************************************/
    /**
     * \brief   succeeded parameter value.
     *          Flag, indicating whether the connection setup succeeded or not.
     **/
    bool                                    mParamsucceeded;

    /**
     * \brief   target parameter value.
     *          The targeted participant to include in chat-room
     **/
    NEDirectConnection::sParticipant        mParamtarget;

    /**
     * \brief   initiator parameter value.
     *          The chat-room initiator
     **/
    NEDirectConnection::sInitiator          mParaminitiator;

    /**
     * \brief   listParticipants parameter value.
     *          List of chat-room participants
     **/
    NEDirectConnection::ListParticipants    mParamlistParticipants;

//////////////////////////////////////////////////////////////////////////
// Hidden methods
//////////////////////////////////////////////////////////////////////////
private:
/************************************************************************
 * Hidden operations
 ************************************************************************/
    /**
     * \brief   Updates attribute and response call variable data received from Stub
     * \param   eventElem   The event object containing buffer with serialized data
     * \param   respId      The response message ID, which received. 
     *                      It is either attribute or response message ID
     **/
    void UpdateData( CEDirectConnectionResponseEvent & eventElem, const NEDirectConnection::eMessageIDs respId );
    /**
     * \brief   Starts processing response message. The received event contains
     *          ID of appropriate message to update and contains result flag.
     *          If request calls failed (busy), the ID in event is a request call ID.
     *          In all other cases, the ID is either response call, info call, 
     *          or attribute update notification call. 
     *          It will also update parameters and states.
     * \param   evenElem    The response object containing message ID, result and data.
     **/
    void ProcessResponse( CEDirectConnectionResponseEvent & evenElem );

    /**
     * \brief   returns reference CEDirectConnectionProxy object
     **/
    inline CEDirectConnectionProxy & self( void );

/************************************************************************
 * Hidden static operations
 ************************************************************************/
private:
    /**
     * \brief   Create proxy object.
     * \param   roleName    The server component role name
     * \param   ownerThread The instance of thread to dispatch messages.
     *                      If NULL, current dispatching thread is used to dispatch messages.
     * \return  Pointer to instantiated proxy object.
     **/
    static CEProxyBase * _createProxy( const char * const roleName, CEDispatcherThread * ownerThread = static_cast<CEDispatcherThread *>(NULL) );
    /**
     * \brief   Creates and returns service interface data object.
     **/
    static const NEService::SInterfaceData & _createInterfaceData( void );
};

//////////////////////////////////////////////////////////////////////////
// CEDirectConnectionProxy class declaration End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CEDirectConnectionProxy class inline functions implementation
//////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Inline operations
 ************************************************************************/

inline void CEDirectConnectionProxy::SetNotification( const NEDirectConnection::eMessageIDs notifyId, IENotificationEventConsumer & listener, bool notifyAlways /* = false */ )
{   CEProxyBase::SetNotification(static_cast<unsigned int>(notifyId), &listener, notifyAlways); }

inline void CEDirectConnectionProxy::ClearNotification( const NEDirectConnection::eMessageIDs notifyId, IENotificationEventConsumer & listener )
{   CEProxyBase::ClearNotification(static_cast<unsigned int>(notifyId), &listener); }

inline void CEDirectConnectionProxy::ClearAllNotifications( IENotificationEventConsumer & listener )
{   CEProxyBase::UnregisterListener( &listener ); }

inline CEDirectConnectionProxy & CEDirectConnectionProxy::self( void )
{   return (*this); }

/************************************************************************
 * Inline attributes
 ************************************************************************/

inline bool CEDirectConnectionProxy::IsInitiatedConnectionsValid( void ) const
{   return (GetProxyData().GetAttributeState(NEDirectConnection::MSG_ID_InitiatedConnections) == NEService::DATA_OK);   }

inline const NEDirectConnection::MapParticipants & CEDirectConnectionProxy::GetInitiatedConnections( NEService::eDataStateType & state ) const
{   state = GetProxyData().GetAttributeState(NEDirectConnection::MSG_ID_InitiatedConnections); return mInitiatedConnections;  }

/************************************************************************
 * Inline parameters
 ************************************************************************/

inline const bool & CEDirectConnectionProxy::GetParamsucceeded( void ) const
{   return mParamsucceeded;  }

inline const NEDirectConnection::sParticipant & CEDirectConnectionProxy::GetParamtarget( void ) const
{   return mParamtarget;  }

inline const NEDirectConnection::sInitiator & CEDirectConnectionProxy::GetParaminitiator( void ) const
{   return mParaminitiator;  }

inline const NEDirectConnection::ListParticipants & CEDirectConnectionProxy::GetParamlistParticipants( void ) const
{   return mParamlistParticipants;  }

#endif   // SHARED_GENERATED_PRIVATE_CEDIRECTCONNECTIONPROXY_HPP

//////////////////////////////////////////////////////////////////////////
// End generate shared/generated/private/CEDirectConnectionProxy.hpp file
//////////////////////////////////////////////////////////////////////////