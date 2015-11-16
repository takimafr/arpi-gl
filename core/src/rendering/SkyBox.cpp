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



#include <string.h>
#include "rendering/SkyBox.hpp"

constexpr auto TAG = "SkyBox";

namespace dma {


    //--------------------------------------------------------------------
    SkyBox::SkyBox() :
            mCubeMap(nullptr), mShaderProgram(nullptr)

    {}


    //--------------------------------------------------------------------
    SkyBox::~SkyBox() {

    }

    //--------------------------------------------------------------------
    void SkyBox::init(std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<ShaderProgram> shaderProgram) {

        GLfloat positions[] = {
                1.0f, -0.0f, - 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f
        };

        GLushort indices[] = {
                1, 2, 3,
                7, 6, 5,
                4, 5, 1,
                5, 6, 2,
                2, 6, 7,
                0, 3, 7,
                0, 1, 3,
                4, 7, 5,
                0, 4, 1,
                1, 5, 2,
                3, 2, 7,
                4, 0, 7
        };

        U32 vertexCount = 8;
        U32 vertexSize = 0;
        VertexElement positionElement(VertexElement::Semantic::POSITION, 3, GL_FLOAT, vertexSize);
        vertexSize += positionElement.getSizeInByte();

        BYTE* data = new BYTE[vertexSize * vertexCount];

        /////////////////////////////////////////////////////////////////////////
        // Fills data
        Log::debug(TAG, "%d, %d", positionElement.getSizeInByte(), positionElement.getOffset());
        for (U32 v = 0; v < vertexCount; ++v) {
            memcpy(&data[v * vertexSize + positionElement.getOffset()], &(positions[3*v]), positionElement.getSizeInByte());
        }

        /////////////////////////////////////////////////////////////////////////
        // Generate vertex buffer and upload data to GPU
        mVertexBuffer.generateBuffer(vertexSize, vertexSize * vertexCount);
        mVertexBuffer.writeData(0, vertexSize * vertexCount, data);
        delete[] data;

        /////////////////////////////////////////////////////////////////////////
        // Generate and fill index buffer
        mIndexBuffer.generateBuffer(36); //index count
        mIndexBuffer.writeData(indices);

        mCubeMap = cubeMap;
        mShaderProgram = shaderProgram;
    }


    //--------------------------------------------------------------------
    void SkyBox::refresh() {
        //wipe();
        init(mCubeMap, mShaderProgram);
    }


    //--------------------------------------------------------------------
    void SkyBox::wipe() {
        mVertexBuffer.wipe();
        mIndexBuffer.wipe();
    }
}
