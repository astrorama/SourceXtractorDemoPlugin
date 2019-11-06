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

#ifndef _PETROSIAN_PETROSIANPLUGIN_H
#define _PETROSIAN_PETROSIANPLUGIN_H

#include <SEFramework/Plugin/Plugin.h>
#include <ElementsKernel/Logging.h>


namespace Petrosian {

/**
 * @brief
 *  Implementation of the Plugin interface for Petrosian photometry.
 *  You can only have one plugin by shared library, but there is no limitation on how many properties
 *  a plugin can register.
 *
 *  Note that this is a dependency inversion: the plugin depends on SourceXtractor, not the other way around
 */
class PetrosianPlugin : public SourceXtractor::Plugin {

public:

  /**
   * Default destructor
   */
  virtual ~PetrosianPlugin() = default;

  /**
   * @return
   *    A string that identifies uniquely this plugin
   */
  std::string getIdString() const override;

  /**
   * @brief
   *    Called by SourceXtractor so the plugin can register the properties, columns, etc.
   * @param plugin_api
   *    Plugin system API
   */
  void registerPlugin(SourceXtractor::PluginAPI& plugin_api) override;

};  // End of PetrosianPlugin class

}  // namespace Petrosian

#endif
