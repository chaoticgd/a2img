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

#ifndef TYPE_H
#define TYPE_H

#include <QString>
#include <typeinfo>

namespace a2img {
	QString demangleTypeName(QString mangledTypeName);

	template <typename T>
	QString typeName(T* object);
}

template <typename T>
QString a2img::typeName(T* object) {
	QString mangledName = typeid(*object).name();
	return demangleTypeName(mangledName);
}

#endif // TYPE_H
