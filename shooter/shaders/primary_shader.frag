#version 430

out vec4 frag_color;

in VS_OUT
{
	vec3 normal;
	vec3 world_pos;
} fs_in;

uniform mat4 view;
uniform vec3 camPos;

void main()
{	
	vec3 color = vec3(1.0);
	
	vec3 diffuseColor = color;
    vec3 ambient = color * 0.3;
	
	// diffuse 
    vec3 norm = normalize(fs_in.normal);
  
    vec3 lightDir = normalize(vec3(-0.5,0.5,1));  
    
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseColor * diff;  
    
    // specular
    vec3 viewDir = normalize(camPos - fs_in.world_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(1) * spec;
        
    vec3 result = ambient + diffuse + specular;

	frag_color = vec4(result, 1.0);
}