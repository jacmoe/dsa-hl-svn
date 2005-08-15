/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "ConfigurationManagerWin32.h"
using namespace std;

rl::ConfigurationManager* rl::ConfigurationManagerWin32::sInstance = 0;


namespace rl
{
	ConfigurationManager* ConfigurationManagerWin32::getSingletonPtr()
	{
		if(sInstance == NULL)
		{
			sInstance = new ConfigurationManagerWin32;
		}
		return sInstance;
	}
	
	ConfigurationManager& ConfigurationManagerWin32::getSingleton()
	{
		if(sInstance == NULL)
		{
			sInstance = new ConfigurationManagerWin32;
		}
		return *sInstance;
	}
	
	ConfigurationManagerWin32::ConfigurationManagerWin32()
	{
		Ogre::String confdir("modules/common/conf/");
		mPluginCfgPath = "./" + confdir + "plugins-win.cfg";
		mRastullahCfgPath = confdir + "rastullah.cfg";
		mOgreLogPath = "logs/ogre.log";
		mRastullahLogPath = "logs/rastullah.log";
		mModulesCfgPath = "./modules/modules.cfg";
		mModuleconfigCfgPath = "/conf/moduleconfig.cfg";
		mModulesRootDirectory = ".";
	}
	
	ConfigurationManagerWin32::~ConfigurationManagerWin32()
	{
	    sInstance = NULL;
	}
	
}
