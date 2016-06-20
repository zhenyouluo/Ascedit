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
 *
 */

#include "cute_color.hpp"

namespace color {

Color from_qt(QColor color)
{
    switch ( color.spec() )
    {
        case QColor::Hsl:
            color.toHsv();
        case QColor::Hsv:
            return Color(
                repr::HSVf(color.hsvHueF(), color.hsvSaturationF(), color.valueF()),
                color.alpha()
            );
        case QColor::Invalid:
            return Color();
        default:
            color.toRgb();
        case QColor::Rgb:
            return Color(color.red(), color.green(), color.blue(), color.alpha());
    }
}

} // namespace color
