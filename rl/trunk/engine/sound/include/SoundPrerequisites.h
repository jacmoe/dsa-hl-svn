/* SoundPrerequisites.h - Verschiedene Einstellung f�r RlSound.
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
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __SoundPrerequisites_H__
#define __SoundPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLSOUND_EXPORTS )
#       define _RlSoundExport __declspec( dllexport )
#   else
#       define _RlSoundExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlSoundExport
#endif

#include "Exception.h"
extern "C" {
#include "AL/al.h"
#include "AL/alut.h"
#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
}


#endif
