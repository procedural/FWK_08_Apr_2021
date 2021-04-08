// game framework
// - rlyeh, public domain

#ifndef FWK_H
#define FWK_H

// -----------------------------------------------------------------------------
// config directives
// debug /O0 /D3 > debugopt /O1 /D2 > release (+ndebug) /O2 /D1 > final (+ndebug+final) /O3 /D0

#ifdef FINAL
#define WITH_COOKER          0
#define WITH_FASTCALL_LUA    1
#define WITH_LEAK_DETECTOR   0
#define WITH_PROFILE         0
#define WITH_XREALLOC_POISON 0
#define WITH_VIDEO_YCBCR     1
#else
#define WITH_COOKER          1
#define WITH_FASTCALL_LUA    0
#define WITH_LEAK_DETECTOR   0
#define WITH_PROFILE         1
#define WITH_XREALLOC_POISON 1
#define WITH_VIDEO_YCBCR     1
#endif

//#define WITH_ASSIMP        0      // Only 3rd/3rd_tools/ass2iqe.c defines this.
#define WITH_COMPRESSOR      LZ4X|0 // Use COMPRESSOR|LEVEL[0..15], where compressor is one of:
                                    // LZMA,DEFL,LZ4X,ULZ,BCM,CRSH,BALZ,LZW3,LZSS,LZP1,PPP,RAW

// -----------------------------------------------------------------------------
// system headers

#ifndef _GNU_SOURCE
#define _GNU_SOURCE   // for linux
#endif

#ifdef _MSC_VER
#include <omp.h>      // compile with /openmp to speed up some computations
#endif
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// stl, forward includes

#define array(t) t*         // "3rd/3rd_ds.h"

#ifdef _MSC_VER
#define __thread         __declspec(thread)
#endif

// -----------------------------------------------------------------------------
// api

#include "fwk_memory.h"
#include "fwk_math.h"
#include "fwk_collide.h"
//---
#include "fwk_audio.h"
#include "fwk_cooker.h"
#include "fwk_data.h"
#include "fwk_editor.h"
#include "fwk_file.h"
#include "fwk_input.h"
#include "fwk_math.h"
#include "fwk_network.h"
#include "fwk_render.h"
#include "fwk_renderdd.h"
#include "fwk_scene.h"
#include "fwk_script.h"
#include "fwk_system.h"
#include "fwk_ui.h"
#include "fwk_video.h"
#include "fwk_window.h"

#endif

// =============================================================================

#ifdef FWK_C
#pragma once

// ----------------------------------------------------------------------------
// 3rd party libs

#define ARCHIVE_C                           // archive.c
#define COMPRESS_C                          // compress.c
#define DS_C                                // ds.c
#define GJK_C                               // gjk
#define GLAD_GL_IMPLEMENTATION              // glad
#define LSH_GLFW_IMPLEMENTATION             // glfw
#define GLFW_INCLUDE_NONE                   // glfw
#define HTTPS_IMPLEMENTATION                // https
#define JAR_MOD_IMPLEMENTATION              // jar_mod
#define JAR_XM_IMPLEMENTATION               // jar_xm
#define JO_MPEG_COMPONENTS 3                // jo_mpeg
#define JSON5_C                             // json5
#define LUA_IMPL                            // lua542
#define MINIAUDIO_IMPLEMENTATION            // miniaudio
#define NK_GLFW_GL3_IMPLEMENTATION          // nuklear
#define NK_IMPLEMENTATION                   // nuklear
#define NK_INCLUDE_DEFAULT_ALLOCATOR        // nuklear
#define NK_INCLUDE_DEFAULT_FONT             // nuklear
#define NK_INCLUDE_FIXED_TYPES              // nuklear
#define NK_INCLUDE_FONT_BAKING              // nuklear
#define NK_INCLUDE_STANDARD_IO              // nuklear
#define NK_INCLUDE_STANDARD_VARARGS         // nuklear
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT     // nuklear
#define NK_KEYSTATE_BASED_INPUT             // nuklear
#define PL_MPEG_IMPLEMENTATION              // pl_mpeg
#define STB_IMAGE_IMPLEMENTATION            // stbi
#define STB_IMAGE_WRITE_IMPLEMENTATION      // stbi_write
#define STS_MIXER_IMPLEMENTATION            // sts_mixer
#define SWRAP_IMPLEMENTATION                // swrap
#define SWRAP_STATIC                        // swrap
#define THREAD_IMPLEMENTATION               // thread

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#define USE_WEBGL2 1
#define USE_GLFW 3
#define WASM 1
#define GLFW3_HEADER <GLFW/glfw3.h>
#else
#define _GLFW_WIN                           // glfw
#define GLFW3_HEADER "3rd/3rd_glfw3.h"
#endif

#include "3rd/3rd_ds.h"
//---
#include "3rd/3rd_glad.h"
#include GLFW3_HEADER // "3rd/3rd_glfw3.h"
#undef timeGetTime
//---
#include "3rd/3rd_swrap.h"
//---
#include "3rd/3rd_jo_mp1.h"
#include "3rd/3rd_sfxr.h"
#define get_bits stb_vorbis_get_bits
#define error stb_vorbis_error
#include "3rd/3rd_stb_vorbis.h"
#undef error
#define channel jar_channel
#include "3rd/3rd_jar_mod.h"
#undef channel
#undef DEBUG
#include "3rd/3rd_jar_xm.h"
#include "3rd/3rd_sts_mixer.h"
#include "3rd/3rd_miniaudio.h"
//---
#undef L
#undef C
#undef R
#define error l_error
#define panic l_panic
#include "3rd/3rd_lua543.h"
//---
#include "3rd/3rd_stb_image.h"
#include "3rd/3rd_stb_image_write.h"
//---
#undef freelist
#include "3rd/3rd_nuklear.h"
#include "3rd/3rd_nuklear_glfw_gl3.h"
//---
#ifdef WITH_ASSIMP
#include "3rd/3rd_assimp.h"
#endif
#include "3rd/3rd_json5.h"
#include "3rd/3rd_gjk.h"
#include "3rd/3rd_compress.h"
#include "3rd/3rd_archive.h"
#ifdef __EMSCRIPTEN__
#define __linux__
#include "3rd/3rd_thread.h"
#undef __linux__
#else
#include "3rd/3rd_thread.h"
#endif
#include "3rd/3rd_plmpeg.h"
#include "3rd/3rd_jo_mpeg.h"
#include "3rd/3rd_https.h"

//-----------------------------------------------------------------------------
// compat (unix & stdio.h)

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <sched.h>    // CPU_ZERO, CPU_COUNT
#endif

#ifdef _MSC_VER
#define alloca        _alloca
#define atoi64        _atoi64
#define popen         _popen
#define pclose        _pclose
//#define strncasecmp   _strnicmp
#define ftruncate     _chsize_s
#define mkdir(p,m)    mkdir(p)
#define chdir         _chdir
#else // gcc
#include <alloca.h>
#include <strings.h> // strncasecmp
#define atoi64        atoll
//#define strstri       strcasestr
//#define strcmpi       strcasecmp
#endif

#if   defined MAX_PATH
#define MAX_PATHFILE MAX_PATH
#elif defined PATH_MAX
#define MAX_PATHFILE PATH_MAX
#else
#define MAX_PATHFILE 260
#endif

#ifdef _MSC_VER
#include <stdio.h>
#include <windows.h>
#include <share.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
FILE *fmemopen(void *buf, size_t len, const char *type) {
    int fd = -1;
    char temppath[MAX_PATHFILE - 14], filename[MAX_PATHFILE + 1];
    if( GetTempPathA(sizeof(temppath), temppath) )
    if( GetTempFileNameA(temppath, "fwk_temp", 0, filename) )
    if( !_sopen_s(&fd, filename, _O_CREAT | _O_SHORT_LIVED | _O_TEMPORARY | _O_RDWR | _O_BINARY | _O_NOINHERIT, _SH_DENYRW, _S_IREAD | _S_IWRITE) )
    for( FILE *fp = fd != -1 ? _fdopen(fd, "w+b") : NULL; fp; )
    return fwrite(buf, len, 1, fp), rewind(fp), fp, unlink(filename); // no need to _close. fclose(on the returned FILE*) also _closes the file descriptor.
    return fd != -1 ? _close(fd), NULL : NULL;
}
#endif

#ifndef _WIN32
#define tmpnam tmpnam2
static char *tmpnam(char *x) {
    (void)x;
    return stringf("%s/fwk-temp.%d", P_tmpdir, rand());
}
#endif

// ----------------------------------------------------------------------------
// compat (string.h)

//#define strstri       strcasestr
#define strcmpi       strcasecmp
#ifdef _MSC_VER
#define strcasecmp    _stricmp
#endif

#if 0
static
const char *pathfile_from_handle(FILE *fp) {
#if is(win32)
    int fd = fileno(fp);
    HANDLE handle = (HANDLE)_get_osfhandle( fd ); // <io.h>
    DWORD size = GetFinalPathNameByHandleW(handle, NULL, 0, VOLUME_NAME_DOS);
    wchar_t name[MAX_PATHFILE] = L"";
    size = GetFinalPathNameByHandleW(handle, name, size, VOLUME_NAME_DOS);
    name[size] = L'\0';
    return wchar16to8(name + 4); // skip \\?\ header
#else
    // In Linux, you can use readlink on /proc/self/fd/NNN where NNN is the file descriptor
    // In OSX:
    //     #include <sys/syslimits.h>
    //     #include <fcntl.h>
    //     char filePath[MAX_PATHFILE];
    //     if (fcntl(fd, F_GETPATH, filePath) != -1) {
    //         // do something with the file path
    //     }
    return 0;
#endif
}
#endif

static
int strcmp_qsort(const void *a, const void *b) {
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

static
bool strbegini(const char *a, const char *b) { // returns true if both strings match at beginning. case insensitive
    int la = strlen(a), lb = strlen(b);
    if( la < lb ) return 0;
    if( lb == 0 ) return 1;
    int len = la < lb ? la : lb;
    for( int i = 0; i < len; ++i ) {
        if( tolower(a[i]) != tolower(b[i]) ) {
            return false;
        }
    }
    return true;
}
static
bool strendi(const char *src, const char *sub) { // returns true if both strings match at end. case insensitive
    int srclen = strlen(src);
    int sublen = strlen(sub);
    if( sublen > srclen ) return 0;
    return !strcmpi(src + srclen - sublen, sub);
}


// Find substring in string, case insensitive. Alias for strcasestr()
// Returns first char of coincidence, or NULL.
static const char *strstri( const char *str, const char *find ){
    while( *str++ ) {
        for( const char *s = str-1, *f = find, *c = s; ; ++f, ++c) {
            if(!*f) return s;
            if(!*c) return NULL;
            if(tolower(*c) != tolower(*f)) break;
        }
    }
    return NULL;
}

// Safely concatenate two strings. Always NUL terminates (unless dstcap == 0).
// Returns length of operation; if retval >= dstcap, truncation occurred.
static size_t strlcat(char *dst, const char *src, size_t dstcap) {
    int dl = strlen(dst), sl = strlen(src);
    if( dstcap ) snprintf(dst + dl, dstcap - dl, "%s", src);
    return dl + sl;
}
// Safely copy two strings. Always NUL terminates (unless dstcap == 0).
// Copy src to string dst of size dstcap. Copies at most dstcap-1 characters.
// Returns length of input; if retval >= dstcap, truncation occurred.
static size_t strlcpy(char *dst, const char *src, size_t dstcap) {
    int sl = strlen(src);
    if( dstcap ) snprintf(dst, dstcap, "%*s", sl, src);
    return sl;// count does not include NUL
}

static char *strswap( char *copy, const char *target, const char *replacement ) {
    // replaced only if new text is shorter than old one
    int rlen = strlen(replacement), diff = strlen(target) - rlen;
    if( diff >= 0 ) {
        for( char *s = copy, *e = s + strlen(copy); /*s < e &&*/ 0 != (s = strstr(s, target)); ) {
            if( rlen ) s = (char*)memcpy( s, replacement, rlen ) + rlen;
            if( diff ) memmove( s, s + diff, (e - (s + diff)) + 1 );
        }
    }
    return copy;
}
static char *strcut( char *copy, const char *target ) {
  return strswap(copy, target, "");
}

static char *wchar16to8_(const wchar_t *str) { // from wchar16(win) to utf8/ascii
    int i = 0;
    int n = wcslen(str) * 6 - 1;
    static __thread char buffer[2048]; assert( n < 2048 );
    while( *str ) {
       if (*str < 0x80) {
          if (i+1 > n) return NULL;
          buffer[i++] = (char) *str++;
       } else if (*str < 0x800) {
          if (i+2 > n) return NULL;
          buffer[i++] = 0xc0 + (*str >> 6);
          buffer[i++] = 0x80 + (*str & 0x3f);
          str += 1;
       } else if (*str >= 0xd800 && *str < 0xdc00) {
          uint32_t c;
          if (i+4 > n) return NULL;
          c = ((str[0] - 0xd800) << 10) + ((str[1]) - 0xdc00) + 0x10000;
          buffer[i++] = 0xf0 + (c >> 18);
          buffer[i++] = 0x80 + ((c >> 12) & 0x3f);
          buffer[i++] = 0x80 + ((c >>  6) & 0x3f);
          buffer[i++] = 0x80 + ((c      ) & 0x3f);
          str += 2;
       } else if (*str >= 0xdc00 && *str < 0xe000) {
          return NULL;
       } else {
          if (i+3 > n) return NULL;
          buffer[i++] = 0xe0 + (*str >> 12);
          buffer[i++] = 0x80 + ((*str >> 6) & 0x3f);
          buffer[i++] = 0x80 + ((*str     ) & 0x3f);
          str += 1;
       }
    }
    buffer[i] = 0;
    return buffer;
}
static char *wchar16to8(const wchar_t *str) { // from wchar16(win) to utf8/ascii
    char *result = wchar16to8_(str);
    return stringf("%s", result ? result : "");
}

static
char *strrepl(char **string, const char *target, const char *replace) {
    char *buf = 0, *aux = *string;
    for( int tgtlen = strlen(target); tgtlen && aux[0]; ) {
        char *found = strstr(aux, target);
        if( found ) {
            buf = stringf_cat(buf, "%.*s%s", (int)(found - aux), aux, replace);
            aux += (found - aux) + tgtlen;
        } else {
            buf = stringf_cat(buf, "%s", aux);
            break;
        }
    }
    if( buf ) {
        strcpy(*string, buf);
        FREE( buf );
    }
    return *string;
}

#define strlerp(numpairs, pairs, ...) strlerp(numpairs, pairs, stringf(__VA_ARGS__))
static
const char *(strlerp)(unsigned numpairs, const char **pairs, const char *str) { // pairs is null-terminated
    if( !pairs[0] ) {
        return str;
    }
    // find & replace all tokens; @fixme: optimize me
    char *buf = REALLOC(0, 64*1024); strcpy(buf, str);
    for( unsigned i = 0; i < numpairs; ++i ) {
        const char *token = pairs[i*2+0];
        const char *repl = pairs[i*2+1];
        while(strstr(buf, token)) {
            strrepl(&buf, token, repl);
        }
    }
    char *ret = stringf("%s", buf);
    FREE(buf);
    return ret;
}

// -----------------------------------------------------------------------------
// if/n/def hell

#define ifdef(macro, yes, /*no*/...)   ifdef_##macro(yes, __VA_ARGS__)
#define ifndef(macro, yes, /*no*/...)  ifdef_##macro(__VA_ARGS__, yes)
#define is(macro)                      ifdef_##macro(1,0)
#define isnt(macro)                    ifdef_##macro(0,1)
#define ifdef_true(yes, /*no*/...)     yes
#define ifdef_false(yes, /*no*/...)    __VA_ARGS__

#ifdef _MSC_VER
#define ifdef_vc                       ifdef_true
#define ifdef_gcc                      ifdef_false
#else
#define ifdef_gcc                      ifdef_true
#define ifdef_vc                       ifdef_false
#endif
#ifdef __cplusplus
#define ifdef_cpp                      ifdef_true
#define ifdef_c                        ifdef_false
#else
#define ifdef_c                        ifdef_true
#define ifdef_cpp                      ifdef_false
#endif
#ifdef _WIN32
#define ifdef_win32                    ifdef_true
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_false
#elif defined __linux__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_true
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_false
#elif defined __APPLE__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_true
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_false
#elif defined __EMSCRIPTEN__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_false
#define ifdef_ems                      ifdef_true
#else // __FreeBSD__ || @todo: __ANDROID_API__
#define ifdef_win32                    ifdef_false
#define ifdef_linux                    ifdef_false
#define ifdef_osx                      ifdef_false
#define ifdef_bsd                      ifdef_true
#define ifdef_ems                      ifdef_false
#endif

// -----------------------------------------------------------------------------
// new C keywords
// @todo: autorun (needed?)

#define countof(x)       (sizeof (x) / sizeof 0[x])

#define macro(name)      concat(name, __LINE__)
#define concat(a,b)      conc4t(a,b)
#define conc4t(a,b)      a##b

#define benchmark        for(double macro(t) = -time_ss(); macro(t) < 0; printf("%.2fs\n", macro(t)+=time_ss()))
#define do_once          static macro(once) = 0; for(;!macro(once);macro(once)=1)
#define defer(begin,end) for(int macro(i) = ((begin), 0); !macro(i); macro(i) = ((end), 1))
#define scope(end)       defer((void)0, end)

#define local            __thread // ifdef(vc, __declspec(thread), __thread) // __STDC_VERSION_ >= 201112L: _Thread_local

#define cast(T)          ifdef(c, void *, decltype(T))
#define literal(T)       ifdef(c, T, (T))

//-----------------------------------------------------------------------------
// new C macros

#define ASSERT(expr, ...)   do { int fool_msvc[] = {0,}; if(!(expr)) { fool_msvc[0]++; breakpoint(stringf("!Expression failed: " #expr " " FILELINE "\n" __VA_ARGS__)); } } while(0)
#define PRINTF(...)         PRINTF(stringf(__VA_ARGS__), 1[#__VA_ARGS__] == '!' ? callstack(+48) : "", __FILE__, __LINE__, __FUNCTION__)

#define FILELINE            __FILE__ ":" STRINGIZE(__LINE__)
#define STRINGIZE(x)        STRINGIZ3(x)
#define STRINGIZ3(x)        #x

#define EXPAND(name, ...)          EXPAND_QUOTE(EXPAND_JOIN(name, EXPAND_COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))
#define EXPAND_QUOTE(x, y)         x y
#define EXPAND_JOIN(name, count)   EXPAND_J0IN(name, count)
#define EXPAND_J0IN(name, count)   EXPAND_J01N(name, count)
#define EXPAND_J01N(name, count)   name##count
#define EXPAND_COUNT_ARGS(...)     EXPAND_ARGS((__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define EXPAND_ARGS(args)          EXPAND_RETURN_COUNT args
#define EXPAND_RETURN_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, count, ...) count

// -----------------------------------------------------------------------------
// profiler & stats (@fixme: threadsafe)

#if WITH_PROFILE
#   define profile_init() do { map_init(profiler, less_str, hash_str); } while(0)
#   define profile(...) for( \
        struct profile_t *found = map_find_or_add(profiler, #__VA_ARGS__ "@" FILELINE, (struct profile_t){NAN} ), *dummy = (\
        found->cost = -time_ms() * 1000, found); found->cost < 0; found->cost += time_ms() * 1000, found->avg = found->cost * 0.25 + found->avg * 0.75)
#   define profile_incstat(name, accum) do { if(profiler) { \
        struct profile_t *found = map_find(profiler, name); \
        if(!found) found = map_insert(profiler, name, (struct profile_t){0}); \
        found->stat += accum; \
        } } while(0)
#   define profile_render() if(profiler) do { \
        for(float _i = ui_begin("Profiler",0), _r; _i ; ui_end(), _i = 0) { \
            for each_map_ptr(profiler, const char *, key, struct profile_t, val ) \
                if( !isnan(val->stat) ) ui_slider2(stringf("Stat: %s", *key), (_r = val->stat, &_r), stringf("%.2f", val->stat)), val->stat = 0; \
            ui_separator(); \
            for each_map_ptr(profiler, const char *, key, struct profile_t, val ) \
                if( isnan(val->stat) ) ui_slider2(*key, (_r = val->avg/1000.0, &_r), stringf("%.2f ms", val->avg/1000.0)); \
        } } while(0)
struct profile_t { double stat; int32_t cost, avg; };
static map(char *, struct profile_t) profiler;
#else
#   define profile_init() do {} while(0)
#   define profile_incstat(name, accum) do {} while(0)
#   define profile(...) if(1) // for(int _p = 1; _p; _p = 0)
#   define profile_render()
#endif

// -----------------------------------------------------------------------------

void fwk_init();
static void fwk_pre_init_subsystems();
static void fwk_post_init_subsystems();
static void fwk_pre_swap_subsystems();

#define AUDIO_C
#define COLLIDE_C
#define COOKER_C
#define DATA_C
#define EDITOR_C
#define FILE_C
#define INPUT_C
#define MATH_C
#define MEMORY_C
#define NETWORK_C
#define RENDER_C
#define RENDERDD_C
#define SCENE_C
#define SCRIPT_C
#define SYSTEM_C
#define UI_C
#define VIDEO_C
#define WINDOW_C

#define  atexit(...) // hack to boost exit time. there are no critical systems that need to quit properly
#include "fwk_audio.h"
#include "fwk_collide.h"
#include "fwk_cooker.h"
#include "fwk_data.h"
#include "fwk_file.h"
#include "fwk_input.h"
#include "fwk_math.h"
#include "fwk_memory.h"
#include "fwk_network.h"
#include "fwk_render.h"
#include "fwk_renderdd.h"
#include "fwk_scene.h"
#include "fwk_script.h"
#include "fwk_system.h"
#include "fwk_ui.h"
#include "fwk_video.h"
#include "fwk_window.h"
#include "fwk_editor.h" // last in place, so it can use all internals from above headers

// ----------------------------------------------------------------------------

//static int threadlocal _thread_id;
//#define PRINTF(...)      (printf("%03d %07.3fs|%-16s|", (((unsigned)(uintptr_t)&_thread_id)>>8) % 1000, time_ss(), __FUNCTION__), printf(__VA_ARGS__), printf("%s", 1[#__VA_ARGS__] == '!' ? callstack(+48) : "")) // verbose logger

static void fwk_pre_init_subsystems() {
    profile_init();
    ddraw_init();
    sprite_init();

    // window_swap();

    script_init();
    audio_init(0);
}
static int fwk_unlock_pre_swap_events = 0;
static void fwk_post_init_subsystems() {
    // mount virtual filesystems
    for( int i = 0; i < 16; ++i) {
        if(!vfs_mount(stringf(".cook[%d].zip", i))) {
            // PANIC("cannot mount fs: .cook[%d].zip", i);
        }
    }

    // unlock preswap events
    fwk_unlock_pre_swap_events = 1;

    // init more subsystems
    scene_push();           // create an empty scene by default
    input_init();
    boot_time = -time_ss(); // measure boot time, this is continued in window_stats()

    // clean any errno setup by cooking stage
    errno = 0;
}
static void fwk_pre_swap_subsystems() {
    if( fwk_unlock_pre_swap_events ) {
        // flush all batched sprites before swap
        sprite_update();

        // queue ui drawcalls for profiler
        // hack: skip first frame, because of conflicts with ui_menu & ui_begin auto-row order
        static int once = 0; if(once) profile_render(); once = 1;

        // flush all debugdraw calls before swap
        dd_ontop = 0;
        ddraw_flush();
        glClear(GL_DEPTH_BUFFER_BIT);
        dd_ontop = 1;
        ddraw_flush();

        // flush all batched ui before swap (creates single dummy if no batches are found)
        ui_create();
        ui_render();
    }
}

static
void fwk_error_callback(int error, const char *description) {
    PANIC("%s", description);
}

void fwk_init() {
    static once; if(once) return; once=1;

    // init glfw
    glfwSetErrorCallback(fwk_error_callback);
    glfwInit();
    atexit(glfwTerminate);

    // init panic handler
    panic_oom_reserve = SYS_REALLOC(panic_oom_reserve, 4<<20); // 4MiB

    // enable ansi console
    tty_init();

    // create or update cook.zip file
#if WITH_COOKER
    cooker( "**", fwk_cook, 0|COOKER_ASYNC );
#endif
}

#endif
