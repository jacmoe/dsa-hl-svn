#include "Action.h"
#include "GameObject.h"
#include "Exception.h"

using namespace std;

namespace rl
{
    GameObject::GameObject(int id,
                           const CeGuiString& name,
                           const CeGuiString& description) : mId(id),
                                                             mName(name),
                                                             mDescription(description)
    {
        // Standardactions registrieren
    }

    GameObject::~GameObject(void)
    {
    }

    int GameObject::getId() const
    {
        return mId;
    }

    const CeGuiString& GameObject::getName() const
    {
        return mName;
    }

	/*const char* GameObject::getName() const
	{
		return mName.c_str();
	}*/

    void GameObject::setName(CeGuiString& name)
    {
        mName = name;
    }

	/*void GameObject::setName(const char* name)
    {
		mName = CeGuiString(name);
    }*/

    const CeGuiString& GameObject::getDescription() const
    {
        return mDescription;
    }

    void GameObject::setDescription(CeGuiString& description)
    {
        mDescription = description;
    }

    void GameObject::addAction(Action* action, int option)
    {
        if (action == NULL)
        {
            Throw(NullPointerException, "Parameter action ist NULL.");
        }

		mActions.push_back(make_pair(action, option));		
    }

    void GameObject::removeAction(Action* action)
    {
		ActionOptionVector::iterator it = findAction(mActions.begin(), mActions.end(), action);
        if (it != mActions.end())
        {
            mActions.erase(it);
        }
    }

    const ActionVector GameObject::getValidActions() const
    {
		ActionVector actions;
		for (ActionOptionVector::const_iterator it = mActions.begin(); it != mActions.end(); it++)
		{
			if ((*it).second == ACT_DISABLED)
				continue;
			//if ((*it).second > ACT_NEEDS_TALENT)
				
			actions.push_back((*it).first);
		}
        return actions;
    }

    void GameObject::doAction(const CeGuiString& actionName,
                              Creature* actor,
                              GameObject* target)
    {
		ActionOptionVector::const_iterator it = findAction(mActions.begin(), mActions.end(), actionName);

		if (it == mActions.end())
        {
            Throw(InvalidArgumentException, "Dem Objekt unbekannte Aktion.");
        }
        
		doAction((*it).first, actor, target);
    }

	void GameObject::doAction(Action* action,
                              Creature* actor,
                              GameObject* target)
	{
		action->doAction(this, actor, target);
	}

	GameObject::ActionOptionVector::iterator 
		GameObject::findAction(
			GameObject::ActionOptionVector::iterator begin, 
			GameObject::ActionOptionVector::iterator end, 
			const CeGuiString& actionName)
	{
		for (ActionOptionVector::iterator iter = begin; iter != end; iter++)
			if ((*iter).first->getName().compare(actionName) == 0)
				return iter;

		return end;
	}

	GameObject::ActionOptionVector::iterator 
		GameObject::findAction(
			GameObject::ActionOptionVector::iterator begin, 
			GameObject::ActionOptionVector::iterator end, 
			const Action* action)
	{
		for (ActionOptionVector::iterator iter = begin; iter != end; iter++)
			if ((*iter).first == action)
				return iter;

		return end;
	}

}
