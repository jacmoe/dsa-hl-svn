/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __TimeSource_H__
#define __TimeSource_H__

#include "CorePrerequisites.h"


#include "GameTask.h"

#include "SaveGameData.h"

namespace rl
{
    class _RlCoreExport TimeSource : public GameTask
    {
    public:
        enum TimeSourceType
        {
            UNKNOWN,
            REALTIME_CONTINUOUS,
            REALTIME_INTERRUPTABLE,
            GAMETIME
        };

        TimeSource(bool interruptable);
        virtual ~TimeSource();

        virtual TimeSourceType getType() const = 0;

        virtual Time getClock() const = 0;
        virtual void setClock(const Time& time) = 0;
        virtual void setTimeFactor(Ogre::Real factor = 1.0) = 0;
    };

    class _RlCoreExport RealTimeContinuous : public TimeSource
    {
    public:
        RealTimeContinuous();

        virtual TimeSourceType getType() const;
        virtual const Ogre::String& getName() const;

        virtual Time getClock() const;
        virtual void setClock(const Time& time);
        virtual void setTimeFactor(Ogre::Real factor = 1.0);

        virtual void run(Ogre::Real elapsedTime);
    private:
        Ogre::Real mTime;
    };

    class _RlCoreExport RealTimeInterruptable : public TimeSource
    {
    public:
        RealTimeInterruptable();
        virtual ~RealTimeInterruptable();

        virtual TimeSourceType getType() const;
        virtual const Ogre::String& getName() const;

        virtual Time getClock() const;
        virtual void setClock(const Time& time);
        virtual void setTimeFactor(Ogre::Real factor = 1.0);

        virtual void run(Ogre::Real elapsedTime);
    private:
        Ogre::Real mTime;
        Ogre::Real mTimeFactor;
    };

    class _RlCoreExport TimeSourceManager 
        : public Ogre::Singleton<TimeSourceManager>, public SaveGameData
    {
    public:
        TimeSourceManager();
		virtual ~TimeSourceManager();

        void registerTimeSource(TimeSource* ts);
        TimeSource* getTimeSource(const TimeSource::TimeSourceType& type) const;
        void setTimeFactor(const Ogre::Real& factor);
        void setPaused(bool paused);

		virtual CeGuiString getXmlNodeIdentifier() const;
        virtual void writeData(SaveGameFileWriter* writer);
        virtual void readData(SaveGameFileReader* reader);
        /// defines the loading/saving order higher priority are saved last and loaded first
        virtual int getPriority() const;
    private:
        typedef std::map<TimeSource::TimeSourceType, TimeSource*> TimeSourceMap;
        TimeSourceMap mTimeSources;
    };
}

#endif // __TimeSource_H__
