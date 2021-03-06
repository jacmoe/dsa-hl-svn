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

#ifndef SYSTEM_PROCESSOR_H
#define SYSTEM_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"
#include "../AimlParser.h"
#include "../DialogSubsystem.h"

namespace rl
{
	/**
	 * Provides control over some features of the system the dialog module runs on
	 *
	 * @author	Philipp Walser
	 */
	class SystemProcessor : public AimlProcessor
	{
	public:
		SystemProcessor(){};
		~SystemProcessor() { }
	
		CeGuiString process(DOMNode* node,Match* m, const CeGuiString& str, NaturalLanguageProcessor* nlp)
		{
			CeGuiString cmd = XmlHelper::getAttributeValueAsString( (DOMElement*)node, "command" );
		//	Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "SYSTEM");
		//	Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, cmd);
			if( cmd == "exit" )
			{
				nlp->mExit=true;
			}
			if( cmd == "reload" )
			{
			//	Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "RELOOOAAAD");
			//	nlp->processOption("load","*.aiml");
			}
			return "";
		}
	};
}
#endif
