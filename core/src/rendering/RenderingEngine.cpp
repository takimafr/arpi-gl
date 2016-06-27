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



#include <cstring>  // strlen

#include "rendering/RenderingEngine.hpp"

#include "utils/Log.hpp"
#include "utils/ExceptionHandler.hpp"
#include "utils/GLES2Logger.hpp"
#include "utils/GLUtils.hpp"
#include "utils/Utils.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"


constexpr auto TAG = "RenderingEngine";

//#define CLEAR_COLOR 0.2f, 0.3f, 0.3f, 1.0f
//#define CLEAR_COLOR 0.52941176470588235294f, 0.80784313725490196078f, 0.92156862745098039215f, 1.0f
#define CLEAR_COLOR 1.f, 1.f, 1.f, 0.0f

namespace dma {


    /* ================= PUBLIC ========================*/


    RenderingEngine::RenderingEngine(ResourceManager& resourceManager) :
            mHUDSystem(resourceManager),
            mSkyBox(nullptr),
            mV(NULL),
            mP(NULL),
            mAspectRatio(0.0f)
    {}



    RenderingEngine::~RenderingEngine() {
        //unload();
    }


    Status RenderingEngine::init() {

        // As openGL context must be created by the host, we have to check if
        // it is actually available & binded.
        bool hasOglContext = GLUtils::hasGlContext();
        if(!hasOglContext) {
            constexpr auto error = "no openGL context has been created. This function cannot work.";
            assert(!error);
            return throwError(TAG, ExceptionType::OPENGL, error);
        }
        assert(hasOglContext);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);
        glClearColor(CLEAR_COLOR);

        return STATUS_OK;
    }




    void RenderingEngine::unload() {
        Log::trace(TAG, "Unloading RenderingEngine...");

        mHUDSystem.unload();

        // force program to not be used anymore, to ensure proper deletion.
        if (GLUtils::hasGlContext()) {
            glUseProgram(0);
        }
        while (!mFrontToBack.empty()) {
            mFrontToBack.pop();
        }
        while (!mBackToFront.empty()) {
            mBackToFront.pop();
        }
        Log::trace(TAG, "RenderingEngine unloaded");
    }



    void RenderingEngine::setViewport(U32 width, U32 height) {
        assert(width > 0);
        assert(height > 0);
        mViewportWidth = width;
        mViewportHeight = height;
        mAspectRatio = (F32) width / (F32) height;
        glViewport(0, 0, width, height);
        mHUDSystem.setViewport(width, height);
    }



    void RenderingEngine::subscribe(RenderingPackage* package, bool back2front, float distanceFromCamera) {
        Entry e;
        e.renderingPackage = package;
        std::string sid = package->mMaterial->getSID(); //TODO remove, used for buildings/tracks demo
        if (sid == "tile") {
            mReverseTransparency.push(e);
        } else if (back2front) {
            e.distance = distanceFromCamera; //the farthest first
            mBackToFront.push(e);

        } else {
            e.distance = -distanceFromCamera; //the closest first
            mFrontToBack.push(e);
        }
    }



    void RenderingEngine::subscribe(const RenderingComponent* component, float distanceFromCamera) {
        for(RenderingPackage* rp : component->getRenderingPackages()) {
            subscribe(rp, rp->isBackToFront(), distanceFromCamera);
        }
    }



    void RenderingEngine::subscribeHUDElement(std::shared_ptr<HUDElement> hudElement) {
        mHUDSystem.addHUDElement(hudElement);
    }



    void RenderingEngine::drawFrame() {
        assert (mV != NULL && "mV not set before rendering starts!");
        assert (mP != NULL && "mP not set before rendering starts!");
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///////////////////////////////////////////
        // 1. Draw front to back
        //TODO Blend ???
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
        while (!mFrontToBack.empty()) {
            mDraw(mFrontToBack.top().renderingPackage, *mV, *mP);
            mFrontToBack.pop();
        }

        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);
        while (!mReverseTransparency.empty()) {
            mDraw(mReverseTransparency.top().renderingPackage, *mV, *mP);
            mReverseTransparency.pop();
        }
        ///////////////////////////////////////////
        // 2. Draw the skybox (early depth testing) if any
        if (mSkyBox) {
            mDrawSkyBox();
        }
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        ///////////////////////////////////////////
        // 3. Draw back to front
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        while (!mBackToFront.empty()) {
            mDraw(mBackToFront.top().renderingPackage, *mV, *mP);
            mBackToFront.pop();
        }
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        ///////////////////////////////////////////
        // 4. Draw the HUD
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (auto hudElem : mHUDSystem.getHUDElements()) {
            for (auto rp : hudElem->mEntity->getRenderingComponent()->getRenderingPackages()) {
                mDraw(rp, mHUDSystem.mV, mHUDSystem.mP);
            }
        }
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }



    /* ================= PRIVATE ========================*/


    void RenderingEngine::mDraw(RenderingPackage* package, const glm::mat4& V, const glm::mat4& P) {
        GLUtils::clearGlErrors();

        assert(package != NULL);

        std::shared_ptr<Mesh> mesh = package->mMesh;
        std::shared_ptr<Material> material = package->mMaterial;

        //Log::debug(TAG, "%s", glm::to_string(*(package->M)).c_str());

        const glm::mat4 MV = V * package->M;
        const glm::mat4 MVP = P * MV;
        const glm::mat3 N = glm::transpose(glm::inverse(glm::mat3(MV)));


        for (U8 i = 0; i < material->getPassCount(); ++i) {

            Pass& pass = material->getPass(i);

            std::shared_ptr<ShaderProgram> shaderProgram = pass.getShaderProgram();

            assert(shaderProgram != NULL && "ShaderProgram is NULL before calling glUseProgram");
            assert(shaderProgram->getHandle() != 0 && "ShaderProgram handle is 0 before calling glUseProgram");
            glUseProgram(shaderProgram->getHandle());

            glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer().getHandle());

            /////////////////////////////////////////////////////////////////////////
            // Setup rendering state according to the material functionalities.    //
            /////////////////////////////////////////////////////////////////////////

            GLint attr;
            U16 attribCount = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            // Setup cull mode
            switch (pass.getCullMode()) {
                case Pass::NONE:
                glDisable(GL_CULL_FACE);
                    break;
                case Pass::CullMode::FRONT:
                glEnable(GL_CULL_FACE);
                    glCullFace(GL_FRONT);
                    break;
                case Pass::BACK:
                glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    break;
                default:
                    Log::error(TAG, "Invalid cull mode");
                    assert(false);
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            // Setup depth writing
            if (pass.getDepthWriting()) {
                glDepthMask(GL_TRUE);
            } else {
                glDepthMask(GL_FALSE);
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            // Setup Transform

            // Uniforms
            glUniformMatrix4fv(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::MVP),
                               1, GL_FALSE, glm::value_ptr(MVP));
            // Attributes
            // Positions
            assert(mesh->hasVertexElement(VertexElement::Semantic::POSITION));
            attr = shaderProgram->getAttributeLocation(ShaderProgram::AttribSem::POS);
            mAttribIndices[attribCount++] = (GLuint) attr;
            glEnableVertexAttribArray((GLuint) attr);
            const VertexElement& positionElement = mesh->getVertexElement(VertexElement::Semantic::POSITION);
            glVertexAttribPointer((GLuint) attr,
                                  positionElement.getCount(),
                                  positionElement.getType(),
                                  GL_FALSE,
                                  mesh->getVertexSize(),
                                  ((GLvoid *) (U64) (positionElement.getOffset())));


            //////////////////////////////////////////////
            // Setup lighting computation
            if (pass.hasFunc(Pass::Func::LIGHTING_FLAT)
                || pass.hasFunc(Pass::Func::LIGHTING_SMOOTH)) {

                // Uniform
                glUniformMatrix4fv(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::MV),
                                   1, GL_FALSE, glm::value_ptr(MV));
                glUniformMatrix3fv(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::N),
                                   1, GL_FALSE, glm::value_ptr(N));
                // Attributes
                // Normal
                assert(mesh->hasVertexElement(VertexElement::Semantic::FLAT_NORMAL)
                       || mesh->hasVertexElement(VertexElement::Semantic::SMOOTH_NORMAL));
                attr = shaderProgram->getAttributeLocation(ShaderProgram::AttribSem::NORMAL);
                mAttribIndices[attribCount++] = (GLuint) attr;
                glEnableVertexAttribArray((GLuint) attr);
                const VertexElement &normalElement = pass.hasFunc(Pass::LIGHTING_FLAT) ?
                                                     mesh->getVertexElement(VertexElement::Semantic::FLAT_NORMAL) :
                                                     mesh->getVertexElement(VertexElement::Semantic::SMOOTH_NORMAL);
                glVertexAttribPointer((GLuint) attr,
                                      normalElement.getCount(),
                                      normalElement.getType(),
                                      GL_FALSE,
                                      mesh->getVertexSize(),
                                      ((GLvoid *) (U64) (normalElement.getOffset())));
            }

            //////////////////////////////////////////////
            // Setup diffuse map
            if (pass.hasFunc(Pass::Func::DIFFUSE_MAP)) {
                // active & bind texture
                glActiveTexture(GL_TEXTURE0);
                std::shared_ptr<Map> diffuseMap = pass.getDiffuseMap();
                glBindTexture(GL_TEXTURE_2D, diffuseMap->getHandle());
                glUniform1i(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::DM),
                            0); //0 means GL_TEXTURE0
                // Attributes
                // UV
                assert(mesh->hasVertexElement(VertexElement::Semantic::UV));
                attr = shaderProgram->getAttributeLocation(ShaderProgram::AttribSem::UV);
                mAttribIndices[attribCount++] = (GLuint) attr;
                glEnableVertexAttribArray((GLuint) attr);
                const VertexElement &uvElement = mesh->getVertexElement(VertexElement::Semantic::UV);
                glVertexAttribPointer((GLuint) attr,
                                      uvElement.getCount(),
                                      uvElement.getType(),
                                      GL_FALSE,
                                      mesh->getVertexSize(),
                                      ((GLvoid *) (U64) (uvElement.getOffset())));

                if (pass.hasFunc(Pass::Func::DIFFUSE_MAP_ACTIVATION)) {
                    glUniformMatrix4fv(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::MV),
                                       1, GL_FALSE, glm::value_ptr(MV));
                    glUniform1i(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::DM_ACTIVATION),
                                pass.isDiffuseMapEnabled());
                }
            }

            //////////////////////////////////////////////
            // Setup scaling
            if (pass.hasFunc(Pass::Func::SCALING)) {
                // Uniform
                glUniformMatrix3fv(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::N),
                                   1, GL_FALSE, glm::value_ptr(N));
                // Attributes
                // Normal
                assert(mesh->hasVertexElement(VertexElement::Semantic::SMOOTH_NORMAL));
                attr = shaderProgram->getAttributeLocation(ShaderProgram::AttribSem::NORMAL);
                mAttribIndices[attribCount++] = (GLuint) attr;
                glEnableVertexAttribArray((GLuint) attr);
                const VertexElement &normalElement = mesh->getVertexElement(VertexElement::Semantic::SMOOTH_NORMAL);
                glVertexAttribPointer((GLuint) attr,
                                      normalElement.getCount(),
                                      normalElement.getType(),
                                      GL_FALSE,
                                      mesh->getVertexSize(),
                                      ((GLvoid *) (U64) (normalElement.getOffset())));
            }

            //////////////////////////////////////////////
            // Setup diffuse color
            if (pass.hasFunc(Pass::Func::DIFFUSE_COLOR)) {
                // Uniforms
                const glm::vec3& diffuseColor = pass.getDiffuseColor();
                glUniform3f(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::DIFFUSE_COLOR),
                            diffuseColor.r, diffuseColor.g, diffuseColor.b);
            }


            //////////////////////////////////////////////
            // Setup lights
            if (shaderProgram->hasUniform(ShaderProgram::UniformSem::LIGHT0_POSITION)) {
                glm::vec4 lightPos = V * glm::vec4(mLight.position, 1.0f);
                lightPos.w = 0.0f;
                glUniform4f(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::LIGHT0_POSITION), lightPos.x, lightPos.y, lightPos.z, lightPos.w);
                glUniform3f(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::LIGHT0_AMBIENT), mLight.ambient.r, mLight.ambient.g, mLight.ambient.b);
                glUniform3f(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::LIGHT0_DIFFUSE), mLight.diffuse.r, mLight.diffuse.g, mLight.diffuse.b);
                glUniform3f(shaderProgram->getUniformLocation(ShaderProgram::UniformSem::LIGHT0_SPECULAR), mLight.specular.r, mLight.specular.g, mLight.specular.b);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer().getHandle());

            glDrawElements(GL_TRIANGLES,
                           mesh->getIndexBuffer().getElementCount(),
                           GL_UNSIGNED_SHORT, 0);


            //////////////////////////////////////////////
            // Disable vertex attrib arrays
            for (U16 j = 0; j < attribCount; ++j) {
                glDisableVertexAttribArray(mAttribIndices[j]);
            }
        }
    }



    void RenderingEngine::mDrawSkyBox() {
        glm::mat4 MVP = *mP * glm::mat4(glm::mat3(*mV)); //remove translation components

        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content

        std::shared_ptr<ShaderProgram> program = mSkyBox->getShaderProgram();

        glUseProgram(program->getHandle());

        glBindBuffer(GL_ARRAY_BUFFER, mSkyBox->getVertexBuffer().getHandle());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSkyBox->getIndexBuffer().getHandle());

        glActiveTexture(GL_TEXTURE0);

        // Uniforms
        glUniformMatrix4fv(program->getUniformLocation(ShaderProgram::UniformSem::MVP),
                           1, GL_FALSE, glm::value_ptr(MVP));
        glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyBox->getCubeMap()->getHandle());
        glUniform1i(program->getUniformLocation(ShaderProgram::UniformSem::CUBE_MAP), 0); //0 means GL_TEXTURE0
        // Attributes
        // UV
        GLuint attr = (GLuint) program->getAttributeLocation(ShaderProgram::AttribSem::POS);
        glEnableVertexAttribArray((GLuint) attr);
        glVertexAttribPointer((GLuint) attr,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              12,
                              ((GLvoid *) (U64) (0)));

        glDrawElements(GL_TRIANGLES,
                       mSkyBox->getIndexBuffer().getElementCount(),
                       GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(attr);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS); // Set depth function back to default
    }
}
