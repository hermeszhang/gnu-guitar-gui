// Copyright (C) 2017 Taylor Holberton
//
//  This file is part of GNU Guitar GUI.
//
//  GNU Guitar GUI is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  GNU Guitar GUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with GNU Guitar GUI.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GNU_GUITAR_GUI_FAKE_DRIVER_HPP
#define GNU_GUITAR_GUI_FAKE_DRIVER_HPP

#include <gnu-guitar/gui/driver.hpp>

namespace GnuGuitar {

namespace Gui {

/// @brief Implements a fake driver.
///  This is useful for developing the
///  project in a standalone manner.
class FakeDriver final : public Driver {
  std::string api;
  std::vector<std::string> effectList;
public:
  FakeDriver() noexcept;
  ~FakeDriver();
  void addEffect(const std::string &effectName) override;
  void listApis(std::vector<ApiSettings> &apis) override;
  void listEffects(std::vector<std::string> &effects) override;
  void listEffectControls(const std::string &effectName,
                          std::vector<std::string> &controlList) override;
  void setApi(const ApiSettings &api) override;
  void setEffectControlValue(const std::string &effectName,
                             const std::string &controlName,
                             float value) override;
  void start() override;
  void stop() override;
protected:
  bool usingApi(const std::string &apiName);
  bool usingEffect(const std::string &effectName);
};

} // namespace Gui

} // namespace GnuGuitar

#endif // GNU_GUITAR_GUI_FAKE_DRIVER_HPP
