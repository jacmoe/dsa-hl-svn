/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "AiSubsystem.h"
#include "AiWorld.h"
#include "AgentManager.h"
//#include "GameLoop.h"

using namespace Ogre;

template<> rl::AiSubsystem* Singleton<rl::AiSubsystem>::ms_Singleton = 0;

using namespace rl;

AiSubsystem& AiSubsystem::getSingleton(void)
{
	return Singleton<AiSubsystem>::getSingleton();
}

AiSubsystem* AiSubsystem::getSingletonPtr(void)
{
	return Singleton<AiSubsystem>::getSingletonPtr();
}

AiSubsystem::AiSubsystem(void)
	: mAgentManager(NULL), 
	  mWorld(NULL)
{
	mAgentManager = new AgentManager();
	mWorld = new AiWorld();
//  maybe the AgentManager need to be a GameTask someday...
//  atm, it is only a PhysicsController
//	GameLoopManager::getSingleton().addSynchronizedTask(AgentManager::getSingletonPtr(),
//		FRAME_STARTED);
}

AiSubsystem::~AiSubsystem(void)
{
	delete mAgentManager;
	delete mWorld;
}

