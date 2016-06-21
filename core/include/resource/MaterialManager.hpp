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


#ifndef _DMA_MATERIALMANAGER_HPP
#define _DMA_MATERIALMANAGER_HPP

#include "resource/Material.hpp"
#include "resource/ResourceManagerHandler.hpp"
#include "resource/ShaderManager.hpp"
#include "resource/MapManager.hpp"

#include <string>
#include <set>
#include <list>

namespace dma {

        class MaterialManager : public ResourceManagerHandler<Material> {

            friend class ResourceManager;
            static const std::string FALLBACK_MATERIAL_SID;

        public:
            //DESTRUCTOR
            virtual ~MaterialManager();

            //METHODS


            /**
             * From disk
             */
            Status reload();

            std::shared_ptr<Material> create();
            std::shared_ptr<Material> create(const std::string& sid);

            virtual bool hasResource(const std::string &) const;

        private:
            //CONSTRUCTORS
            MaterialManager(const std::string& rootDir,
                            ShaderManager& shaderManager,
                            MapManager& mapManager);
            MaterialManager(const MaterialManager&) = delete;
            void operator=(const MaterialManager&) = delete;


            //METHODS
            Status load(std::shared_ptr<Material> material,  const std::string& sid) const;

            //FIELDS
            std::string                     mLocalDir;

            ShaderManager&                  mShaderManager;
            MapManager&                     mMapManager;
            std::map<std::string, std::shared_ptr<Material>> mMaterials;
            std::shared_ptr<Material>       mFallbackMaterial;
        };
}

#endif //_DMA_MATERIALMANAGER_HPP
