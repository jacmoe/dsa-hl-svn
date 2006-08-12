/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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

%{
//----------- Includes - Common -----------
#include "FixRubyHeaders.h"
#include "CommonPrerequisites.h"
#include "GameTask.h"
#include <set>
#include "EventListener.h"
#include "EventObject.h"
#include "EventCaster.h"
#include "PlaylistObject.h"
#include "Playlist.h"
#include "PlaylistEvent.h"
#include "SimplePlaylist.h"
#include "LinkedPlaylist.h"
#include "PauseObject.h"
%}