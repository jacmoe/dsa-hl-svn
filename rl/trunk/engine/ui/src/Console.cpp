/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "UiPrerequisites.h"
#include <renderers/OgreGUIRenderer/ogrerenderer.h>
#include <boost/bind.hpp>

#include "Interpreter.h"
#include "ListboxWrappedTextItem.h"
#include "CoreSubsystem.h"
#include "Console.h"

using namespace Ogre;

template<> rl::Console* Singleton<rl::Console>::ms_Singleton = 0;

using CEGUI::WindowManager; using CEGUI::utf8; using CEGUI::ListboxTextItem;
using CEGUI::KeyEventArgs; using CEGUI::Key; using CEGUI::colour;
using CEGUI::ListboxWrappedTextItem; using CEGUI::TextFormatting;

namespace rl 
{
	Console& Console::getSingleton()
    {
        return Singleton<Console>::getSingleton();
    }
	Console* Console::getSingletonPtr()
    {
        return Singleton<Console>::getSingletonPtr();
    }

	Console::Console() : 
		CeGuiWindow("console.xml", WND_KEYBOARD_INPUT)
	{
		using namespace CEGUI;
		
		mDisplay = getListbox("Console/Display");
		mCommandLine = getEditbox("Console/Inputbox");

		mWindow->subscribeEvent(
			FrameWindow::EventKeyDown, 
			boost::bind(&Console::handleKeyDown, this, _1));
		mCommandLine->subscribeEvent(
			Editbox::EventKeyDown, 
			boost::bind(&Console::handleKeyDown, this, _1));
		mDisplay->moveToFront();

		mHistory.clear();
        setVisible(false);
		addToRoot(mWindow);	
	}

	bool Console::beforeShow()
	{
		mCommandLine->activate();
		return true;
	}

	bool Console::handleKeyDown(const CEGUI::EventArgs& e)
	{
		KeyEventArgs ke = static_cast<const KeyEventArgs&>(e);
        if (ke.scancode == Key::ArrowDown)
		{
			cycleHistory(1);
			return true;
		}
		else if (ke.scancode == Key::ArrowUp)
		{
			cycleHistory(-1);
			return true;
		}
		else if (ke.scancode == Key::Return)
		{
			CeGuiString command = mCommandLine->getText();
			CeGuiString printCommand = ">" + command;
			appendTextRow(printCommand, 0xFF7FFF7F);
				
			mPrompt = CoreSubsystem::getSingleton().getInterpreter()->execute(command.c_str());

			mHistory.push_back(command.c_str());
			mHistoryMarker = mHistory.size();
			mCommandLine->setText((utf8*)"");
			return true;
		}
		
		return false;		
	}

	void Console::write(String output)
	{
        CeGuiString temp = CeGuiString(output);		
		appendTextRow(temp, 0xFF7F7F7F);
	}

	void Console::appendTextRow(CeGuiString& text, const colour color)
	{
		/*const float MIN_SPACE_POS = 0.5;

		CeGuiString textLeft = CeGuiString(text);
		const CEGUI::Font* font = mDisplay->getFont();
		unsigned int width = mDisplay->getAbsoluteWidth()*0.95;

		while (textLeft.length() > 0)
		{
			CeGuiString textLine;

			if (font->getTextExtent(textLeft) > width)
			{
				unsigned int numLastChar = font->getCharAtPixel(textLeft, width);
				unsigned int numSpace = textLeft.find_last_of(" \t\n", numLastChar);

				if (numSpace == CeGuiString::npos || numSpace < MIN_SPACE_POS*numLastChar)
				{
					textLine = textLeft.substr(0, numLastChar);
					textLeft = textLeft.substr(numLastChar);
				}
				else
				{
					textLine = textLeft.substr(0, numSpace);
					textLeft = textLeft.substr(numSpace+1);
				}
			}
			else
			{
				textLine = textLeft;
				textLeft = "";
			}

			ListboxTextItem* item = new ListboxTextItem(textLine);
			item->setTextColours(color);
			mDisplay->addItem(item);
			mDisplay->ensureItemIsVisible(item); // scroll to bottom;
		}*/
		ListboxWrappedTextItem* item = new ListboxWrappedTextItem(text);
		item->setTextColours(color);
		item->setTextFormatting(CEGUI::WordWrapLeftAligned);
		mDisplay->addItem(item);
		mDisplay->ensureItemIsVisible(item); // scroll to bottom;*/
	}
	
	void Console::setInterpreter(Interpreter* interpreter)
	{
		mInterpreter = interpreter;
	}

	Interpreter* Console::getInterpreter()
	{
		return mInterpreter;
	}

	void Console::cycleHistory(int skip)
	{
		if (mHistory.size() == 0)
			return;
		
		if (mHistoryMarker + skip < 0)
			mHistoryMarker = 0;
		else if (mHistoryMarker + skip > mHistory.size())
			mHistoryMarker = mHistory.size();
		else
			mHistoryMarker = (unsigned int)(mHistoryMarker + skip);

		if (mHistoryMarker == mHistory.size())
			mCommandLine->setText((utf8*)"");
		else
			mCommandLine->setText(mHistory[mHistoryMarker]);
	}
}

