/**
 * @file	lcd_menu_tree.cpp
 *
 * @date Jan 6, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "stddef.h"
#include "lcd_menu_tree.h"
#include "error_handling.h"

MenuTree::MenuTree(MenuItem *root) {
	this->root = root;
	current = NULL;
	linesCount = 0;
	topVisible = NULL;
}

void MenuTree::init(MenuItem *first, int linesCount) {
	this->linesCount = linesCount;
	current = first;
	topVisible = first;
}

void MenuTree::enterSubMenu(void) {
	if (current->firstChild != NULL) {
		current = topVisible = current->firstChild;
	} else if (current->callback != NULL) {
		VoidCallback cb = current->callback;
		cb();
	}
}

void MenuTree::back(void) {
	if (current->parent == root)
		return; // we are on the top level already
	current = topVisible = current->parent->topOfTheList;
}

void MenuTree::nextItem(void) {
	if (current->next == NULL) {
		current = topVisible = current->topOfTheList;
		return;
	}
	current = current->next;
	if (current->index - topVisible->index == linesCount)
		topVisible = topVisible->next;
}

/**
 * This constructor created a menu item and associates a callback with it
 */
MenuItem::MenuItem(MenuItem * parent, const char *text, VoidCallback callback) {
	baseConstructor(parent, LL_STRING, text, callback);
}

/**
 * Looks like this constructor is used to create
 */
MenuItem::MenuItem(MenuItem * parent, const char *text) {
	baseConstructor(parent, LL_STRING, text, NULL);
}

/**
 * This constructor is used for lines with dynamic content
 */
MenuItem::MenuItem(MenuItem * parent, lcd_line_e lcdLine)  {
	baseConstructor(parent, lcdLine, NULL, NULL);
}

void MenuItem::baseConstructor(MenuItem * parent, lcd_line_e lcdLine, const char *text, VoidCallback callback) {
	this->parent = parent;
	this->lcdLine = lcdLine;
	this->text = text;
	this->callback = callback;

	firstChild = NULL;
	lastChild = NULL;
	topOfTheList = NULL;
	next = NULL;
	index = 0;

	// root element has NULL parent
	if (parent != NULL) {
		if (parent->firstChild == NULL) {
			topOfTheList = this;
			parent->firstChild = this;
		}
		if (parent->lastChild != NULL) {
			index = parent->lastChild->index + 1;
			topOfTheList = parent->lastChild->topOfTheList;
			parent->lastChild->next = this;
		}
		parent->lastChild = this;
	}
}
