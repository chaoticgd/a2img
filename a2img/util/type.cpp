#include "type.h"

#include <cxxabi.h>

QString a2img::demangleTypeName(QString mangledTypeName) {
	char demangledName[256];
	int status;
	size_t length = 256;

	if(mangledTypeName == "??") {
		return "??";
	}

	std::string mangledTypeNameStr = mangledTypeName.toStdString();
	abi::__cxa_demangle(
		mangledTypeNameStr.c_str(),
		demangledName,
		&length,
		&status
	);

	if(status != 0) {
		return mangledTypeName;
	}

	return QString(demangledName);
}
