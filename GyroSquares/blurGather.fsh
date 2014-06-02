
out vec4 outFragColor;
in vec2 st;

uniform sampler2D uColorTexture;
//uniform sampler2D uVelocityTexture;
//uniform sampler2D uDepthTexture;

void main(){
//    vec2 ivX = vec2(gl_FragCoord.xy);
//    vec4 vCX = texelFetch(uColorTexture, ivX, 0);
    //vec4 dd = texture(uVelocityTexture, st);
    outFragColor = texture(uColorTexture, st);
}