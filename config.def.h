/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
static unsigned int borderpx = 3;     /* border pixel of windows */
static const unsigned int snap = 32;  /* snap pixel */
static const unsigned int gappih = 5; /* horiz inner gap between windows */
static const unsigned int gappiv = 5; /* vert inner gap between windows */
static const unsigned int gappoh =
    5; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    5; /* vert outer gap between windows and screen edge */
static const int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const int swallowfloating =
    1;                        /* 1 means swallow floating windows by default */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
// static double defaultopacity = 0.93;
static const char *fonts[] = {"Liberation Mono:size=11",
                              "Twitter Color Emoji:pixelsize=11",
                              "Noto Color Emoji:pizelsize=11"};
static const char dmenufont[] = "Liberation Mono:size = 11";

static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#005577";
static char selbgcolor[] = "#005577";

#define baralpha 0xd0
#define borderalpha OPAQUE

static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
};

const unsigned int alphas[][3] = {
    /*               fg      bg        border     */
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel] = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "📹", "🌐"};

/**
  const char *class;
  const char *instance;
  const char *title;
  unsigned int tags;
  int iscentered;
  int isfloating;
  int isterminal;
  int noswallow;
  int monitor;
 * */
static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     iscentered   isfloating
       isterminal  noswallow  monitor */
    //{ "Gimp",     NULL,       NULL,       0,            0,           1, -1 },
    {"brave-browser", NULL, NULL, 1 << 8, 0, 0, 0, 0, -1},
    {"qutebrowser", NULL, NULL, 1 << 8, 0, 0, 0, 0, -1},
    {"firefox", NULL, NULL, 1 << 8, 0, 0, 0, 0, -1},
    {"youdu", NULL, NULL, 1 << 7, 0, 0, 0, 0, -1},
    {"st-256color", NULL, NULL, 0, 0, 0, 1, 0, -1},
    {"St", "st", NULL, 0, 0, 0, 1, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 0, 1, -1}, /* xev */
};

/* layout(s) */
static float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static int nmaster = 1;     /* number of clients in master area */
static int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT                                                           \
  1 /* nrowgrid layout: force two clients to always split vertically */

#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    // t
    {"[]=", tile},   /* first entry is default */
    {"TTT", bstack}, /*backstack*/
    //{"===", bstackhoriz}, /*back horizon*/

    // y
    {"[@]", spiral},   /* fib  */
    {"[\\]", dwindle}, /* fib */

    // u
    {"|M|", centeredmaster},         /*center master*/
    {"~M~", centeredfloatingmaster}, /*center float*/

    // i
    {"[M]", monocle}, /* full screen */
    {"H[]", deck},    /*dech*/

    // [
    {"HHH", grid},     /**/
    {"###", nrowgrid}, /**/

    // ]
    {"---", horizgrid},   /**/
    {":::", gaplessgrid}, /**/

    {"><>", NULL}, /* no layout function means floating behavior */
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

#define STACKKEYS(MOD, ACTION)                                                 \
  {MOD, XK_j, ACTION##stack, {.i = INC(+1)}},                                  \
      {MOD, XK_k, ACTION##stack, {.i = INC(-1)}},                              \
      {MOD,                                                                    \
       XK_v,                                                                   \
       ACTION##stack,                                                          \
       {.i = 0}}, /* {MOD, XK_grave, ACTION##stack, {.i = PREVSEL}}, \ */
/* {MOD, XK_a, ACTION##stack, {.i = 1}},                                    \ */
/* {MOD, XK_z, ACTION##stack, {.i = 2}},                                    \ */
/* {MOD, XK_x, ACTION##stack, {.i = -1}}, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon,       "-fn",
                                 dmenufont,   "-nb", normbgcolor,    "-nf",
                                 normfgcolor, "-sb", selbordercolor, "-sf",
                                 selfgcolor,  NULL};

static const char *termcmd[] = {"st", NULL};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    // {"dmenufont", STRING, &dmenufont},

    {"color0", STRING, &normbordercolor},
    {"color8", STRING, &selbordercolor},
    {"color0", STRING, &normbgcolor},
    {"color4", STRING, &normfgcolor},
    {"color0", STRING, &selbgcolor},
    {"color4", STRING, &selfgcolor},
    //{"alpha", STRING, &defaultopacity},

    {"borderpx", INTEGER, &borderpx},
    /* {"snap", INTEGER, &snap}, */
    /* {"showbar", INTEGER, &showbar}, */
    /* {"topbar", INTEGER, &topbar}, */
    /* {"nmaster", INTEGER, &nmaster}, */
    /* {"resizehints", INTEGER, &resizehints}, */
    /* {"mfact", FLOAT, &mfact}, */
};

#include <X11/XF86keysym.h>

#define BIND_SET_LAYOUT(I, K)                                                  \
  {MODKEY, XK_##K, setlayout, {.v = &layouts[I]}}, {                           \
    MODKEY | ShiftMask, XK_##K, setlayout, { .v = &layouts[I + 1] }            \
  }

#include "keepfloatingposition.c"
static Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_d, spawn, SHCMD("show_emoji")},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_e, spawn, SHCMD("emacsclient -c -a \"''\"")},

    STACKKEYS(MODKEY, focus)            /* focus */
    STACKKEYS(MODKEY | ShiftMask, push) /* push  */

    {MODKEY | ShiftMask, XK_b, togglebar, {0}},

    // adjust layout, horizon or vertical
    {MODKEY, XK_o, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_o, incnmaster, {.i = -1}},

    // adjust height or width of the windows
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    // { MODKEY,                       XK_Return, zoom,           {0} },
    // ???
    {MODKEY, XK_Tab, view, {0}},

    // quit window
    {MODKEY, XK_q, killclient, {0}},

    ////////////////////// layouts
    BIND_SET_LAYOUT(0, t),
    BIND_SET_LAYOUT(2, y),
    BIND_SET_LAYOUT(4, u),
    BIND_SET_LAYOUT(6, i),
    BIND_SET_LAYOUT(8, bracketleft),
    BIND_SET_LAYOUT(10, bracketright),

    // cycle layouts
    {MODKEY, XK_space, cyclelayout, {.i = +1}},
    {MODKEY | ShiftMask, XK_space, cyclelayout, {.i = -1}},

    {MODKEY | ShiftMask, XK_F5, xrdb, {.v = NULL}},

    // float window or not
    {MODKEY, XK_0, view, {.ui = ~0}},

    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ControlMask, XK_q, quit, {0}},

    // function keys
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("xbacklight -inc 5")},
    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 5")},
    {0, XF86XK_AudioMute, spawn, SHCMD("pulsemixer --toggle-mute")},
    {0, XF86XK_AudioMicMute, spawn,
     SHCMD("pulsemixer --id source-1 --toggle-mute")},
    {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pulsemixer --change-volume +5")},
    {0, XF86XK_AudioLowerVolume, spawn, SHCMD("pulsemixer --change-volume -5")},

    // open apps
    {MODKEY | ShiftMask, XK_w, spawn, SHCMD("firefox-bin")},
    {MODKEY | ShiftMask, XK_e, spawn, SHCMD("everywhere")},
    {MODKEY | ShiftMask, XK_p, spawn, SHCMD("screenshot")},
    {MODKEY, XK_f, spawn, SHCMD("setsid st -e lf")},
    // toggle client status
    // float
    {MODKEY | ShiftMask, XK_f, togglefloating, {0}},
    // full screen
    {MODKEY, XK_m, togglefullscr, {0}},
    // sticky
    {MODKEY | ShiftMask, XK_s, togglesticky, {0}},
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument
     */

    // click status bar and do somwthing
    // show toph
    {ClkStatusText, 0, Button1, sigstatusbar, {.i = 1}},
    {ClkStatusText, 0, Button2, sigstatusbar, {.i = 2}},
    {ClkStatusText, 0, Button3, sigstatusbar, {.i = 3}},
    {ClkStatusText, 0, Button4, sigstatusbar, {.i = 4}},
    {ClkStatusText, 0, Button5, sigstatusbar, {.i = 5}},

    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, ShiftMask, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};

void setlayoutex(const Arg *arg) { setlayout(&((Arg){.v = &layouts[arg->i]})); }

void viewex(const Arg *arg) { view(&((Arg){.ui = 1 << arg->ui})); }

void viewall(const Arg *arg) { view(&((Arg){.ui = ~0})); }

void toggleviewex(const Arg *arg) { toggleview(&((Arg){.ui = 1 << arg->ui})); }

void tagex(const Arg *arg) { tag(&((Arg){.ui = 1 << arg->ui})); }

void toggletagex(const Arg *arg) { toggletag(&((Arg){.ui = 1 << arg->ui})); }

void tagall(const Arg *arg) { tag(&((Arg){.ui = ~0})); }

void set_xrdb(const Arg *arg) { xrdb(&((Arg){.v = NULL})); }

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"`
 */
static Signal signals[] = {
    /* signum           function */
    {"focusstack", focusstack},
    {"setmfact", setmfact},
    {"togglebar", togglebar},
    {"incnmaster", incnmaster},
    {"togglefloating", togglefloating},
    {"focusmon", focusmon},
    {"tagmon", tagmon},
    {"zoom", zoom},
    {"view", view},
    {"viewall", viewall},
    {"viewex", viewex},
    {"toggleview", view},
    {"toggleviewex", toggleviewex},
    {"tag", tag},
    {"tagall", tagall},
    {"tagex", tagex},
    {"toggletag", tag},
    {"toggletagex", toggletagex},
    {"killclient", killclient},
    {"quit", quit},
    {"setlayout", setlayout},
    {"setlayoutex", setlayoutex},
    {"xrdb", xrdb},
};
