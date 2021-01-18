/*
 * Copyright (c) 2019 LUG Nuernberg JK
 *
 * SPDX-License-Identifier: Apache-2.0
*/

/*  Für Umlaute folgende Escape-Sequenzen benutzen:
 *  Ä = \xc4   ,  Ö = \xd6    ,  Ü = \xdc
 *  ä = \xe4   ,  ö = \xf6    ,  ü = \xfc    ,   ß = \xdf
 *
*/

// Globale Variablen, Feste Anteile der VCD
char *name_vcd = "Neuer Name";
static const char *qr_text = "https://www.lug-noris.de/index.php";
static const char *text_right = "LUG-Noris.de";
static const char *text_bottom2 = "Linux User Group N\xfcrnberg";
//static const char *text_bottom2 = ["Linux User Group Nürnberg" UTF8String];
//static const char *text_right2 = CONFIG_BOARD;

// Variable Anteile der VCD, werden via Bluetooth in reel_board.c gesetzt:
//static char *text_right2 = "";  //"Position/Aufgabenbereich";
//static char *text_bottom = "";  //"Vorname Nachname";


