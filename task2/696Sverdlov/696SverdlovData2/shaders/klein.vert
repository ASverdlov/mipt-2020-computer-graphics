#version 330

//стандартные матрицы для преобразования координат
uniform mat4 modelMatrix; //из локальной в мировую
uniform mat4 viewMatrix; //из мировой в систему координат камеры
uniform mat4 projectionMatrix; //из системы координат камеры в усеченные координаты
uniform float morphismAlpha; // для анимации

//матрица для преобразования нормалей из локальной системы координат в систему координат камеры
uniform mat3 normalToCameraMatrix;

layout(location = 0) in vec3 vertex1Position; //координаты вершины в локальной системе координат
layout(location = 1) in vec3 vertex1Normal; //нормаль в локальной системе координат
layout(location = 2) in vec3 vertex2Position; //координаты вершины в локальной системе координат
layout(location = 3) in vec3 vertex2Normal; //нормаль в локальной системе координат
layout(location = 4) in vec2 vertexTexCoord; //текстурные координаты вершины

out vec3 normalCamSpace; //нормаль в системе координат камеры
out vec4 posCamSpace; //координаты вершины в системе координат камеры
out vec2 texCoord; //текстурные координаты

void main()
{
	texCoord = vertexTexCoord;

	// Преобразуем поверхность 1 в поверхность 2.
	vec3 vertexPosition = morphismAlpha * vertex1Position + (1.0 - morphismAlpha) * vertex2Position;
    vec3 vertexNormal = morphismAlpha * vertex1Normal + (1.0 - morphismAlpha) * vertex2Normal;

	posCamSpace = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0); //преобразование координат вершины в систему координат камеры
	normalCamSpace = normalize(normalToCameraMatrix * vertexNormal); //преобразование нормали в систему координат камеры
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
