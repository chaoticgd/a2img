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

#ifndef VECTORUTIL_H
#define VECTORUTIL_H

#include <vector>
#include <memory>
#include <algorithm>

namespace a2img {
	// Takes a vector of unique pointers, returns a vector of the raw pointers.
	template <typename T_DataType>
	std::vector<T_DataType*> uniqueVectorToRaw(std::vector<std::unique_ptr<T_DataType>>& in);

	// Splits a vector into fixed-size chunks.
	// All chunks will be the same size except the last chunk
	// if the number of input elements is not a multiple of the
	// elementsPerChunk argument.
	template <typename T_DataType>
	std::vector<std::vector<T_DataType>> splitIntoChunks(std::vector<T_DataType> in, int elementsPerChunk);
}

template <typename T_DataType>
std::vector<T_DataType*> a2img::uniqueVectorToRaw(std::vector<std::unique_ptr<T_DataType>>& in)
{
	std::vector<T_DataType*> out(in.size());
	std::transform(
		in.begin(),
		in.end(),
		out.begin(),
		[](std::unique_ptr<T_DataType>& e) {
			return e.get();
		}
	);
	return out;
}

template <typename T_DataType>
std::vector<std::vector<T_DataType>> a2img::splitIntoChunks(std::vector<T_DataType> in, int elementsPerChunk)
{
	std::vector<std::vector<T_DataType>> buckets;
	buckets.emplace_back();
	auto current = buckets.begin();
	for(int i = 0; i < in.size(); i++) {
		// If the current bucket is full,
		// move onto the next one.
		if(current->size() >= elementsPerChunk) {
			buckets.emplace_back();
			current = buckets.end() - 1;
		}

		current->push_back(in[i]);
	}
	return buckets;
}

#endif // VECTORUTIL_H
