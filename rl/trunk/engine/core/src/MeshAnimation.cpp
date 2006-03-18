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

#include "Exception.h"
#include "AnimationManager.h"
#include "ActorManager.h"
#include "ScriptWrapper.h"

#include "MeshAnimation.h"


namespace rl {

MeshAnimation::MeshAnimation( Ogre::AnimationState* animState, MeshObject* mesh,
                     Ogre::Real speed, unsigned int timesToPlay, bool paused ) :
    mMeshObject( mesh ),
    BaseAnimation( animState->getLength(), speed, timesToPlay, paused ) 
{
	setAnimationState(animState);
}

MeshAnimation::~MeshAnimation()
{
	if( mAnimState != NULL )
		mAnimState->setEnabled(false);
}

void MeshAnimation::setAnimationState( Ogre::AnimationState* animState )
{
	if( animState == 0 )
		Throw( NullPointerException,"Ogre::AnimationState darf nicht null sein" );

	mAnimState = animState;
	
	if( mTimesToPlay != 1 )
		mAnimState->setLoop( true );
	
	// Wenn die Zeit negativ ist, beginnen wir am Ende
	if( mSpeed < 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );

	mAnimState->setEnabled( true );
}

void MeshAnimation::setSpeed( Ogre::Real speed ) 
{
	// Eventuell auf Anfang/Ende setzen
	if( speed < 0 && mAnimState->getTimePosition() == 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );
	else if( speed > 0 && mAnimState->getTimePosition() == mAnimState->getLength() )
		mAnimState->setTimePosition( 0 );

    BaseAnimation::setSpeed(speed);
}

MeshObject* MeshAnimation::getMeshObject()
{
    return mMeshObject;
}

void MeshAnimation::resetTimesPlayed()
{
	// Zurückspulen
	if( mSpeed < 0 )
		mAnimState->setTimePosition( mLength );
	else if( mSpeed > 0 )
		mAnimState->setTimePosition( 0 );

    BaseAnimation::resetTimesPlayed();
}

// Gewicht (Einfluss) der Animation
Ogre::Real MeshAnimation::getWeight(void) const
{
	return mAnimState->getWeight();
}

void MeshAnimation::setWeight(Ogre::Real weight)
{
	mAnimState->setWeight( weight );
}

void MeshAnimation::doAddTime(Ogre::Real timePassed)
{
	mAnimState->addTime( timePassed );
}

void MeshAnimation::setLoop( bool loop )
{
    mAnimState->setLoop(loop);
}

Ogre::AnimationState* MeshAnimation::getAnimationState() const
{
    return mAnimState;
}

}
