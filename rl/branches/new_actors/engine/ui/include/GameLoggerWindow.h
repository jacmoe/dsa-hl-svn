/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __GameLoggerWindow_H__
#define __GameLoggerWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {

	class _RlUiExport GameLoggerWindow : public CeGuiWindow
	{
	public:
		GameLoggerWindow();

		void logDialogEvent(const std::string text);
		void logFightEvent(const std::string text);
		void logLevelupEvent(const std::string text);
		void logMiscEvent(const std::string text);

		void logEvent(const std::string text, const CEGUI::colour color);
	};


}

#endif
