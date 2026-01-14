#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <cmath>
#include <unistd.h>

static const char *vs_src =
    "attribute vec2 aPos;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "  gl_PointSize = 16.0;\n"
    "}\n";

static const char *fs_src =
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.1, 0.8, 1.0, 1.0);\n"
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

    XSetWindowAttributes swa;
    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
    Window win = XCreateWindow(dpy, root, 0, 0, 640, 480, 0,
                               CopyFromParent, InputOutput,
                               CopyFromParent, CWEventMask, &swa);
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "GLES Debug Dots");

    EGLSurface surf = eglCreateWindowSurface(egl_dpy, cfg, (EGLNativeWindowType)win, nullptr);
    EGLint ctx_attrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext ctx = eglCreateContext(egl_dpy, cfg, EGL_NO_CONTEXT, ctx_attrs);
    eglMakeCurrent(egl_dpy, surf, surf, ctx);

    GLuint prog = make_program();
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float pts[4] = { -0.5f, 0.0f, 0.5f, 0.0f };
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_DYNAMIC_DRAW);

    glUseProgram(prog);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    while (true) {
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            if (e.type == KeyPress) return 0;
        }

        static float t = 0.0f;
        t += 0.02f;
        float moved[4] = {
            -0.5f + 0.25f * sinf(t),
             0.5f + 0.25f * cosf(t)
        };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(moved), moved);

        glViewport(0, 0, 640, 480);
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_POINTS, 0, 2);
        eglSwapBuffers(egl_dpy, surf);
        usleep(16000);
    }
}
