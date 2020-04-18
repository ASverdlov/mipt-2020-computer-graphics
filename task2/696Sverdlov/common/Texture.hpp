#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include <string>
#include <vector>
#include <memory>
#include "Common.h"

/**
Класс для управления текстурным объектом
*/
class Texture
{
public:
    /**
    Создает текстурный объект
    \param target тип текстуры (GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BUFFER и другие)
    */
    Texture(GLenum target = GL_TEXTURE_2D) :
        _target(target)
    {
	    if (USE_DSA) {
		    // В DSA-версии тип текстуры указывается при создании.
		    glCreateTextures(target, 1, &_tex);
	    } else {
		    glGenTextures(1, &_tex);
	    }
    }

    /**
    Использует уже созданный текстурный объект
    */
    Texture(GLuint tex, GLenum target) :
        _tex(tex),
        _target(target)
    {
    }

    ~Texture() {
        glDeleteTextures(1, &_tex);
    }


    /**
    Копирует данные из оперативной памяти в видеопамять, выделяя память под данные при необходимости
    \param target тип текстуры (GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z и другие)
    \param level уровень детализации (мипмап-уровень), 0 - базовая текстура
    \param internalFormat внутренний формат хранения текстуры в видеопамяти (GL_RGB8 и другие)
    \param width ширина текстуры в пикселях
    \param height высота текстуры в пикселях
    \param format содержит количество компонетов цвета пикселя во входном массиве данных (GL_RED, GL_RG, GL_RGB, GL_RGBA и другие)
    \param type тип данных компонент цвета пикселя во входном массиве данных (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, GL_FLOAT и другие)
    \param data указатель на начало массива данных в оперативной памяти
    */
    void setTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
    {
	    /*
		 * К сожалению или к счастью bindless-аналога для создания image нет.
		 * Можно создать immutable storage, но только один раз :)
		 * Фокус в том, что создание immutable storage требует явного указания числа mip уровней.
		 * Отложим это на потом.
		 */
	    glBindTexture(_target, _tex);
	    glTexImage2D(target, level, internalFormat, width, height, 0, format, type, data);
	    glBindTexture(_target, 0);
    }

    void setTexImage1D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid* data) {
        glBindTexture(_target, _tex);
        glTexImage1D(target, level, internalFormat, width, 0, format, type, data);
        glBindTexture(_target, 0);
    }

    void initStorage2D(GLsizei mipmaps, GLint internalFormat, GLsizei width, GLsizei height) {
        assert(GLEW_ARB_texture_storage);
        glTextureStorage2D(_tex, mipmaps, internalFormat, width, height);
    }

    /**
    Автоматически создает мипмап-уровни для текстуры
    */
    void generateMipmaps()
    {
        if (USE_DSA) {
            glGenerateTextureMipmap(_tex);
        }
        else {
            glBindTexture(_target, _tex);
            glGenerateMipmap(_target);
            glBindTexture(_target, 0);
        }
    }

    /**
    Прикрепляет текстуру к фреймбуферу (разбирается на 7м семинаре)
    */
    void attachToFramebuffer(GLenum attachment)
    {
        glBindTexture(_target, _tex);
        glFramebufferTexture(GL_FRAMEBUFFER, attachment, _tex, 0);
        glBindTexture(_target, 0);
    }

    // Bindless версия прикрепления текстуры к фреймбуферу
    void attachToFramebuffer(GLuint fbo, GLenum attachment) {
        if (USE_DSA) {
        	glNamedFramebufferTexture(fbo, attachment, _tex, 0);
        }
        else {
        	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        	attachToFramebuffer(attachment);
        	glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    // Функции bind() и unbind() не нужны, если есть Direct State Access.

    void bind() const
    {
        glBindTexture(_target, _tex);
    }

    void unbind() const
    {
        glBindTexture(_target, 0);
    }

    GLuint texture() const { return _tex; }

    GLenum getInternalFormat() {
        GLint internalFormat;
        if (USE_DSA) {
            glGetTextureLevelParameteriv(_tex, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
        }
        else {
            bind();
            glGetTexLevelParameteriv(_target, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
            unbind();
        }
        return static_cast<GLenum>(internalFormat);
    }

    void getSize(GLsizei &width, GLsizei &height) {
        if (USE_DSA) {
            glGetTextureLevelParameteriv(_tex, 0, GL_TEXTURE_WIDTH, &width);
            glGetTextureLevelParameteriv(_tex, 0, GL_TEXTURE_HEIGHT, &height);
        }
        else {
            bind();
            glGetTexLevelParameteriv(_target, 0, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(_target, 0, GL_TEXTURE_HEIGHT, &height);
            unbind();
        }
    }

    void saveRGBA8_PNG(const char *filename);

protected:
    Texture(const Texture&) = delete;
    void operator=(const Texture&) = delete;

    GLuint _tex;
    GLenum _target;
};

typedef std::shared_ptr<Texture> TexturePtr;

//=========== Функции для создания текстур

enum class SRGB
{
    YES,
    NO
};

/**
 * Загружает текстуру с автоматической её обработкой SOIL'ом.
 */
TexturePtr loadTextureGL(const std::string& filename);

/**
Загружает текстуру из файла PNG, JPEG и других форматов.
Иногда эта функция даёт порченную текстуру, тогда используем loadTextureGL.
*/
TexturePtr loadTexture(const std::string& filename, SRGB srgb = SRGB::NO, bool prefer1D = false);

/**
Загружает текстуру из файла DDS
*/
TexturePtr loadTextureDDS(const std::string& filename);

/**
Загружает кубическую текстуру
*/
TexturePtr loadCubeTexture(const std::string& basefilename);

/**
Пример программной генерации текстуры
*/
TexturePtr makeProceduralTexture();