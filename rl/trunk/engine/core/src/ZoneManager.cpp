/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Clarified Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Clarified Artistic License for more details.
*
*  You should have received a copy of the Clarified Artistic License
*  along with this program; if not you can get it here
*  http://www.jpaulmorrison.com/fbp/artistic2.htm.
*/
#include "stdinc.h" //precompiled header

#include "ZoneManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "CoreSubsystem.h"
#include "GameEventManager.h"
#include "Zone.h"
#include "World.h"
#include "Sound.h"
#include "SoundManager.h"
#include "SoundDriver.h"
#include "Trigger.h"


using namespace Ogre;


template<> rl::ZoneManager* Ogre::Singleton<rl::ZoneManager>::ms_Singleton = 0;

namespace rl
{
	ZoneManager::ZoneManager()
		: Ogre::Singleton<ZoneManager>(),
        GameTask(false)
	{
		// the default zone is active when no other Zone is active
		mDefaultZone = new Zone(0,false);
        mNextZoneId = 1;
		mActiveZones.push_front(mDefaultZone);
	}

	ZoneManager::~ZoneManager()
	{
        GameEventManager::getSingleton().removeAreaListener(this);
        GameEventManager::getSingleton().removeQueuedDeletionSources();
		for (ZoneMap::iterator it = mZones.begin(); it != mZones.end(); ++it)
		{
			Zone* curZone = (*it).second;
            delete curZone;
		}
		delete mDefaultZone;
	}

	Zone* ZoneManager::getDefaultZone()
	{
		return mDefaultZone;
	}

	Zone* ZoneManager::getZone(const Ogre::String& name)
	{
		ZoneMap::const_iterator it = mZones.find(name);
		if (it == mZones.end())
		{
			return NULL;
		}
		else
		{
			return (*it).second;
		}
	}

	Zone* ZoneManager::getZone(long id)
	{
		std::map<long, Zone*>::const_iterator it = mZonesIdMap.find(id);
		if (it == mZonesIdMap.end())
		{
			return NULL;
		}
		else
		{
			return (*it).second;
		}
	}

    Zone* ZoneManager::createZone(const Ogre::String& name, bool needsToBeSaved)
	{
        Zone* zone = new Zone(mNextZoneId, needsToBeSaved);
        mZonesIdMap[mNextZoneId] = zone;
        mZones[name] = zone;
        mNextZoneId++;

		return zone;
	}

    void ZoneManager::destroyZone(const Ogre::String& name)
    {
        Zone* zone = getZone(name);
        if(zone == NULL)
            return;

        // remove from active zones
        if( isZoneActive(zone) )
            mActiveZones.remove(zone);

        ZoneMap::iterator it = mZones.find(name);
        if( it != mZones.end() )
            mZones.erase(it);

        std::map<long, Zone*>::iterator it_ = mZonesIdMap.find(zone->getId());
        if( it_ != mZonesIdMap.end() )
            mZonesIdMap.erase(it_);


        mZonesToDelete.push_back(zone);
    }

    void ZoneManager::doDestroyZone(Zone *zone)
    {
        //destroy all areas
        GameAreaEventSourceList::iterator iter = zone->getEventSources().begin();
        for( ; iter != zone->getEventSources().end(); iter++ )
        {
            // if we have our own actors, remove them
            if( (*iter)->getActor() )
                ActorManager::getSingleton().destroyActor( (*iter)->getActor() ); // this also removes all areaeventsources
            else
		        GameEventManager::getSingleton().removeAreaEventSource(
                    (*iter));
        }

        // ask all triggers if they want to be deleted
        std::list<Trigger*> triggerList = zone->getTriggers();
        std::list<Trigger*>::iterator trig = triggerList.begin();
        for( ; trig != triggerList.end(); trig++ )
        {
            if( (*trig)->deleteIfZoneDestroyed() )
                delete (*trig);
        }

        delete zone;
    }

    const Ogre::String& ZoneManager::getName() const
    {
        static Ogre::String NAME = "ZoneManager";

        return NAME;
    }

    void ZoneManager::run(Ogre::Real elapsedTime)
    {
        std::list<Zone*>::iterator it = mZonesToDelete.begin();
        for( ; it != mZonesToDelete.end(); it++)
            doDestroyZone(*it);
        mZonesToDelete.clear();
    }

    bool ZoneManager::isZoneActive(const Zone *zone) const
    {
        if( zone == NULL )
            return false;

        std::list<Zone*>::const_iterator iter = std::find(mActiveZones.begin(), mActiveZones.end(), zone);
        return iter != mActiveZones.end();
    }

    void ZoneManager::addAreaToZone(const Ogre::String& name, 
        AxisAlignedBox aabb, GeometryType geom,
        Vector3 position, Vector3 offset, Quaternion orientation,
        Real transitionDistance,
        unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if( !zone )
            return;


        GameAreaEventSource* gam =
            GameEventManager::getSingleton().addAreaListener(NULL, 
                aabb, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId(zone->getId());
        zone->addEventSource(gam);
    }
    
    void ZoneManager::subtractAreaFromZone(const Ogre::String& name, 
        AxisAlignedBox aabb, GeometryType geom,
        Vector3 position, Vector3 offset, Quaternion orientation,
        Real transitionDistance,
        unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if( !zone )
            return;

        GameAreaEventSource* gam =
            GameEventManager::getSingleton().addAreaListener(NULL, 
                aabb, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId( - (zone->getId())); // a negative id indicates to subtract this area from the zone
        zone->addEventSource(gam);
    }

    void ZoneManager::addMeshAreaToZone(const Ogre::String& name,
        const Ogre::String& meshname, GeometryType geom,
        Vector3 position,
        Vector3 scale, Vector3 offset, Quaternion orientation,
        Real transitionDistance,
        unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if( !zone )
            return;

        Entity* entity = CoreSubsystem::getSingletonPtr()->getWorld()
            ->getSceneManager()->createEntity("Zone_"+name+"_MeshArea_"+meshname, meshname);
        if( !entity )
            return;

        GameAreaEventSource* gam =
            GameEventManager::getSingleton().addMeshAreaListener(NULL, 
                entity, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId(zone->getId());

        CoreSubsystem::getSingletonPtr()->getWorld()
            ->getSceneManager()->destroyEntity(entity);
        zone->addEventSource(gam);
    }

    void ZoneManager::subtractMeshAreaFromZone(const Ogre::String& name,
        const Ogre::String& meshname, GeometryType geom,
        Vector3 position,
        Vector3 scale, Vector3 offset, Quaternion orientation,
        Real transitionDistance,
        unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if( !zone )
            return;

        Entity* entity = CoreSubsystem::getSingletonPtr()->getWorld()
            ->getSceneManager()->createEntity("Zone_"+name+"_MeshArea_"+meshname, meshname);
        if( !entity )
            return;


        GameAreaEventSource* gam =
            GameEventManager::getSingleton().addMeshAreaListener(NULL, 
                entity, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId( - (zone->getId()) ); // a negative id indicates to subtract this area from the zone

        CoreSubsystem::getSingletonPtr()->getWorld()
            ->getSceneManager()->destroyEntity(entity);
        zone->addEventSource(gam);
    }

	void ZoneManager::areaLeft(GameAreaEvent* gae)
	{
        long id = gae->getSource()->getId();
        if( id != 0 )
        {
            if( id > 0 )
            {
                Zone *zone = getZone(id);
                if( zone )
                    zoneLeft(zone);
            }
            else
            {
                Zone *zone = getZone(-id); // means we have to subtract this area from the zone
                if( zone )
                    zoneEntered(zone);
            }

		    update();
        }
	}

	void ZoneManager::areaEntered(GameAreaEvent* gae)
	{
        long id = gae->getSource()->getId();
        if( id != 0 )
        {
            if( id > 0 )
            {
                Zone *zone = getZone(id);
                if( zone )
                    zoneEntered(zone);
            }
            else
            {
                Zone *zone = getZone(-id); // means we have to subtract this area from the zone
                if( zone )
                    zoneLeft(zone);
            }

		    update();
        }
	}

    void ZoneManager::zoneEntered(Zone *zone)
    {
        // perhaps the trigger destroys the zone, so we should put the zone in the list, before the triggers does so
        mActiveZones.push_front(zone);

        std::list<Trigger*> triggerList = zone->getTriggers();
        std::list<Trigger*>::iterator iter = triggerList.begin();

        for( ; iter != triggerList.end(); iter++)
        {
            if( (*iter)->activate() )
            {
                zone->removeTrigger(*iter);
                delete (*iter);                
            }
        }
    }

    void ZoneManager::zoneLeft(Zone *zone)
    {
        // perhaps the trigger destroys the zone, so we should put the zone in the list, before the triggers does so
        mActiveZones.remove(zone);

        std::list<Trigger*> triggerList = zone->getTriggers();
        std::list<Trigger*>::iterator iter = triggerList.begin();

        for( ; iter != triggerList.end(); iter++)
        {
            if( (*iter)->deactivate() )
            {
                zone->removeTrigger(*iter);
                delete (*iter);                
            }
        }
    }

    void ZoneManager::update()
    {
        switchLights();
        switchEaxSettings();
        switchSounds();
    }

	void ZoneManager::switchLights()
	{
		Zone* currentZone = mActiveZones.front();
		std::set<Actor*> activeLights;

		std::list<Actor*> curLights = currentZone->getLights();
		for (std::list<Actor*>::const_iterator it = curLights.begin(); it != curLights.end(); it++)
		{
			activeLights.insert(*it);
			(*it)->setVisible(true);
		}

		for (ZoneMap::const_iterator itZones = mZones.begin(); itZones != mZones.end(); itZones++)
		{
			std::list<Actor*> curLights = (*itZones).second->getLights();
			for (std::list<Actor*>::const_iterator itLights = curLights.begin(); itLights != curLights.end(); itLights++)
			{
				if (activeLights.find(*itLights) == activeLights.end())
				{
					(*itLights)->setVisible(false);
				}
			}
		}

		std::list<Actor*> defLights = mDefaultZone->getLights();
		for (std::list<Actor*>::const_iterator itLights = defLights.begin(); itLights != defLights.end(); itLights++)
		{
			if (activeLights.find(*itLights) == activeLights.end())
			{
				(*itLights)->setVisible(false);
			}
		}
	}

	void ZoneManager::switchSounds()
	{
		Zone* currentZone = mActiveZones.front();
		SoundMap newActiveSounds;

        // create all new sounds, or take existing ones
		std::list<Ogre::String> curSounds = currentZone->getSounds();
		for (std::list<Ogre::String>::const_iterator it = curSounds.begin(); it != curSounds.end(); it++)
		{
            SoundMap::iterator it_ = mActiveSounds.find(*it);
            if( it_ != mActiveSounds.end() )
            {
                // this sound already exists, only copy and don't destroy later
                newActiveSounds.insert(*it_);
                mActiveSounds.erase(it_); // remember, it_ is now invalid
            }
            else
            {
                // we need to create a new sound
                Sound *sound = SoundManager::getSingleton().createSound(*it, ST_STREAM);
                sound->setLooping(true);
                sound->play(false);
                newActiveSounds.insert(make_pair(*it, sound));
            }
		}

        // destroy old sounds
        for( SoundMap::iterator it = mActiveSounds.begin(); it != mActiveSounds.end(); it++ )
        {
            SoundManager::getSingleton().destroySound(it->second);
        }

        // copy new active sounds
        mActiveSounds = newActiveSounds;
	}

    void ZoneManager::switchEaxSettings()
    {
        if( mActiveZones.front()->getEaxPreset() != "" )
        {
            SoundManager::getSingleton().getActiveDriver()->setEaxPreset(
                mActiveZones.front()->getEaxPreset() );
        }
        else
        {
            SoundManager::getSingleton().getActiveDriver()->setEaxPreset("Off");
        }
    }
}
