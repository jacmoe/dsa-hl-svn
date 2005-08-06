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

#include "ScriptObjectRepository.h"

template<> rl::ScriptObjectRepository* Ogre::Singleton<rl::ScriptObjectRepository>::ms_Singleton = 0;

namespace rl {

    ScriptObjectRepository& ScriptObjectRepository::getSingleton()
    {
        return Ogre::Singleton<ScriptObjectRepository>::getSingleton();
    }

    ScriptObjectRepository* ScriptObjectRepository::getSingletonPtr()
    {
        return Ogre::Singleton<ScriptObjectRepository>::getSingletonPtr();
    }

    ScriptObjectRepository::ScriptObjectRepository()
    {

    }

    ScriptObjectRepository::~ScriptObjectRepository()
    {

    }

}