/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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


#ifndef __MovingCreature_H__
#define __MovingCreature_H__

#include "RulesPrerequisites.h"
#include "PhysicsController.h"
#include "PhysicsGenericContactCallback.h"
#include "Creature.h"
#include "Actor.h"
#include <map>



namespace rl
{
    class AbstractMovement;
    class MovingCreatureManager;

    /** 
     * This class provides an interface to control the movement of a creature.
     * It handles all nessessary things like animations and calculates the movement speed.
     */
    class _RlRulesExport MovingCreature :
        public PhysicsController,
        public PhysicsGenericContactCallback
    {
    public:
        /** Constructor.
		 * @param creature the creature of the bot/char
		 * @param actor the actor of the bot/char
		 */
        MovingCreature(Creature *character);
        ~MovingCreature();

        /**
         * This function is not intented to be called directly
         * @retval true, if this MovingCreature still needs to be called every frame
         */
        bool run(Ogre::Real elapsedTime);

        /// Newton force and torque callback
        void OnApplyForceAndTorque(PhysicalThing* thing);

        /// Newton contact callback called by the movingCreatureManager
        int userProcess();

        Creature *getCreature() {return mCreature;}

        /** 
         * the different movements a creature can perform, 
         * there must be an adequate movement-class
         * (derived from AbstractMovement)
         */
        typedef enum
        {
            // these movements can be used
            MT_NONE,
            MT_STEHEN,
            MT_GEHEN,
            MT_JOGGEN,
            MT_LAUFEN,
            MT_RENNEN,
            MT_RUECKWAERTS_GEHEN,
            MT_RUECKWAERTS_JOGGEN,
            MT_SEITWAERTS_GEHEN,
            MT_SCHLEICHEN,
            MT_HOCHSPRUNG,
            MT_WEITSPRUNG,
            // these movements are only for internal purposes and should not be used directly
            // instead use one of the movements above
            MT_DREHEN
        } MovementType;

        /// The generalization of the place (in the air, on the floor, in the water...)
        typedef enum { 
            AL_AIRBORNE, // the creature is floating/falling in the air
            AL_FLOOR // the creature is standing/lying/moving/... on the floor
        } AbstractLocation;

        /**
         * sets wether the creature is airborne or not
         * this is done by this class itself every frame, so using this method does probably not
         * have the desired effect
         */
        void setAbstractLocation(AbstractLocation type) {mAbstractLocation = type;}

        AbstractLocation getAbstractLocation() const {return mAbstractLocation;}


        /**
         * the most important function: sets a new movement or changes the direction or rotation of a movement
         * if this movement isn't possible, the fallback-movement is called.
         * The movement doesn't change until another movement is set, so it is not necessary to call this function 
         * every frame. Only the rotation is resettet to zero every new frame!
         * @param type the id of the movement
         * @param direction if the direction isn't possible (see AbstractMovement::isDirectionPossible), only a "part" of the direction is applied
         * @param rotation like above
         * @retval false signifies that the change to this movement was not possible (possibly because the present movement forbade it)
         */
        bool setMovement(MovementType type, Ogre::Vector3 direction, Ogre::Vector3 rotation);


        MovementType getMovementId() const;
        Ogre::Vector3 getDirection() const {return mDirection;}

        /// This function does probably not return the expected value, because the rotation is resetted every frame!
        Ogre::Vector3 getRotation() const {return mRotation;}

        /// This function can return NULL
        AbstractMovement *getMovement() {return mMovement;}

        /// This function can return NULL
        AbstractMovement *getMovementFromId(MovementType id);

        // some methods used by movements
        int getCurrentGS() const;
        Ogre::Vector3 getVelocity() const; // in local axes
        Ogre::Vector3 getOmega() const;
        void setAnimation(const Ogre::String &name, 
                          Ogre::Real speed = 1, 
                          unsigned int timesToPlay = 0, 
                          const Ogre::String &collisionName = "" // the name of the animation the collision is based on
                          );


    protected:
        Creature *mCreature;
        AbstractLocation mAbstractLocation;
        
        AbstractMovement *mMovement;
        Ogre::Vector3 mDirection;
        Ogre::Vector3 mRotation;

        typedef std::map<MovementType, AbstractMovement*> MovementMap;
        MovementMap mMovementMap;

        // in order to copy the contactcallback members correctly;
        friend class MovingCreatureManager;

    private:
        // only used in setAnimation
        Ogre::String mLastAnimationName;
        Ogre::String mLastCollisionName;
        Ogre::Real mLastAnimationSpeed;

        // only used in userProcess
        Ogre::Real mLastFloorContact;

    };

    /// this is the base class of all movements
    class AbstractMovement : 
        public OgreNewt::ContactCallback
    {
    public:
        AbstractMovement(MovingCreature *movingCreature) : mMovingCreature(movingCreature) {}

        /// returns the id of this movement
        virtual MovingCreature::MovementType getId() const = 0;

        /// returns the id of the movement that is used, if this movement isn't possible (any more)
        virtual MovingCreature::MovementType getFallBackMovement() const = 0;

        /**
         * this method calculates the basis velocity (without any changes due to Talentproben etc)
         * @param velocity this parameter returns the calculated "velocity" ( m/s, rpm, or width/height of a jump)
         * @retval false indicates, that this movement isn't possible, so the calculated values can be incorrect
         */
        virtual bool calculateBaseVelocity(Ogre::Real &velocity) = 0;

        /**
         * this method specifies, if the movement is possible
         */
        virtual bool isPossible() const = 0;

        /**
         * this method is called, when this movement is currently used to enable 
         * an individual procession of collisions contacts for each movement
         */
        virtual int userProcess(OgreNewt::Body *body0, OgreNewt::Body *body1) {return 1;}

        /**
         * this method is called by OnApplyTorqueAndForceCallback of the MovingCreature
         * the PhysicalThing can be acquired via the Creature
         */
        virtual void calculateForceAndTorque(Ogre::Vector3 &force, Ogre::Vector3 &torque, Ogre::Real timestep) = 0;

        /**
         * this method indicates if it is possible to change to the specified movement at this moment
         */
        virtual bool canChangeToMovement(MovingCreature::MovementType id) {return true;}

        /**
         * this signifies that someone tried to change to another movement, but this was not possible (due to canChangeToMovement returning false)
         */
        virtual void requestChangeToMovement(MovingCreature::MovementType id) {}

        /**
         * here all the stuff not (directly) relating the physics should be done
         * this function is called every frame if this movement is activated and the
         * Creature is in an active moving state. It is important to handle all 
         * "Talentproben" etc here and not in the physics function!
         * The parameter direction an rotation don't need to obey the rules from isDirectionPossible and isRotationPossible
         * @retval true indicates that the creature should remain active (in order to activate it use MovingCreatureManager::setActive)
         */
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation) = 0;

        /**
         * this function is called if this movement is activated.
         * it can be used to determine if a new "Talentprobe" is needed
         */
        virtual void activate() {}

        /**
         * this function is called every time this movement is deactivated.
         */
        virtual void deactivate() {}

        /**
         * this method is used to get to know if a movement can be used with a certain direction
         * @retval true if this movement can be used with the direction, false otherwise
         * @param direction (in local axes); if this function returns false, this variable contains another valid direction (constructed from the given direction)
         */
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const = 0;

        /**
         * this method is used to get to know if the given rotation can be used with this movement
         * @retval true if the creature can perform this rotation with this movement
         * @param rotation (in local axes and rpm); if the function returns false, this variable contains another valid rotation (constructed from the given rotation)
         */
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const = 0;

    protected:
        MovingCreature *mMovingCreature;
    };
}
#endif