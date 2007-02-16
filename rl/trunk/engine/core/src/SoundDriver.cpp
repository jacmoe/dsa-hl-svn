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
#include "SoundDriver.h"
#include "ConfigFile.h"

#include <OgreStringConverter.h>

using namespace Ogre;

namespace rl
{

SoundDriver::SoundDriver(ResourceManager* soundResourceManager)
 :  mSoundResourceManager(soundResourceManager),
    mStreamSet(),
    mSampleSet(),
    mDefaultMusicVolume(40),
    mDefaultSoundVolume(100),
    mMasterVolume(100)
{
}

SoundDriver::~SoundDriver()
{
    SoundSet sounds;
    sounds.insert(mSampleSet.begin(), mSampleSet.end());
    sounds.insert(mSampleSet.begin(), mSampleSet.end());
    for (SoundSet::iterator it = sounds.begin(); it != sounds.end(); ++it)
    {
        if ((*it)->isPlaying())
        {
            (*it)->stop();
        }
        delete *it;
    }
}

/// Die Standardlautst�rke f�r Musik einstellen
void SoundDriver::setDefaultMusicVolume(const Ogre::Real& vol)
{
    mDefaultMusicVolume = vol;
    SoundSet::iterator it;
    for(it = mStreamSet.begin(); it != mStreamSet.end(); it++)
    {
        (*it)->setVolume(vol);
    }
}

/// Die Standardlautst�rke f�r Musik zur�ckgeben.
const Ogre::Real SoundDriver::getDefaultMusicVolume() const
{
    return mDefaultMusicVolume;
}

/// Die Standardlautst�rke f�r Musik einstellen
void SoundDriver::setDefaultSoundVolume(const Ogre::Real& vol)
{
    mDefaultSoundVolume = vol;
    SoundSet::iterator it;
    for(it = mSampleSet.begin(); it != mSampleSet.end(); it++)
    {
        (*it)->setVolume(vol);
    }
}

/// Die Standardlautst�rke f�r Musik zur�ckgeben.
const Ogre::Real SoundDriver::getDefaultSoundVolume() const
{
    return mDefaultSoundVolume;
}

void SoundDriver::setMasterVolume(const Ogre::Real& vol)
{
    mMasterVolume = vol;
}

const Ogre::Real SoundDriver::getMasterVolume() const
{
    return mMasterVolume;
}

    Ogre::NameValuePairList SoundDriver::getSettings() const
    {
        Ogre::NameValuePairList SoundSettings;

        // Append the settings to the list
        SoundSettings.insert(make_pair("MasterVolume", Ogre::StringConverter::toString(mMasterVolume)));
        SoundSettings.insert(make_pair("DefaultMusicVolume", Ogre::StringConverter::toString(mDefaultMusicVolume)));
        SoundSettings.insert(make_pair("DefaultSoundVolume", Ogre::StringConverter::toString(mDefaultSoundVolume)));

        return SoundSettings;
    }

    void SoundDriver::applySettings(const Ogre::NameValuePairList& settings)
    {
        Ogre::NameValuePairList::const_iterator it;

        // Set the Master volume
        if ((it = settings.find("MasterVolume")) == settings.end())
        {
            // Set a sane default value
            setMasterVolume(Real(1.0));
        }
        else
        {
            setMasterVolume(Ogre::StringConverter::parseReal(it->second));
        }

        // Set the Default Music Volume
        if ((it = settings.find("DefaultMusicVolume")) == settings.end())
        {
            // Set a sane default value
            setDefaultMusicVolume(Real(0.3));
        }
        else
        {
            setDefaultMusicVolume(Ogre::StringConverter::parseReal(it->second));
        }

        // Set the Default Sound Volume
        if ((it = settings.find("DefaultSoundVolume")) == settings.end())
        {
            // Set a sane default value
            setDefaultSoundVolume(Real(1.0));
        }
        else
        {
            setDefaultSoundVolume(Ogre::StringConverter::parseReal(it->second));
        }
    }

    Sound* SoundDriver::createStream(const Ogre::String& name)
    {
        SoundResourcePtr ptr = mSoundResourceManager->load(name,
            ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
        if (ptr.isNull())
        {
            Throw(IllegalArgumentException, "Sound " + name + "nicht gefunden");
        }
        return createStream(ptr);
    }

    Sound* SoundDriver::createStream(const SoundResourcePtr &res)
    {
        Sound* sound = createStreamImpl(res);
        if (sound != NULL)
        {
            mStreamSet.insert(sound);
        }
        return sound;
    }

    Sound* SoundDriver::createSample(const Ogre::String& name)
    {
        SoundResourcePtr ptr = mSoundResourceManager->load(name,
            ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
        if (ptr.isNull())
        {
            Throw(IllegalArgumentException, "Sound " + name + "nicht gefunden");
        }
        return createSample(ptr);
    }

    Sound* SoundDriver::createSample(const SoundResourcePtr &res)
    {
        Sound* sound = createSampleImpl(res);
        if (sound != NULL)
        {
            mSampleSet.insert(sound);
        }
        return sound;
    }

    void SoundDriver::destroySound(Sound* sound)
    {
        if (sound->isPlaying())
        {
            sound->stop();
        }
        mStreamSet.erase(sound);
        mSampleSet.erase(sound);
        delete sound;
    }
}
