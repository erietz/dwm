/*
 * Author      : Ethan Rietz
 * Date        : 2021-09-11
 * Description : Config file for DWM
 */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const unsigned int gappoh_uw = 90;       /* horiz outer gap between windows and screen edge (my custom "ultrawide monitor mode") */
static const unsigned int gappov_uw = 800;      /* vert outer gap between windows and screen edge (my custom "ultrawide monitor mode") */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrains Mono:size=13" };
static const char dmenufont[]       = "JetBrains Mono:size=13";
static const char col_gray1[]       = "#1c1f1a"; // deep mossy black (background)
static const char col_gray2[]       = "#2e3f38"; // muted forest green (status bar)
static const char col_gray3[]       = "#d6e3dd"; // soft pale mint (text)
static const char col_accent[]      = "#8fbf9f"; // sage green (highlight)
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray3, col_gray2, col_accent },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *  WM_CLASS(STRING) = instance, class
	 *  WM_NAME(STRING) = title
	*/
	/* class      instance    title      tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,      0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,      1 << 1,       0,           -1 },
	{ "discord",  NULL,       NULL,      1 << 8,       0,           -1 },
	{ "Spotify",  NULL,       NULL,      1 << 7,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "{M}",      monoclegaps },
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY                  Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
		{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
		{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
		{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { 
	"dmenu_run",
	"-m", dmenumon,
	"-fn", dmenufont,   // defines the font or font set
	"-nb", col_gray1,   // defines the normal background color
	"-nf", col_gray3,   // defines the normal foreground color
	"-sb", col_gray2,   // defines the selected background color
	"-sf", col_gray3,   // defines the selected foreground color
	NULL
};
static const char *termcmd[]  = { "st", NULL };
static const char *roficmd[] = { "rofi", "-show", "drun", NULL };
static const char *scrot[] = { "scrot", "-e", "notify-send 'screenshot taken'", NULL };
static const char *scrot_select[] = { "scrot", "--select", "-e", "notify-send 'screenshot taken'", NULL };
static const char *scrot_window[] = { "scrot", "--focused", "-e", "notify-send 'screenshot taken'", NULL };

/* key bindings */
static Key keys[] = {
	/* modifier            key        function        argument */
	{ MODKEY,             XK_Return, spawn,          {.v = termcmd}      },
	{ MODKEY|ShiftMask,   XK_Return, zoom,           {0}                 },
	{ MODKEY,             XK_p,      spawn,          {.v = dmenucmd}     },
	{ MODKEY|ShiftMask,   XK_p,      spawn,          {.v = roficmd}      },
	{ MODKEY,             XK_s,      spawn,          {.v = scrot_window} },
	{ MODKEY|ShiftMask,   XK_s,      spawn,          {.v = scrot_select} },
	{ MODKEY|ControlMask, XK_s,      spawn,          {.v = scrot}        },
	{ MODKEY,             XK_b,      togglebar,      {0}                 },
	{ MODKEY,             XK_j,      focusstack,     {.i = +1}           },
	{ MODKEY|ShiftMask,   XK_j,      incrohgaps,     {.i = +40}          },
	{ MODKEY,             XK_k,      focusstack,     {.i = -1}           },
	{ MODKEY|ShiftMask,   XK_k,      incrohgaps,     {.i = -40}          },
	{ MODKEY,             XK_i,      incnmaster,     {.i = +1}           },
	{ MODKEY,             XK_d,      incnmaster,     {.i = -1}           },
	{ MODKEY,             XK_h,      setmfact,       {.f = -0.05}        },
	{ MODKEY|ShiftMask,   XK_h,      incrovgaps,     {.i = -40}          },
	{ MODKEY,             XK_l,      setmfact,       {.f = +0.05}        },
	{ MODKEY|ShiftMask,   XK_l,      incrovgaps,     {.i = +40}          },
	{ MODKEY,             XK_u,      ultrawidegaps,  {0}                 },
	{ MODKEY|ShiftMask,   XK_u,      defaultgaps,    {0}                 },
	{ MODKEY,             XK_g,      togglegaps,     {0}                 },
	{ MODKEY,             XK_w,      togglesticky,   {0}                 },
	{ MODKEY,             XK_Tab,    view,           {0}                 },
	{ MODKEY,             XK_q,      killclient,     {0}                 },
	{ MODKEY|ShiftMask,   XK_q,      quit,           {0}                 },
	{ MODKEY,             XK_t,      setlayout,      {.v = &layouts[0]}  },
	{ MODKEY,             XK_f,      setlayout,      {.v = &layouts[1]}  },
	{ MODKEY,             XK_m,      setlayout,      {.v = &layouts[2]}  },
	{ MODKEY|ShiftMask,   XK_m,      setlayout,      {.v = &layouts[3]}  },
	{ MODKEY,             XK_space,  setlayout,      {0}                 },
	{ MODKEY|ShiftMask,   XK_space,  togglefloating, {0}                 },
	{ MODKEY|ShiftMask,   XK_f,      togglefullscr,  {0}                 },
	{ MODKEY,             XK_0,      view,           {.ui = ~0}          },
	{ MODKEY|ShiftMask,   XK_0,      tag,            {.ui = ~0}          },
	{ MODKEY,             XK_comma,  focusmon,       {.i  = -1}          },
	{ MODKEY|ShiftMask,   XK_comma,  tagmon,         {.i  = -1}          },
	{ MODKEY,             XK_period, focusmon,       {.i  = +1}          },
	{ MODKEY|ShiftMask,   XK_period, tagmon,         {.i  = +1}          },
	TAGKEYS(              XK_1,      0)
	TAGKEYS(              XK_2,      1)
	TAGKEYS(              XK_3,      2)
	TAGKEYS(              XK_4,      3)
	TAGKEYS(              XK_5,      4)
	TAGKEYS(              XK_6,      5)
	TAGKEYS(              XK_7,      6)
	TAGKEYS(              XK_8,      7)
	TAGKEYS(              XK_9,      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
