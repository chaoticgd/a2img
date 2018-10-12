#ifndef TREENODE_H
#define TREENODE_H

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "vectorutil.h"

namespace a2img {
	// A tree data structure using the curiously recurring template pattern.
	template <typename T_DataType>
	class TreeNode
	{
	protected:
		TreeNode();

		virtual ~TreeNode() {}

	public:
		// Creates a std::vector of raw pointers to all children of this node.
		std::vector<T_DataType*> children();

		// Creates an object of type T_DataType and appends it as a child.
		template <typename T_ChildType, typename... T_ConstructorArgs>
		T_ChildType* createChild(T_ConstructorArgs... args);

		// Takes ownership of a passed node and appends it as a child.
		T_DataType* appendChild(std::unique_ptr<T_DataType>& child);

		// Takes ownership of a passed node and inserts it at a specific position.
		T_DataType* insertChild(std::unique_ptr<T_DataType>& child, size_t position);

		// Removes the passed child from this node and returns a unique pointer to it.
		std::unique_ptr<T_DataType> removeChild(T_DataType* child);

		// Returns a raw pointer to the parent node.
		// If none exists, returns nullptr.
		T_DataType* parent();

	private:
		std::vector<std::unique_ptr<T_DataType>> children_;
		T_DataType* parent_;
	};
}

template <typename T_DataType>
a2img::TreeNode<T_DataType>::TreeNode()
	: parent_(nullptr) {}

template <typename T_DataType>
std::vector<T_DataType*> a2img::TreeNode<T_DataType>::children()
{
	return uniqueVectorToRaw(children_);
}

template <typename T_DataType>
template <typename T_ChildType, typename... T_ConstructorArgs>
T_ChildType* a2img::TreeNode<T_DataType>::createChild(T_ConstructorArgs... args)
{
	T_ChildType* child = new T_ChildType(args...);
	child->parent_ = static_cast<T_DataType*>(this);
	children_.emplace_back(child);
	return child;
}

template <typename T_DataType>
T_DataType* a2img::TreeNode<T_DataType>::appendChild(std::unique_ptr<T_DataType>& child)
{
	child->parent_ = static_cast<T_DataType*>(this);
	children_.emplace_back(std::move(child));
	return (children_.end() - 1)->get();
}

template <typename T_DataType>
T_DataType* a2img::TreeNode<T_DataType>::insertChild(std::unique_ptr<T_DataType>& child, size_t position)
{
	child->parent_ = static_cast<T_DataType*>(this);
	T_DataType* raw = child.get();
	children_.insert(children_.begin() + position, std::move(child));
	return raw;
}

template <typename T_DataType>
std::unique_ptr<T_DataType> a2img::TreeNode<T_DataType>::removeChild(T_DataType* child)
{
	std::unique_ptr<T_DataType> removedChild(nullptr);
	auto iter = std::remove_if(
		children_.begin(), children_.end(),
		[=, &removedChild](std::unique_ptr<T_DataType>& current) {
			bool removeCurrent = current.get() == child;
			if(removeCurrent) {
				removedChild.reset(current.release());
			}
			return removeCurrent;
		});
	children_.erase(iter, children_.end());

	return std::move(removedChild); // Pass ownership to the caller.
}

template <typename T_DataType>
T_DataType* a2img::TreeNode<T_DataType>::parent()
{
	return parent_;
}

#endif // TREENODE_H
