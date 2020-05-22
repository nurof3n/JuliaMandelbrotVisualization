precision highp float;

uniform vec2 Resolution;
uniform vec2 RedDotPos;
uniform bool IsLMBPressed;
uniform bool IsExample;
uniform bool UBER;
uniform int ColorScheme;

varying float rzMand;
varying vec2 juliaParam;

// colors used to blend on one color scheme
const vec3 colorMix1 = vec3(0.2824, 0.2824, 0.9686);
const vec3 colorMix2 = vec3(0.0, 0.0, 0.0);

// hue-saturation-value to rgb conversion
vec3 hsv2rgb(float c)
{
    vec3 rgb = clamp(abs(mod(c * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return rgb;
}

void main()
{
    // normalize coordinates, such that y axis goes from -1 to 1, (0, 0) is the center of the screen, keep aspect ratio
    vec2 uv = (gl_FragCoord.xy - Resolution * 0.5) / Resolution.y;
    
    // set and apply zoom
    float zoom = 0.4;
    uv /= zoom;
    
    // Julia Set
    vec2 z = uv;
    vec2 c = juliaParam;
    vec2 dz = vec2(0.0);
    float rz = 0.0;
    float rdz = 0.0;
    int juliaIter = 300;
    
    if (UBER)
    juliaIter = 2000;
    
    // Julia Set iteration
    int i;
    for(i = 0; i < juliaIter; i ++ )
    {
        if (rz > 1024.0)
        break;
        
        if (ColorScheme == 3)
        // Z' -> 2*Z*Z' + 1
        dz = 2.0 * vec2(z.x * dz.x - z.y * dz.y, z.x * dz.y + z.y * dz.x) + vec2(1.0, 0.0);
        // Z -> Z^2 + c
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        rz = dot(z, z);
    }
    
    // (COLORING ALGORITHMS) https://www.mi.sanu.ac.rs/vismath/javier/b3.htm
    vec3 color;
    if (rz < 4.0)
    color = vec3(0.0);
    else {
        // normalized iteration count: https://iquilezles.org/www/articles/mset_smooth/mset_smooth.htm
        float nic = float(i) + 1.0 - log2(log2(rz));
        
        if (ColorScheme == 1)
        // multicolored
        color = (hsv2rgb(nic * 0.015));
        else if (ColorScheme == 2)
        // (discrete) escape time coloring: Base the color on the number of iterations
        color = mix(colorMix1, colorMix2, fract(float(i) * 0.02));
        else if (ColorScheme == 3) {
            // (DISTANCE ESTIMATOR) https://iquilezles.org/www/articles/distancefractals/distancefractals.htm
            rdz = dot(dz, dz);
            float distJulia = 0.5 * sqrt(rz / rdz) * log(rz);
            distJulia = clamp(pow(6.0 * distJulia / zoom, 0.2), 0.0, 1.0);
            // greyscale, distance estimation
            color = vec3(distJulia);
        }
        else
        // default color scheme, like on wikipedia
        color = 0.5 + 0.5 * cos(3.0 + nic * 0.2 + vec3(0.0, 0.6, 1.0));
    }
    
    // Mandelbrot Set (shown when left mouse button is pressed)
    if (IsLMBPressed) {
        // set the color of the Mandelbrot Set, according to the inclusion of the juliaParam in it
        vec3 incolor;
        if (rzMand < 4.0)
        incolor = vec3(0.3, 1.0, 0.3); // is in
        else if (ColorScheme == 1)// adjust red color for the multicolored one
        incolor = vec3(0.4, 0.0667, 0.0039); // is out
        else
        incolor = vec3(0.9059, 0.1843, 0.0588); // is out
        
        // draw the Mandelbrot Set
        z = vec2(0.0);
        rz = 0.0;
        c = uv;
        for(i = 0; i < 50; i ++ )
        {
            if (rz > 16.0)
            break;
            
            // Z -> Z^2 + c
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
            rz = dot(z, z);
        }
        
        vec3 color2;
        if (rz < 4.0)
        color2 = incolor;
        else {
            float nic = float(i) + 1.0 - log2(log2(rz));
            color2 = mix(color, vec3(0.6314, 0.8549, 0.0275), smoothstep(0.0, 50.0, nic));
        }
        
        color = mix(color, color2, 0.3);
    }
    
    // red dot
    color = mix(color, vec3(1.0, 0.0, 0.0), smoothstep(5.0 / zoom / Resolution.y, 0.0, length(uv - juliaParam)));
    
    gl_FragColor = vec4(color, 1.0);
}