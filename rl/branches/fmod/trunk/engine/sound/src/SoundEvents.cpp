/* SoundEvent.cpp - Diese Klassse repräsentiert ein Event das in RlSound auftritt.
 * (C) 2003-2005. Team Pantheon. www.team-pantheon.de
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
 
#include "SoundEvents.h"

namespace rl {

SoundEvent::SoundEvent(EventSource *source) :
    EventObject(source)
{}

SoundEvent::~SoundEvent()
{}

SoundFadeEvent::SoundFadeEvent(EventSource *source):
    SoundEvent(source)
{
}

SoundFadeEvent::~SoundFadeEvent()
{
}

SoundPlayEvent::SoundPlayEvent(EventSource *source) :
    SoundEvent(source)
{
}

SoundPlayEvent::~SoundPlayEvent()
{
}

SoundTimingEvent::SoundTimingEvent(EventSource *source) :
    SoundEvent(source),
    mTime(0.0)
{
    setReason(TIMEEVENT);
}

SoundTimingEvent::~SoundTimingEvent()
{
}

}
