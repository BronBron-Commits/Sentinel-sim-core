#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <cmath>
#include <unistd.h>
#include <vector>

static const char *vs_src =
    "attribute vec2 aPos;\n"
    "attribute float aAge;\n"
    "varying float vAge;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "  gl_PointSize = 14.0;\n"
    "  vAge = aAge;\n"
    "}\n";

static const char *fs_src =
    "precision mediump float;\n"
    "varying float vAge;\n"
    "void main() {\n"
    "  float alpha = 1.0 - vAge;\n"
    "  gl_FragColor = vec4(0.1, 0.9, 0.7, alpha);\n"
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
    glBindAttribLocation(p, 1, "aAge");
    glLinkProgram(p);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

struct TrailPoint {
    float x;
    float y;
    float age;
};

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
    swa.event_mask = ExposureMask | KeyPressMask;
    Window win = XCreateWindow(dpy, root, 0, 0, 640, 480, 0,
                               CopyFromParent, InputOutput,
                               CopyFromParent, CWEventMask, &swa);
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Sentinel Sim — Trail View");

    EGLSurface surf = eglCreateWindowSurface(
        egl_dpy, cfg, (EGLNativeWindowType)win, nullptr);

    EGLint ctx_attrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext ctx = eglCreateContext(
        egl_dpy, cfg, EGL_NO_CONTEXT, ctx_attrs);

    eglMakeCurrent(egl_dpy, surf, surf, ctx);

    GLuint prog = make_program();
    GLuint vbo;
    glGenBuffers(1, &vbo);

    constexpr int TRAIL_LEN = 96;
    std::vector<TrailPoint> trail(TRAIL_LEN);
    int head = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (true) {
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            if (e.type == KeyPress) return 0;
        }

        static float t = 0.0f;
        t += 0.02f;

        float x = -0.5f + 0.25f * sinf(t);
        float y =  0.0f;

        trail[head] = { x, y, 0.0f };
        head = (head + 1) % TRAIL_LEN;

        for (auto &p : trail)
            p.age = fminf(p.age + 1.0f / TRAIL_LEN, 1.0f);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(TrailPoint) * TRAIL_LEN,
            trail.data(),
            GL_DYNAMIC_DRAW
        );

        glViewport(0, 0, 640, 480);
        glClearColor(0.04f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE,
            sizeof(TrailPoint),
            (void*)0
        );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 1, GL_FLOAT, GL_FALSE,
            sizeof(TrailPoint),
            (void*)(2 * sizeof(float))
        );

        glDrawArrays(GL_POINTS, 0, TRAIL_LEN);
        eglSwapBuffers(egl_dpy, surf);
        usleep(16000);
    }
}
