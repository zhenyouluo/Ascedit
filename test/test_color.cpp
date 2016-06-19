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

#define BOOST_TEST_MODULE Test_Color

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "color/color.hpp"

using boost::test_tools::output_test_stream;
using namespace color;

BOOST_AUTO_TEST_CASE( test_default_ctor )
{
    Color color;

    BOOST_CHECK( !color.valid() );
}

BOOST_AUTO_TEST_CASE( test_rgb_ctor )
{
    Color color1(1, 2, 3);
    BOOST_CHECK( color1.valid() );
    BOOST_CHECK_EQUAL( color1.red(), 1 );
    BOOST_CHECK_EQUAL( color1.green(), 2 );
    BOOST_CHECK_EQUAL( color1.blue(), 3 );
    BOOST_CHECK_EQUAL( color1.alpha(), 255 );

    Color color2(1, 2, 3, 4);
    BOOST_CHECK( color2.valid() );
    BOOST_CHECK_EQUAL( color2.red(), 1 );
    BOOST_CHECK_EQUAL( color2.green(), 2 );
    BOOST_CHECK_EQUAL( color2.blue(), 3 );
    BOOST_CHECK_EQUAL( color2.alpha(), 4 );
}

BOOST_AUTO_TEST_CASE( test_comparison )
{
    BOOST_CHECK_EQUAL( Color(1, 2, 3), Color(1, 2, 3) );
    BOOST_CHECK_NE( Color(1, 2, 3), Color(1, 2, 3, 4) );
    BOOST_CHECK_EQUAL( Color(1, 2, 3, 4), Color(1, 2, 3, 4) );
    BOOST_CHECK_NE( Color(1, 2, 3), Color() );
    BOOST_CHECK_NE( Color(), Color(1, 2, 3) );
    BOOST_CHECK_EQUAL( Color(), Color() );
    BOOST_CHECK_NE( Color(1, 2, 3), Color(1, 2, 0) );
    BOOST_CHECK_NE( Color(1, 2, 3), Color(1, 0, 3) );
    BOOST_CHECK_NE( Color(1, 2, 3), Color(0, 2, 3) );
}

BOOST_AUTO_TEST_CASE( test_stream )
{
    output_test_stream output;

    output << Color();
    BOOST_CHECK( output.is_equal( "rgb()" ) );

    output << Color(1, 2, 3);
    BOOST_CHECK( output.is_equal( "rgb(1, 2, 3)" ) );

    output << Color(1, 2, 3, 4);
    BOOST_CHECK( output.is_equal( "rgba(1, 2, 3, 4)" ) );
}

BOOST_AUTO_TEST_CASE( test_repr_ctor )
{
    Color a(repr::RGB(1, 2, 3));
    BOOST_CHECK( a.valid() );
    BOOST_CHECK_EQUAL( a.red(), 1 );
    BOOST_CHECK_EQUAL( a.green(), 2 );
    BOOST_CHECK_EQUAL( a.blue(), 3 );
    BOOST_CHECK_EQUAL( a.alpha(), 255 );

    Color b(repr::RGB(1, 2, 3), 4);
    BOOST_CHECK( b.valid() );
    BOOST_CHECK_EQUAL( b.red(), 1 );
    BOOST_CHECK_EQUAL( b.green(), 2 );
    BOOST_CHECK_EQUAL( b.blue(), 3 );
    BOOST_CHECK_EQUAL( b.alpha(), 4 );

    Color c(repr::RGBf(0, 1, 0));
    BOOST_CHECK( c.valid() );
    BOOST_CHECK_EQUAL( c.red(), 0 );
    BOOST_CHECK_EQUAL( c.green(), 255 );
    BOOST_CHECK_EQUAL( c.blue(), 0 );
    BOOST_CHECK_EQUAL( c.alpha(), 255 );

    Color d(repr::RGBf(0, 1, 0), 0.5);
    BOOST_CHECK( d.valid() );
    BOOST_CHECK_EQUAL( d.red(), 0 );
    BOOST_CHECK_EQUAL( d.green(), 255 );
    BOOST_CHECK_EQUAL( d.blue(), 0 );
    BOOST_CHECK_EQUAL( d.alpha(), 128 );
}

BOOST_AUTO_TEST_CASE( test_from_rgbf )
{
    Color c(repr::RGBf(0, 0.5, 1));
    BOOST_CHECK( c.valid() );
    BOOST_CHECK_EQUAL( c.red(), 0 );
    BOOST_CHECK_EQUAL( c.green(), 128 );
    BOOST_CHECK_EQUAL( c.blue(), 255 );
}

BOOST_AUTO_TEST_CASE( test_from_hsvf )
{
    BOOST_CHECK_EQUAL(Color(repr::HSVf(0, 0, 0)), Color(0, 0, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(0.5, 0, 0)), Color(0, 0, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(0.5, 1, 0)), Color(0, 0, 0));

    BOOST_CHECK_EQUAL(Color(repr::HSVf(0, 1, 1)), Color(255, 0, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(-1, 1, 1)), Color(255, 0, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(1, 1, 1)), Color(255, 0, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(0.5, 1, 1)), Color(0, 255, 255));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(1.5, 1, 1)), Color(0, 255, 255));

    BOOST_CHECK_EQUAL(Color(repr::HSVf(0/6.0, 1, 1)), Color(255, 0, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(1/6.0, 1, 1)), Color(255, 255, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(2/6.0, 1, 1)), Color(0, 255, 0));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(3/6.0, 1, 1)), Color(0, 255, 255));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(4/6.0, 1, 1)), Color(0, 0, 255));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(5/6.0, 1, 1)), Color(255, 0, 255));
    BOOST_CHECK_EQUAL(Color(repr::HSVf(6/6.0, 1, 1)), Color(255, 0, 0));
}
