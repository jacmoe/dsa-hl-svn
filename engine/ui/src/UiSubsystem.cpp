#include <CEGUI.h>
#include <renderers/OgreGUIRenderer/ogrerenderer.h>

#include "FixRubyHeaders.h"

#include "UiSubsystem.h"
#include "CoreSubsystem.h"

#include "RubyInterpreter.h"
#include "CeConsole.h"
#include "DebugWindow.h"
#include "ThirdPersonGameController.h"
#include "InputManager.h"

#include "GameLoop.h"
#include "ActorFactory.h"
#include "CameraActor.h"
#include "World.h"

template<> rl::UiSubsystem* Singleton<rl::UiSubsystem>::ms_Singleton = 0;

namespace rl {

	UiSubsystem& UiSubsystem::getSingleton(void)
	{
		return Singleton<UiSubsystem>::getSingleton();
	}

	UiSubsystem* UiSubsystem::getSingletonPtr(void)
	{
		return Singleton<UiSubsystem>::getSingletonPtr();
	}

	UiSubsystem::UiSubsystem()
	{
        mRequestExit = false;

        initializeUiSubsystem();
	}

    UiSubsystem::~UiSubsystem() 
    {  
		delete CeConsole::getSingletonPtr();
        delete InputManager::getSingletonPtr();

        GameLoop::getSingleton().removeSynchronizedTask(mGameController);
    }

    void UiSubsystem::initializeUiSubsystem( void )
    {
		using namespace CEGUI;

		CEGUI::OgreRenderer* rend = 
			new CEGUI::OgreRenderer(Ogre::Root::getSingleton().getAutoCreatedWindow(), 
									Ogre::RENDER_QUEUE_OVERLAY, 
									false, 
									3000,
									CoreSubsystem::getSingleton().getWorld()->getSceneManager());
		new System(rend);

		// load scheme and set up defaults
		CEGUI::SchemeManager::getSingleton().loadScheme((utf8*)"testscheme.xml");
		System::getSingleton().setDefaultMouseCursor((utf8*)"TaharezImagery", (utf8*)"MouseArrow");
		System::getSingleton().setDefaultFont((utf8*)"Tahoma-14");
		CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet", (utf8*)"root_wnd");
		sheet->setSize(
			Absolute, 
			CEGUI::Size(
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
		sheet->setPosition(Absolute, CEGUI::Point(0, 0));
		System::getSingleton().setGUISheet(sheet);

		//Initializing InputManager
        new InputManager();
		
		new CeConsole();
        new RubyInterpreter();        
        CeConsole::getSingleton().setInterpreter( new RubyInterpreter() );
        CeConsole::getSingleton().getInterpreter()->initializeInterpreter();

        new DebugWindow();
        DebugWindow::getSingleton().setVisible(false);

        InputManager::getSingleton().addKeyListener(DebugWindow::getSingletonPtr());

        CameraActor* camera = dynamic_cast<CameraActor*>(
            ActorFactory::getSingleton().getActor("DefaultCamera"));
		
        if( camera != 0 )
        {
            mGameController = new ThirdPersonGameController(camera->getOgreCamera(),
                0, //mHero->getEntity(),
                Ogre::Vector3(-120, 0, -64));
            GameLoop::getSingleton().addSynchronizedTask(mGameController);
        }
    }

    void UiSubsystem::requestExit()
    {
        mRequestExit = true;
    }
    
    bool UiSubsystem::isRequestingExit() const
    {
        return mRequestExit;
    }

}
