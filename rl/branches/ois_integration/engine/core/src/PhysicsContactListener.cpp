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

#include "PhysicsContactListener.h"
#include "PhysicalThing.h"

namespace rl {

PhysicsContactListener::PhysicsContactListener()
{
}

PhysicsContactListener::~PhysicsContactListener()
{
}

void PhysicsContactListener::contactOccured(PhysicalThing* thing1, PhysicalThing* thing2) 
{
	contactOccured(thing1->getActor(), thing2->getActor());
}

}