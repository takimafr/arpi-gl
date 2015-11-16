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
 * gl2Logger.hpp
 *
 *  Created on: 1 juin 2015
 *      Author: excilys
 */

#ifndef _GLES2LOGGER_HPP_
#define _GLES2LOGGER_HPP_ "GLES2Logger"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cassert>
#include <sstream>

#include "utils/Log.hpp"
#include "utils/Utils.hpp"
#include "utils/GLUtils.hpp"

#ifdef DEBUG

#define __GL_HAS_ERROR(name) dma::GLUtils::hasGlError(std::string(__FILE__) + ":" + to_string(__LINE__), name)



#define glActiveTexture(texture) glActiveTexture(texture); __GL_HAS_ERROR("glActiveTexture")
#define glAttachShader(program, shader) glAttachShader(program, shader); __GL_HAS_ERROR("glAttachShader")
#define glBindAttribLocation(program, index, name) glBindAttribLocation(program, index, name); __GL_HAS_ERROR("glBindAttribLocation")
#define glBindBuffer(target, buffer) glBindBuffer(target, buffer); __GL_HAS_ERROR("glBindBuffer")
#define glBindFramebuffer(target, framebuffer) glBindFramebuffer(target, framebuffer); __GL_HAS_ERROR("glBindFramebuffer")
#define glBindRenderbuffer(target, renderbuffer) glBindRenderbuffer(target, renderbuffer); __GL_HAS_ERROR("glBindRenderbuffer")
#define glBindTexture(target, texture) glBindTexture(target, texture); __GL_HAS_ERROR("glBindTexture")
#define glBlendColor(red, green, blue, alpha) glBlendColor(red, green, blue, alpha); __GL_HAS_ERROR("glBlendColor")
#define glBlendEquation(mode) glBlendEquation(mode); __GL_HAS_ERROR("glBlendEquation")
#define glBlendEquationSeparate(modeRGB, modeAlpha) glBlendEquationSeparate(modeRGB, modeAlpha); __GL_HAS_ERROR("glBlendEquationSeparate")
#define glBlendFunc(sfactor, dfactor) glBlendFunc(sfactor, dfactor); __GL_HAS_ERROR("glBlendFunc")
#define glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha) glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha); __GL_HAS_ERROR("glBlendFuncSeparate")
#define glBufferData(target, size, data, usage) glBufferData(target, size, data, usage); __GL_HAS_ERROR("glBufferData")
#define glBufferSubData(target, offset, size, data) glBufferSubData(target, offset, size, data); __GL_HAS_ERROR("glBufferSubData")
#define glCheckFramebufferStatus(target) glCheckFramebufferStatus(target); __GL_HAS_ERROR("glCheckFramebufferStatus")
#define glClear(mask) glClear(mask); __GL_HAS_ERROR("glClear")
//#define glClearColor(red, green, blue, alpha) glClearColor(red, green, blue, alpha); __GL_HAS_ERROR("GLFUNC")
#define glClearDepthf(d) glClearDepthf(d); __GL_HAS_ERROR("glClearDepthf")
#define glClearStencil(s) glClearStencil(s); __GL_HAS_ERROR("glClearStencil")
#define glColorMask(red, green, blue, alpha) glColorMask(red, green, blue, alpha); __GL_HAS_ERROR("glColorMask")
#define glCompileShader(shader) glCompileShader(shader); __GL_HAS_ERROR("glCompileShader")
#define glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data) glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data); __GL_HAS_ERROR("glCompressedTexImage2D")
#define glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data) glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data); __GL_HAS_ERROR("glCompressedTexSubImage2D")
#define glCopyTexImage2D(target, level, internalformat, x, y, width, height, border) glCopyTexImage2D(target, level, internalformat, x, y, width, height, border); __GL_HAS_ERROR("glCopyTexImage2D")
#define glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height) glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height); __GL_HAS_ERROR("glCopyTexSubImage2D")
#define glCreateProgram() glCreateProgram(); __GL_HAS_ERROR("glCreateProgram")
#define glCreateShader(type) glCreateShader(type); __GL_HAS_ERROR("glCreateShader")
#define glCullFace(mode) glCullFace(mode); __GL_HAS_ERROR("glCullFace")
#define glDeleteBuffers(n, buffers) glDeleteBuffers(n, buffers); __GL_HAS_ERROR("glDeleteBuffers")
#define glDeleteFramebuffers(n, framebuffers) glDeleteFramebuffers(n, framebuffers); __GL_HAS_ERROR("glDeleteFramebuffers")
#define glDeleteProgram(program) glDeleteProgram(program); __GL_HAS_ERROR("glDeleteProgram")
#define glDeleteRenderbuffers(n, renderbuffers) glDeleteRenderbuffers(n, renderbuffers); __GL_HAS_ERROR("glDeleteRenderbuffers")
#define glDeleteShader(shader) glDeleteShader(shader); __GL_HAS_ERROR("glDeleteShader")
#define glDeleteTextures(n, textures) glDeleteTextures(n, textures); __GL_HAS_ERROR("glDeleteTextures")
#define glDepthFunc(func) glDepthFunc(func); __GL_HAS_ERROR("glDepthFunc")
#define glDepthMask(flag) glDepthMask(flag); __GL_HAS_ERROR("glDepthMask")
#define glDepthRangef(n, f) glDepthRangef(n, f); __GL_HAS_ERROR("glDepthRangef")
#define glDetachShader(program, shader) glDetachShader(program, shader); __GL_HAS_ERROR("glDetachShader")
#define glDisable(cap) glDisable(cap); __GL_HAS_ERROR("glDisable")
#define glDisableVertexAttribArray(index) glDisableVertexAttribArray(index); __GL_HAS_ERROR("glDisableVertexAttribArray")
#define glDrawArrays(mode, first, count) glDrawArrays(mode, first, count); __GL_HAS_ERROR("glDrawArrays")
#define glDrawElements(mode, count, type, indices) glDrawElements(mode, count, type, indices); __GL_HAS_ERROR("glDrawElements")
#define glEnable(cap) glEnable(cap); __GL_HAS_ERROR("glEnable")
#define glEnableVertexAttribArray(index) glEnableVertexAttribArray(index); __GL_HAS_ERROR("glEnableVertexAttribArray")
#define glFinish() glFinish(); __GL_HAS_ERROR("glFinish")
#define glFlush() glFlush(); __GL_HAS_ERROR("glFlush")


//#define glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); __GL_HAS_ERROR("GLFUNC")
//#define glFramebufferTexture2D(target, attachment, textarget, texture, level); __GL_HAS_ERROR("GLFUNC")
#define glFrontFace(mode) glFrontFace(mode); __GL_HAS_ERROR("glFrontFace")
#define glGenBuffers(n,buffers) glGenBuffers(n,buffers); __GL_HAS_ERROR("glGenBuffers")
#define glGenerateMipmap(target) glGenerateMipmap(target); __GL_HAS_ERROR("glGenerateMipmap")

#define glGenFramebuffers(n,framebuffers) glGenFramebuffers(n,framebuffers); __GL_HAS_ERROR("glGenFramebuffers")
#define glGenRenderbuffers(n,renderbuffers) glGenRenderbuffers(n,renderbuffers); __GL_HAS_ERROR("glGenRenderbuffers")
#define glGenTextures(n,textures) glGenTextures(n,textures); __GL_HAS_ERROR("glGenTextures")
#define glGetActiveAttrib(program, index, bufSize,length,size,type, name) glGetActiveAttrib(program, index, bufSize,length,size,type, name); __GL_HAS_ERROR("glGetActiveAttrib")
#define glGetActiveUniform(program, index, bufSize,length,size,type, name) glGetActiveUniform(program, index, bufSize,length,size,type, name); __GL_HAS_ERROR("glGetActiveUniform")
#define glGetAttachedShaders(program, maxCount,count,shaders) glGetAttachedShaders(program, maxCount,count,shaders); __GL_HAS_ERROR("glGetAttachedShaders")
#define glGetAttribLocation(program, name) glGetAttribLocation(program, name); __GL_HAS_ERROR("glGetAttribLocation")
#define glGetBooleanv(pname,data) glGetBooleanv(pname,data); __GL_HAS_ERROR("glGetBooleanv")
#define glGetBufferParameteriv(target, pname,params) glGetBufferParameteriv(target, pname,params); __GL_HAS_ERROR("glGetBufferParameteriv")
#define glGetFloatv(pname, data) glGetFloatv(pname, data); __GL_HAS_ERROR("glGetFloatv")
#define glGetFramebufferAttachmentParameteriv(target, attachment, pname,params) glGetFramebufferAttachmentParameteriv(target, attachment, pname,params); __GL_HAS_ERROR("glGetFramebufferAttachmentParameteriv")
#define glGetIntegerv(pname,data) glGetIntegerv(pname,data); __GL_HAS_ERROR("glGetIntegerv")
#define glGetProgramiv(program, pname,params) glGetProgramiv(program, pname,params); __GL_HAS_ERROR("glGetProgramiv")
#define glGetProgramInfoLog(program, bufSize,length, infoLog) glGetProgramInfoLog(program, bufSize,length, infoLog); __GL_HAS_ERROR("glGetProgramInfoLog")
#define glGetRenderbufferParameteriv(target, pname,params) glGetRenderbufferParameteriv(target, pname,params); __GL_HAS_ERROR("glGetRenderbufferParameteriv")

#define glGetShaderiv(shader, pname,params) glGetShaderiv(shader, pname,params); __GL_HAS_ERROR("glGetShaderiv")
#define glGetShaderInfoLog(shader, bufSize,length, infoLog) glGetShaderInfoLog(shader, bufSize,length, infoLog); __GL_HAS_ERROR("glGetShaderInfoLog")

#define glGetShaderPrecisionFormat(shadertype, precisiontype,range,precision) glGetShaderPrecisionFormat(shadertype, precisiontype,range,precision); __GL_HAS_ERROR("glGetShaderPrecisionFormat")
#define glGetShaderSource(shader, bufSize,length, source) glGetShaderSource(shader, bufSize,length, source); __GL_HAS_ERROR("glGetShaderSource")
#define glGetString(name) glGetString(name); __GL_HAS_ERROR("glGetString")
//#define glGetTexParameterfv(target, pname, params); __GL_HAS_ERROR("GLFUNC")
//#define glGetTexParameteriv(target, pname,params); __GL_HAS_ERROR("GLFUNC")
//#define glGetUniformfv(program, location, params); __GL_HAS_ERROR("GLFUNC")
//#define glGetUniformiv(program, location,params); __GL_HAS_ERROR("GLFUNC")
//#define glGetUniformLocation(program, name); __GL_HAS_ERROR("GLFUNC")
//#define glGetVertexAttribfv(index, pname, params); __GL_HAS_ERROR("GLFUNC")
//#define glGetVertexAttribiv(index, pname,params); __GL_HAS_ERROR("GLFUNC")
//#define glGetVertexAttribPointerv(index, pname,pointer); __GL_HAS_ERROR("GLFUNC")
//#define glHint(target, mode); __GL_HAS_ERROR("GLFUNC")

//#define glIsBuffer(buffer) glIsBuffer(buffer); __GL_HAS_ERROR("glIsBuffer")
#define glIsEnabled(cap) glIsEnabled(cap); __GL_HAS_ERROR("glIsEnabled")
#define glIsFramebuffer(framebuffer) glIsFramebuffer(framebuffer); __GL_HAS_ERROR("glIsFramebuffer")
//NO need for this. #define glIsProgram(program) glIsProgram(program); __GL_HAS_ERROR("glIsProgram")
#define glIsRenderbuffer(renderbuffer) glIsRenderbuffer(renderbuffe;AS_ERROR("glIsRenderbuffer")
#define glIsShader(shader) glIsShader(shader); __GL_HAS_ERROR("glIsShader")
//#define glIsTexture(texture) glIsTexture(texture); __GL_HAS_ERROR("glIsTexture")
#define glLineWidth(width) glLineWidth(width); __GL_HAS_ERROR("glLineWidth")
#define glLinkProgram(program) glLinkProgram(program); __GL_HAS_ERROR("glLinkProgram")
//#define glPixelStorei(pname, param); __GL_HAS_ERROR("GLFUNC")
//#define glPolygonOffset(factor, units); __GL_HAS_ERROR("GLFUNC")
//#define glReadPixels(x, y, width, height, format, type, pixels); __GL_HAS_ERROR("GLFUNC")
//#define glReleaseShaderCompiler(); __GL_HAS_ERROR("GLFUNC")
//#define glRenderbufferStorage(target, internalformat, width, height); __GL_HAS_ERROR("GLFUNC")
//#define glSampleCoverage(value, invert); __GL_HAS_ERROR("GLFUNC")
//#define glScissor(x, y, width, height); __GL_HAS_ERROR("GLFUNC")
//#define glShaderBinary(count, shaders, binaryformat, binary, length); __GL_HAS_ERROR("GLFUNC")

#define glShaderSource(shader, count, string, length) glShaderSource(shader, count, string, length); __GL_HAS_ERROR("glShaderSource")
#define glStencilFunc(func, ref, mask) glStencilFunc(func, ref, mask); __GL_HAS_ERROR("glStencilFunc")
#define glStencilFuncSeparate(face, func, ref, mask) glStencilFuncSeparate(face, func, ref, mask); __GL_HAS_ERROR("glStencilFuncSeparate")
#define glStencilMask(mask) glStencilMask(mask); __GL_HAS_ERROR("glStencilMask")
#define glStencilMaskSeparate(face, mask) glStencilMaskSeparate(face, mask); __GL_HAS_ERROR("glStencilMaskSeparate")
#define glStencilOp(fail, zfail, zpass) glStencilOp(fail, zfail, zpass); __GL_HAS_ERROR("glStencilOp")
#define glStencilOpSeparate(face, sfail, dpfail, dppass) glStencilOpSeparate(face, sfail, dpfail, dppass); __GL_HAS_ERROR("glStencilOpSeparate")

#define glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels) glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels); __GL_HAS_ERROR("glTexImage2D")
#define glTexParameterf(target, pname, param) glTexParameterf(target, pname, param); __GL_HAS_ERROR("glTexParameterf")
#define glTexParameterfv(target, pname, params) glTexParameterfv(target, pname, params); __GL_HAS_ERROR("glTexParameterfv")
#define glTexParameteri(target, pname, param) glTexParameteri(target, pname, param); __GL_HAS_ERROR("glTexParameteri")
#define glTexParameteriv(target, pname, params) glTexParameteriv(target, pname, params); __GL_HAS_ERROR("glTexParameteriv")
#define glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels)  glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels); __GL_HAS_ERROR("glTexSubImage2D")

#define glUniform1f(location, v0) glUniform1f(location, v0); __GL_HAS_ERROR("glUniform1f")
#define glUniform1fv(location, count, value) glUniform1fv(location, count, value); __GL_HAS_ERROR("glUniform1fv")
#define glUniform1i(location, v0) glUniform1i(location, v0); __GL_HAS_ERROR("glUniform1i")
#define glUniform1iv(location, count, value) glUniform1iv(location, count, value); __GL_HAS_ERROR("glUniform1iv")
#define glUniform2f(location, v0, v1) glUniform2f(location, v0, v1); __GL_HAS_ERROR("glUniform2f")
#define glUniform2fv(location, count, value) glUniform2fv(location, count, value); __GL_HAS_ERROR("glUniform2fv")
#define glUniform2i(location, v0, v1) glUniform2i(location, v0, v1); __GL_HAS_ERROR("glUniform2i")
#define glUniform2iv(location, count, value) glUniform2iv(location, count, value); __GL_HAS_ERROR("glUniform2iv")
#define glUniform3f(location, v0, v1, v2) glUniform3f(location, v0, v1, v2); __GL_HAS_ERROR("glUniform3f")
#define glUniform3fv(location, count, value) glUniform3fv(location, count, value); __GL_HAS_ERROR("glUniform3fv")
#define glUniform3i(location, v0, v1, v2) glUniform3i(location, v0, v1, v2); __GL_HAS_ERROR("glUniform3i")
#define glUniform3iv(location, count, value) glUniform3iv(location, count, value); __GL_HAS_ERROR("glUniform3iv")
#define glUniform4f(location, v0, v1, v2, v3) glUniform4f(location, v0, v1, v2, v3); __GL_HAS_ERROR("glUniform4f")
#define glUniform4fv(location, count, value) glUniform4fv(location, count, value); __GL_HAS_ERROR("glUniform4fv")
#define glUniform4i(location, v0, v1, v2, v3) glUniform4i(location, v0, v1, v2, v3); __GL_HAS_ERROR("glUniform4i")
#define glUniform4iv(location, count, value) glUniform4iv(location, count, value); __GL_HAS_ERROR("glUniform4iv")
#define glUniformMatrix2fv(location, count, transpose, value) glUniformMatrix2fv(location, count, transpose, value); __GL_HAS_ERROR("glUniformMatrix2fv")
#define glUniformMatrix3fv(location, count, transpose, value) glUniformMatrix3fv(location, count, transpose, value); __GL_HAS_ERROR("glUniformMatrix3fv")
#define glUniformMatrix4fv(location, count, transpose, value) glUniformMatrix4fv(location, count, transpose, value); __GL_HAS_ERROR("glUniformMatrix4fv")
#define glUseProgram(program) glUseProgram(program); __GL_HAS_ERROR("glUseProgram")
#define glValidateProgram(program) glValidateProgram(program); __GL_HAS_ERROR("glValidateProgram")

//#define glVertexAttrib1f(index, x); __GL_HAS_ERROR("GLFUNC")
//#define glVertexAttrib1fv(index, v); __GL_HAS_ERROR("GLFUNC")
//#define glVertexAttrib2f(index, x, y); __GL_HAS_ERROR("GLFUNC")
//#define glVertexAttrib2fv(index, v); __GL_HAS_ERROR("GLFUNC")
//#define glVertexAttrib3f(index, x, y, z); __GL_HAS_ERROR("GLFUNC")
//#define glVertexAttrib3fv(index, v); __GL_HAS_ERROR("GLFUNC")
//#define glVertexAttrib4f(index, x, y, z, w); __GL_HAS_ERROR("GLFUNC")
//#define glVertexAttrib4fv(index, v); __GL_HAS_ERROR("GLFUNC")
#define glVertexAttribPointer(index, size, type, normalized, stride, pointer) glVertexAttribPointer(index, size, type, normalized, stride, pointer); __GL_HAS_ERROR("glVertexAttribPointer")
#define glViewport(x, y, width, height) glViewport(x, y, width, height); __GL_HAS_ERROR("glViewport")

#endif // DEBUG


#endif /* _GLES2LOGGER_HPP_ */
