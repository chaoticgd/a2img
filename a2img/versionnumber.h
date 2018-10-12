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

#ifndef VERSIONNUMBER_H
#define VERSIONNUMBER_H

#include <QString>

namespace a2img {
	// Stores a version number with major and minor compontents.
	// For the effects API version:
	//	- The major number should be incremented when there is an.
	//	  incompatible change that may break existing effects.
	//	- The minor number should be incremented when there is
	//	  an addition that does not affect existing functionality.
	// If the major version is 0, the current build can be
	// considered a testing build and any guarentees on API
	// stability can be ignored.
	//	e.g. 0.1 is a testing build,
	//	     1.0 is a stable build,
	//	     1.1 is a stable build that is backwards compatible with 1.0 and,
	//	     2.0 is a stable build incompatible with 1.x.
	class VersionNumber {
	public:
		VersionNumber(int major, int minor);

		// Convert string in the form "<major>.<minor>" to VersionNumber object.
		VersionNumber(QString version);

		// Returns an integer representing the compatibile-ness of the two versions.
		// 0 means the versions are the same.
		// -1 means the versions are incompatible.
		// Positive values mean the versions are partially compatible.
		int compareCompatibility(VersionNumber other);

		// Returns a string in the form "<major>.<minor>".
		QString toString();

		// Returns a byte array storing the major and minor version numbers, each as 32-bit signed integers.

		QByteArray toQByteArray();
	private:
		int32_t major_;
		int32_t minor_;
	};
};

#endif // VERSIONNUMBER_H
