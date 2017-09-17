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

#ifndef GNU_GUITAR_GUI_CORE_DRIVER_HPP
#define GNU_GUITAR_GUI_CORE_DRIVER_HPP

#include <gnu-guitar/gui/driver.hpp>

namespace GnuGuitar {

namespace Core {

class Session;
class CompositeProcessor;
class LadspaPlugins;

} // namespace Core

namespace Gui {

/// @brief A driver implementation that
///  directly uses the GNU Guitar Core library.
class CoreDriver final : public Driver {
  Core::Session *session;
  Core::LadspaPlugins *ladspaPlugins;
  Core::CompositeProcessor *processor;
public:
  CoreDriver();
  ~CoreDriver();
  void addEffect(const std::string &effectName) override;
  void listApis(std::vector<ApiSettings> &apiList) override;
  void listEffects(std::vector<std::string> &effectList) override;
  void listEffectControls(const std::string &effectName,
                          std::vector<std::string> &controlList) override;
  void setApi(const ApiSettings &apiSettings) override;
  void setEffectControlValue(const std::string &effectName,
                             const std::string &controlName,
                             float value) override;
  void start() override;
  void stop() override;
};

} // namespace Gui

} // namespace GnuGuitar

#endif // GNU_GUITAR_GUI_CORE_DRIVER_HPP

