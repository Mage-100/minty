#version 330 core
layout(location = 0) in vec2 aPos; // 0..1 quad

uniform vec2 u_pos;   // pixel-space top-left of prompt region
uniform vec2 u_size;  // pixel-space width/height of prompt region
uniform vec2 u_screen; // framebuffer size

out vec2 v_uv; // 0..1 across the prompt quad, left to right

void main() {
    v_uv = aPos;

   // aPos (0,0) = top-left of quad, (1,1) = bottom-right
   vec2 pixel_tl = u_pos;
   vec2 pixel    = pixel_tl + aPos * u_size;

   // convert to NDC: pixel (0,0) is top-left of screen
   float nx =  (pixel.x / u_screen.x) * 2.0 - 1.0;
   float ny = -((pixel.y / u_screen.y) * 2.0 - 1.0);

   gl_Position = vec4(nx, ny, 0.0, 1.0);
}
