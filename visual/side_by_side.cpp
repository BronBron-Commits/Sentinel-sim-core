#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <cmath>
#include <cstdio>
#include <cstdint>

#include <simcore/sim_state.hpp>
#include <simcore/sim_update.hpp>
#include <simcore/sim_hash.hpp>

static const char *vs_src =
    "attribute vec2 aPos;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "  gl_PointSize = 12.0;\n"
    "}\n";

static const char *fs_src =
    "precision mediump float;\n"
    "uniform vec4 uColor;\n"
    "void main() {\n"
    "  gl_FragColor = uColor;\n"
    "}\n";

static GLuint compile(GLenum type, const char *src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    return s;
}

static GLuint make_program() {
    GLuint vs = compile(GL_VERTEX_SHADER, vs_src);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fs_src);
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glBindAttribLocation(p, 0, "aPos");
    glLinkProgram(p);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

int main() {
    Display *dpy = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(dpy);

    EGLDisplay egl_dpy = eglGetDisplay((EGLNativeDisplayType)dpy);
    eglInitialize(egl_dpy, nullptr, nullptr);

    EGLint cfg_attrs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8,
        EGL_NONE
    };

    EGLConfig cfg;
    EGLint n;
    eglChooseConfig(egl_dpy, cfg_attrs, &cfg, 1, &n);

    XSetWindowAttributes swa{};
    swa.event_mask = ExposureMask | KeyPressMask;

    Window win = XCreateWindow(
        dpy, root, 0, 0, 640, 480, 0,
        CopyFromParent, InputOutput,
        CopyFromParent, CWEventMask, &swa
    );

    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Sentinel — initializing…");

    EGLSurface surf =
        eglCreateWindowSurface(egl_dpy, cfg, (EGLNativeWindowType)win, nullptr);

    EGLint ctx_attrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext ctx =
        eglCreateContext(egl_dpy, cfg, EGL_NO_CONTEXT, ctx_attrs);

    eglMakeCurrent(egl_dpy, surf, surf, ctx);

    GLuint prog = make_program();
    GLint uColor = glGetUniformLocation(prog, "uColor");

    GLuint vbo;
    glGenBuffers(1, &vbo);

    SimState left{}, right{};
    right.x = Fixed::from_int(200);

    bool injected = false;

    while (true) {
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            if (e.type == KeyPress)
                return 0;
        }

        sim_update(left);
        sim_update(right);

        if (!injected && left.tick == 120) {
            right.x += Fixed::from_int(50);
            injected = true;
        }

        uint64_t hl = sim_hash(left);
        uint64_t hr = sim_hash(right);
        bool match = (hl == hr);

        char title[256];
        snprintf(
            title, sizeof(title),
            "LEFT: 0x%016lx | RIGHT: 0x%016lx | %s",
            hl, hr, match ? "MATCH" : "DIVERGED"
        );
        XStoreName(dpy, win, title);

        float pts[4] = {
            static_cast<float>(-0.5 + 0.001 * left.x.to_double()),  0.0f,
            static_cast<float>( 0.5 + 0.001 * right.x.to_double()), 0.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_DYNAMIC_DRAW);

        glViewport(0, 0, 640, 480);
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glUniform4f(
            uColor,
            match ? 0.2f : 1.0f,
            match ? 0.9f : 0.2f,
            0.2f,
            1.0f
        );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_POINTS, 0, 2);

        eglSwapBuffers(egl_dpy, surf);
        usleep(16000);
    }
}
