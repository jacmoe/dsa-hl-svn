/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#include "TalentWertPredicates.h"
#include "Creature.h"

using namespace rl;

TalentWertPredicates::TalentWertPredicates(Creature* pCharacter)
	: CreaturePredicates(pCharacter)
{
}

TalentWertPredicates::~TalentWertPredicates(void)
{
}

CeGuiString TalentWertPredicates::getPredicate(const CeGuiString& pName) const
{
	try
	{
		return Ogre::StringConverter::toString(mCharacter->getTalent(pName));
	}
	catch(...)
	{
	//  TODO: excpetionhandling
	}
	return "";
}

void TalentWertPredicates::setPredicate(const CeGuiString& pName, const CeGuiString& pValue)
{
}

CeGuiString TalentWertPredicates::getType() const
{
	return "talentwert";
}