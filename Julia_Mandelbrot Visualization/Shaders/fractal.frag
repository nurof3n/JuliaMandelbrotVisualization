precision highp float;

uniform vec2 Resolution;
uniform vec2 MousePos;
uniform bool IsLMBPressed;

const vec3 colorMix1 = vec3(0.3, 0.3, 0.7);
const vec3 colorMix2 = vec3(0.0, 0.0, 0.0);

vec3 hsv2rgb(float c)
{
    vec3 rgb = clamp(abs(mod(c * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return rgb;
}

void main()
{
    // normalize coordinates, such that y axis goes from -1 to 1 (0, 0) is the center of the screen
    vec2 uv = (gl_FragCoord.xy - Resolution * 0.5) / Resolution.y;
    vec2 redDot = (MousePos - Resolution * 0.5) / Resolution.y;
    // invert the mouse y coordinate;
    redDot.y = -redDot.y;
    
    // set and apply zoom
    vec2 zoompoint = vec2(0.0);
    float zoom = 0.4;
    uv /= zoom;
    redDot /= zoom;
    uv += zoompoint;
    redDot += zoompoint;
    
    // Julia Set
    vec2 juliaParam = redDot;
    vec2 z = uv;
    vec2 c = juliaParam;
    vec2 dz = vec2(0.0);
    float rz = 0.0;
    float rdz = 0.0;
    int nri = 0;
    
    // Julia Set iteration
    for(int i = 0; i < 500; i ++ )
    {
        if (rz > 1024.0)
        break;
        
        // Z' -> 2*Z*Z' + 1
        dz = 2.0 * vec2(z.x * dz.x - z.y * dz.y, z.x * dz.y + z.y * dz.x) + vec2(1.0, 0.0);
        // Z -> Z^2 + c
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        nri = i;
        rz = dot(z, z);
    }
    
    // (DISTANCE ESTIMATOR) https://iquilezles.org/www/articles/distancefractals/distancefractals.htm
    rdz = dot(dz, dz);
    float d = 0.5 * sqrt(rz / rdz) * log(rz);
    d = clamp(pow(6.0 * d / zoom, 0.2), 0.0, 1.0);
    
    // (COLORING ALGORITHMS) https://www.mi.sanu.ac.rs/vismath/javier/b3.htm
    vec3 color;
    if (rz < 4.0)
    color = vec3(0.0);
    else {
        float nic = float(nri) + 1.0 - log2(log2(rz)); //normalized iteration count: https://iquilezles.org/www/articles/mset_smooth/mset_smooth.htm
        //color = hsv2rgb((nic * .015));								//multicolored
        color = 0.5 + 0.5 * cos(3.0 + nic * 0.2 + vec3(0.0, 0.6, 1.0)); //like on wiki?
        //color = mix(colorf, colorf2, fract(float(nri) * .02));		//(discrete) escape time coloring: Base the color on the number of iterations
        //color = vec3(d);
    }
    
    // Mandelbrot Set (shown when left mouse button is pressed)
    vec3 color2;
    if (IsLMBPressed) {
        // check if juliaParam is in mandelbrot set (check its orbit)
        // this is an invariant of the frame, so maybe move this out??
        z = vec2(0.0);
        rz = 0.0;
        c = juliaParam;
        for(int i = 0; i < 100; i ++ )
        {
            if (rz > 4.0)
            break;
            
            // Z -> Z^2 + c
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
            rz = dot(z, z);
        }
        
        // set the color of the Mandelbrot Set, according to the inclusion of the juliaParam in it
        vec3 incolor;
        if (rz < 4.0)
        incolor = vec3(0.3, 1.0, 0.3); // is in
        else
        incolor = vec3(1.0, 0.3, 0.3); // is out
        
        // draw the Mandelbrot Set
        z = vec2(0.0);
        rz = 0.0;
        c = uv;
        for(int i = 0; i < 100; i ++ )
        {
            if (rz > 128.0)
            break;
            
            // Z -> Z^2 + c
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
            rz = dot(z, z);
        }
        
        if (rz < 4.0)
        color2 = incolor;
        else
        color2 = color;
        
        color = mix(color, color2, 0.2);
    }
    
    color = mix(color, vec3(1.0, 0.0, 0.0), smoothstep(5.0 / zoom / Resolution.y, 0.0, length(uv - juliaParam)));
    
    gl_FragColor = vec4(color, 1.0);
}