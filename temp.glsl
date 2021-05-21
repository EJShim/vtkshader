2: #ifdef GL_ES
3: #ifdef GL_FRAGMENT_PRECISION_HIGH
4: precision highp float;
5: precision highp sampler2D;
6: precision highp sampler3D;
7: #else
8: precision mediump float;
9: precision mediump sampler2D;
10: precision mediump sampler3D;
11: #endif
12: #define texelFetchBuffer texelFetch
13: #define texture1D texture
14: #define texture2D texture
15: #define texture3D texture
16: #else // GL_ES
17: #define highp
18: #define mediump
19: #define lowp
20: #if __VERSION__ == 150
21: #define texelFetchBuffer texelFetch
22: #define texture1D texture
23: #define texture2D texture
24: #define texture3D texture
25: #endif
26: #endif // GL_ES
27: #define varying in
28: 
29: 
30: /*=========================================================================
31: 
32:   Program:   Visualization Toolkit
33:   Module:    vtkPolyDataFS.glsl
34: 
35:   Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
36:   All rights reserved.
37:   See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
38: 
39:      This software is distributed WITHOUT ANY WARRANTY; without even
40:      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
41:      PURPOSE.  See the above copyright notice for more information.
42: 
43: =========================================================================*/
44: // Template for the polydata mappers fragment shader
45: 
46: uniform int PrimitiveIDOffset;
47: 
48: 
49: 
50: 
51: // VC position of this fragment
52: in vec4 vertexVCVSOutput;
53: 
54: // Camera prop
55: uniform int cameraParallel;
56: 
57: 
58: // optional color passed in from the vertex shader, vertexColor
59: uniform float ambientIntensity; // the material ambient
60: uniform float diffuseIntensity; // the material diffuse
61: uniform float opacityUniform; // the fragment opacity
62: uniform vec3 ambientColorUniform; // ambient color
63: uniform vec3 diffuseColorUniform; // diffuse color
64: uniform float specularIntensity; // the material specular intensity
65: uniform vec3 specularColorUniform; // intensity weighted color
66: uniform float specularPowerUniform;
67: 
68: 
69: // optional surface normal declaration
70: uniform mat3 normalMatrix;
71: in vec3 normalVCVSOutput;
72: 
73: // extra lighting parameters
74: uniform vec3 lightColor0;
75:   uniform vec3 lightDirectionVC0; // normalized
76: 
77: 
78: // Texture maps
79: //VTK::TMap::Dec
80: 
81: // Texture coordinates
82: //VTK::TCoord::Dec
83: 
84: // picking support
85: //VTK::Picking::Dec
86: 
87: // Depth Peeling Support
88: //VTK::DepthPeeling::Dec
89: 
90: // clipping plane vars
91: //VTK::Clip::Dec
92: 
93: // the output of this shader
94: out vec4 fragOutput0;
95: 
96: 
97: // Apple Bug
98: //VTK::PrimID::Dec
99: 
100: // handle coincident offsets
101: //VTK::Coincident::Dec
102: 
103: // Value raster
104: //VTK::ValuePass::Dec
105: 
106: // surface with edges
107: //VTK::Edges::Dec
108: in vec3 pickedVCoutput;
109: void main()
110: {
111:   // VC position of this fragment. This should not branch/return/discard.
112:   vec4 vertexVC = vertexVCVSOutput;
113: 
114:   // Place any calls that require uniform flow (e.g. dFdx) here.
115:   //VTK::UniformFlow::Impl
116: 
117:   // Set gl_FragDepth here (gl_FragCoord.z by default)
118:   //VTK::Depth::Impl
119: 
120:   // Early depth peeling abort:
121:   //VTK::DepthPeeling::PreColor
122: 
123:   // Apple Bug
124:   //VTK::PrimID::Impl
125: 
126:   //VTK::Clip::Impl
127: 
128:   //VTK::ValuePass::Impl
129: 
130:     vec3 specularColor = specularIntensity * specularColorUniform;
131:   float specularPower = specularPowerUniform;
132:   vec3 ambientColor = ambientIntensity * ambientColorUniform;
133:   vec3 diffuseColor = diffuseIntensity * diffuseColorUniform;
134:   float opacity = opacityUniform;
135: 
136: 
137:   //VTK::Edges::Impl
138: 
139:   // Generate the normal if we are not passed in one
140:   vec3 normalVCVSOutput = normalize(normalVCVSOutput);
141:   if (gl_FrontFacing == false) { normalVCVSOutput = -normalVCVSOutput; }
142: 
143: 
144:     vec3 diffuse = vec3(0,0,0);
145:   vec3 specular = vec3(0,0,0);
146:   float df;
147:   float sf;
148:     df = max(0.0, dot(normalVCVSOutput, -lightDirectionVC0));
149:   diffuse += (df * lightColor0);
150:   sf = sign(df)*pow(max(0.0, dot( reflect(lightDirectionVC0, normalVCVSOutput), normalize(-vertexVC.xyz))), specularPower);
151:   specular += (sf * lightColor0);
152:   diffuse = diffuse * diffuseColor;
153:   specular = specular * specularColor;
154:   fragOutput0 = vec4(ambientColor + diffuse + specular, opacity);  //VTK::Light::Impl
155: 
156:   //VTK::TCoord::Impl
157: 
158:   // if(length(vertexPosition) < 0.5){
159:   //   diffuse = vertexPosition;
160:   // }
161:   
162:   
163:   vec3 pickedVC = pickedVCoutput;
164: d
165:   fragOutput0 = vec4(ambientColor + diffuse + specular ,opacity);
166: 
167: 
168:   //  fragOutput0 = vec4(vertexPosition, opacity);
169:   if (fragOutput0.a <= 0.0)
170:     {
171:     discard;
172:     }
173: 
174:   //VTK::DepthPeeling::Impl
175: 
176:   //VTK::Picking::Impl
177: 
178:   // handle coincident offsets
179:   //VTK::Coincident::Impl
180: }
