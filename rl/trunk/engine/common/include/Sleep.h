/* Sleep.h - Plattformunabhaengigkeit f�r sleep
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here:
 *  http://www.perldoc.com/perl5.6/Artistic.html
 */


#include "RastullahPrerequisites.h"

namespace rl {
/**
 * @param sleeptime Die Zeit in Millisekunden.
 * Legen den aktuellen Thread f�r sleeptime Millisekungen schlafen.
 */
inline void msleep(unsigned long sleeptime)
{
#if OGRE_PLATFORM == PLATFORM_WIN32
    Sleep(sleeptime);
#else
    usleep(sleeptime * 1000 * 1000);
#endif
}

}
