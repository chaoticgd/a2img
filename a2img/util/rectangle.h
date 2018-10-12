/*
	a2img - a free and open source image editor
	Copyright (C) 2018 chaoticgd

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <array>

#include <QRectF>

#include "vec2.h"

namespace a2img {
	// A rectangle that can be easily converted between coordinate spaces.
	template <typename T>
    class Rectangle {
    public:
		Rectangle();
		Rectangle(T x1_, T y1_, T x2_, T y2_);
		Rectangle(Vec2<T> firstPoint, Vec2<T> secondPoint);

		template <typename T_RhsType>
		Rectangle(const Rectangle<T_RhsType>& rhs);

		// Returns the bottom, left position of the rectangle.
		Vec2<T> position();

		// Returns the size of the rectangle.
		Vec2<T> size();

		// Returns the coordinates of the corners.
		Vec2<T> bottomLeft();
		Vec2<T> bottomRight();
		Vec2<T> topLeft();
		Vec2<T> topRight();

		bool contains(Vec2<T> point);
		Rectangle<T> moved(Vec2<T> position);
		Rectangle<T> normalised();

		// Converts from document coordinates to OpenGL coordinates.
        // e.g. (0, 0) -> (-1, -1), (w, h) -> (1, 1)
		Rectangle<T> toScreenSpace(Rectangle<T> viewport);

		// Returns OpenGL vertex (geometry) data.
		std::array<T, 18> vertexData();

		// Returns OpenGl UV (texture alignment) data.
		std::array<T, 12> uvData();

		// Lower left, top right.
		T x1, y1, x2, y2;
    };
}

template <typename T>
a2img::Rectangle<T>::Rectangle()
	: x1(0), y1(0), x2(0), y2(0)
{}

template <typename T>
a2img::Rectangle<T>::Rectangle(T x1_, T y1_, T x2_, T y2_)
	: x1(x1_),
	  y1(y1_),
	  x2(x2_),
	  y2(y2_)
{}


template <typename T>
template <typename T_RhsType>
a2img::Rectangle<T>::Rectangle(const a2img::Rectangle<T_RhsType>& rhs)
	: x1(static_cast<T>(rhs.x1)),
	  y1(static_cast<T>(rhs.y1)),
	  x2(static_cast<T>(rhs.x2)),
	  y2(static_cast<T>(rhs.y2))
{}

template <typename T>
a2img::Rectangle<T>::Rectangle(Vec2<T> firstPoint, Vec2<T> secondPoint)
	: x1(firstPoint.x), y1(firstPoint.y), x2(secondPoint.x), y2(secondPoint.y)
{}

template <typename T>
a2img::Vec2<T> a2img::Rectangle<T>::position()
{ return { x1, y1 }; }

template <typename T>
a2img::Vec2<T> a2img::Rectangle<T>::size()
{ return { x2 - x1, y2 - y1 }; }

template <typename T>
a2img::Vec2<T> a2img::Rectangle<T>::bottomLeft()
{ return { x1, y1 }; }

template <typename T>
a2img::Vec2<T> a2img::Rectangle<T>::bottomRight()
{ return { x2, y1 }; }

template <typename T>
a2img::Vec2<T> a2img::Rectangle<T>::topLeft()
{ return { x1, y2 }; }

template <typename T>
a2img::Vec2<T> a2img::Rectangle<T>::topRight()
{ return { x2, y2 }; }

template <typename T>
bool a2img::Rectangle<T>::contains(Vec2<T> point)
{
	return point.x > x1 && point.y > y1 && point.x < x2 && point.y < y2;
}

template <typename T>
a2img::Rectangle<T> a2img::Rectangle<T>::moved(Vec2<T> position)
{
	return {
		position,
		position + size()
	};
}

template <typename T>
a2img::Rectangle<T> a2img::Rectangle<T>::normalised()
{
	Rectangle<T> rect = *this;
	if(x1 > x2) {
		std::swap(rect.x1, rect.x2);
	}
	if(y1 > y2) {
		std::swap(rect.y1, rect.y2);
	}
	return rect;
}

template <typename T>
a2img::Rectangle<T> a2img::Rectangle<T>::toScreenSpace(a2img::Rectangle<T> viewport)
{
	return {
		2 * (x1 - viewport.x1) / viewport.size().x - 1,
		2 * (y1 - viewport.y1) / viewport.size().y - 1,

		2 * (x2 - viewport.x1) / viewport.size().x - 1,
		2 * (y2 - viewport.y1) / viewport.size().y - 1,
	};
}

template <typename T>
std::array<T, 18> a2img::Rectangle<T>::vertexData()
{
	// Vertex data is 3D (z is always 0 for our purposes).
	return {
		x1, y2, 0,
		x2, y2, 0,
		x1,	y1, 0,
		x2, y1, 0,
		x2, y2,	0,
		x1, y1, 0
	};
}

template <typename T>
std::array<T, 12> a2img::Rectangle<T>::uvData()
{
	// UV data is 2D.
	return {
		x1, y2,
		x2, y2,
		x1,	y1,
		x2, y1,
		x2, y2,
		x1, y1
	};
}


#endif // RECTANGLE_H
