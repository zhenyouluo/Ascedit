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

#define BOOST_TEST_MODULE Test_Document

#include <boost/test/unit_test.hpp>

#include "document/layer.hpp"

using namespace doc;

BOOST_AUTO_TEST_CASE( test_layer_point_cmp )
{
    auto cmp = Layer::QPointCmp();
    BOOST_CHECK(cmp(QPoint(10, 1), QPoint(6, 2)));
    BOOST_CHECK(cmp(QPoint(10, 1), QPoint(11, 1)));
    BOOST_CHECK(!cmp(QPoint(10, 1), QPoint(10, 1)));
    BOOST_CHECK(!cmp(QPoint(10, 3), QPoint(6, 2)));
}

BOOST_AUTO_TEST_CASE( test_layer_char )
{
    Layer layer(0);
    BOOST_CHECK_EQUAL(layer.to_string(), "");
    layer.set_char({0, 0}, 'P');
    BOOST_CHECK_EQUAL(layer.to_string(), "P");
    layer.set_char({1, 1}, '!');
    layer.set_char({2, 0}, 'o');
    BOOST_CHECK_EQUAL(layer.to_string(), "P o\n!");
    BOOST_CHECK_EQUAL(layer.char_at({2, 0}), 'o');
    BOOST_CHECK_EQUAL(layer.char_at({20, 20}), ' ');
    layer.remove_char({2, 0});
    BOOST_CHECK_EQUAL(layer.to_string(), "P\n!");
    layer.set_char({1, 1}, ' ');
    BOOST_CHECK_EQUAL(layer.to_string(), "P");
}

BOOST_AUTO_TEST_CASE( test_layer_color )
{
    Layer layer(0);
    unsigned changed_to = 0;
    bool changed = false;
    QObject::connect(&layer, &Layer::color_changed, [&changed_to,&changed](unsigned col){
        changed_to = col;
        changed = true;
    });

    BOOST_CHECK_EQUAL(layer.color(), 0);

    layer.set_color(1);
    BOOST_CHECK_EQUAL(layer.color(), 1);
    BOOST_CHECK(changed);
    BOOST_CHECK_EQUAL(changed_to, 1);

    changed = false;
    layer.set_color(1);
    BOOST_CHECK(!changed);
}
