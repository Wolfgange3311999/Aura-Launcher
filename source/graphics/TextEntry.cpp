/*-----------------------------------------------------------------
 Copyright (C) 2015
	Matthew Scholefield

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

------------------------------------------------------------------*/

#include "TextEntry.h"
#include <algorithm>

using namespace std;

int TextEntry::calcAlpha()
{
	const int MAX_ALPHA = 31;

	if (delay > 0)
		return 0;
	else if (delay == TextEntry::ACTIVE && fade != FadeType::NONE)
	{
		int routeLength = abs(initX - finalX) + abs(initY - finalY);
		if (routeLength == 0)
			return MAX_ALPHA;

		routeLength *= TextEntry::PRECISION;
		int curDist = abs(x - finalX * TextEntry::PRECISION) + abs(y - finalY * TextEntry::PRECISION);
		int alpha = -(MAX_ALPHA * curDist) / routeLength;
		if (fade == TextEntry::FadeType::OUT)
			alpha *= -1;
		else
			alpha += MAX_ALPHA;
		return max(alpha, 0);
	}
	else
		return MAX_ALPHA;
}

bool TextEntry::update()
{
	if (delay > 0)
		--delay;
	else if (delay == TextEntry::ACTIVE)
	{
		int dX = (finalX * TextEntry::PRECISION - x) / invAccel;
		int dY = (finalY * TextEntry::PRECISION - y) / invAccel;
		x += dX;
		y += dY;
		if (fade == TextEntry::FadeType::OUT && (abs(dX) + abs(dY)) == 0)
			return true;
		/*if (x / TextEntry::PRECISION == finalX)
			delay = TextEntry::COMPLETE;*/
	}
	return false;
}

TextEntry::TextEntry(bool large, int x, int y, const char* message)
: large(large), immune(false), fade(FadeType::NONE), initX(x), initY(y)
, x(x*PRECISION), y(y*PRECISION), finalX(x), finalY(y)
, invAccel(6), delay(1), polyID(1), message(message) { }