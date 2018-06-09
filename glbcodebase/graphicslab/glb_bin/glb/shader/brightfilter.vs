//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 24
// Brief: Get high light pass through shader
//-----------------------------------------------------------
#version 330

in vec3 glb_Pos;
in vec2 glb_TexCoord;
out vec2 vs_texcoord;
	 
void main() {
	gl_Position = vec4(glb_Pos, 1.0);
	vs_texcoord = glb_TexCoord;
}