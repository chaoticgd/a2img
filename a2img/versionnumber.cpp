#include "versionnumber.h"

#include <exception>

#include <QStringList>
#include <QDataStream>

a2img::VersionNumber::VersionNumber(int major, int minor)
	: major_(major),
	  minor_(minor)
{
}

a2img::VersionNumber::VersionNumber(QString version)
{
	// Find the major, minor parts (in the format "major.minor").
	QStringList parts = version.split('.');

	if(parts.length() != 2) {
		throw std::runtime_error("Failed to parse version number: Invalid format.");
	}

	major_ = parts[0].toInt();
	minor_ = parts[1].toInt();
}

int a2img::VersionNumber::compareCompatibility(VersionNumber other)
{
	if(major_ != other.major_) {
		return -1;
	}
	return std::abs(minor_ - other.minor_);
}

QString a2img::VersionNumber::toString()
{
	return QString::number(major_) + "." + QString::number(minor_);
}

QByteArray a2img::VersionNumber::toQByteArray()
{
	QByteArray result;
	QDataStream stream(&result, QIODevice::WriteOnly);

	stream << major_;
	stream << minor_;

	return result;
}
