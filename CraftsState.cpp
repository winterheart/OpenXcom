/*
 * Copyright 2010 Daniel Albano
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "CraftsState.h"
#include <sstream>
#include "Game.h"
#include "ResourcePack.h"
#include "Language.h"
#include "LangString.h"
#include "Font.h"
#include "Palette.h"
#include "TextButton.h"
#include "Window.h"
#include "Text.h"
#include "TextList.h"
#include "Craft.h"
#include "RuleCraft.h"
#include "Base.h"
#include "CraftInfoState.h"

using namespace std;

/**
 * Initializes all the elements in the Equip Craft screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
CraftsState::CraftsState(Game *game, Base *base) : State(game), _base(base)
{
	// Create objects
	_window = new Window(this, 320, 200, 0, 0);
	_btnOk = new TextButton(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 288, 16, 16, 176);
	_txtTitle = new Text(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 298, 16, 16, 8);
	_txtBase = new Text(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 298, 16, 16, 24);
	_txtName = new Text(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 94, 9, 16, 40);
	_txtStatus = new Text(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 50, 9, 110, 40);
	_txtWeapon = new Text(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 50, 16, 160, 40);
	_txtCrew = new Text(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 58, 9, 210, 40);
	_txtHwp = new Text(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 46, 9, 268, 40);
	_lstCrafts = new TextList(game->getResourcePack()->getFont("BIGLETS.DAT"), game->getResourcePack()->getFont("SMALLSET.DAT"), 298, 118, 16, 58);
	
	// Set palette
	_game->setPalette(_game->getResourcePack()->getPalette("BACKPALS.DAT")->getColors(Palette::blockOffset(3)), Palette::backPos, 16);

	add(_window);
	add(_btnOk);
	add(_txtTitle);
	add(_txtBase);
	add(_txtName);
	add(_txtStatus);
	add(_txtWeapon);
	add(_txtCrew);
	add(_txtHwp);
	add(_lstCrafts);

	// Set up objects
	_window->setColor(Palette::blockOffset(15)+4);
	_window->setBackground(game->getResourcePack()->getSurface("BACK14.SCR"));

	_btnOk->setColor(Palette::blockOffset(13)+13);
	_btnOk->setText(_game->getResourcePack()->getLanguage()->getString(STR_OK));
	_btnOk->onMouseClick((EventHandler)&CraftsState::btnOkClick);

	_txtTitle->setColor(Palette::blockOffset(15)+1);
	_txtTitle->setBig();
	_txtTitle->setText(_game->getResourcePack()->getLanguage()->getString(STR_INTERCEPTION_CRAFT));

	_txtBase->setColor(Palette::blockOffset(15)+1);
	_txtBase->setBig();
	string baseName = _game->getResourcePack()->getLanguage()->getString(STR_BASE_);
	baseName += _base->getName();
	_txtBase->setText(baseName);

	_txtName->setColor(Palette::blockOffset(15)+1);
	_txtName->setText(_game->getResourcePack()->getLanguage()->getString(STR_NAME));

	_txtStatus->setColor(Palette::blockOffset(15)+1);
	_txtStatus->setText(_game->getResourcePack()->getLanguage()->getString(STR_STATUS));

	_txtWeapon->setColor(Palette::blockOffset(15)+1);
	_txtWeapon->setText(_game->getResourcePack()->getLanguage()->getString(STR_WEAPON_SYSTEMS));

	_txtCrew->setColor(Palette::blockOffset(15)+1);
	_txtCrew->setText(_game->getResourcePack()->getLanguage()->getString(STR_CREW));

	_txtHwp->setColor(Palette::blockOffset(15)+1);
	_txtHwp->setText(_game->getResourcePack()->getLanguage()->getString(STR_HWPS));

	_lstCrafts->setColor(Palette::blockOffset(13)+10);
	_lstCrafts->setColumns(5, 94, 66, 47, 46, 45);
	_lstCrafts->setSelectable(true);
	_lstCrafts->setBackground(_window);
	_lstCrafts->onMouseClick((EventHandler)&CraftsState::lstCraftsClick);
}

/**
 *
 */
CraftsState::~CraftsState()
{
	
}

/**
 * The soldier names can change
 * after going into other screens.
 */
void CraftsState::init()
{
	_lstCrafts->clearList();
	for (vector<Craft*>::iterator i = _base->getCrafts()->begin(); i != _base->getCrafts()->end(); i++)
	{
		stringstream ss, ss2, ss3, ss4;
		ss << _game->getResourcePack()->getLanguage()->getString((*i)->getRules()->getType()) << "-" << (*i)->getId();
		ss2 << (*i)->getNumWeapons() << "/" << (*i)->getRules()->getWeapons();
		ss3 << (*i)->getNumSoldiers(_base->getSoldiers());
		ss4 << (*i)->getNumHWPs();
		_lstCrafts->addRow(5, ss.str().c_str(), _game->getResourcePack()->getLanguage()->getString(STR_READY).c_str(), ss2.str().c_str(), ss3.str().c_str(), ss4.str().c_str());
	}
}

/**
 * Returns to the previous screen.
 * @param ev Pointer to the SDL_Event.
 * @param scale Scale of the screen.
 */
void CraftsState::btnOkClick(SDL_Event *ev, int scale)
{
	_game->popState();
}

/**
 * Shows the selected craft's info.
 * @param ev Pointer to the SDL_Event.
 * @param scale Scale of the screen.
 */
void CraftsState::lstCraftsClick(SDL_Event *ev, int scale)
{
	_game->pushState(new CraftInfoState(_game, _base, _lstCrafts->getSelectedRow()));
}
