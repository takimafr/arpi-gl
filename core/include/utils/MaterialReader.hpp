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


#ifndef _DMA_MATERIALREADER_HPP_
#define _DMA_MATERIALREADER_HPP_

#include "common/Types.hpp"
#include "rapidjson.h"
#include "document.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>


namespace dma {

    class MaterialReader {

    private:
        //FIELDS
        std::string mPath;
        rapidjson::Document mDocument;
        rapidjson::Value mPasses;
        U32 mPassCount;
        I32 mPassIndex;

    public:
        /**
         * Creates a new MaterialReader ready to parse the file
         * supplied as parameter.
         */
        MaterialReader(const std::string& path);
        MaterialReader(const MaterialReader&) = delete;
        void operator=(const MaterialReader&) = delete;
        virtual ~MaterialReader();

        /**
         * 1. Open the file
         * 2. Stringify it
         * 3. Create the DOM ready to be queried
         * Must be called before any other method
         */
        Status parse();

        bool isBackToFront() const;

        /**
         * Setup the context to parse the next pass.
         */
        bool nextPass();

        bool hasCullMode() const;

        /**
         * Returns the cull mode used by the material as a std::string.
         */
        std::string getCullMode() const;


        bool hasDepthWriting() const;

        /**
         * Returns the shader used by the material as a std::string.
         * An empty string means invalid
         */
        std::string getShader() const;

        bool hasDiffuseMap() const;

        /**
         * Returns the diffuse map used by the material as a std::string.
         * An empty string means invalid
         */
        std::string getDiffuseMap() const;

        bool hasLighting() const;

        /**
         * Returns the lighting method used by the material as a std::string.
         * Can be flat or smooth
         * An empty string means invalid
         */
        std::string getLighting() const;

        bool hasScaling() const;

        bool hasDiffuseColor() const;

        /**
         * Returns the diffuse color used by the material as a rgb triplet
         */
        const glm::vec3 getDiffuseColor() const;

        bool hasDiffuseMapActivation();
    };

}

#endif //_DMA_MATERIALREADER_HPP_
