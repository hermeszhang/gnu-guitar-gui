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

#ifndef GNU_GUITAR_QT_DRIVER_HPP
#define GNU_GUITAR_QT_DRIVER_HPP

#include <string>
#include <vector>

namespace GnuGuitar {

namespace Qt {

/// @brief This class is designed for binding
///  the @ref Controller class to the program logic.
class Driver {
public:
  /// @brief Standard deconstructor.
  virtual ~Driver();
  /// @brief Adds an effect to use next time the
  ///  driver starts streaming the audio.
  virtual void addEffect(const std::string &ladspaEffect) = 0;
  /// @brief Lists the available APIs.
  virtual void listApis(std::vector<std::string> &apis) = 0;
  /// @brief Lists the available effects.
  virtual void listEffects(std::vector<std::string> &ladspaEffects) = 0;
  /// @brief Lists the available controls of a certain
  ///  effect. The effect must have been added via @ref addEffect
  ///  before this function is called.
  virtual void listEffectControls(const std::string &effectName,
                                  std::vector<std::string> &effectControls) = 0;
  /// @brief Sets the API that the driver will use.
  ///  This API must be one of the APIs listed by @ref listApis.
  virtual void setApi(const std::string &apiName) = 0;
  /// @brief Sets the value of an effect control.
  virtual void setEffectControlValue(const std::string &effectName,
                                     const std::string &controlName,
                                     float value) = 0;
  /// @brief Begins streaming the audio.
  virtual void start() = 0;
  /// @brief Stops the streaming of audio.
  virtual void stop() = 0;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_DRIVER_HPP
