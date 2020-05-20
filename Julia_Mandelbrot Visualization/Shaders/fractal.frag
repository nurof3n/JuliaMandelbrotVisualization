precision highp float;

uniform vec2 Resolution;
uniform vec2 MousePos;
uniform bool IsLMBPressed;
uniform int colorScheme;

// colors used to blend on one color scheme
const vec3 colorMix1 = vec3(0.3, 0.3, 0.7);
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
    vec2 redDot = (MousePos - Resolution * 0.5) / Resolution.y;
    // invert the mouse y coordinates
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
    float distJulia = 0.5 * sqrt(rz / rdz) * log(rz);
    distJulia = clamp(pow(6.0 * distJulia / zoom, 0.2), 0.0, 1.0);
    
    // (COLORING ALGORITHMS) https://www.mi.sanu.ac.rs/vismath/javier/b3.htm
    vec3 color;
    if (rz < 4.0)
    color = vec3(0.0);
    else {
        // normalized iteration count: https://iquilezles.org/www/articles/mset_smooth/mset_smooth.htm
        float nic = float(nri) + 1.0 - log2(log2(rz));
        
        if (colorScheme == 1)
        // multicolored
        color = hsv2rgb(nic * 0.015) + vec3(0.0, 0.0, 0.4);
        else if (colorScheme == 2)
        // (discrete) escape time coloring: Base the color on the number of iterations
        color = mix(colorMix1, colorMix2, fract(float(nri) * 0.02));
        else if (colorScheme == 3)
        // greyscale
        color = vec3(distJulia);
        else
        // default color scheme, like on wikipedia
        color = 0.5 + 0.5 * cos(3.0 + nic * 0.2 + vec3(0.0, 0.6, 1.0));
    }
    
    // Mandelbrot Set (shown when left mouse button is pressed)
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
        else if (colorScheme == 1)// adjust red color for the multicolored one
        incolor = vec3(0.4, 0.0667, 0.0039); // is out
        else
        incolor = vec3(0.9059, 0.1843, 0.0588); // is out
        
        // draw the Mandelbrot Set
        z = vec2(0.0);
        rz = 0.0;
        c = uv;
        for(int i = 0; i < 100; i ++ )
        {
            if (rz > 256.0)
            break;
            
            // Z' -> 2*Z*Z' + 1
            dz = 2.0 * vec2(z.x * dz.x - z.y * dz.y, z.x * dz.y + z.y * dz.x) + vec2(1.0, 0.0);
            // Z -> Z^2 + c
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
            rz = dot(z, z);
        }
        
        vec3 color2;
        if (rz < 4.0)
        color2 = incolor;
        else {
            rdz = dot(dz, dz);
            float distMand = 0.5 * sqrt(rz / rdz) * log(rz);
            distMand = clamp(pow(6.0 * distMand / zoom, 0.2), 0.0, 1.0);
            color2 = mix(incolor, color, smoothstep(0.0, 0.7, distMand));
        }
        
        color = mix(color, color2, 0.3);
    }
    
    color = mix(color, vec3(1.0, 0.0, 0.0), smoothstep(5.0 / zoom / Resolution.y, 0.0, length(uv - juliaParam)));
    
    gl_FragColor = vec4(color, 1.0);
}