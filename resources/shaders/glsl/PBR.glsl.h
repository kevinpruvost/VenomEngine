#include "Resources.frag.glsl.h"
#include "Eevee.glsl.h"

// Constants
const float PI = 3.14159265358979323846;

// Utility Functions
float sqr(float x) { return x * x; }

float SchlickFresnel(float u) {
    float m = clamp(1.0 - u, 0.0, 1.0);
    float m2 = m * m;
    return m2 * m2 * m; // Equivalent to pow(m, 5)
}

float GTR1(float NdotH, float a) {
    if (a >= 1.0) return 1.0 / PI;
    float a2 = a * a;
    float t = 1.0 + (a2 - 1.0) * NdotH * NdotH;
    return (a2 - 1.0) / (PI * log(a2) * t);
}

float GTR2(float NdotH, float a) {
    float a2 = a * a;
    float t = 1.0 + (a2 - 1.0) * NdotH * NdotH;
    return a2 / (PI * t * t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay) {
    return 1.0 / (PI * ax * ay * sqr(sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH));
}

float smithG_GGX(float NdotV, float alphaG) {
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1.0 / (NdotV + sqrt(a + b - a * b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay) {
    return 1.0 / (NdotV + sqrt(sqr(VdotX * ax) + sqr(VdotY * ay) + sqr(NdotV)));
}

vec3 mon2lin(vec3 x) {
    return vec3(pow(x.x, 2.2), pow(x.y, 2.2), pow(x.z, 2.2));
}

// Luminance approximation
float luminance(vec3 color) {
    return dot(color, vec3(0.3, 0.6, 0.1));
}

vec3 Reflection(vec3 V, vec3 N, vec3 baseColor, float metallic, float roughness)
{
    // Simplified Fresnel-weighted reflection
    float NDotV = dot(N, V);

    ivec2 imageSizeBrdfLUT = imageSize(brdfLUT);
    ivec2 coords = ivec2(NDotV * imageSizeBrdfLUT.x, roughness * imageSizeBrdfLUT.y);
    vec2 brdf = imageLoad(brdfLUT, coords).rg;

    vec3 R = normalize(reflect(-V, N));
    vec3 specularReflectionColor = GetPanoramaRadiance(R, roughness).rgb;
    vec3 specularReflectionBaseColor = mix(vec3(0.04), baseColor, metallic);
    vec3 specularReflection = specularReflectionColor * (specularReflectionBaseColor * brdf.x + brdf.y);

    vec3 diffuseLight = baseColor * (1.0 - metallic) * (1.0 - 0.04);
//    vec2 irradianceUvF = mod(PanoramaUvFromDir(N), vec2(1.0));
//    ivec2 irradianceMapSize = imageSize(irradianceMap);
//    ivec2 irradianceUV = ivec2(irradianceUvF.x * irradianceMapSize.x, irradianceUvF.y * irradianceMapSize.y);
//    vec3 irradiance = imageLoad(irradianceMap, irradianceUV).rgb;
    vec3 irradiance = GetPanoramaIrradiance(N).rgb;
    vec3 diffuseReflection = diffuseLight * irradiance;

    // TODO: Make specular reflection more blurry with roughness

    return
        specularReflection
        + diffuseReflection
        ;
}

// Disney Principled BSDF Function
vec3 DisneyPrincipledBSDF(
    vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y,
    vec3 baseColor, float metallic, float roughness,
    float subsurface, float specular, float specularTint,
    float anisotropic, float sheen, float sheenTint,
    float clearcoat, float clearcoatGloss
) {
    float NdotL = dot(N,L);
    float NdotV = dot(N,V);
    if (NdotL < 0 || NdotV < 0) return vec3(0);

    vec3 H = normalize(L+V);
    float NdotH = dot(N,H);
    float LdotH = dot(L,H);

    vec3 Cdlin = (baseColor);
    float Cdlum = 0.3 * Cdlin.x + 0.6 * Cdlin.y + 0.1 * Cdlin.z; // Luminance approximation

    vec3 Ctint = (Cdlum > 0.0) ? Cdlin / Cdlum : vec3(1.0, 1.0, 1.0); // Normalize luminance
    vec3 Cspec0 = mix(specular * 0.08 * mix(vec3(1.0, 1.0, 1.0), Ctint, specularTint), Cdlin, metallic);
    vec3 Csheen = mix(vec3(1.0, 1.0, 1.0), Ctint, sheenTint);

    // Diffuse Fresnel
    float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
    float Fd90 = 0.5 + 2.0 * LdotH * LdotH * roughness;
    float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

    // Subsurface
    float Fss90 = LdotH * LdotH * roughness;
    float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
    float ss = 1.25 * (Fss * (1.0 / (NdotL + NdotV) - 0.5) + 0.5);

    // Specular
    float aspect = sqrt(1.0 - anisotropic * 0.9);
    float ax = max(0.001, sqr(roughness) / aspect);
    float ay = max(0.001, sqr(roughness) * aspect);
    float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
    float FH = SchlickFresnel(LdotH);
    vec3 Fs = mix(Cspec0, vec3(1.0, 1.0, 1.0), FH);
    float Gs = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay) *
               smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);
    vec3 specularRes = Fs * Ds * Gs;

    // Sheen
    vec3 Fsheen = FH * sheen * Csheen;

    // Clearcoat
    float Dr = GTR1(NdotH, mix(0.1, 0.001, clearcoatGloss));
    float Fr = mix(0.04, 1.0, FH);
    float Gr = smithG_GGX(NdotL, 0.25) * smithG_GGX(NdotV, 0.25);

    return ((1.0 / PI) * mix(Fd, ss, subsurface) * Cdlin + Fsheen) * (1.0 - metallic)
           + specularRes
           + 0.25 * clearcoat * Gr * Fr * Dr;
}

vec3 LambertCookTorrance(
    vec3 L, vec3 V, vec3 N, vec3 baseColor, float metallic, float roughness
) {
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    if (NdotL <= 0.0 || NdotV <= 0.0) return vec3(0.0);

    vec3 H = normalize(L + V);
    float NdotH = dot(N, H);
    float LdotH = dot(L, H);

    // Fresnel-Schlick Approximation
    vec3 F0 = mix(vec3(0.04), baseColor, metallic); // Fresnel at normal incidence
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - LdotH, 5.0);

    // Geometry term (GGX)
    float alpha = roughness * roughness;
    float G_NdotL = smithG_GGX(NdotL, alpha);
    float G_NdotV = smithG_GGX(NdotV, alpha);
    float G = G_NdotL * G_NdotV;

    // Distribution term (GGX Normal Distribution Function)
    float D = GTR2(NdotH, alpha);

    // Cook-Torrance Specular
    vec3 specular = (D * F * G) / max(4.0 * NdotL * NdotV, 0.001);

    // Lambertian Diffuse
    vec3 diffuse = (1.0 - metallic) * baseColor / PI;

    // Combine diffuse and specular
    return (diffuse + specular) * NdotL;
}