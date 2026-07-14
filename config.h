/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* alt-tab configuration */
static const unsigned int tabModKey = 0x40;   /* if this key is hold the alt-tab functionality stays acitve. This key must be the same as key that is used to active functin altTabStart `*/
static const unsigned int tabCycleKey = 0x17; /* if this key is hit the alt-tab program moves one position forward in clients stack. This key must be the same as key that is used to active functin altTabStart */
static const unsigned int tabPosY = 1;        /* tab position on Y axis, 0 = bottom, 1 = center, 2 = top */
static const unsigned int tabPosX = 1;        /* tab position on X axis, 0 = left, 1 = center, 2 = right */
static const unsigned int maxWTab = 600;      /* tab menu width */
static const unsigned int maxHTab = 200;      /* tab menu height */

/* appearance */
static unsigned int borderpx = 3; /* border pixel of windows */
static unsigned int snap = 32;    /* snap pixel */
static unsigned int gappih = 20;  /* horiz inner gap between windows */
static unsigned int gappiv = 10;  /* vert inner gap between windows */
static unsigned int gappoh = 10;  /* horiz outer gap between windows and screen edge */
static unsigned int gappov = 30;  /* vert outer gap between windows and screen edge */
static int swallowfloating = 0;   /* 1 means swallow floating windows by default */
static int smartgaps = 0;         /* 1 means no outer gap when there is only one window */
static int showbar = 1;           /* 0 means no bar */
static int topbar = 1;            /* 0 means bottom bar */
static char *fonts[] = {
    "monospace:size=10",
    "HackNerdFontMono:size=10",
    "NotoColorEmoji:pixelsize=8:antialias=true:autohint=true"};
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#770000";
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
};

typedef struct
{
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-f", "HackNerdFontMono:size=12", "-g", "100x30", "-e", "env", "NO_TMUX=1", "bash", NULL};
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "HackNerdFontMono:size=12", "-g", "100x30", "-e", "qalc", NULL};
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm", spcmd1},
    {"spcalc", spcmd2},
};

/* tagging */
static const char *tags[] = {"b", "i", "g", "p", "e", "n", "g", "u", "s"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
    {TERMCLASS, NULL, NULL, 0, 0, 1, 0, -1},
    {TERMCLASS, "floatterm", NULL, 0, 1, 1, 0, -1},
    {TERMCLASS, "bg", NULL, 1 << 7, 0, 1, 0, -1},
    {TERMCLASS, "spterm", NULL, SPTAG(0), 1, 1, 0, -1},
    {TERMCLASS, "spcalc", NULL, SPTAG(1), 1, 1, 0, -1},
};

/* layout(s) */
static float mfact = 0.55;           /* factor of master area size [0.05..0.95] */
static int nmaster = 1;              /* number of clients in master area */
static int resizehints = 0;          /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int lpm[] = {0, 1};     /* Index of preferred layout, if LENGTH(lpm)<#monitors -> default layout */
#define FORCE_VSPLIT 1               /* nrowgrid layout: force two clients to always split vertically */

#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol                        arrange function */
    {"[]=", tile},                   /* Default: Master on left, slaves on right */
    {"=[]", rtile},                  /* Master on right, slaves on left */
    {"TTT", bstack},                 /* Master on top, slaves on bottom */
    {"[@]", spiral},                 /* Fibonacci spiral */
    {"[\\]", dwindle},               /* Decreasing in size right and leftward */
    {"[D]", deck},                   /* Master on left, slaves in monocle-like mode on right */
    {"[M]", monocle},                /* All windows on top of eachother */
    {"|M|", centeredmaster},         /* Master in middle, slaves on sides */
    {">M>", centeredfloatingmaster}, /* Same but master floats */
    {"><>", NULL},                   /* no layout function means floating behavior */
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
    {MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
        {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
        {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
        {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                           \
    {                                                        \
        .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
    }

/* commands */
static const char *termcmd[] = {TERMINAL, NULL};
/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    {"color0", STRING, &normbordercolor},
    {"color8", STRING, &selbordercolor},
    {"color0", STRING, &normbgcolor},
    {"color4", STRING, &normfgcolor},
    {"color0", STRING, &selfgcolor},
    {"color4", STRING, &selbgcolor},
    {"borderpx", INTEGER, &borderpx},
    {"snap", INTEGER, &snap},
    {"showbar", INTEGER, &showbar},
    {"topbar", INTEGER, &topbar},
    {"nmaster", INTEGER, &nmaster},
    {"resizehints", INTEGER, &resizehints},
    {"mfact", FLOAT, &mfact},
    {"gappih", INTEGER, &gappih},
    {"gappiv", INTEGER, &gappiv},
    {"gappoh", INTEGER, &gappoh},
    {"gappov", INTEGER, &gappov},
    {"swallowfloating", INTEGER, &swallowfloating},
    {"smartgaps", INTEGER, &smartgaps},
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
    /* modifier                     key        function        argument */
    TAGKEYS(XK_1, 0)
        TAGKEYS(XK_2, 1)
            TAGKEYS(XK_3, 2)
                TAGKEYS(XK_4, 3)
                    TAGKEYS(XK_5, 4)
                        TAGKEYS(XK_6, 5)
                            TAGKEYS(XK_7, 6)
                                TAGKEYS(XK_8, 7)
                                    TAGKEYS(XK_9, 8)
    {MODKEY, XK_Tab, viewnext, {0}},
    {MODKEY | ShiftMask, XK_Tab, viewprev, {0}},
    /* keys set by me*/
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY, XK_w, spawn, {.v = (const char *[]){BROWSER, NULL}}},
    {MODKEY, XK_e, spawn, {.v = (const char *[]){"dolphin", NULL}}},
    {MODKEY, XK_r, spawn, {.v = (const char *[]){"dmenu_run", NULL}}},
    {MODKEY, XK_t, spawn, {.v = termcmd}},
    {MODKEY | ShiftMask, XK_t, togglescratch, {.ui = 0}},
    {MODKEY, XK_y, togglesmartgaps, {0}},
    {MODKEY, XK_u, incnmaster, {.i = +1}},
    {MODKEY, XK_i, incnmaster, {.i = -1}},
    {MODKEY, XK_o, spawn, {.v = (const char *[]){TERMINAL, "-t", "lf", "-e", "lfub", NULL}}},
    {MODKEY, XK_p, spawn, {.v = (const char *[]){BROWSER, "--private-window", NULL}}},
    {MODKEY | ShiftMask, XK_p, spawn, {.v = (const char *[]){"mpc", "toggle", NULL}}},
    {MODKEY, XK_minus, spawn, SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -44 $(pidof dwmblocks)")},
    {MODKEY, XK_equal, spawn, SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -44 $(pidof dwmblocks)")},
    {MODKEY, XK_bracketleft, cyclelayout, {.i = -1}},
    {MODKEY, XK_bracketright, cyclelayout, {.i = +1}},
    {MODKEY, XK_a, togglegaps, {0}},
    {MODKEY | ShiftMask, XK_a, defaultgaps, {0}},
    {MODKEY, XK_s, togglesticky, {0}},
    {MODKEY, XK_d, togglescratch, {.ui = 1}},
    {MODKEY, XK_f, togglefullscr, {0}},
    {MODKEY, XK_g, view, {0}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_j, focusstack, {.i = INC(+1)}},
    {MODKEY, XK_k, focusstack, {.i = INC(-1)}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_h, spawn, {.v = (const char *[]){TERMINAL, "-e", "man", "dwm", NULL}}},
    {MODKEY | ShiftMask, XK_j, pushstack, {.i = INC(+1)}},
    {MODKEY | ShiftMask, XK_k, pushstack, {.i = INC(-1)}},
    {MODKEY | ShiftMask, XK_l, spawn, {.v = (const char *[]){"slock", NULL}}},
    {MODKEY, XK_semicolon, focusmon, {.i = -1}},
    {MODKEY, XK_apostrophe, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_semicolon, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_apostrophe, tagmon, {.i = +1}},
    {MODKEY, XK_z, incrgaps, {.i = -3}},
    {MODKEY, XK_x, incrgaps, {.i = +3}},
    {MODKEY, XK_c, zoom, {0}},
    {MODKEY, XK_v, togglefloating, {0}},
    {MODKEY | ShiftMask , XK_v, spawn, {.v = (const char *[]){"clipmenu", NULL}}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_n, spawn, {.v = (const char *[]){TERMINAL, "-e", "nvim", NULL}}},
    {MODKEY | ShiftMask, XK_n, spawn, {.v = (const char *[]){TERMINAL, "-e", "weechat", NULL}}},
    {MODKEY, XK_m, spawn, {.v = (const char *[]){TERMINAL, "-e", "ncmpcpp-art", NULL}}},
    {MODKEY | ShiftMask, XK_m, spawn, SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)")},
    {MODKEY, XK_comma, spawn, {.v = (const char *[]){"mpc", "prev", NULL}}},
    {MODKEY, XK_period, spawn, {.v = (const char *[]){"mpc", "next", NULL}}},
    {MODKEY, XK_slash, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_slash, tag, {.ui = ~0}},
    {Mod1Mask, XK_Tab, altTabStart, {0}},
    {MODKEY, XK_space, spawn, {.v = (const char *[]){"devnav", NULL}}},
    {0, XK_Print, spawn, {.v = (const char *[]){"maimpick", "area", NULL}}},
    {ControlMask, XK_Print, spawn, {.v = (const char *[]){"maimpick", "window", NULL}}},
    {MODKEY, XK_Print, spawn, {.v = (const char *[]){"maimpick", "screen", NULL}}},
    {Mod1Mask, XK_Print, spawn, {.v = (const char *[]){"maimpick", "all screens", NULL}}},
    {MODKEY, XK_F1, spawn, {.v = (const char *[]){TERMINAL, "-e", "man", "dwm", NULL}}},
    {MODKEY, XK_F2, spawn, {.v = (const char *[]){"toggle-audio-devices.sh", NULL}}},
    {MODKEY, XK_F3, spawn, {.v = (const char *[]){TERMINAL, "-e", "cava", NULL}}},
    {MODKEY, XK_F4, spawn, SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)")},
    {MODKEY, XK_F5, xrdb, {.v = NULL}},
    {MODKEY, XK_F6, spawn, {.v = (const char *[]){"toggle-vpn.sh", NULL}}},
    {MODKEY, XK_BackSpace, spawn, {.v = (const char *[]){"sysact", NULL}}},
    {MODKEY | ShiftMask, XK_BackSpace, spawn, {.v = (const char *[]){"sysact", "delay", NULL}}},

    /* function/laptop keys*/
    {0, XF86XK_AudioMute, spawn, SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%- && wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_AudioLowerVolume, spawn, SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%+ && wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_AudioPrev, spawn, {.v = (const char *[]){"mpc", "prev", NULL}}},
    {0, XF86XK_AudioNext, spawn, {.v = (const char *[]){"mpc", "next", NULL}}},
    {0, XF86XK_AudioPause, spawn, {.v = (const char *[]){"mpc", "pause", NULL}}},
    {0, XF86XK_AudioPlay, spawn, {.v = (const char *[]){"mpc", "toggle", NULL}}},
    {0, XF86XK_AudioStop, spawn, {.v = (const char *[]){"mpc", "stop", NULL}}},
    {0, XF86XK_AudioRewind, spawn, {.v = (const char *[]){"mpc", "seek", "-10", NULL}}},
    {0, XF86XK_AudioForward, spawn, {.v = (const char *[]){"mpc", "seek", "+10", NULL}}},
    {0, XF86XK_AudioMedia, spawn, {.v = (const char *[]){TERMINAL, "-e", "ncmpcpp", NULL}}},
    {0, XF86XK_AudioMicMute, spawn, SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle")},
    {0, XF86XK_TouchpadToggle, spawn, SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1")},
    {0, XF86XK_TouchpadOff, spawn, {.v = (const char *[]){"synclient", "TouchpadOff=1", NULL}}},
    {0, XF86XK_TouchpadOn, spawn, {.v = (const char *[]){"synclient", "TouchpadOff=0", NULL}}},
    {0, XF86XK_MonBrightnessUp, spawn, {.v = (const char *[]){"xbacklight", "-inc", "15", NULL}}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = (const char *[]){"xbacklight", "-dec", "15", NULL}}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button1, sigdwmblocks, {.i = 1}},
    {ClkStatusText, 0, Button2, sigdwmblocks, {.i = 2}},
    {ClkStatusText, 0, Button3, sigdwmblocks, {.i = 3}},
    {ClkStatusText, 0, Button4, sigdwmblocks, {.i = 4}},
    {ClkStatusText, 0, Button5, sigdwmblocks, {.i = 5}},
    {ClkStatusText, ShiftMask, Button1, sigdwmblocks, {.i = 6}},
#endif
    {ClkStatusText, ShiftMask, Button3, spawn, {.v = (const char *[]){TERMINAL, "-e", "nvim", "Documents/Github/Repos/dwmblocks/config.h", NULL}}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, defaultgaps, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkClientWin, MODKEY, Button4, incrgaps, {.i = +1}},
    {ClkClientWin, MODKEY, Button5, incrgaps, {.i = -1}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
    {ClkTagBar, 0, Button4, shiftview, {.i = -1}},
    {ClkTagBar, 0, Button5, shiftview, {.i = 1}},
    {ClkRootWin, 0, Button2, togglebar, {0}},
};
