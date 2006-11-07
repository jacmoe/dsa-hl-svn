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

#ifndef __EFFECTMANAGER_H__
#define __EFFECTMANAGER_H__

#include "RulesPrerequisites.h"
#include "Date.h"
#include "Effect.h"
#include <set>


namespace rl
{
    /**
     * Verwaltet die Effekte die auf einer Kreatur wirken.
     * Diese Klasse verwaltet alle Effekte, die auf einer Kreatur liegen.
     * Jede Kreatur hat ihren eigenen EffectManager. Die Funktion checkeffects
     * sollte w�hrend jeder Aktion (gemeint ist die DSA-Zeiteinheit) aufgerufen
     * werden.
     **/

	class _RlRulesExport EffectManager
	{
	public:
		EffectManager();
		~EffectManager();

        /**
         *  Ueberprueft die Effekte auf Veraenderungen.
         **/
		void checkEffects();
        /** 
         *  Mit dieser Funktion kann ein Effekt dem Manager mitteilen,
         *  in welcher Zeit er wieder ueberprueft werden muss.
         *  @param time Die Zeitspanne wann die naechste Ueberpruefung faellig ist.
         *  @param effect Der zu ueberpruefende Effekt.
         *  @throws IllegalArgumentException time parameter is <= 0
         *  @throws IllegalArgumentException effect pointer is NULL
         *  @see addCheckDate Date
         **/
        void addCheck(RL_LONGLONG timeUntilCheck, Effect* effect);
        /**
         *  Mit dieser Funktion kann ein Effekt dem Manager mitteilen,
         *  zu welchem Zeitpunkt er wieder ueberprueft werden muss.
         *  @param date Das Datum der naechsten Ueberpruefung.
         *  @param effect Der zu ueberpruefende Effekt.
         *  @throws IllegalArgumentException date lies in the past
         *  @throws IllegalArgumentException effect pointer is NULL
         *  @see addCheck, Date
         **/
        void addCheckDate(RL_LONGLONG date, Effect* effect);
        /**
         * Fuegt einen neuen Effekt in die Liste ein.
         * @param effect Ein Zeiger auf den einzufuegenden Effekt.
         **/
		void addEffect(Effect* effect);

	private:
        /// Eine einfache Menge von Effekten
		typedef std::set<Effect*> Effects;
		Effects mEffects;
        /**
         * Eine Liste von Tags und Effekten die auf das hinzufuegen
         * eines Effekts mit diesem Tag reagieren.
         **/
        typedef std::map<Effect::EffectTag, Effects> Watchlist;
		Watchlist mWatchlist;
        /**
         * Eine Liste die wiedergibt, wann welcher Effekt
         * ueberprueft werden muss.
         **/
        typedef std::map<RL_LONGLONG, Effects> Checklist;
        Checklist mChecklist;
	};
}

#endif //__EFFECTMANAGER_H__
		