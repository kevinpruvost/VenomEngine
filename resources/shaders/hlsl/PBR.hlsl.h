#include "Resources.ps.hlsl.h"

float3 BlinnPhongBRDF(MaterialPBR mat, float3 position, float3 lightDir, float3 viewDir)
{
    float3 H = normalize(lightDir + viewDir);

    return mat.baseColor * (mat.specular + 2) * pow(saturate(dot(mat.normal, H)), mat.specular);
};

// Constants
static const float PI = 3.14159265358979323846;

// Utility Functions
float sqr(float x) { return x * x; }

float SchlickFresnel(float u) {
    float m = saturate(1 - u);
    float m2 = m * m;
    return m2 * m2 * m; // Equivalent to pow(m, 5)
}

float GTR1(float NdotH, float a) {
    if (a >= 1) return 1 / PI;
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return (a2 - 1) / (PI * log(a2) * t);
}

float GTR2(float NdotH, float a) {
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return a2 / (PI * t * t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay) {
    return 1 / (PI * ax * ay * sqr(sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH));
}

float smithG_GGX(float NdotV, float alphaG) {
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1 / (NdotV + sqrt(a + b - a * b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay) {
    return 1 / (NdotV + sqrt(sqr(VdotX * ax) + sqr(VdotY * ay) + sqr(NdotV)));
}

float3 mon2lin(float3 x) {
    return float3(pow(x.x, 2.2), pow(x.y, 2.2), pow(x.z, 2.2));
}

// Disney Principled BSDF Function
float3 DisneyPrincipledBSDF(
    float3 L, float3 V, float3 N, float3 X, float3 Y,
    float3 baseColor, float metallic, float roughness,
    float subsurface, float specular, float specularTint,
    float anisotropic, float sheen, float sheenTint,
    float clearcoat, float clearcoatGloss
) {
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    if (NdotL <= 0.0f || NdotV <= 0.0f) return float3(0, 0, 0);

    float3 H = normalize(L + V);
    float NdotH = dot(N, H);
    float LdotH = dot(L, H);

    float3 Cdlin = mon2lin(baseColor);
    float Cdlum = 0.3 * Cdlin.x + 0.6 * Cdlin.y + 0.1 * Cdlin.z; // Luminance approximation

    float3 Ctint = (Cdlum > 0) ? Cdlin / Cdlum : float3(1, 1, 1); // Normalize luminance
    float3 Cspec0 = lerp(specular * 0.08 * lerp(float3(1, 1, 1), Ctint, specularTint), Cdlin, metallic);
    float3 Csheen = lerp(float3(1, 1, 1), Ctint, sheenTint);

    // Diffuse Fresnel
    float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
    float Fd90 = 0.5 + 2 * LdotH * LdotH * roughness;
    float Fd = lerp(1.0, Fd90, FL) * lerp(1.0, Fd90, FV);

    // Subsurface
    float Fss90 = LdotH * LdotH * roughness;
    float Fss = lerp(1.0, Fss90, FL) * lerp(1.0, Fss90, FV);
    float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - 0.5) + 0.5);

    // Specular
    float aspect = sqrt(1 - anisotropic * 0.9);
    float ax = max(0.001, sqr(roughness) / aspect);
    float ay = max(0.001, sqr(roughness) * aspect);
    float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
    float FH = SchlickFresnel(LdotH);
    float3 Fs = lerp(Cspec0, float3(1, 1, 1), FH);
    float Gs = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay) *
               smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

    // Sheen
    float3 Fsheen = FH * sheen * Csheen;

    // Clearcoat
    float Dr = GTR1(NdotH, lerp(0.1, 0.001, clearcoatGloss));
    float Fr = lerp(0.04, 1.0, FH);
    float Gr = smithG_GGX(NdotL, 0.25) * smithG_GGX(NdotV, 0.25);

    return ((1 / PI) * lerp(Fd, ss, subsurface) * Cdlin + Fsheen) * (1 - metallic) +
           Gs * Fs * Ds + 0.25 * clearcoat * Gr * Fr * Dr;
}

float3 SimpleBRDF(
    float3 baseColor,           // Surface base color
    float roughness,            // Surface roughness
    float3 normal,              // Surface normal vector
    float3 viewDir,             // View direction vector
    float3 lightDir,            // Light direction vector
    float3 lightColor           // Incident light color
)
{
    // Normalize input vectors
    normal = normalize(normal);
    viewDir = normalize(viewDir);
    lightDir = normalize(lightDir);

    // Halfway vector (used for specular calculations)
    float3 halfVector = normalize(viewDir + lightDir);

    // Diffuse calculation (Lambert)
    float NdotL = max(dot(normal, lightDir), 0.0);
    float3 diffuseTerm = baseColor * NdotL;

    // Specular calculation (Blinn-Phong)
    float NdotH = max(dot(normal, halfVector), 0.0);
    float NdotV = max(dot(normal, viewDir), 0.0);

    // Adjust specular based on roughness
    float specPower = exp2(10 * (1.0 - roughness) + 1.0);
    float specularTerm = pow(NdotH, specPower) * (specPower + 8.0) / 8.0;

    // Fresnel approximation (Schlick's approximation)
    float3 F0 = float3(0.04, 0.04, 0.04); // Base reflectivity
    float3 fresnelTerm = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);

    // Combine terms
    float3 brdfResult = (diffuseTerm * (1.0 - fresnelTerm)) +
                        (specularTerm * fresnelTerm);

    // Multiply by light color and light intensity
    return brdfResult * lightColor * NdotL;
}
