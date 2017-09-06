// Copyright (C) 2017 Taylor Holberton
//
//  This file is part of GNU Guitar Qt.
//
//  GNU Guitar Qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  GNU Guitar Qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with GNU Guitar Qt.  If not, see <http://www.gnu.org/licenses/>.

#include <gnu-guitar-qt/driver.hpp>

#include <rtaudio/session.hpp>

namespace RtAudio {

class CompositeProcessor;

} // namespace RtAudio

namespace GnuGuitar::Qt {

/// @brief A driver implementation that
///  directly uses the GNU Guitar Core library.
class CoreDriver final : public Driver {
  RtAudio::Session session;
  CompositeProcessor *processor;
public:
  CoreDriver();
  ~CoreDriver();
};

} // namespace GnuGuitar::Qt

