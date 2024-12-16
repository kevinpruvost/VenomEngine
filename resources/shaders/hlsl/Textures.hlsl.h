// Bindless textures
Texture2D bindlessTextures[] : register(t0, space2);
//StructuredBuffer<int> isTextureHDR : register(t1, space2);

float4 GetTexture(int u, float2 texCoord, SamplerState sampler = g_sampler)
{
    //if (isTextureHDR[u])
        return bindlessTextures[u].Sample(sampler, texCoord);
    //else
        return bindlessTextures[u].Sample(sampler, texCoord);
}

Texture2D panoramaTexture : register(t0, space6);
cbuffer panoramaProps : register(b1, space6) {
    float panoramaPeakLuminance;
};

float atan2_custom(in float y, in float x)
{
    bool s = (abs(x) > abs(y));
    return mix(M_PI/2.0 - atan(x,y), atan(y,x), s);
}

vec4 GetPanoramaTexture(vec2 uv, vec3 dir) {
    // Normalize the view direction
    vec3 viewDir = normalize(viewDir);
    // Convert the view direction to spherical coordinates
    float phi = atan2_custom(viewDir.z, viewDir.x); // Azimuth angle
    float theta = asin(viewDir.y);                 // Inclination angle

    // Map spherical coordinates to UVs in the range [0, 1]ts
    vec2 uv;
    uv.x = phi / (2.0 * M_PI) + 0.5;  // Horizontal, azimuth
    uv.y = 1.0 - theta / M_PI + 0.5;   // Vertical, inclination

    // Sample the texture using the provided UV coordinates
    vec4 color = texture(sampler2D(panoramaTexture, g_sampler), uv);
    float exposure = sceneSettings.targetLuminance / panoramaPeakLuminance;
    color = vec4(color.rgb * exposure, 1.0); // Applying exposure factor

    return color;
}