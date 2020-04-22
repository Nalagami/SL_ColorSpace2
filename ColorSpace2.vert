uniform float factor;
void main()
{
	//í∏ì_ç¿ïWÇÃèoóÕ
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color * (1.0 - factor) + (vec4(1.0) - gl_Color) * factor;
}