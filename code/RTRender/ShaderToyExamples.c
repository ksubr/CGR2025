// https://p5js.org/tutorials/intro-to-shaders/ 
// plain colour, gradient, mouse interaction, iTime,
// distance field to mouse click
// circle
// Light, shade circle




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