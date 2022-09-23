/*
    This file is part of IngenicHAL.
    Copyright (C) 2022 Reimu NotMoe <reimu@sudomaker.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#define XHAL_BIT(x)			(1 << (x))

#define XHAL_BIT_GET(x, n)		(((x) >> (n)) & 1)
#define XHAL_BIT_SET(x, n)		((x) |= XHAL_BIT(n))
#define XHAL_BIT_CLR(x, n)		((x) &= ~XHAL_BIT(n))
