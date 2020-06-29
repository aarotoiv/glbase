#include "Shader.h"

Shader::Shader() {
    shaderId = 0;
    uniformModel = 0;
    uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

Shader::~Shader() {
    clearShader();
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
    compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = readFile(vertexLocation);
    std::string fragmentString = readFile(fragmentLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char* fileLocation) {
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in); 

    if (!fileStream.is_open()) {
        printf("Failed to read %s. File doesn't exist.", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::getProjectionLocation() {
    return uniformProjection;
}

GLuint Shader::getModelLocation() {
    return uniformModel;
}

GLuint Shader::getViewLocation() {
	return uniformView;
}

GLuint Shader::getAmbientIntensityLocation() {
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getAmbientColorLocation() {
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::getDiffuseIntensityLocation() {
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getDirectionLocation() {
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getSpecularIntensityLocation() {
	return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation() {
	return uniformShininess;
}

GLuint Shader::getEyePositionLocation() {
	return uniformEyePosition;
}

void Shader::setDirectionalLight(DirectionalLight * dLight) {
	dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight * pLight, unsigned int lightCount) {
	if (lightCount > MAX_POINT_LIGHTS)	
		lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
		uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
		uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::setSpotLights(SpotLight * sLight, unsigned int lightCount) {
	if (lightCount > MAX_SPOT_LIGHTS)	
		lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
		uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection, uniformSpotLight[i].uniformEdge,
		uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent);
	}	
}

void Shader::setTexture(GLuint textureUnit) {
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit) {
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4 *lTransform) {
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::useShader() {
    glUseProgram(shaderId);
}

void Shader::clearShader() {
    if (shaderId != 0) {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}


void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
    shaderId = glCreateProgram();
 
	if (!shaderId) {
		printf("Error creating shader program!\n");
		return;
	}
 
	addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);
 
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
 
	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}
 
	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		printf("Error Validating program: '%s'\n", eLog);
		return;
	}

    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
	uniformView = glGetUniformLocation(shaderId, "view");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderId, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++){
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderId, "spotLightCount");
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++){
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderId, locBuff);
	}
	uniformTexture = glGetUniformLocation(shaderId, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderId, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderId, "directionalShadowMap");

}

void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
 
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
 
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
 
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
 
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
 
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n",shaderType, eLog);
		return;
	}
 
	glAttachShader(theProgram, theShader);
 
	return;
}
