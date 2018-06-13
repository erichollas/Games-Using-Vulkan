/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		BaseState.h																	*
* DETAILS:	This file defines the base state to handle each state of each character		*
*				separately. Everything listed in this file is a pure virtual function.	*
*																						*
*****************************************************************************************/


#pragma once
#include "stdafx.h"
#include "Messanger.h"

template <class character_type>
class state {
public:

	virtual void execute(character_type *) = 0;
	virtual void onMessage(character_type *, message &) = 0;
};