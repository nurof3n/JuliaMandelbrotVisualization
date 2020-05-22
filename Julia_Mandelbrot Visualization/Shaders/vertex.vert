varying float rzMand;
varying vec2 juliaParam;

uniform vec2 Resolution;
uniform vec2 RedDotPos;
uniform bool IsExample;
uniform bool IsLMBPressed;

void main() {
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    // forward the vertex color
    gl_FrontColor = gl_Color;
    
    float zoom = 0.4;
    // update redDot as mouse position or as example position
    if (!IsExample) {
        juliaParam = (RedDotPos - Resolution * 0.5) / Resolution.y;
        // invert the mouse y coordinates
        juliaParam.y = -juliaParam.y;
        juliaParam /= zoom;
    }
    else
    juliaParam = RedDotPos;
    
    // check if juliaParam is in mandelbrot set (check its orbit)
    // only when LMB is pressed
    if (IsLMBPressed) {
        vec2 z = vec2(0.0);
        rzMand = 0.0;
        vec2 c = juliaParam;
        for(int i = 0; i < 50; i ++ )
        {
            if (rzMand > 4.0)
            break;
            
            // Z -> Z^2 + c
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
            rzMand = dot(z, z);
        }
    }
}