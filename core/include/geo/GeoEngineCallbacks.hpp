/*
 * Copyright (C) 2015  eBusiness Information
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


/*
 * GeoEngineCallbacks.hpp
 *
 *  Created on: 3 août 2015
 *      Author: Nicolas THIERION.
 */

#ifndef DMA_GEOENGINECALLBACKS_HPP_
#define DMA_GEOENGINECALLBACKS_HPP_

#include "utils/Log.hpp"

namespace dma {

        /**
         * This class provides a way to execute additional tasks on several Engine events.
         * IE : fetch resource from a data provider when the engine request a Tile or a Poi.
         * By default, this doesn't do anything.
         */
        class GeoEngineCallbacks {

            static constexpr auto TAG = "GeoEngineCallbacks";

        public:

            /* ***
             * CONSTRUCTOR / DESTRUCTOR
             */
            GeoEngineCallbacks();
            virtual ~GeoEngineCallbacks();

            /* ***
             * DELETED METHODS
             */
            GeoEngineCallbacks(const GeoEngineCallbacks&) = delete;
            void operator=(const GeoEngineCallbacks&) = delete;

            /* ***
             * CALLBAK METHODS
             */

            /**
             * Called each time the engine is missing resources for a tile.
             * This happens when the position moves on an area,
             * and the needed png is not on the storage to feed the tilemap.
             *
             * @param int
             *          x coord of the requested tile.
             * @param int
             *          y coord of the requested tile.
             * @param int
             *          z coord of the requested tile.
             */
            virtual inline void onTileRequest(int x, int y, int z) {
                Log::error(TAG, "Not implemented tile request (tile (%d, %d, %d))", x, y, z);
            }

            /**
             * Called each time the engine displays a tile.
             * This happens for example each time the position moves on an new area.
             *
             * @param int
             *          x coord of the requested tile.
             * @param int
             *          y coord of the requested tile.
             * @param int
             *          z coord of the requested tile.
             */
            virtual inline void onTileDiplayed(int x, int y, int z) {
                Log::warn(TAG, "Not implemented tile update (tile (%d, %d, %d))", x, y, z);
            }

            virtual inline void onPoiSelected(const std::string& sid) {
                Log::warn(TAG, "Not implemented on poi selected");
            }

            virtual inline void onPoiDeselected(const std::string& sid) {
                Log::warn(TAG, "Not implemented on poi deselected");
            }

        };

} /* namespace dma */

#endif /* DMA_GEOENGINECALLBACKS_HPP_ */
