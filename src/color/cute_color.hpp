/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright (C) 2016 Mattia Basaglia
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ASCEDIT_COLOR_CUTECOLOR_HPP
#define ASCEDIT_COLOR_CUTECOLOR_HPP

#include <QColor>

#include "color.hpp"

namespace color {

Color from_qt(QColor color);

inline QColor to_qt(Color color)
{
    if ( !color.valid() )
        return QColor();
    return QColor(color.red(), color.green(), color.blue(), color.alpha());
}

inline QRgb to_qrgb(Color color)
{
    return qRgba(color.red(), color.green(), color.blue(), color.alpha());
}

} // namespace color
#endif // ASCEDIT_COLOR_CUTECOLOR_HPP
