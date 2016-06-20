/**
 * \file
 * \author Mattia Basaglia
 * \copyright Copyright 2015-2016 Mattia Basaglia
 * \section License
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_MODULE Test_Cute_Color

#include <boost/test/unit_test.hpp>

#include "color/cute_color.hpp"

using namespace color;

BOOST_AUTO_TEST_CASE( test_from_qt )
{
    BOOST_CHECK_EQUAL( from_qt(QColor(1, 2, 3, 4)), Color(1, 2, 3, 4) );
    BOOST_CHECK_EQUAL(
        from_qt(QColor::fromHsvF(0.1, 0.2, 0.3, 0.4)),
        Color(repr::HSVf(0.1, 0.2, 0.3), 0.4)
    );
    BOOST_CHECK_EQUAL( from_qt(QColor()), Color() );
}
