/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#ifdef WITH_OAL
#include "OalDriver.h"
extern "C" {
    #include "AL/al.h"
    #include "AL/altypes.h"
    #include "AL/alc.h"
    #include "AL/alctypes.h"
}

namespace rl
{

/** 
 * Konstruktor
 * @author JoSch
 * @date 12-23-2005
 */
OalDriver::OalDriver()
{
}

/** 
 * Destruktor
 * @author JoSch
 * @date 12-23-2005
 */
OalDriver::~OalDriver()
{
}

/** 
 * Zeigt an, ob der Treiber funktionsfähig ist.
 * @return Immer TRUE.
 * @author JoSch
 * @date 12-23-2005
 */
bool OalDriver::isDriverAvailable()
{
    try {
        // Clear error code
        alGetError();
        ALCdevice *device = alcOpenDevice(0);
        
        if (device == 0)
        {
            return false;
        }
        
        //Create context(s)
        ALCcontext *context = alcCreateContext(device, 0);
        //Set active context
        alcMakeContextCurrent(context);
        // Get Error Code
        ALenum success = alGetError();
        //Disable context
        alcMakeContextCurrent(0);
        //Release context(s)
        alcDestroyContext(context);
        //Close device
        alcCloseDevice(device);
        
        return (success == AL_NO_ERROR);
    } catch (...)
    {
        return false;
    }
}

/** 
 * Starte den Nulltreiber und mache alle Initialisierung.
 * @author JoSch
 * @date 12-23-2005
 */
void OalDriver::init()
{
}

/** 
 * Beeende den Nulltreiber und gebe alle Resourcen frei.
 * @author JoSch
 * @date 12-23-2005
 */
void OalDriver::deInit()
{
}

/** 
 * Gibt den Anzeigename des Treiber zurück.
 * @return Anzeigename
 * @author JoSch
 * @date 12-23-2005
 */
CeGuiString OalDriver::getName() const
{
    return "OpenAL";
}


}
#endif // WITH_OAL
