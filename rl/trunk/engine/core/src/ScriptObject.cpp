#include "ScriptObject.h"
#include "CoreSubsystem.h"
#include "Interpreter.h"
namespace rl {

ScriptObject::ScriptObject( const String& name ) 
{	
    mName = name;
	CoreSubsystem::getSingleton().getInterpreter()->registerScriptObject( this, mName );
 //   CeConsole::getSingleton().getInterpreter()->registerScriptObject( this, mName );
}

ScriptObject::~ScriptObject()
{
	CoreSubsystem::getSingleton().getInterpreter()->unregisterScriptObject( mName );
 //   CeConsole::getSingleton().getInterpreter()->unregisterScriptObject( mName );
}

void ScriptObject::setScript( const String& scriptname, const String& classname, int argc, const String args[] )
{
	CoreSubsystem::getSingleton().getInterpreter()->setScript( mName, scriptname, classname, argc, args );
//	CeConsole::getSingleton().getInterpreter()->setScript( mName, scriptname, classname, argc, args );
}

void ScriptObject::callFunction( const String& funcname, int argc, const String args[] )
{
	CoreSubsystem::getSingleton().getInterpreter()->callFunction( mName, funcname, argc, args );
//	CeConsole::getSingleton().getInterpreter()->callFunction( mName, funcname, argc, args );
}

int ScriptObject::callIntegerFunction( const String& funcname, int argc, const String args[] )
{
	return CoreSubsystem::getSingleton().getInterpreter()->callIntegerFunction( mName, funcname, argc, args );
}

}
