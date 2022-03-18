#ifndef CHAMS_H
#define CHAMS_H
#include <GLES2/gl2.h>
#include "src/GL/include/Error404Cham.h"
#include <dlfcn.h>

#include <src/Substrate/SubstrateHook.h>

static void *handle;
static const char* shaderName;
static bool enableWallhack;
void Setc(int color);

void Disable_Cham();

void Enable_Antena_Head();

void Disable_Antena_Head();

struct {
	int red = 0;
	int green = 1;
	int blue = 2;
	int white = 3;
	int black = 4;
	int purpleSense = 5;
	int yellow = 6;
	int magenta = 7;
	int gray = 8;

	void set_red() {
		Setc(red);
	}

	void set_green() {
		Setc(green);
	}

	void set_blue() {
		Setc(blue);
	}

	void set_white() {
		Setc(white);
	}

	void set_black() {
		Setc(black);
	}

	void set_purpleSense() {
		Setc(purpleSense);
	}

	void set_yellow() {
		Setc(yellow);
	}

	void set_magenta() {
		Setc(magenta);
	}

	void set_gray() {
		Setc(gray);
	}
} scc;


void setShader(const char* s) {
    shaderName = s;
}
const char* getShader() {
    return shaderName;
}

void SetWallhack(bool enable){
    enableWallhack = enable;
}
bool getWallhackEnabled(){
    return enableWallhack;
}


FILE *Elog = fopen("/sdcard/Android/Chams.log", "w+");

GLint (*old_glGetUniformLocation)(GLuint program, const char *name);
GLint new_glGetUniformLocation(GLuint program, const char *name) {
    //Log name to find used shaders.

    fprintf(Elog, "%s\n", name);
    return old_glGetUniformLocation(program, name);
}

bool isCurrentShader(const char *shader) {
    GLint currProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
    return old_glGetUniformLocation(currProgram, shader) != -1;
}
#define _DRAW_RGB_ 255,0,0
void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
void new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    old_glDrawElements(mode, count, type, indices);
    if (mode != GL_TRIANGLES || count < 10000) return; {
        if (isCurrentShader(getShader())) {
            if (getWallhackEnabled()) {
                glDisable(GL_DEPTH_TEST);
                old_glDrawElements(mode, count, type, indices);
               glEnable(GL_DEPTH_TEST);

            }
            old_glDrawElements(mode, count, type, indices);
            glDepthRangef(0.5, 1);
            glColorMask(1, 1, 1, 1);
            glDisable(GL_BLEND);
        }
    }
}

bool mlovinit() {
    handle = 0;
    handle = dlopen("libAegisGL.a", RTLD_LAZY);
    if(!handle) {
        LOGE(("Cannot open library: %s"), dlerror());
        return false;
    }
    return true;
}

void LogShaders() {
	//constant void
    auto p_glGetUniformLocation = (const void*(*)(...))dlsym(handle, ("glGetUniformLocation"));
    const char *dlsym_error = dlerror();
    if(dlsym_error) {
        LOGE(("Cannot load symbol 'glGetUniformLocation': %s"), dlsym_error);
        return;
    } else {
        MSHookFunction(reinterpret_cast<void*>(p_glGetUniformLocation), reinterpret_cast<void*>(new_glGetUniformLocation), reinterpret_cast<void**>(&old_glGetUniformLocation));
    }
}

void Wallhack() {
    auto p_glDrawElements = (const void*(*)(...))dlsym(handle, "glDrawElements");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        LOGE(("Cannot load symbol 'glDrawElements': %s"), dlsym_error);
        return;
    } else {
        MSHookFunction(reinterpret_cast<void*>(p_glDrawElements), reinterpret_cast<void*>(new_glDrawElements), reinterpret_cast<void**>(&old_glDrawElements));
    }
}

#endif



