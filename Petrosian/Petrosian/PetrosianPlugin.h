/**
 * @file Petrosian/PetrosianPlugin.h
 * @date 11/04/19
 * @author aalvarez
 *
 * @copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef _PLUGIN_PETROSIANPLUGIN_H
#define _PLUGIN_PETROSIANPLUGIN_H

#include <SEFramework/Plugin/Plugin.h>
#include <ElementsKernel/Logging.h>


namespace Petrosian {

extern Elements::Logging s_logger;

class PetrosianPlugin : public SourceXtractor::Plugin {

public:

  virtual ~PetrosianPlugin() = default;

  std::string getIdString() const override;

  void registerPlugin(SourceXtractor::PluginAPI& plugin_api) override;

};  // End of PetrosianPlugin class

}  // namespace Petrosian

#endif
