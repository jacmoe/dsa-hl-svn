/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "XmlResourceManager.h"

#include "CoreSubsystem.h"

#include <OgreStringConverter.h>
#include <OgreRoot.h>
#include <OgreLog.h>
#include <OgreConfigFile.h>
#include <OgreMeshManager.h>

#if OGRE_PLATFORM == PLATFORM_LINUX
#    include <iostream>
    using namespace std;
#endif

#include "DotSceneOctreeWorld.h"
#include "PhysicsManager.h"
#include "ActorManager.h"
#include "GameLoop.h"
#include "RubyInterpreter.h"
#include "Exception.h"
#include <ctime>


template<> rl::CoreSubsystem* Singleton<rl::CoreSubsystem>::ms_Singleton = 0;

namespace rl {

	CoreSubsystem& CoreSubsystem::getSingleton()
	{
		return Singleton<CoreSubsystem>::getSingleton();
	}

	CoreSubsystem* CoreSubsystem::getSingletonPtr()
	{
		return Singleton<CoreSubsystem>::getSingletonPtr();
	}

	CoreSubsystem::CoreSubsystem()
		: 	mWorld(NULL),
			mInterpreter(NULL),
			mActiveModule(""),
			mRootDir(".")
	{
		resetClock();
		initializeCoreSubsystem();        
		resetClock();
	}

    CoreSubsystem::~CoreSubsystem() 
    {  
		if(mWorld != 0)
			delete mWorld;

		mWorld = 0;        
    }

	void CoreSubsystem::startCore()
    {
		getInterpreter()->execute("load 'startup-global.rb'");
		Root::getSingleton().startRendering();
    }

    void CoreSubsystem::log(const String& msg)
    {
        if (LogManager::getSingletonPtr() == 0)
        {
            new LogManager();
        }
        LogManager::getSingleton().getLog( "logs/rlCore.log" )->logMessage(msg);
    }
    
	bool  CoreSubsystem::setupConfiguration()
    {
        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
        // Ein kleiner C-Trick: Wir versuchen erstmal die Konfiguration
        // wiederherzustellen. Klappt das nicht (Rueckgabe: FALSE), dann
        // zeigen wir den Konf.dialog.
        if(Root::getSingleton().restoreConfig()
            || Root::getSingleton().showConfigDialog())
        {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering window by passing 'true'
			Root::getSingleton().initialise(true);
            return true;
        }
        else
        {
            return false;
        }
    }

#if OGRE_PLATFORM == PLATFORM_LINUX
    const String CoreSubsystem::findConfRootDir()
    {
    	static String CURRENT_DIR = ".";
        char line[255];
        ifstream config;
        // Erstmal im Homeverzeichnis suchen.
        String path = ::getenv("HOME");
        path += "/.rl.conf";
        config.open(path.c_str());
        if (!config.is_open())
        {
            // Wir schliessen die Datei sicherheitshalber und
            // setzen den Zustand zur�ck (wichtig!)
            config.close();
            config.clear();
            // Jetzt schauen wir mal, ob in etc was zu finden ist.
            config.open("/etc/rl/rl.conf");
        } 
        // Haben  wir jetzt eine Datei?
        if (config.is_open())
        {
            while (!config.eof())
            {
                config.getline(line, 255);
                if (strlen(line) != 0)
                {
                    // Wir geben die erste nichtleere Zeile zur�ck.
                    return String(line);
                }
            }
        }
        // Klappt alles nicht.
        cerr<<"line "<<endl;
        return CURRENT_DIR;
    }

    const String CoreSubsystem::findRastullahConf()
    {
        char line[255];
        ifstream config;
        // Erstmal im Homeverzeichnis suchen.
        String home = ::getenv("HOME");
        String path = home + "/.rastullah.cfg";
        config.open(path.c_str());
        if (!config.is_open())
        {
            // Wir schliessen die Datei sicherheitshalber und
            // setzen den Zustand zur�ck (wichtig!)
            config.close();
            config.clear();
            // Jetzt schauen wir mal, ob in etc was zu finden ist.
            path = "/etc/rl/rastullah.cfg";
            config.open(path.c_str());
        }
        // Haben  wir jetzt eine Datei?
        if (config.is_open())
        {
            config.close();
            return path;
        }
        return home + "/.rastullah.cfg";
    }
#endif

	bool CoreSubsystem::initializeCoreSubsystem()
    {
		static String CONF_DIR = "modules/common/conf/";

		#if OGRE_PLATFORM == PLATFORM_WIN32
			new Root( 
				mRootDir+"/"+CONF_DIR+"plugins-win.cfg", 
				CONF_DIR+"rastullah.cfg", 
				"logs/ogre.log" );
		#elif OGRE_PLATFORM == PLATFORM_LINUX
            mRootDir = findConfRootDir();
			new Root( 
				mRootDir+"/"+CONF_DIR+"plugins-linux.cfg", 
				findRastullahConf(), 
				"logs/ogre.log" ); //TODO: Logfiles im aktuellen Verzeichnis? Nicht gut. Entweder in /var/log oder dort, wo die modules sind.
		#else
			new Root( 
				CONF_DIR+"plugins-mac.cfg", 
				CONF_DIR+"rastullah.cfg", 
				"logs/ogre.log" ); //TODO: siehe Linux
		#endif

        // Muss vor dem Laden der Ressourcen geschehen,
        // weil es sonst sofort angewandt wird.
        MeshManager::getSingleton().setBoundsPaddingFactor(0.0);
        
        initializeResources();

        bool carryOn = setupConfiguration();
        
		if (!carryOn) 
			return false;

		// Set default mipmap level (NB some APIs ignore this)
		TextureManager::getSingleton().setDefaultNumMipMaps(5);
		MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_TRILINEAR);
        MaterialManager::getSingleton().setDefaultAnisotropy(1);
        Log* log = LogManager::getSingleton().createLog( "logs/rlCore.log" );
        log->setLogDetail( LL_BOREME );
		
		mWorld = new DotSceneOctreeWorld();
		mInterpreter=new RubyInterpreter();
		new GameLoop();
        GameLoop::getSingleton().addSynchronizedTask(
            PhysicsManager::getSingletonPtr());
		new ActorManager();

        return true;
    }

	void CoreSubsystem::initializeResources()
    {
		new XmlResourceManager();
		
		addCommonSearchPath(mRootDir);
		addCommonSearchPath(mRootDir+"/modules");

        // Load resource paths from config file
        ConfigFile cf;
        cf.load(mRootDir+"/modules/modules.cfg");

        // Go through all settings in the file
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();

        String key, value;
        while (i.hasMoreElements())
        {
            key = i.peekNextKey();
            value = i.getNext();

			initializeModuleTextures(value);
			if (key.compare("common") == 0)
			{
				initializeModule(value);
				mCommonModules.push_back(value);
			}
			else if (key.compare("module") == 0)
				mActivatableModules.push_back(value);
        }		
    }

	void CoreSubsystem::initializeModuleTextures(const std::string& module)
	{
		std::string moduleDir = mRootDir+"/modules/"+module;
		ConfigFile cf;
		cf.load(moduleDir+"/conf/moduleconfig.cfg");
        ConfigFile::SettingsIterator i = cf.getSettingsIterator();

		std::string key, value;
        while (i.hasMoreElements())
        {
            key = i.peekNextKey();
            value = i.getNext();

			if (key.compare("TextureArchive") == 0)
				ResourceManager::addCommonArchiveEx(moduleDir+"/materials/"+value, "Zip");
			else if (key.compare("TextureDir") == 0)
				ResourceManager::addCommonSearchPath(moduleDir+"/materials/"+value);
			else if (key.compare("Archive") == 0)
				ResourceManager::addCommonArchiveEx(moduleDir+"/"+value, "Zip");
		}
		addCommonSearchPath(moduleDir+"/materials");
	}
	
	void CoreSubsystem::initializeModule(const std::string& module)
	{
		std::string moduleDir = mRootDir+"/modules/"+module;
		
	    addCommonSearchPath(moduleDir+"/conf");
        addCommonSearchPath(moduleDir+"/dsa");
		addCommonSearchPath(moduleDir+"/maps");
		addCommonSearchPath(moduleDir+"/models");
		addCommonSearchPath(moduleDir+"/sound");
        addCommonSearchPath(moduleDir+"/gui");
		addCommonSearchPath(moduleDir+"/gui/fonts");
		addCommonSearchPath(moduleDir+"/gui/imagesets");
		addCommonSearchPath(moduleDir+"/gui/schemes");
		addCommonSearchPath(moduleDir+"/gui/windows");
		addCommonSearchPath(moduleDir+"/gui/windows/buttons");
		addCommonSearchPath(moduleDir+"/dialogs");     

		if (getInterpreter() != NULL)
		{
			getInterpreter()->addSearchPath(moduleDir+"/scripts");
			getInterpreter()->addSearchPath(moduleDir+"/scripts/maps");
		}
	}

	void CoreSubsystem::addCommonSearchPath(const std::string& path)
	{
		try 
		{
		     ResourceManager::addCommonSearchPath(path);
        } 
		catch(...) 
		{} // and forget
	}

	void CoreSubsystem::unloadModule(const std::string& module)
	{
		//TODO: unloadModule
	}

	void CoreSubsystem::setActiveModule(const std::string& module)
	{
		StringVector::iterator mod;
		for (mod = mActivatableModules.begin(); 
			 mod != mActivatableModules.end(); mod++)
			 if ((*mod).compare(module) == 0)
				 break;

		if (mod == mActivatableModules.end())
			Throw(InvalidArgumentException, "Unknown Module '"+module+"'");

		if (mActiveModule.length() > 0)
			unloadModule(mActiveModule);

		initializeModule(module);
		mActiveModule = module;

		getInterpreter()->execute("load 'startup-module.rb'");
	}

	World* CoreSubsystem::getWorld()
    { 
        return mWorld;
    }

	void CoreSubsystem::setInterpreter(Interpreter* interpreter)
	{
		mInterpreter = interpreter;
	}

	Interpreter* CoreSubsystem::getInterpreter()
	{
		return mInterpreter;
	}

    void CoreSubsystem::makeScreenshot( const String& sName )
	{
		Ogre::Root::getSingleton().getAutoCreatedWindow()->
            writeContentsToTimestampedFile(sName, ".jpg");
	}

	StringVector CoreSubsystem::getActiveModules() const
	{
		StringVector modules(mCommonModules);
		modules.push_back(mActiveModule);
		return modules;
	}

	const StringVector& CoreSubsystem::getCommonModules() const
	{
		return mCommonModules;
	}

	const StringVector& CoreSubsystem::getActivatableModules() const
	{
		return mActivatableModules;
	}

	void CoreSubsystem::loadMap(const String type, const String filename, const String startupScript)
	{
		/*if (type.compare("BSP") == 0)
			mWorld = new BSPWorld( );
		else if (type.compare("Octree") == 0)
			mWorld = new DotSceneOctreeWorld();
		else if (type.compare("Nature") == 0)
			mWorld = new NatureWorld();
		else if (type.compare("Terrain") == 0)
			mWorld = new TerrainWorld();
		else
			Throw(RuntimeException, "Unknown world type");*/

		mWorld->loadScene(filename);
			
		if (startupScript.length() > 0)
            getInterpreter()->execute(String("load '") + startupScript + String("'"));
	}

	void CoreSubsystem::resetClock()
	{
		mClockStartTime = getCurrentTime();
	}

	RL_LONGLONG CoreSubsystem::getClock()
	{
		return getCurrentTime() - mClockStartTime;
	}

	RL_LONGLONG CoreSubsystem::getCurrentTime()
	{
		timeval timebuffer;
		gettimeofday(&timebuffer, NULL);
        
		return static_cast<RL_LONGLONG>(timebuffer.tv_sec) * 1000L + static_cast<RL_LONGLONG>(timebuffer.tv_usec/1000);
	}
}
