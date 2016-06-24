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
#ifndef ASCEDIT_LAYER_HPP
#define ASCEDIT_LAYER_HPP

#include <map>
#include <string>

#include <QObject>
#include <QPoint>

namespace doc {

class Layer : public QObject
{
private:
    Q_OBJECT
    Q_PROPERTY(unsigned color READ color WRITE set_color NOTIFY color_changed)

public:
    struct QPointCmp
    {
        bool operator()(const QPoint& a, const QPoint& b) const
        {
            return a.y() < b.y() || (a.y() == b.y() && a.x() < b.x());
        }
    };

    /// \todo Pick the right mapping type
    typedef std::map<QPoint, char, QPointCmp> CharacterMap;

    Layer(unsigned color)
        : _color(color)
    {}

    unsigned color() const
    {
        return _color;
    }

    const CharacterMap& characters() const
    {
        return _characters;
    }

    void set_char(QPoint pos, char ch)
    {
        // all ascii characters less than plain space are either
        // spaces or special
        if ( ch <= ' ' )
            remove_char(pos);
        else
            _characters[pos] = ch;
    }

    void remove_char(QPoint pos)
    {
        _characters.erase(pos);
    }

    char char_at(QPoint pos) const
    {
        auto it = _characters.find(pos);
        return it == _characters.end() ? ' ' : it->second;
    }

    std::string to_string() const
    {
        std::string ret;
        QPoint last;
        for ( const auto& pair : _characters )
        {
            if ( last.y() < pair.first.y() )
                ret += std::string(pair.first.y() - last.y(), '\n');
            if ( last.x() + 1 < pair.first.x() )
                ret += std::string(pair.first.x() - last.x() - 1, ' ');
            ret += pair.second;
        }
        return ret;
    }

public slots:
    void set_color(unsigned color)
    {
        if ( color != _color )
        {
            _color = color;
            emit color_changed(color);
        }
    }

signals:
    void color_changed(unsigned color);

private:
    CharacterMap _characters;
    unsigned     _color;
};

} // namespace doc
#endif // ASCEDIT_LAYER_HPP
