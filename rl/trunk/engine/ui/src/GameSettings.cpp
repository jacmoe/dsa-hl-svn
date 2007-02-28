/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include <boost/bind.hpp>

#include "GameSettings.h"

namespace rl
{
	GameSettings::GameSettings()
	    : CeGuiWindow("gamesettings.xml", WND_MOUSE_INPUT)
	{
		// Connect actions to callbacks
		getWindow()->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,
		                            boost::bind(&GameSettings::onCancel, this));

		centerWindow();
	}

	GameSettings::~GameSettings()
	{
	}

	void GameSettings::registerSoundDriverConfig(SoundDriverConfigComponent* scc)
	{
	}

	void GameSettings::registerRenderSystemConfig(RenderSystemConfigComponent* rcc)
	{
	}

	void GameSettings::setVisible(bool visible, bool destroyAfterHide)
	{
		CeGuiWindow::setVisible(visible, destroyAfterHide);
	}

	void GameSettings::setConfigComponent(const CEGUI::String& drivername)
	{
		// Search the sound configuration component list
		for (std::list<SoundDriverConfigComponent*>::iterator it = mSoundDriverConfigs.begin();
		     it != mSoundDriverConfigs.end(); it++)
		{
			SoundDriverConfigComponent* cmp = *it;
			if (drivername == cmp->getDriverName() && mCurrentSoundDriverConfig != cmp)
			{
				if (mCurrentSoundDriverConfig != NULL)
				{
					mCurrentSoundDriverConfig->setVisible(false);
				}
				cmp->setVisible(true);
				mCurrentSoundDriverConfig = cmp;
				return;
			}
		}

		// Search the graphic configuration component list
		for (std::list<RenderSystemConfigComponent*>::iterator it = mRenderSystemConfigs.begin();
		     it != mRenderSystemConfigs.end(); it++)
		{
			RenderSystemConfigComponent* cmp = *it;
			if (drivername == cmp->getDriverName() && mCurrentRenderSystemConfig != cmp)
			{
				if (mCurrentRenderSystemConfig != NULL)
				{
					mCurrentRenderSystemConfig->setVisible(false);
				}
				cmp->setVisible(true);
				mCurrentRenderSystemConfig = cmp;
				return;
			}
		}
	}

	bool GameSettings::onApply()
	{
	}

	bool GameSettings::onCancel()
	{
	}

	bool GameSettings::onSoundDriverChanged()
	{
	}
}