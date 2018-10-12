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
