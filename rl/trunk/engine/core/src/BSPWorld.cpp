#include "BSPWorld.h"

#include <OgreTextureManager.h>

#include "GameLoop.h"

#include "CameraActor.h"
#include "ActorManager.h"
#include "PhysicsManager.h"

namespace rl {

	BSPWorld::BSPWorld( ) : World(ST_INTERIOR, true)
	{
        getSceneManager()->getRootSceneNode()->pitch(Degree(90));
	}

	BSPWorld::~BSPWorld()
	{
		clearScene();
	}
	
	void BSPWorld::removeAllLightmaps()
	{
		int iID = 0;
		Resource* pRes;

		while((pRes = TextureManager::getSingleton( ).getByName( String( "@lightmap" ) + StringConverter::toString(iID) ) ) != 0 )
		{
			TextureManager::getSingleton( ).unload( pRes->getName() );
			iID++;
		}
	}

	void BSPWorld::initializeDefaultCamera(void)
	{
	    // Also change position, and set Quake-type orientation
        // Get random player start point
		mCamera = ActorManager::getSingleton().createCameraActor("DefaultCamera");
        ViewPoint defaultVP = mSceneMgr->getSuggestedViewpoint(true);

		mCamera->setNearClipDistance(10);
        mCamera->setFarClipDistance(10000);
//mCamera->setPosition( defaultVP.position );
  
		// Quake uses X/Y horizon, Z up
		mCamera->setFOVy(60);
//mCamera->rotate(defaultVP.orientation);
        // Don't yaw along variable axis, causes leaning
        mCamera->setFixedYawAxis(false, Vector3::UNIT_Y);
        // Look at the boxes
//mCamera->lookAt(-150,40,30);

		// Create one viewport, entire window
        Viewport* newVp = Ogre::Root::getSingleton().getAutoCreatedWindow()->addViewport(mCamera->getOgreCamera(),1);
        newVp->setBackgroundColour(ColourValue(0,0,0));
	}

	void BSPWorld::loadScene( const String&  levelName )
	{
		if( mbSceneLoaded )
			clearScene();
        
        // Setup BSPWorld
		try
		{
			mSceneMgr->setWorldGeometry(levelName);
            mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
			initializeDefaultCamera();
            mbSceneLoaded = true;
		}
		catch(...)
		{
			///@todo dokumentieren oder ausfuellen
		}		
	}

    //Enables / disables a 'sky box' i.e.
    void BSPWorld::setSkyBox(bool enable, const String &materialName, Real distance, bool drawFirst )
    {
        mSceneMgr->setSkyBox(enable, materialName,
            distance, drawFirst, Quaternion(Radian(Math::HALF_PI), Vector3::UNIT_X));
    }

	void BSPWorld::clearScene( )
    {
		Ogre::Root::getSingleton().getAutoCreatedWindow()->removeAllViewports();

        ActorManager::getSingleton().deleteAllActors();

		BSPWorld::removeAllLightmaps();

		mSceneMgr = Root::getSingleton().getSceneManager( ST_INTERIOR );
        PhysicsManager::getSingleton().setWorldScene( this );
		mbSceneLoaded = false;
    }
}
