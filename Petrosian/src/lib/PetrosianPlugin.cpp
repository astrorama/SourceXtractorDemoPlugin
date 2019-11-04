/**
 * @file src/lib/PetrosianPlugin.cpp
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

#include "Petrosian/PetrosianPlugin.h"
#include "Petrosian/PetrosianConfig.h"
#include "Petrosian/PetrosianRadius/PetrosianRadius.h"
#include "Petrosian/PetrosianRadius/PetrosianRadiusTaskFactory.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryArray.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryTaskFactory.h"
#include <boost/dll/alias.hpp>

namespace Petrosian {

static std::string s_pluginName{"PetrosianPlugin"};

Elements::Logging s_logger = Elements::Logging::getLogger(s_pluginName);

std::string PetrosianPlugin::getIdString() const {
  return s_pluginName;
}

void PetrosianPlugin::registerPlugin(SourceXtractor::PluginAPI& plugin_api) {
  plugin_api.getTaskFactoryRegistry()
    .registerTaskFactory<PetrosianRadiusTaskFactory, PetrosianRadius>();
  plugin_api.getTaskFactoryRegistry()
    .registerTaskFactory<PetrosianPhotometryTaskFactory, PetrosianPhotometry>();
  plugin_api.getTaskFactoryRegistry()
    .registerTaskFactory<PetrosianPhotometryTaskFactory, PetrosianPhotometryArray>();

  // Petrosian aperture

  plugin_api.getOutputRegistry().registerColumnConverter<PetrosianRadius, double>(
    "petrosian_radius",
    &PetrosianRadius::getRadius,
    "[pixel]",
    "Petrosian radius"
  );

  // Petrosian photometry

  plugin_api.getOutputRegistry().registerColumnConverter<PetrosianPhotometryArray, std::vector<double>>(
    "petrosian_flux",
    &PetrosianPhotometryArray::getFluxes,
    "[count]",
    "Flux within a Petronian-like elliptical aperture"
  );

  plugin_api.getOutputRegistry().registerColumnConverter<PetrosianPhotometryArray, std::vector<double>>(
    "petrosian_flux_err",
    &PetrosianPhotometryArray::getFluxErrors,
    "[count]",
    "Flux error within a Petronian-like elliptical aperture"
  );

  plugin_api.getOutputRegistry().registerColumnConverter<PetrosianPhotometryArray, std::vector<double>>(
    "petrosian_mag",
    &PetrosianPhotometryArray::getMags,
    "[count]",
    "Magnitude within a Petronian-like elliptical aperture"
  );

  plugin_api.getOutputRegistry().registerColumnConverter<PetrosianPhotometryArray, std::vector<double>>(
    "petrosian_mag_err",
    &PetrosianPhotometryArray::getMagErrors,
    "[count]",
    "Magnitude error within a Petronian-like elliptical aperture"
  );

  plugin_api.getOutputRegistry().registerColumnConverter<PetrosianPhotometryArray, std::vector<int64_t>>(
    "petrosian_flags",
    [](const PetrosianPhotometryArray& prop) {
      return SourceXtractor::flags2long(prop.getFlags());
    },
    "[]",
    "Flags for the Petrosian photometry"
  );

  plugin_api.getOutputRegistry().enableOutput<PetrosianRadius>("PetrosianRadius");
  plugin_api.getOutputRegistry().enableOutput<PetrosianPhotometryArray>("PetrosianPhotometry");
}

std::shared_ptr<SourceXtractor::Plugin> createPetrosianPlugin() {
  return std::make_shared<Petrosian::PetrosianPlugin>();
}

}  // namespace Petrosian

BOOST_DLL_ALIAS(Petrosian::createPetrosianPlugin, create_plugin)

