/*
MIT License

Copyright (c) 2026 Patoke

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdafx.h"
#include "Renderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

const float *Renderer::MatrixGet(int type)
{
    Context &c = getContext();
    const int depth = c.stackPos[type];
    return reinterpret_cast<const float *>(&c.matrixStacks[type][depth]);
}

void Renderer::MatrixMode(int type)
{
    Context &c = getContext();
    assert(type >= 0);
    assert(type < STACK_TYPES);
    c.stackType = type;
}

void Renderer::MatrixMult(float *mat)
{
	glm::mat4 matrix;
    std::memcpy(&matrix, mat, sizeof(matrix));
    MultWithStack(matrix);
}

void Renderer::MatrixOrthogonal(float left, float right, float bottom, float top, float zNear, float zFar)
{
    const glm::mat4 matrix = glm::orthoRH(left, right, bottom, top, zNear, zFar);
    MultWithStack(matrix);
}

void Renderer::MatrixPerspective(float fovy, float aspect, float zNear, float zFar)
{
	const float fovRadians = glm::radians(fovy);
    const glm::mat4 matrix = glm::perspective(fovRadians, aspect, zNear, zFar);
    MultWithStack(matrix);
}

void Renderer::MatrixPop()
{
    Context &c = getContext();

    assert(c.stackPos[c.stackType] > 0);

    const int mode = c.stackType;
    --c.stackPos[mode];
    c.matrixDirty[mode] = true;
}

void Renderer::MatrixPush()
{
    Context &c = getContext();
    
    assert(c.stackPos[c.stackType] < (STACK_SIZE - 1));

    const int mode = c.stackType;
    const int depth = c.stackPos[mode];
    c.matrixStacks[mode][depth + 1] = c.matrixStacks[mode][depth];
    ++c.stackPos[mode];
}

void Renderer::MatrixRotate(float angle, float x, float y, float z)
{
    const glm::vec3 axis = glm::vec3(x, y, z);
    const glm::mat4 matrix = glm::rotate(glm::mat4(), angle, axis);
    MultWithStack(matrix);
}

void Renderer::MatrixScale(float x, float y, float z)
{
	const glm::vec3 scale = glm::vec3(x,y,z);
    const glm::mat4 matrix = glm::scale(glm::mat4(), scale);
    MultWithStack(matrix);
}

void Renderer::MatrixSetIdentity()
{
    Context &c = getContext();
    const int mode = c.stackType;
    const int depth = c.stackPos[mode];
    c.matrixStacks[mode][depth] = glm::identity<glm::mat4>();	
    c.matrixDirty[mode] = true;
}

void Renderer::MatrixTranslate(float x, float y, float z)
{
	const glm::vec3 translate = glm::vec3(x,y,z);
    const glm::mat4 matrix = glm::translate(glm::mat4(), translate);
    MultWithStack(matrix);
}

void Renderer::MultWithStack(glm::mat4 matrix)
{
    Context &c = getContext();
    const int mode = c.stackType;
    const int depth = c.stackPos[mode];
    glm::mat4 &current = c.matrixStacks[mode][depth];
    current *= matrix;
    c.matrixDirty[mode] = true;
}

void Renderer::Set_matrixDirty()
{
    Context &c = getContext();
    const glm::mat4 identity = glm::identity<glm::mat4>();

    c.matrixStacks[MATRIX_MODE_MODELVIEW][0] = identity;
    c.matrixStacks[MATRIX_MODE_MODELVIEW_PROJECTION][0] = identity;
    c.matrixStacks[MATRIX_MODE_MODELVIEW_TEXTURE][0] = identity;
    c.matrixStacks[MATRIX_MODE_MODELVIEW_CBUFF][0] = identity;

    c.matrixDirty[MATRIX_MODE_MODELVIEW] = true;
    c.matrixDirty[MATRIX_MODE_MODELVIEW_PROJECTION] = true;
    c.matrixDirty[MATRIX_MODE_MODELVIEW_TEXTURE] = true;
    c.matrixDirty[MATRIX_MODE_MODELVIEW_CBUFF] = true;

    activeVertexType = -1;
    activePixelType = -1;
}
