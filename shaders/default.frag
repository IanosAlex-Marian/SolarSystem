#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform bool isLightSource;

void main() {
    if (isLightSource) {
        FragColor = vec4(objectColor, 1.0);
    } else {
        // Ambient lighting
        vec3 ambient = ambientLight * objectColor;
        
        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * objectColor;
        
        vec3 result = (ambient + diffuse);
        FragColor = vec4(result, 1.0);
    }
}