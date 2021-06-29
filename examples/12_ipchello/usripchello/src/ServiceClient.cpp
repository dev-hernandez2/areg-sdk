/************************************************************************
 * \file        ServiceClient.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework examples
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       Collection of AREG SDK examples.
 *              This file contains simple implementation of service client to
 *              request message output
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "ServiceClient.hpp"
#include "areg/trace/GETrace.h"
#include "areg/component/ComponentThread.hpp"
#include "areg/appbase/Application.hpp"

DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_serviceConnected);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_onConnectedClientsUpdate);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_onRemainOutputUpdate);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_broadcastHelloClients);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_broadcastServiceUnavailable);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_responseHelloWorld);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_requestHelloWorldFailed);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_requestClientShutdownFailed);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_processTimer);
DEF_TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_ServiceClient);

const unsigned int  ServiceClient::TIMEOUT_VALUE    = 491;

Component * ServiceClient::CreateComponent(const NERegistry::ComponentEntry & entry, ComponentThread & owner)
{
    return DEBUG_NEW ServiceClient(entry, owner);
}

void ServiceClient::DeleteComponent(Component & compObject, const NERegistry::ComponentEntry & entry)
{
    delete (&compObject);
}


ServiceClient::ServiceClient(const NERegistry::ComponentEntry & entry, ComponentThread & owner)
    : Component             ( owner, entry.mRoleName )
    , HelloWorldClientBase  ( entry.mDependencyServices[0].mRoleName, static_cast<Component &>(self()) )
    , IETimerConsumer       ( )

    , mTimer                ( static_cast<IETimerConsumer &>(self()), timerName( static_cast<Component &>(self()) ) )
    , mID                   ( 0 )
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_ServiceClient);
    TRACE_DBG("Client: roleName [ %s ] of service [ %s ] owner [ %s ] in thread [ %s ] has timer [ %s ]"
                    , entry.mDependencyServices[0].mRoleName.getString()
                    , getServiceName().getString()
                    , getRoleName().getString()
                    , owner.getName().getString()
                    , mTimer.getName().getString());
    TRACE_DBG("Proxy: [ %s ]", ProxyAddress::convAddressToPath(getProxy()->getProxyAddress()).getString());
}

ServiceClient::~ServiceClient(void)
{
}

bool ServiceClient::serviceConnected(bool isConnected, ProxyBase & proxy)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_serviceConnected);
    bool result = HelloWorldClientBase::serviceConnected(isConnected, proxy);

    TRACE_DBG("Proxy [ %s ] is [ %s ]"
                , ProxyAddress::convAddressToPath(proxy.getProxyAddress()).getString()
                , isConnected ? "connected" : "disconnected");

    if (isConnected)
    {
        notifyOnRemainOutputUpdate(true);
        notifyOnBroadcastServiceUnavailable(true);
        mTimer.startTimer(ServiceClient::TIMEOUT_VALUE);
    }
    else
    {
        TRACE_WARN("Shutting down application!");
        mTimer.stopTimer();
        clearAllNotifications();
        Application::signalAppQuit();
    }

    return result;
}

void ServiceClient::onConnectedClientsUpdate(const NEHelloWorld::ConnectionList & ConnectedClients, NEService::eDataStateType state)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_onConnectedClientsUpdate);
    TRACE_DBG("Active client list of [ %s ] service is updated, active clients [ %d ], data is [ %s ]"
                    , getServiceRole().getString()
                    , ConnectedClients.getSize()
                    , NEService::getString(state));
}

void ServiceClient::onRemainOutputUpdate(short RemainOutput, NEService::eDataStateType state)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_onRemainOutputUpdate);
    TRACE_DBG("Service [ %s ]: Remain greeting outputs [ %d ], data is [ %s ]", getServiceRole().getString(), RemainOutput, NEService::getString(state));
}

void ServiceClient::responseHelloWorld(const NEHelloWorld::sConnectedClient & clientInfo)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_responseHelloWorld);
    TRACE_DBG("Service [ %s ]: Made output of [ %s ], client ID [ %d ]", getServiceRole().getString(), clientInfo.ccName.getString(), clientInfo.ccID);
    ASSERT(clientInfo.ccName == mTimer.getName());

    if ( mID == 0)
    {
        TRACE_DBG("Registring ID [ %d ] for service client [ %s ]", clientInfo.ccID, mTimer.getName().getString());
        mID = clientInfo.ccID;
    }

    if (isNotificationAssigned(NEHelloWorld::MSG_ID_broadcastHelloClients) == false)
    {
        notifyOnBroadcastHelloClients(true);
        notifyOnConnectedClientsUpdate(true);
    }
}

void ServiceClient::broadcastHelloClients(const NEHelloWorld::ConnectionList & clientList)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_broadcastHelloClients);

    TRACE_DBG("[ %d ] clients use service [ %s ]", clientList.getSize(), getServiceName().getString());
}

void ServiceClient::broadcastServiceUnavailable(void)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_broadcastServiceUnavailable);
    TRACE_WARN("Service reached message output maximum, starting shutdown procedure");
    TRACE_INFO("Sending request to shutdown service for client [ %s ] with ID [ %d ]", mTimer.getName().getString(), mID);

    requestClientShutdown(mID, mTimer.getName());
    mID = 0;
}

void ServiceClient::requestHelloWorldFailed(NEService::eResultType FailureReason)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_requestHelloWorldFailed);
    TRACE_ERR("Request to output greetings failed with reason [ %s ]", NEService::getString(FailureReason));
}

void ServiceClient::requestClientShutdownFailed(NEService::eResultType FailureReason)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_requestClientShutdownFailed);
    TRACE_ERR("Request to notify client shutdown failed with reason [ %s ]", NEService::getString(FailureReason));
}

void ServiceClient::processTimer(Timer & timer)
{
    TRACE_SCOPE(examples_12_ipchello_usripchello_ServiceClient_processTimer);
    ASSERT(&timer == &mTimer);

    TRACE_DBG("Timer [ %s ] expired, send request to output message.", timer.getName().getString());
    requestHelloWorld(timer.getName(), "remote");
}

inline String ServiceClient::timerName( Component & owner ) const
{
    String result = "";
    result += owner.getRoleName();
    result += NEUtilities::DEFAULT_SPECIAL_CHAR;
    result += getServiceRole();
    result += NEUtilities::DEFAULT_SPECIAL_CHAR;
    result += getServiceName();
    
    return result;
}