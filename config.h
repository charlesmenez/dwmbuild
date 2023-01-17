/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx = 2;     /* border pixel of windows */
static const unsigned int gappx = 20; /* gaps between windows */
static unsigned int snap = 0;         /* snap pixel */
static const int swallowfloating =
    0;                  /* 1 means swallow floating windows by default */
static int showbar = 1; /* 0 means no bar */
static int topbar = 1;  /* 0 means bottom bar */
static const char *fonts[] = {"monospace:size=8"};
static const char dmenufont[] = "monospace:size=8";
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#005577";
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
    [SchemeTitle] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeTitleSel] = {selfgcolor, selbgcolor, selbordercolor},
};

static const char *statuscolors[][3] = {
    /*                   fg         bg         border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
    [SchemeTitle] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeTitleSel] = {selfgcolor, selbgcolor, selbordercolor},
};
/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class     instance  title           tags mask  isfloating  isterminal
       noswallow  monitor */
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, 0, -1, -1},
    {"St", NULL, NULL, 0, 0, 1, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1}, /* xev */
};

/* layout(s) */
static float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static int nmaster = 1;     /* number of clients in master area */
static int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[@]", spiral},  {"[\\]", dwindle},
    {"[M]", monocle}, {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon,       "-fn",
                                 dmenufont,   "-nb", normbgcolor,    "-nf",
                                 normfgcolor, "-sb", selbordercolor, "-sf",
                                 selfgcolor,  NULL};
static const char *termcmd[] = {"st", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = {"st", "-t",     scratchpadname,
                                      "-g", "120x34", NULL};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    {"color0", STRING, &normbordercolor},
    {"color4", STRING, &selbordercolor},
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
};

#include "selfrestart.c"
static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY | ControlMask, XK_r, self_restart, {0}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_f, fullscreen, {0}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY | ShiftMask, XK_Return, togglescratch, {.v = scratchpadcmd}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_space, zoom, {0}},
    {MODKEY, XK_s, togglesticky, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_q, killclient, {0}},
    {MODKEY | ControlMask, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_v, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_c, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_z, setlayout, {.v = &layouts[3]}},
    {MODKEY, XK_x, setlayout, {.v = &layouts[4]}},
    {MODKEY, XK_Escape, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY, XK_q, quit, {0}},
    {MODKEY, XK_r, spawn, {.v = (const char *[]){"st", "-e", "ranger", NULL}}},

    {MODKEY | ShiftMask,
     XK_r,
     spawn,
     {.v = (const char *[]){"st", "-e", "htop", NULL}}},

    {MODKEY | ShiftMask,
     XK_s,
     spawn,
     {.v = (const char *[]){"flameshot", "gui", NULL}}},

    {MODKEY, XK_BackSpace, spawn, {.v = (const char *[]){"sysact", NULL}}},

    {MODKEY | ShiftMask,
     XK_d,
     spawn,
     {.v = (const char *[]){"keepassxc", NULL}}},

    {MODKEY, XK_y, spawn, {.v = (const char *[]){"ytfzf", "-D", NULL}}},
    {MODKEY, XK_w, spawn, {.v = (const char *[]){"firefox", NULL}}},

    {MODKEY | ControlMask,
     XK_m,
     spawn,
     {.v = (const char *[]){"Dmenu-Streaming", NULL}}},

    {MODKEY | ControlMask,
     XK_y,
     spawn,
     {.v = (const char *[]){"st", "-e", "ytccf", "--no-thumbnails", NULL}}},

    {MODKEY | ControlMask,
     XK_b,
     spawn,
     {.v = (const char *[]){"bookmarks", NULL}}},

    {MODKEY | ShiftMask,
     XK_y,
     spawn,
     {.v = (const char *[]){"youtube-dmenu", NULL}}},

    {MODKEY, XK_t, spawn, {.v = (const char *[]){"telegram-choice", NULL}}},

    {MODKEY,
     XK_m,
     spawn,
     {.v = (const char *[]){"st", "-e", "termusic", NULL}}},

    {MODKEY | ShiftMask, XK_m, spawn, SHCMD("st -e pulsemixer")},

    {MODKEY, XK_p, spawn, SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png")},

    {MODKEY | ShiftMask,
     XK_w,
     spawn,
     {.v = (const char *[]){"dwm-colors", NULL}}},

    {MODKEY | ControlMask,
     XK_s,
     spawn,
     {.v = (const char *[]){"Dmenu-Streaming", NULL}}},

    {MODKEY, XK_e, spawn, {.v = (const char *[]){"dmenu-emoji", NULL}}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
