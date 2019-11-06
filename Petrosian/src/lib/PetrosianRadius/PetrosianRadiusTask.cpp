/**
 * @file src/lib/PetrosianRadius/PetrosianRadiusTask.cpp
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

#include "Petrosian/PetrosianRadius/PetrosianRadius.h"
#include "Petrosian/PetrosianRadius/PetrosianRadiusTask.h"

#include <SEFramework/Property/DetectionFrame.h>
#include <SEFramework/Aperture/EllipticalAperture.h>
#include <SEFramework/Aperture/NeighbourInfo.h>

#include <SEImplementation/Measurement/MultithreadedMeasurement.h>
#include <SEImplementation/Plugin/PixelCentroid/PixelCentroid.h>
#include <SEImplementation/Plugin/ShapeParameters/ShapeParameters.h>
#include <SEImplementation/Property/PixelCoordinateList.h>

namespace Petrosian {

PetrosianRadiusTask::PetrosianRadiusTask(double eta, double factor, double minrad)
  : m_eta(eta), m_factor(factor), m_minrad(minrad) {}

static const double PETRO_NSIGMAS = 6.;


void PetrosianRadiusTask::computeProperties(SourceXtractor::SourceInterface& source) const {
  // When accessing directly the underlying image, we need to make sure no one else is
  // If this plugin only used other properties - including stamps -, then it would not need to do this
  std::lock_guard<std::recursive_mutex> lock(SourceXtractor::MultithreadedMeasurement::g_global_mutex);

  // We compute the radius on the detection frame, so we get it
  // A frame comprises the image, but also its variance map, threshold, coordinate system...
  // Note that the detection frame is, itself, a property
  const auto& detection_frame = source.getProperty<SourceXtractor::DetectionFrame>().getFrame();

  // Get, from the frame, the detection image, variance map, threshold, and already thresholded image
  const auto& detection_image = detection_frame->getSubtractedImage();
  const auto& detection_variance = detection_frame->getVarianceMap();
  const auto& variance_threshold = detection_frame->getVarianceThreshold();
  const auto& threshold_image = detection_frame->getThresholdedImage();

  // Get the pixel centroid for the source. It is another property, computed by a task inside
  // the main SourceXtractor
  const auto& centroid_x = source.getProperty<SourceXtractor::PixelCentroid>().getCentroidX();
  const auto& centroid_y = source.getProperty<SourceXtractor::PixelCentroid>().getCentroidY();

  // Similarly, get the shape parameters
  const auto& shape_parameters = source.getProperty<SourceXtractor::ShapeParameters>();
  const auto& cxx = shape_parameters.getEllipseCxx();
  const auto& cyy = shape_parameters.getEllipseCyy();
  const auto& cxy = shape_parameters.getEllipseCxy();

  // As using apertures is quite common, SourceXtractor has a couple of helper classes to handle them.
  // Note that the last parameter scales the ellipse (so 6 times bigger here)
  auto ell_aper = std::make_shared<SourceXtractor::EllipticalAperture>(cxx, cyy, cxy, PETRO_NSIGMAS);

  // Get the corners of the stamp covered by the aperture
  const auto& min_pixel = ell_aper->getMinPixel(centroid_x, centroid_y);
  const auto& max_pixel = ell_aper->getMaxPixel(centroid_x, centroid_y);

  // Get the list of pixels that belong to this source
  const auto& pix_list = source.getProperty<SourceXtractor::PixelCoordinateList>().getCoordinateList();

  // This is another helper class that helps identifying pixels from the stamp that belong
  // to some other source, so flags can be set appropiately
  SourceXtractor::NeighbourInfo neighbour_info(min_pixel, max_pixel, pix_list, threshold_image);

  // ------------------------------------------------------------------------
  // Look for the Petrosian radius
  // This has been heavily adapted from SExtractor 2
  // ------------------------------------------------------------------------

  // Step size for the rings
  double step_size = PETRO_NSIGMAS / 20.;

  // We are looking for r
  // kmean corresponds to this r, kmin to 0.9*r and kmax to 1.1*r (or ~1.2 kmin!)
  double kmin, kmax, kmean = 0.;

  // We step from the inner possible ring, to the outer
  for (kmin = step_size; (kmax = kmin * 1.2) < PETRO_NSIGMAS; kmin += step_size) {

    // This is the target we are testing
    kmean = (kmin + kmax) / 2.0;

    // Squared, to check if the radius of a given pixel falls inside
    double kmin2 = kmin * kmin;
    double kmean2 = kmean * kmean;
    double kmax2 = kmax * kmax;

    double flux_outer = 0., flux_inner = 0.;
    double area_outer = 0., area_inner = 0.;

    // We iterate over the stamp
    for (int y = min_pixel.m_y; y < max_pixel.m_y; ++y) {
      for (int x = min_pixel.m_x; x < max_pixel.m_x; ++x) {

        // The aperture could go over the border!
        if (x >= 0 && y >= 0 && x < detection_image->getWidth() && y < detection_image->getHeight()) {
          double pixel_variance = detection_variance ? detection_variance->getValue(x, y) : 1;
          double pixel_value = 0.;

          if (pixel_variance < variance_threshold)
            pixel_value = detection_image->getValue(x, y);

          double r2 = ell_aper->getRadiusSquared(centroid_x, centroid_y, x, y);

          // Is the pixel inside the outer radius?
          if (r2 <= kmax2) {
            // The pixel is inside the outer ring
            if (r2 >= kmin2) {
              flux_outer += pixel_value;
              ++area_outer;
            }
            // The pixel is inside the inner area
            // Note there is an overlap between kmin and kmean
            if (r2 < kmean2) {
              flux_inner += pixel_value;
              ++area_inner;
            }
          }
        }
      }
    }

    // Avoid division by 0!
    if (area_inner && area_outer) {
      // Check the fraction that corresponds to the outer ring flux, if it is below the
      // threshold, we are done
      flux_outer /= area_outer;
      flux_inner /= area_inner;
      if (flux_outer < m_eta * flux_inner) {
        break;
      }
    }
  }

  // Finally set the property
  double radius = std::max(kmean * m_factor, m_minrad);
  source.setProperty<PetrosianRadius>(radius);
}

}  // namespace Petrosian
