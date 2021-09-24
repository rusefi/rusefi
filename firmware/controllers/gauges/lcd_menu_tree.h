/**
 * @file	lcd_menu_tree.h
 *
 * @date Jan 6, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

typedef enum {
	LL_STRING,
	LL_VERSION,
	LL_CONFIG,
	LL_ALGORITHM,
	LL_INJECTION,
	LL_IGNITION,
	LL_ING_FLOW,

	LL_RPM,
	LL_TRIGGER_ERRORS,
	LL_TRIGGER_DUTY,

	LL_CLT_TEMPERATURE,
	LL_IAT_TEMPERATURE,
	LL_AFR,
	LL_TPS,
	LL_VBATT,
	LL_MAF,
	LL_MAP,
	LL_MAF_V,
	LL_MAF_KG_HR,
	LL_EGO,
	LL_BARO,

	LL_FUEL_BASE,
	LL_FUEL_TOTAL,
	LL_FUEL_CLT_CORRECTION,
	LL_FUEL_IAT_CORRECTION,
	LL_FUEL_INJECTOR_LAG,
} lcd_line_e;

typedef void (*VoidCallback)(void);

class MenuItem {
public:
	void baseConstructor(MenuItem * parent, lcd_line_e lcdLine, const char *text, VoidCallback callback);
	MenuItem(MenuItem * parent, const char *text, VoidCallback callback);
	MenuItem(MenuItem * parent, const char *text);
	MenuItem(MenuItem * parent, lcd_line_e lcdLine);
	const char *text;
	lcd_line_e lcdLine;
	int index;
	// that's upper level menu item
	MenuItem *parent;
	MenuItem *topOfTheList;
	MenuItem *firstChild;
	MenuItem *lastChild;
	MenuItem *next;
	VoidCallback callback;
};

class MenuTree {
public:
	explicit MenuTree(MenuItem *root);
	void nextItem(void);
	void back(void);
	void enterSubMenu(void);
	void init(MenuItem *first, int linesCount);
	MenuItem *root;

	int linesCount = 0;
	MenuItem *current = nullptr;
	MenuItem *topVisible = nullptr;
};
