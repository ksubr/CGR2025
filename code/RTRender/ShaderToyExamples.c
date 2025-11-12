// https://p5js.org/tutorials/intro-to-shaders/ 
// OpenGL example
// plain colour, gradient, mouse interaction, iTime,
// distance field to mouse click
// circle
// Light, shade circle
// Cubemap
// Raytracing spheres with shadows





// Plain colour fragment shader
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Output to screen
    fragColor = vec4(1.0,0.0,0.0,1.0); // Red color
}    


// Gradient based on pixel coordinates
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    // Output to screen
    fragColor = vec4(uv.x,uv.y,0.5,1.0);
}


// Gradient based on pixel coordinates, with blue channel using iMouse.x
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    // Output to screen
    fragColor = vec4(uv.x,uv.y,iMouse.x/iResolution.x,1.0);
}

// Distance field to mouse click
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy; 
    // Distance to mouse click
    float dist = distance(uv, iMouse.xy/iResolution.xy);
    // Output to screen
    fragColor = vec4(dist,dist,dist,1.0);
}

// Distance field to mouse click but adjust aspect ratio
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)               
    vec2 uv = fragCoord/iResolution.xy;
    uv.x *= iResolution.x/iResolution.y; // Correct aspect ratio
    // Distance to mouse click
    vec2 mouse = iMouse.xy/iResolution.xy;
    mouse.x *= iResolution.x/iResolution.y; // Correct aspect ratio
    float dist = distance(uv, mouse);
    // Output to screen
    fragColor = vec4(dist,dist,dist,1.0);
}

// Shade circle of radius r around  mouse click like a sphere
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)               
    vec2 uv = fragCoord/iResolution.xy;
    uv.x *= iResolution.x/iResolution.y; // Correct aspect ratio
    // Mouse position
    vec2 mouse = iMouse.xy/iResolution.xy;
    mouse.x *= iResolution.x/iResolution.y; // Correct aspect ratio
    // Distance to mouse click
    float dist = distance(uv, mouse);
    // Sphere radius
    float r = 0.25;
    // Shade like a sphere
    float z = sqrt(r*r - dist*dist);
    if(dist < r) {
        
        // blin-phong shading
        vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
        vec3 normal = normalize(vec3(uv - mouse, z));
        float diff = max(dot(lightDir, normal), 0.0);
        //add specular component blinn-phong
        float specularStrength = 0.5;
        vec3 viewDir = normalize(vec3(0.0, 0.0, 1.0));
        vec3 halfDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfDir), 0.0), 32.0);
        diff += specularStrength * spec;

        fragColor = vec4(vec3(diff), 1.0);
    } else {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}


// Shadertoy cubemap example mainImage
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord.xy / iResolution.xy;                
    uv = uv * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;  
    // Camera
    vec3 camO = vec3(0, 0, 0);
    vec3 camL = normalize(vec3(uv.x, uv.y, 1.0));

    // Rotate camera using iMouse
    float angleX = (iMouse.y / iResolution.y - 0.5) * 3.14; // Vertical angle
    float angleY = (iMouse.x / iResolution.x - 0.5) * 6.28; // Horizontal angle
    // Rotation matrices
    mat3 rotX = mat3(1, 0, 0,
                     0, cos(angleX), -sin(angleX),
                     0, sin(angleX), cos(angleX));
    mat3 rotY = mat3(cos(angleY), 0, sin(angleY),
                     0, 1, 0,
                     -sin(angleY), 0, cos(angleY));
    // Apply rotations
    camL = rotY * rotX * camL;  

    // Sample cubemap
    fragColor = texture(iChannel0, camL);      
}   

///////////////////////////////////
// Raytracing example
///////////////////////////////////

#define COUNT 5
#define SIZE 2

#define LIGHT_POWER 10.
#define SPECULAR_POWER 20.
#define AMBIENT .3

vec4 spheres[] = vec4[COUNT * SIZE](
    // Spheres: center and radius, color
    // Light source
    vec4(-7.5, 0., 36., 0.1), vec4(1., 1., 1., 1.),
    // 1
    vec4(-4., 0., 39., 2), vec4(.2, .9, .2, 1.),
    // 2
    vec4(3., 0., 42., 3.), vec4(.9, .4, .5, 1.),
    // 3
    vec4(2, -1., 43., 1.5), vec4(.2, .3, 1., 1.),
    // "Floor"
    vec4(0, -800006, 0, 800000), vec4(1.7, 1.0, .7, 1)
);

// Calculate a moving sphere center
vec3 sphereCenter(int sphere) {
    vec4 sD = spheres[sphere * SIZE];
    return sD.xyz;    
}

// Find a nearest ray-sphere intersection
// o - ray origin
// l - ray direction (normalized)
// ignore - sphere to ignore when detecting intersection (used for shadows)
// d - distance output
// Returns sphere number, -1 if no intersection found
int findIntersection(vec3 o, vec3 l, int ignore, out float d) {
    int sphere = -1;
    d = 1e5;
    
    for (int i = 0; i < COUNT; i++) {
        if (i == ignore) {
            continue;
        }
        vec3 c = sphereCenter(i);
        float r = spheres[i * SIZE].w;

        // Ray-sphere intersection formula
        vec3 t1 = o - c;
        float t1l = length(t1);
        float t2 = dot(l, t1);
        float s = t2 * t2 - t1l * t1l + r * r;
        if (s >= 0.) {
            float ss = sqrt(s);
            float sd = min(-t2 + ss, -t2 - ss);
            if (sd >= 0. && sd < d) {
                sphere = i;
                d = sd;
            }
        }
    }
    return sphere;
}

// Trace a single ray
// camO - camera origin
// camL - camera ray direction (normalized)
// Returns a fragment color
vec3 trace(vec3 camO, vec3 camL) {
    float d = 0.;
    int sphere = findIntersection(camO, camL, -1, d);
    
    if (sphere == -1) {
        // There was no intersection, return background color
        return vec3(0, 0, 0);
    }
    
    vec3 lightColor = spheres[1].xyz;

    if (sphere == 0) {
        // It's a light source, don't need to shade it
        return lightColor;
    }
    
    vec3 lightPoint = sphereCenter(0);
    
    // Sphere color
    vec3 sColor = spheres[sphere * SIZE + 1].xyz;
    vec3 aColor = sColor * vec3(AMBIENT, AMBIENT, AMBIENT);

    // Intersection point
    vec3 iPoint = camO + camL * d;
    vec3 iNormal = normalize(iPoint - sphereCenter(sphere));

    // Light direction vector
    vec3 lightDir = normalize(lightPoint - iPoint);
    
    // Check if there's another sphere between this one and the light source
    float dShadow = 0.;
    int shadowedBy = findIntersection(iPoint, lightDir, sphere, dShadow);
    dShadow = float(shadowedBy + 1) / 5.0;
    if (shadowedBy != 0) {
        // We're under shadow, use ambient color
        return aColor;
    }
    
    // Lighting (diffusion and specular)
    float cosA = clamp(dot(iNormal, lightDir), 0., 1.);
    float cosS = clamp(dot(-camL, reflect(-lightDir, iNormal)), 0., 1.);

    float dSquared = pow(length(iPoint - lightPoint), 2.);
    
    return aColor +
        sColor * lightColor * cosA * LIGHT_POWER / dSquared +
        lightColor * pow(cosS, SPECULAR_POWER) * LIGHT_POWER / dSquared;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord.xy / iResolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;
    
    // Camera
    vec3 camO = vec3(0, 0, 0);
    vec3 camL = normalize(vec3(uv.x, uv.y, 7));
    
    // Ray-tracing
    fragColor = vec4(trace(camO, camL), 1.);      
}



