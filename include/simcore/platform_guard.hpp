#pragma once

// =======================================================
// simcore platform guard
// =======================================================
//
// This header is included by ALL simcore public headers.
//
// If ANY platform header leaks in before simcore,
// compilation MUST fail.
//
// This enforces a pure, platform-agnostic core.
//

#if defined(SIMCORE_NO_PLATFORM_HEADERS)

// POSIX
#if defined(_POSIX_VERSION)
#error "POSIX headers are forbidden in simcore"
#endif

// Windows
#if defined(_WIN32)
#error "Windows headers are forbidden in simcore"
#endif

// SDL
#if defined(SDL_h_) || defined(_SDL_H)
#error "SDL headers are forbidden in simcore"
#endif

// X11
#if defined(_X11_XLIB_H_) || defined(_XLIB_H_)
#error "X11 headers are forbidden in simcore"
#endif

// EGL / GLES
#if defined(__egl_h_) || defined(__gles2_h_)
#error "EGL/GLES headers are forbidden in simcore"
#endif

#endif // SIMCORE_NO_PLATFORM_HEADERS
