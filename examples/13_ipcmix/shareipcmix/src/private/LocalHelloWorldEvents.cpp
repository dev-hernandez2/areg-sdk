//////////////////////////////////////////////////////////////////////////
// Begin generate shareipcmix/src/private/LocalHelloWorldEvents.cpp file
//////////////////////////////////////////////////////////////////////////

/************************************************************************
 * (c) copyright    2021
 *                  Create by AREG SDK code generator tool from source LocalHelloWorld.
 * Generated at     11.06.2021  21:11:04 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file            shareipcmix/src/private/LocalHelloWorldEvents.cpp
 * \ingroup         LocalHelloWorld Service Interface
 * \brief           This is an automatic generated code of LocalHelloWorld Service Interface Event classes implementation.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
 #include "shareipcmix/src/private/LocalHelloWorldEvents.hpp"
 
//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldRequestEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldRequestEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   LocalHelloWorldRequestEvent Event runtime implementation
 **/
IMPLEMENT_RUNTIME_EVENT( LocalHelloWorldRequestEvent, LocalRequestEvent )

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldRequestEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

LocalHelloWorldRequestEvent::LocalHelloWorldRequestEvent( const ProxyAddress & fromSource, const StubAddress & toTarget, unsigned int reqId )
    : LocalRequestEvent( fromSource, toTarget, reqId )
{
    ; // do nothing
}

LocalHelloWorldRequestEvent::LocalHelloWorldRequestEvent( const EventDataStream & args, const ProxyAddress & fromSource, const StubAddress & toTarget, unsigned int reqId )
    : LocalRequestEvent( args, fromSource, toTarget, reqId, NELocalHelloWorld::getString( static_cast<NELocalHelloWorld::eMessageIDs>(reqId) ) )
{
    ; // do nothing
}


LocalHelloWorldRequestEvent::~LocalHelloWorldRequestEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldNotifyRequestEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldNotifyRequestEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_RUNTIME_EVENT( LocalHelloWorldNotifyRequestEvent, LocalNotifyRequestEvent )

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldNotifyRequestEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

LocalHelloWorldNotifyRequestEvent::LocalHelloWorldNotifyRequestEvent( const ProxyAddress & fromProxy, const StubAddress & toStub, unsigned int msgId, NEService::eRequestType reqType )
    : LocalNotifyRequestEvent( fromProxy, toStub, msgId, reqType )
{
    ; // do nothing
}


LocalHelloWorldNotifyRequestEvent::~LocalHelloWorldNotifyRequestEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldResponseEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldResponseEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_RUNTIME_EVENT( LocalHelloWorldResponseEvent, LocalResponseEvent )

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldResponseEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

LocalHelloWorldResponseEvent::LocalHelloWorldResponseEvent( const ProxyAddress & proxyTarget, NEService::eResultType result, unsigned int responseId, unsigned int seqNr /* = NEService::SEQUENCE_NUMBER_NOTIFY */ )
    : LocalResponseEvent( proxyTarget, result, responseId, seqNr )
{
    ; // do nothing
}

LocalHelloWorldResponseEvent::LocalHelloWorldResponseEvent( const EventDataStream & args, const ProxyAddress & proxyTarget, NEService::eResultType result, unsigned int responseId, unsigned int seqNr /* = NEService::SEQUENCE_NUMBER_NOTIFY */ )
    : LocalResponseEvent( args, proxyTarget, result, responseId, seqNr, NELocalHelloWorld::getString(static_cast<NELocalHelloWorld::eMessageIDs>(responseId)) )
{
    ; // do nothing
}

LocalHelloWorldResponseEvent::LocalHelloWorldResponseEvent( const ProxyAddress & proxyTarget, const LocalResponseEvent & source )
    : LocalResponseEvent( proxyTarget, source )
{
    ; // do nothing
}

LocalHelloWorldResponseEvent::~LocalHelloWorldResponseEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldResponseEvent class, Methods
//////////////////////////////////////////////////////////////////////////

ServiceResponseEvent* LocalHelloWorldResponseEvent::cloneForTarget( const ProxyAddress & target ) const
{
    return static_cast<ServiceResponseEvent *>( DEBUG_NEW LocalHelloWorldResponseEvent(target, static_cast<const LocalResponseEvent &>(*this)) );
}

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldNotificationEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldNotificationEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_RUNTIME_EVENT( LocalHelloWorldNotificationEvent, NotificationEvent )

//////////////////////////////////////////////////////////////////////////
// LocalHelloWorldNotificationEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

LocalHelloWorldNotificationEvent::LocalHelloWorldNotificationEvent( const NotificationEventData & data )
    : NotificationEvent   ( data )
{
    ; // do nothing
}

LocalHelloWorldNotificationEvent::~LocalHelloWorldNotificationEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// End generate shareipcmix/src/private/LocalHelloWorldEvents.cpp file
//////////////////////////////////////////////////////////////////////////