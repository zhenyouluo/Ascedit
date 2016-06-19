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
#ifndef MELANO_COLOR_HPP
#define MELANO_COLOR_HPP

#include <cstdint>
#include <string>
#include <ostream>
#include "melanolib/math/math.hpp"

namespace color {

/**
 * \brief Color representation format
 */
enum class Format
{
    Invalid,
    RGB,
    RGBf,
    HSVf,
    Lab,
    XYZ,
};

/**
 * \brief Color representation namespace
 */
namespace repr {

/**
 * \brief 24bit interger RGB
 */
struct RGB
{
    static constexpr auto format = Format::RGB;

    uint8_t r, g, b;

    constexpr RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    constexpr RGB() : r(0), g(0), b(0) {}
};

/**
 * \brief Floating-point RGB
 */
struct RGBf
{
    static constexpr auto format = Format::RGBf;

    float r, g, b;

    constexpr RGBf(float r, float g, float b) : r(r), g(g), b(b) {}
};

/**
 * \brief Floating-point HSV
 */
struct HSVf
{
    static constexpr auto format = Format::HSVf;

    float h, s, v;

    constexpr HSVf(float h, float s, float v) : h(h), s(s), v(v) {}
};

/**
 * \brief CIE L*a*b
 */
struct Lab
{
    static constexpr auto format = Format::Lab;

    float l, a, b;

    constexpr Lab(float l, float a, float b) : l(l), a(a), b(b) {}
};

/**
 * \brief CIE XYZ
 */
struct XYZ
{
    static constexpr auto format = Format::XYZ;

    float x, y, z;

    constexpr XYZ(float x, float y, float z) : x(x), y(y), z(z) {}
};

} // namespace repr


class Color
{
public:

    template<class Repr>
        constexpr Color(Repr repr, uint8_t alpha = 255)
        : _alpha(alpha)
    {
        from(repr);
    }

    template<class Repr, class Float, class = std::enable_if_t<std::is_floating_point<Float>::value>>
        constexpr Color(Repr repr, Float alpha)
        : _alpha(melanolib::math::round<uint8_t>(alpha * 255))
    {
        from(repr);
    }

    constexpr Color()
        : _alpha(0), _valid(false)
    {}

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : _rgb(r, g, b), _alpha(a)
    {}

    constexpr auto alpha() const
    {
        return _alpha;
    }

    constexpr auto red() const
    {
        return _rgb.r;
    }

    constexpr auto green() const
    {
        return _rgb.g;
    }

    constexpr auto blue() const
    {
        return _rgb.b;
    }

    constexpr auto valid() const
    {
        return _valid;
    }

    template<class Repr>
        Repr to() const;

    constexpr bool operator==(const Color& oth) const
    {
        return (!_valid && !oth._valid) || (
            _rgb.r == oth._rgb.r &&
            _rgb.g == oth._rgb.g &&
            _rgb.b == oth._rgb.b &&
            _alpha == oth._alpha
        );
    }

    constexpr bool operator!=(const Color& oth) const
    {
        return !(*this == oth);
    }

    friend std::ostream& operator<< (std::ostream& os, const Color& color)
    {
        if ( !color._valid )
            return os << "rgb()";

        if ( color._alpha == 255 )
            return os << "rgb("
                      << int(color._rgb.r) << ", "
                      << int(color._rgb.g) << ", "
                      << int(color._rgb.b) << ")";

        return os << "rgba("
                  << int(color._rgb.r) << ", "
                  << int(color._rgb.g) << ", "
                  << int(color._rgb.b) << ", "
                  << int(color._alpha) << ")";
    }


private:
    template<class Repr>
        void from(Repr value);
    
private:
    repr::RGB _rgb;
    uint8_t   _alpha;
    bool      _valid = true;
};

template<>
    constexpr void Color::from<repr::RGB>(repr::RGB value)
{
    _rgb = value;
}

template<>
    constexpr void Color::from<repr::RGBf>(repr::RGBf value)
{
    _rgb = repr::RGB{
        melanolib::math::round<uint8_t>(value.r * 255),
        melanolib::math::round<uint8_t>(value.g * 255),
        melanolib::math::round<uint8_t>(value.b * 255)
    };
}

template<>
    constexpr void Color::from<repr::HSVf>(repr::HSVf value)
{
    using namespace melanolib::math;

    auto h = value.h;
    if ( h < 0 )
        h = 0;
    else if ( h > 1 )
        h = fractional(h);
    h *= 6;

    auto s = bound(0.f, value.s, 1.f);
    auto v = bound(0.f, value.v, 1.f);

    auto c = v * s;
    auto m = v - c;

    auto h1 = truncate(h);
    auto f = h - h1;
    auto n = v - c * f;
    auto k = v - c * (1 - f);

    auto iv = round<uint8_t>(v * 255);
    auto im = round<uint8_t>(m * 255);
    auto in = round<uint8_t>(n * 255);
    auto ik = round<uint8_t>(k * 255);

    switch ( h1 )
    {
        case 0:
            _rgb = repr::RGB(iv, ik, im);
            break;
        case 1:
            _rgb = repr::RGB(in, iv, im);
            break;
        case 2:
            _rgb = repr::RGB(im, iv, ik);
            break;
        case 3:
            _rgb = repr::RGB(im, in, iv);
            break;
        case 4:
            _rgb = repr::RGB(ik, im, iv);
            break;
        case 5:
            _rgb = repr::RGB(iv, im, in);
            break;
        case 6:
        default:
            _rgb = repr::RGB(iv, ik, im);
            break;
    }
}

template<>
    constexpr repr::RGBf Color::to<repr::RGBf>() const
{
    return {
        _rgb.r / 255.0f,
        _rgb.g / 255.0f,
        _rgb.b / 255.0f,
    };
}

template<>
    constexpr repr::HSVf Color::to<repr::HSVf>() const
{
    auto rgbf = to<repr::RGBf>();

    auto cmax = melanolib::math::max(rgbf.r, rgbf.g, rgbf.b);
    auto cmin = melanolib::math::min(rgbf.r, rgbf.g, rgbf.b);
    float h = 0;
    auto delta = cmax - cmin;

    if ( delta > 0 )
    {

        if ( cmax == rgbf.r )
            h = rgbf.g - rgbf.b / delta;
        else if ( cmax == rgbf.g )
            h = (rgbf.b - rgbf.r) / delta + 2;
        else // cmax == b
            h = (rgbf.r - rgbf.g) / delta + 4;

        if ( h < 0 )
            h = 6 + h;
    }

    h /= 6;
    auto s = cmax > 0 ? delta / cmax : 0;
    auto v = cmax;

    return {h, s, v};
}


} // namespace color
#endif // MELANO_COLOR_HPP
