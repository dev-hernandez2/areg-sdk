/************************************************************************
 * \file            edge/services/ConnectionServicing.hpp
 * \brief           The connection manager client component implementation
 ************************************************************************/

#include "edge/res/stdafx.h"
#include "areg/base/GEGlobal.h"
#include "edge/services/ConnectionServicing.hpp"
#include "areg/component/NERegistry.hpp"
#include "areg/component/ComponentLoader.hpp"
#include "areg/component/ComponentThread.hpp"
#include "shared/NECommonSettings.hpp"
#include "edge/NEDistributedApp.hpp"
#include "edge/DistrbutedApp.hpp"
#include "edge/ui/DistributedDialog.hpp"

BEGIN_MODEL(NECommonSettings::MODEL_NAME_DISTRIBUTED_CLIENT)

    BEGIN_REGISTER_THREAD( NEDistributedApp::THREAD_DISTRIBUTED )
        BEGIN_REGISTER_COMPONENT( NECommonSettings::COMP_NAME_DISTRIBUTED_CLIENT, ConnectionServicing )
            REGISTER_DEPENDENCY( NECommonSettings::COMP_NAME_CENTRAL_SERVER )
        END_REGISTER_COMPONENT( NECommonSettings::COMP_NAME_DISTRIBUTED_CLIENT )
    END_REGISTER_THREAD( NEDistributedApp::THREAD_DISTRIBUTED )

END_MODEL(NECommonSettings::MODEL_NAME_DISTRIBUTED_CLIENT)

DEF_TRACE_SCOPE( distrbutedapp_ConnectionServicing_CreateComponent );
DEF_TRACE_SCOPE( distrbutedapp_ConnectionServicing_DeleteComponent );

Component * ConnectionServicing::CreateComponent( const NERegistry::ComponentEntry & entry, ComponentThread & owner )
{
    TRACE_SCOPE( distrbutedapp_ConnectionServicing_CreateComponent );
    return new ConnectionServicing( owner, entry.mRoleName.getBuffer() );
}

void ConnectionServicing::DeleteComponent( Component & compObject, const NERegistry::ComponentEntry & entry )
{
    TRACE_SCOPE( distrbutedapp_ConnectionServicing_DeleteComponent );
    delete (&compObject);
}

ConnectionServicing::ConnectionServicing( ComponentThread & masterThread, const char * const roleName )
    : Component       ( masterThread, roleName )
{
}

ConnectionServicing::~ConnectionServicing( void )
{
}

void ConnectionServicing::startupComponent( ComponentThread & comThread )
{
    Component::startupComponent(comThread);
    DistributedDialog::PostServiceMessage( NEDistributedApp::CmdServiceStartup, 1, reinterpret_cast<LPARAM>(this) );
}

void ConnectionServicing::shutdownComponent( ComponentThread & comThread )
{
    DistributedDialog::PostServiceMessage( NEDistributedApp::CmdServiceStartup, 0, 0 );
    Component::shutdownComponent(comThread);
}
