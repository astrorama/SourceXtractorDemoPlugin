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


std::string PetrosianPlugin::getIdString() const {
  return s_pluginName;
}

void PetrosianPlugin::registerPlugin(SourceXtractor::PluginAPI& plugin_api) {

  // ------------------------------------------------------------------------
  // For the Property machinery to works, it needs to know which task factory
  // takes care of creating taks for which properties. The following lines do
  // that.
  // ------------------------------------------------------------------------

  // PetrosianRadiusTaskFactory takes care of the property PetrosianRadius
  plugin_api.getTaskFactoryRegistry()
    .registerTaskFactory<PetrosianRadiusTaskFactory, PetrosianRadius>();

  // PetrosianPhotometryTaskFactory takes care of both PetrosianPhotometry and
  // PetrosianPhotometryArray
  // Have a look at the documentation of each one of these properties, and at
  // their task, to see why do we have these two, instead of a single one
  plugin_api.getTaskFactoryRegistry()
    .registerTaskFactory<PetrosianPhotometryTaskFactory, PetrosianPhotometry, PetrosianPhotometryArray>();

  // ------------------------------------------------------------------------
  // Now that we have set up the factories for the properties, we configure
  // here which columns are generated when these properties are demanded
  // with --output-properties
  // Note that we can have properties that are used internally by other properties,
  // without an associated output: PetrosianPhotometry has no associated output,
  // it is consumed by PetrosianPhotometryArray
  // ------------------------------------------------------------------------

  // PetrosianRadius has a single associated column: the radius

  plugin_api.getOutputRegistry().registerColumnConverter<PetrosianRadius, double>(
    "petrosian_radius",
    &PetrosianRadius::getRadius,
    "[pixel]",
    "Petrosian radius"
  );

  // PetrosianPhotometryArray has several columns, which are multidimensional.
  // This is because SourceXtractor supports multiple measurement images, so you would have one
  // measurement per image.
  // If SourceXtractor is run with only a detection image, then there will be one single
  // elements on the vector, which is transparently exported as a scalar column

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
      // Note that we need to convert the internal representation to an integer
      return SourceXtractor::flags2long(prop.getFlags());
    },
    "[]",
    "Flags for the Petrosian photometry"
  );

  // ------------------------------------------------------------------------
  // Finally, we tell the plugin system how these properties are named. These
  // are the strings to be put in --output-properties, and reported by
  // --list-output-properties
  // ------------------------------------------------------------------------
  plugin_api.getOutputRegistry().enableOutput<PetrosianRadius>("PetrosianRadius");
  plugin_api.getOutputRegistry().enableOutput<PetrosianPhotometryArray>("PetrosianPhotometry");
}

/**
 * This function is the entry point of the whole plugin. It just created the PetrosianPlugin,
 * and return a pointer to it.
 */
std::shared_ptr<SourceXtractor::Plugin> createPetrosianPlugin() {
  return std::make_shared<Petrosian::PetrosianPlugin>();
}

}  // namespace Petrosian

/**
 * Create an alias called "create_plugin", which is the symbol for which SourceXtractor looks
 * on the generated shared library
 */
BOOST_DLL_ALIAS(Petrosian::createPetrosianPlugin, create_plugin)
