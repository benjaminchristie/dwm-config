/* See LICENSE file for copyright and license details. */

/* appearance */
// systray

static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayonleft =
    1; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 3; /* systray spacing */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const int showsystray = 0; /* 0 means no systray */
// end systray
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 4;     /* snap pixel */
static const unsigned int gappih = 8; /* 12, horiz inner gap between windows */
static const unsigned int gappiv = 8; /* 25, vert inner gap between windows */
static const unsigned int gappoh =
    38; /* 4, horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    16; /* 6,  vert outer gap between windows and screen edge */
static int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const int showbar = 0; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
static const int usealtbar = 1;

static const char *fonts[] = {"RedHatMono:size=10:style=Medium"};
static const char dmenufont[] = "RedHatMono:size=12:style=Medium";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_white[] = "#ffffff";
static const char col_cyan[] = "#005577";
static const char col_red[] = "#9E0000";
static const char col_mag[] = "#4e2878";
static const char col_grn[] = "#224005";
static const char col_yel[] = "#c4a000";
static const char col_black[] = "#000000";
static const char col_darkblue[] = "#031942";
static const char col_orangeish[] = "#471b0a";

static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_white, col_gray1, col_gray1},
    [SchemeSel] = {col_white, col_mag, col_gray2},
    [SchemeStatus] = {col_white, col_gray1,
                      "#000000"},  // Statusbar right {text,background,not used
                                   // but cannot be empty}
    [SchemeTagsSel] = {col_white, col_mag,
                       "#000000"},  // Tagbar left selected {text,background,not
                                    // used but cannot be empty}
    [SchemeTagsNorm] =
        {col_white, col_gray1,
         "#000000"},  // Tagbar left unselected {text,background,not used but
                      // cannot be empty}
    [SchemeInfoSel] =
        {col_white, col_gray1,
         "#000000"},  // infobar middle  selected {text,background,not used but
                      // cannot be empty}
    [SchemeInfoNorm] =
        {col_white, col_gray1,
         "#000000"},  // infobar middle  unselected {text,background,not used
                      // but cannot be empty}
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;   /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT \
    1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile},    /* first entry is default */
    {"[M]", monocle}, /* first entry is default */
    {"[@]", spiral},
    //{ "[\\]",     dwindle },
    {"H[]", deck},
    {"TTT", bstack},
    //{ "===",      bstackhoriz },
    //{ "HHH",      grid },
    {"###", nrowgrid},
    //{ "---",      horizgrid },
    //{ ":::",      gaplessgrid },
    {"|M|", centeredmaster},
    //{ ">M>",      centeredfloatingmaster },
    {"><>", NULL}, /* no layout function means floating behavior */
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
#include <X11/XF86keysym.h>  // for audio keyboard shortcuts
/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-c",    "-g",      "4",       "-l",      "10",  "-m",
    dmenumon,    "-fn",   dmenufont, "-nb",     col_gray1, "-nf", col_gray3,
    "-sb",       col_mag, "-sf",     col_gray4, NULL};
static const char *roficmd[] = {
    "rofi", "-combi-modi", "drun,ssh",
    "-font", "\"RedHatText 16\"", "-show", "combi", "-icon-theme",
    "\"Paper\"", "-show-icons", "-m", "-auto-select", "monitor", "-5"};

static const char *rofifilescmd[] = {"rofi", "-mode", "filebrowser", "-theme",
                                     //"base16-tomorrow-night",
                                     "tokyonight", "-font", "\"RedHatText 16\"",
                                     "-show", "filebrowser", "-icon-theme",
                                     "\"Paper\"", "-show-icons", "-m",
                                     "-auto-select", "monitor", "-5"};
static const char *rofiwindowscmd[] = {
    "rofi", "-mode", "window", 
    //"base16-tomorrow-night",
    "-font", "\"RedHatText 16\"", "-show", "window",
    "-icon-theme", "\"Paper\"", "-show-icons", "-m", "-auto-select", "monitor",
    "-5"};
static const char *termcmd[] = {"alacritty", NULL};
static const char *xsecurelock[] = {"xsecurelock", NULL};
static const char *quitcmd[] = {"safe_dwm_quit", NULL};
static const char *firefoxcmd[] = {"firefox", NULL};
static const char *chromecmd[] = {"chromium", NULL};
static const char *pcmanfmcmd[] = {"thunar", NULL};
static const char *mutecmd[] = {"pactl", "set-sink-mute", "@DEFAULT_SINK@",
                                "toggle", NULL};
static const char *volupcmd[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK@",
                                 "+5%", NULL};
static const char *voldowncmd[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK@",
                                   "-5%", NULL};
static const char *brupkbcmd[] = {
    "brightnessctl", "--device='spi::kbd_backlight", "set", "+50%", NULL};
static const char *brdownkbcmd[] = {
    "brightnessctl", "--device='spi::kbd_backlight", "set", "50%-", NULL};
static const char *brupcmd[] = {"light", "-A", "5", NULL};
static const char *brdowncmd[] = {"light", "-U", "5", NULL};
static const char *shotcmd[] = {"flameshot", "gui", NULL};
static const char *polybartogglecmd[] = {"toggle-polybar", NULL};
/* static const char *emacscmd[] = {"emacsclient", "-c", "-a", "'emacs'", NULL};
 */
// previous 5 lines were for volume and backlight control
static Key keys[] = {
/* modifier                     key        function        argument */
#define KeyboardUp 0x10081000 + 0x0ef
#define KeyboardDown 0x10081000 + 0x0f0
    {0, XF86XK_AudioMute, spawn, {.v = mutecmd}},
    {0, XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd}},
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd}},
    {0, XF86XK_KbdBrightnessUp, spawn, {.v = brupkbcmd}},
    {0, XF86XK_KbdBrightnessDown, spawn, {.v = brdownkbcmd}},
    {0, KeyboardUp, spawn, {.v = brupkbcmd}},
    {0, KeyboardDown, spawn, {.v = brdownkbcmd}},
    {0, XF86XK_MonBrightnessUp, spawn, {.v = brupcmd}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd}},
    {MODKEY | ShiftMask, XK_l, spawn, {.v = polybartogglecmd}},
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_a, spawn, {.v = termcmd}},
    {MODKEY | ShiftMask, XK_m, spawn, {.v = rofiwindowscmd}},
    {MODKEY | ShiftMask, XK_f, spawn, {.v = rofifilescmd}},
    {MODKEY | ShiftMask, XK_s, spawn, {.v = shotcmd}},
    {MODKEY | Mod1Mask | ShiftMask, XK_Return, spawn, {.v = chromecmd}},
    {MODKEY | Mod1Mask | ShiftMask, XK_p, spawn, {.v = pcmanfmcmd}},
    {MODKEY, XK_m, spawn, {.v = roficmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY | Mod1Mask, XK_4, spawn, {.v = brdowncmd}},
    {MODKEY | Mod1Mask, XK_5, spawn, {.v = brupcmd}},
    {MODKEY | Mod1Mask, XK_Delete, spawn, {.v = xsecurelock}},
    {MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY | Mod1Mask, XK_u, incrgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_u, incrgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_i, incrigaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_i, incrigaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_o, incrogaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_o, incrogaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_6, incrihgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_6, incrihgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_7, incrivgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_7, incrivgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_8, incrohgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_8, incrohgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_9, incrovgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_9, incrovgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_0, togglegaps, {0}},
    {MODKEY | Mod1Mask | ShiftMask, XK_0, defaultgaps, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY | ControlMask, XK_comma, cyclelayout, {.i = -1}},
    {MODKEY | ControlMask, XK_period, cyclelayout, {.i = +1}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_q, spawn, {.v = quitcmd}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument
     */
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

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
    IPCCOMMAND(view, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(toggleview, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(tag, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(toggletag, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(tagmon, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(focusmon, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(focusstack, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(zoom, 1, {ARG_TYPE_NONE}),
    IPCCOMMAND(incnmaster, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(killclient, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(togglefloating, 1, {ARG_TYPE_NONE}),
    IPCCOMMAND(setmfact, 1, {ARG_TYPE_FLOAT}),
    IPCCOMMAND(setlayoutsafe, 1, {ARG_TYPE_PTR}),
    IPCCOMMAND(quit, 1, {ARG_TYPE_NONE})};
