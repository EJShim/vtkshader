//VTK::System::Dec

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPolyDataVS.glsl

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

attribute vec4 vertexMC;

// frag position in VC
//VTK::PositionVC::Dec

// optional normal declaration
//VTK::Normal::Dec

// extra lighting parameters
//VTK::Light::Dec

// Texture coordinates
//VTK::TCoord::Dec

// material property values
//VTK::Color::Dec

// clipping plane vars
//VTK::Clip::Dec

// camera and actor matrix values
//VTK::Camera::Dec

// Apple Bug
//VTK::PrimID::Dec

// picking support
//VTK::Picking::Dec

uniform vec3 pickedUniform;
out float intensity;
out vec3 normalMCOutput;
void main()
{
  //VTK::Color::Impl

  //VTK::Normal::Impl

  //VTK::TCoord::Impl

  //VTK::Clip::Impl

  //VTK::PrimID::Impl

  //VTK::PositionVC::Impl

  //VTK::Light::Impl

  //VTK::Picking::Impl

  
    
  intensity = length( vertexMC.xyz - pickedUniform);

  vec4 fun = vec4(0.0, 0.0, 0.0, 0.0);
  fun = max(0.0, 15 - intensity) *  vec4(normalMC.xyz, 0.0);
  
  normalMCOutput = normalMatrix * normalMC;

  vec4 tmpPos =  MCDCMatrix  * vec4(vertexMC.xyz, 1.0);
  gl_Position = tmpPos + fun;
}
