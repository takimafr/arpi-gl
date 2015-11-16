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



#include "resource/QuadFactory.hpp"

#include <string.h>

constexpr auto TAG = "QuadFactory";

namespace dma {

    //--------------------------------------------------
    QuadFactory::QuadFactory() :
            mVertexSize(0),
            mVertexCount(0),
            mVertexBuffer(std::make_shared<VertexBuffer>()),
            mIndexBuffer(std::make_shared<IndexBuffer>())
    {}


    //--------------------------------------------------
    QuadFactory::~QuadFactory() {

    }

    //--------------------------------------------------
    void QuadFactory::init() {
        Log::trace(TAG, "Loading QuadFactory...");

        GLfloat positions[] = {-1.0f, -1.0f, 0.0f,  // bottom left corner
                               -1.0f,  1.0f, 0.0f,  // top left corner
                               1.0f,  1.0f, 0.0f,  // top right corner
                               1.0f, -1.0f, 0.0f}; // bottom right corner

        GLfloat uvs[] = {0.0f, 0.0f,
                         0.0f, 1.0f,
                         1.0f, 1.0f,
                         1.0f, 0.0f,};


        GLfloat flatNormals[] = {0.0f, 0.0f, 1.0f,
                                 0.0f, 0.0f, 1.0f,
                                 0.0f, 0.0f, 1.0f,
                                 0.0f, 0.0f, 1.0f};

        float s = (float) (1.0f / M_SQRT1_2);

        GLfloat smoothNormals[] = {-s, -s, 0.0f,
                                   -s, s, 0.0f,
                                   s, s, 0.0f,
                                   s, -s, 0.0f};

        GLushort indices[] = {0, 2, 1,  // first triangle (bottom left - top left - top right)
                              0, 3, 2}; // second triangle (bottom left - top right - bottom right)

        U32 vertexCount = 4;
        U32 vertexSize = 0;
        VertexElement positionElement(VertexElement::Semantic::POSITION, 3, GL_FLOAT, vertexSize);
        vertexSize += positionElement.getSizeInByte();
        VertexElement uvElement(VertexElement::Semantic::UV, 2, GL_FLOAT, vertexSize);
        vertexSize += uvElement.getSizeInByte();
        VertexElement flatNormalElement(VertexElement::Semantic::FLAT_NORMAL, 3, GL_FLOAT, vertexSize);
        vertexSize += flatNormalElement.getSizeInByte();
        VertexElement smoothNormalElement(VertexElement::Semantic::SMOOTH_NORMAL, 3, GL_FLOAT, vertexSize);
        vertexSize += smoothNormalElement.getSizeInByte();

        mVertexCount = vertexCount;
        mVertexSize = vertexSize;

        BYTE* data = new BYTE[vertexSize * vertexCount];
        Log::debug(TAG, "vertexSize=%d, vertexCount=%d", vertexSize, vertexCount);

        /////////////////////////////////////////////////////////////////////////
        // Fills data
        Log::debug(TAG, "%d, %d", positionElement.getSizeInByte(), positionElement.getOffset());
        for (U32 v = 0; v < vertexCount; ++v) {
            memcpy(&data[v * vertexSize + positionElement.getOffset()], &(positions[3*v]), positionElement.getSizeInByte());
            memcpy(&data[v * vertexSize + uvElement.getOffset()], &(uvs[2*v]), uvElement.getSizeInByte());
            memcpy(&data[v * vertexSize + flatNormalElement.getOffset()], &(flatNormals[3*v]), flatNormalElement.getSizeInByte());
            memcpy(&data[v * vertexSize + smoothNormalElement.getOffset()], &(smoothNormals[3*v]), smoothNormalElement.getSizeInByte());
        }

        /////////////////////////////////////////////////////////////////////////
        // Generate vertex buffer and upload data to GPU
        mVertexBuffer->generateBuffer(vertexSize, vertexSize * vertexCount);
        mVertexBuffer->writeData(0, vertexSize * vertexCount, data);
        delete[] data;

        /////////////////////////////////////////////////////////////////////////
        // Generate and fill index buffer
        mIndexBuffer->generateBuffer(6);
        mIndexBuffer->writeData(indices);

        Log::trace(TAG, "QuadFactory loaded");
    }


    //--------------------------------------------------
    void QuadFactory::unload() {
        Log::trace(TAG, "Unloading QuadFactory...");

        mVertexBuffer->wipe();
        mIndexBuffer->wipe();

        Log::trace(TAG, "QuadFactory unloaded");
    }


    //--------------------------------------------------
    void QuadFactory::wipe() {
        Log::trace(TAG, "Wiping QuadFactory...");

        mVertexBuffer->wipe();
        mIndexBuffer->wipe();

        Log::trace(TAG, "QuadFactory wiped");
    }



    //--------------------------------------------------
    void QuadFactory::refresh() {
        //wipe();
        init();
    }


    //--------------------------------------------------
    std::shared_ptr<Quad> QuadFactory::createQuad(F32 width, F32 height) {
        return std::make_shared<Quad>(width, height, mVertexSize, mVertexCount, mVertexBuffer, mIndexBuffer);
    }


    //--------------------------------------------------
    void QuadFactory::deleteQuad(const Quad &quad) {
        delete &quad;
    }

}
