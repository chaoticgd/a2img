#ifndef VEC2_H
#define VEC2_H

#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QVector2D>


namespace a2img {

	// Used to store document coordinates, NOT Qt scene coordinates.
	// Circumvents Qt's "kitchen sink" approach to coordinate systems.
	template <typename T>
	class Vec2
	{
	public:
		Vec2(int px, int py);
		Vec2(QPoint point);
		Vec2(QPointF point);
		Vec2(QSize size);
		Vec2(QSizeF size);
		Vec2(QVector2D vector);

		template <typename T_RhsType>
		Vec2(const Vec2<T_RhsType>& rhs);

		/*
			Operators
		*/

		Vec2<T> operator+(const Vec2<T>& rhs) const;
		Vec2<T> operator-(const Vec2<T>& rhs) const;
		Vec2<T> operator*(const Vec2<T>& rhs) const;
		Vec2<T> operator/(const Vec2<T>& rhs) const;
		Vec2<T> operator*(const T& rhs) const;
		Vec2<T> operator/(const T& rhs) const;

		/*
			Conversion
		*/

		QPoint toQPoint() const;
		QPointF toQPointF() const;
		QSize toQSize() const;
		QSizeF toQSizeF() const;
		QVector2D toQVector2D() const;

		T x, y;
	};
}

template <typename T>
a2img::Vec2<T>::Vec2(int px, int py)
	: x(px), y(py)
{}

template <typename T>
a2img::Vec2<T>::Vec2(QPoint point)
	: x(point.x()), y(point.y())
{}

template <typename T>
a2img::Vec2<T>::Vec2(QPointF point)
	: x(point.x()), y(point.y())
{}

template <typename T>
a2img::Vec2<T>::Vec2(QSize size)
	: x(size.width()), y(size.height())
{}

template <typename T>
a2img::Vec2<T>::Vec2(QSizeF size)
	: x(size.width()), y(size.height())
{}

template <typename T>
a2img::Vec2<T>::Vec2(QVector2D vector)
	: x(vector.x()), y(vector.y())
{}

template <typename T>
template <typename T_RhsType>
a2img::Vec2<T>::Vec2(const a2img::Vec2<T_RhsType>& rhs)
	: x(static_cast<T>(rhs.x)),
	  y(static_cast<T>(rhs.y))
{}

/*
	Operators
*/

template <typename T>
a2img::Vec2<T> a2img::Vec2<T>::operator+(const Vec2<T>& rhs) const
{ return { x + rhs.x, y + rhs.y }; }

template <typename T>
a2img::Vec2<T> a2img::Vec2<T>::operator-(const Vec2<T>& rhs) const
{ return { x - rhs.x, y - rhs.y }; }

template <typename T>
a2img::Vec2<T> a2img::Vec2<T>::operator*(const Vec2<T>& rhs) const
{ return { x * rhs.x, y * rhs.y }; }

template <typename T>
a2img::Vec2<T> a2img::Vec2<T>::operator/(const Vec2<T>& rhs) const
{ return { x / rhs.x, y / rhs.y }; }

template <typename T>
a2img::Vec2<T> a2img::Vec2<T>::operator*(const T& rhs) const
{ return { x * rhs, y * rhs }; }

template <typename T>
a2img::Vec2<T> a2img::Vec2<T>::operator/(const T& rhs) const
{ return { x / rhs, y / rhs }; }

/*
	Conversion
*/

template <typename T>
QPoint a2img::Vec2<T>::toQPoint() const
{ return { static_cast<int>(x), static_cast<int>(y) }; }

template <typename T>
QPointF a2img::Vec2<T>::toQPointF() const
{ return { static_cast<double>(x), static_cast<double>(y) }; }

template <typename T>
QSize a2img::Vec2<T>::toQSize() const
{ return { static_cast<int>(x), static_cast<int>(y) }; }

template <typename T>
QSizeF a2img::Vec2<T>::toQSizeF() const
{ return { static_cast<double>(x), static_cast<double>(y) }; }

template <typename T>
QVector2D a2img::Vec2<T>::toQVector2D() const
{ return { static_cast<double>(x), static_cast<double>(y) }; }

#endif // VEC2_H
