#version 450	
	

in vec2 fTexcoord;
out vec4 FragColor;


uniform sampler2D gNormals;
uniform sampler2D gDepth;
uniform sampler2D gAlbedo;
uniform sampler2D texNoise;


uniform mat4 Projection;
uniform mat4 InverseProjection;
uniform mat4 SceneView;

uniform vec3 SSAOKernel[16];


vec3 readNormal(in vec2 coord) 
{       
	return normalize(texture2D(gNormals, coord).xyz*2.0  - 1.0);  
}
vec3 posFromDepth(vec2 coord)
{    
	float d = texture2D(gDepth, coord).r;
	vec3 tray = mat3x3(InverseProjection)*vec3((coord.x-0.5)*2.0,(coord.y-0.5)*2.0,1.0);
	return tray*d;
}    

//Ambient Occlusion form factor:    

float aoFF(in vec3 ddiff,in vec3 cnorm, in float c1, in float c2)
{          
	vec3 vv = normalize(ddiff);
	float rd = length(ddiff);
	return (1.0-clamp(dot(readNormal(fTexcoord+vec2(c1,c2)),-vv),0.0,1.0)) *
	clamp(dot( cnorm,vv ),0.0,1.0)*
	(1.0 - 1.0/sqrt(1.0/(rd*rd) + 1.0));    
}    


//GI form factor:    
float giFF(in vec3 ddiff,in vec3 cnorm, in float c1, in float c2)
{          
	vec3 vv = normalize(ddiff);
	float rd = length(ddiff); 
	return 1.0*clamp(dot(readNormal(fTexcoord+vec2(c1,c2)),-vv),0.0,1.0)* 
	clamp(dot( cnorm,vv ),0.0,1.0)/ (rd*rd+1.0);
	
}
void main()
{    
//read current normal,position and color. 
   vec3 n = readNormal(fTexcoord); 
   vec3 p = posFromDepth(fTexcoord);  
   vec3 col = texture2D(gAlbedo, fTexcoord).rgb; 
   //randomization texture    
   vec2 fres = vec2(2560.0/128.0*5,1440.0/128.0*5);
   vec3 random = texture2D(texNoise, fTexcoord*fres.xy).rgb;
   random = random*2.0-vec3(1.0); 
   //initialize variables:   
   float ao = 0.0;
   vec3 gi = vec3(0.0,0.0,0.0); 
   float incx = 1.0/2560.0*0.1;
   float incy = 1.0/1440.0*0.1;
   float pw = incx; 
   float ph = incy; 
   float cdepth = texture2D(gDepth, fTexcoord).r;
   
   //3 rounds of 8 samples each.  
   for(float i=0.0; i<3.0; ++i)    
   {       
		float npw = (pw+0.0007*random.x)/cdepth;       
		float nph = (ph+0.0007*random.y)/cdepth;      
		vec3 ddiff = posFromDepth(fTexcoord.st+vec2(npw,nph))-p;       
		vec3 ddiff2 = posFromDepth(fTexcoord.st+vec2(npw,-nph))-p;       
		vec3 ddiff3 = posFromDepth(fTexcoord.st+vec2(-npw,nph))-p;       
		vec3 ddiff4 = posFromDepth(fTexcoord.st+vec2(-npw,-nph))-p;       
		vec3 ddiff5 = posFromDepth(fTexcoord.st+vec2(0,nph))-p;      
		vec3 ddiff6 = posFromDepth(fTexcoord.st+vec2(0,-nph))-p;       
		vec3 ddiff7 = posFromDepth(fTexcoord.st+vec2(npw,0))-p;       
		vec3 ddiff8 = posFromDepth(fTexcoord.st+vec2(-npw,0))-p;      
		
		ao+=  aoFF(ddiff,n,npw,nph);       
		ao+=  aoFF(ddiff2,n,npw,-nph);       
		ao+=  aoFF(ddiff3,n,-npw,nph);       
		ao+=  aoFF(ddiff4,n,-npw,-nph);       
		ao+=  aoFF(ddiff5,n,0,nph);       
		ao+=  aoFF(ddiff6,n,0,-nph);       
		ao+=  aoFF(ddiff7,n,npw,0);       
		ao+=  aoFF(ddiff8,n,-npw,0);      
		gi+=  giFF(ddiff,n,npw,nph)*	texture2D(gAlbedo, fTexcoord+vec2(npw,nph)).rgb;      
		gi+=  giFF(ddiff2,n,npw,-nph)*	texture2D(gAlbedo, fTexcoord+vec2(npw,-nph)).rgb;      
		gi+=  giFF(ddiff3,n,-npw,nph)*	texture2D(gAlbedo, fTexcoord+vec2(-npw,nph)).rgb;       
		gi+=  giFF(ddiff4,n,-npw,-nph)*	texture2D(gAlbedo, fTexcoord+vec2(-npw,-nph)).rgb;      
		gi+=  giFF(ddiff5,n,0,nph)*		texture2D(gAlbedo, fTexcoord+vec2(0,nph)).rgb;       
		gi+=  giFF(ddiff6,n,0,-nph)*	texture2D(gAlbedo, fTexcoord+vec2(0,-nph)).rgb;       
		gi+=  giFF(ddiff7,n,npw,0)*		texture2D(gAlbedo, fTexcoord+vec2(npw,0)).rgb;      
		gi+=  giFF(ddiff8,n,-npw,0)*	texture2D(gAlbedo, fTexcoord+vec2(-npw,0)).rgb;       
		//increase sampling area:       
		pw += incx;
		ph += incy;
        
	}     
		
	ao/=24.0;   
	gi/=24.0;   
	FragColor = vec4(col-vec3(ao)+gi*5.0,1.0);
}