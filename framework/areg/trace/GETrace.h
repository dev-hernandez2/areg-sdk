#ifndef AREG_TRACE_GETRACE_H
#define AREG_TRACE_GETRACE_H
/************************************************************************
 * \file        areg/trace/GETrace.h
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       NETrace namespace contains , structures and types.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/trace/NETrace.hpp"
#include "areg/trace/TraceScope.hpp"
#include "areg/trace/TraceMessage.hpp"

/**
 * \brief   Use these MACRO instead of direct declaration of scopes
 *          and messaging. The MACRO gives possibility to compile application(s)
 *          either with logging, or only with output on Debug output window,
 *          or ignore any logging activity.
 *
 *          ENABLE_TRACES is a global preprocessor definition
 *          indicating whether the applications should be compiled with
 *          logging or not. If this is not defined or zero,
 *          logging functionalities will be ignored, unless developer 
 *          is not setting scopes and activating tracing manually.
 *
 *          TRACE_DEBUG_OUTPUT is a global preprocessor definition
 *          indicating that the message outputs should be done on Debug
 *          output window.
 *          The TRACE_DEBUG_OUTPUT can be set only for Debug version.
 *          In release version this is reset.
 *
 *          If both preprocessor definitions are not defined or are zero,
 *          the application(s) will be compiled with no logging.
 *          To compile application(s) without logging, it is enough to
 *          set ENABLE_TRACES zero and not define TRACE_DEBUG_OUTPUT
 * 
 * \example     Start Logging:
 *              In this example, the application configures logging from default configuration file
 *              and start logging, to that the messages can be logged on target.
 *                  ...
 *              TRACER_START_LOGGING(NULL);
 * 
 * \example     Logging:
 *              In this example, the application created scope, which are used to start output messages.
 *              Depending on scope enabled priority, the appropriate message is logged.
 *              If scope priority is DEBUG, all message are logged.
 *              If scope enabled to output enter and exit, before any message is logged,
 *              the scope 'enter' message is logged on output target and when function is
 *              executed, at the end scope 'exit' message is logged.
 *              The scope should be declared before it is used. It can be used just before
 *              method where it is used. The scope should be declared only once, but can be
 *              used in more than one method.
 *                  ...
 *              DEF_TRACE_SCOPE(my_message_scope_in_MyFunction);
 *              void MyFunction( void )
 *              {
 *                  TRACE_SCOPE(my_message_scope_in_MyFunction);
 * 
 *                  TRACE_DBG("If debug priority is enabled, this message is logged");
 *                  TRACE_INFO("If logging priority is info of debug, this message is logged");
 *                  TRACE_WARN("If logging priority is warn, info or debug, this message is logged");
 *                  TRACE_ERROR("If logging priority is error, warn, info or debug, this message is logged");
 *                  TRACE_FATAL("If logging priority is set, this message is logged");
 *              }
 **/

#if defined(ENABLE_TRACES) || defined(_ENABLE_TRACES)

//////////////////////////////////////////////////////////////////////////
// if ENABLE_TRACES is defined and not zero
//////////////////////////////////////////////////////////////////////////

    /**
     * \brief   Returns true if logging is already configured and started
     **/
    #define IS_TRACE_STARTED()                          NETrace::isStarted()

    /**
     * \brief   Use this macro to load configuration file and start tracer.
     *          If config file name is NULL, it will load from default folder "./config/log.init"
     **/
    #define TRACER_START_LOGGING(configFile)            NETrace::startLogging((configFile))

    /**
     * \brief   Call to force to start logging.
     **/
    #define TRACER_FORCE_LOGGING()                      NETrace::forceStartLogging()

    /**
     * \brief   Either configures logging values from file or sets default values, enables and starts logging
     **/
    #define TRACER_CONFIGURE_AND_START(configFile)      NETrace::configAndStart((configFile))

    /**
     * \brief   Use this macro to stop logging
     **/
    #define TRACER_STOP_LOGGING()                       NETrace::stopLogging( )

    /**
     * \brief   Use this macro to define scope in source code. This will create scope variable and set name
     **/
    #define DEF_TRACE_SCOPE(scope)                      static TraceScope _##scope( #scope )

    /**
     * \brief   Use this macro to define message object by passing scope in constructor.
     *          This should be used in the same source file where scope was defined.
     *          The scope object should be defined before it is used.
     **/
    #define TRACE_SCOPE(scope)                          TraceMessage      _messager( _##scope )

    /**
     * \brief   Use this macro to log Debug priority messages in logging target (file or remote host)
     **/
    #define TRACE_DBG(...)                              if (_messager.isDbgEnabled())   _messager.logMessage( NETrace::PrioDebug    , __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Information priority messages in logging target (file or remote host)
     **/
    #define TRACE_INFO(...)                             if (_messager.isInfoEnabled())  _messager.logMessage( NETrace::PrioInfo     , __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Warning priority messages in logging target (file or remote host)
     **/
    #define TRACE_WARN(...)                             if (_messager.isWarnEnabled())  _messager.logMessage( NETrace::PrioWarning  , __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Error priority messages in logging target (file or remote host)
     **/
    #define TRACE_ERR(...)                              if (_messager.isErrEnabled())   _messager.logMessage( NETrace::PrioError    , __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Fatal Error priority messages in logging target (file or remote host)
     **/
    #define TRACE_FATAL(...)                            if (_messager.isFatalEnabled()  _messager.logMessage( NETrace::PrioFatal    , __VA_ARGS__ )

    /**
     * \brief   Use this macro to define global scope and global message object.
     *          There can be only one global scope defined in the every single source file.
     *          Call global tracing to use global scope. The global scope is used to make
     *          output generic messages withing single source.
     **/
    #define GLOBAL_TRACE_SCOPE(scope)                   static TraceMessage & _getGlobalScope( void )   \
                                                        {                                               \
                                                            static TraceScope     _##scope(#scope);     \
                                                            static TraceMessage  _messager(_##scope);   \
                                                            NETrace::activateScope( _##scope );         \
                                                            return _messager;                           \
                                                        }

    /**
     * \brief   Use this macro to log Debug priority messages in logging target (file or remote host).
     *          This macro will use global scope for logging. There can be only one global scope
     *          per source file defined.
     **/
    #define GLOBAL_DBG(...)                             _getGlobalScope().logDebug( __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Information priority messages in logging target (file or remote host)
     *          This macro will use global scope for logging. There can be only one global scope
     *          per source file defined.
     **/
    #define GLOBAL_INFO(...)                            _getGlobalScope().logInfo( __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Warning priority messages in logging target (file or remote host)
     *          This macro will use global scope for logging. There can be only one global scope
     *          per source file defined.
     **/
    #define GLOBAL_WARN(...)                            _getGlobalScope().logWarning( __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Error priority messages in logging target (file or remote host)
     *          This macro will use global scope for logging. There can be only one global scope
     *          per source file defined.
     **/
    #define GLOBAL_ERR(...)                             _getGlobalScope().logError( __VA_ARGS__ )
    /**
     * \brief   Use this macro to log Fatal Error priority messages in logging target (file or remote host)
     *          This macro will use global scope for logging. There can be only one global scope
     *          per source file defined.
     **/
    #define GLOBAL_FATAL(...)                           _getGlobalScope().logFatal( __VA_ARGS__ )

#else   // !defined(ENABLE_TRACES) && !defined(_ENABLE_TRACES)

//////////////////////////////////////////////////////////////////////////
// if TRACE_DEBUG_OUTPUT and TRACE_DEBUG_OUTPUT are not defined or both are zero
//////////////////////////////////////////////////////////////////////////

    /**
     * \brief   If ENABLE_TRACES, returns true, makes no effect
     **/
    #define IS_TRACE_STARTED()                          (true)

    /**
     * \brief   If ENABLE_TRACES, returns true, makes no effect
     **/
    #define TRACER_START_LOGGING(configFile)            (true)
    /**
     * \brief   If ENABLE_TRACES, returns true, makes no effect
     **/
    #define TRACER_FORCE_LOGGING()                      (true)
    /**
     * \brief   If ENABLE_TRACES, returns true, makes no effect
     **/
    #define TRACER_CONFIGURE_AND_START(configFile)      (true)
    /**
     * \brief   If ENABLE_TRACES is zero, does nothing
     **/
    #define TRACER_STOP_LOGGING()                       {}

    /**
     * \brief   If ENABLE_TRACES is zero, does nothing, no trace scope is declared.
     **/
    #define DEF_TRACE_SCOPE(scope)

    /**
     * \brief   If ENABLE_TRACES is zero, does nothing, no logging message is created.
     **/
    #define TRACE_SCOPE(scope)

    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define TRACE_DBG(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define TRACE_INFO(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define TRACE_WARN(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define TRACE_ERR(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define TRACE_FATAL(...)

    /**
     * \brief   If ENABLE_TRACES, does nothing, no global scope is initialized.
     **/
    #define GLOBAL_TRACE_SCOPE(scope)

    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define GLOBAL_DBG(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define GLOBAL_INFO(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define GLOBAL_WARN(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define GLOBAL_ERR(...)
    /**
     * \brief   If ENABLE_TRACES, does nothing, all parameters are ignored.
     **/
    #define GLOBAL_FATAL(...)

#endif  // (defined(ENABLE_TRACES) && defined(_ENABLE_TRACES))

#endif  // AREG_TRACE_GETRACE_H