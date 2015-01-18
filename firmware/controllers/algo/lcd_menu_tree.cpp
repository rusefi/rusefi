/**
 * @file	lcd_menu_tree.cpp
 *
 * @date Jan 6, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "stddef.h"
#include "lcd_menu_tree.h"
#include "error_handling.h"

MenuTree::MenuTree(MenuItem *root) {
	this->root = root;
	current = NULL;
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

MenuItem::MenuItem(MenuItem * parent, const char *text, VoidCallback callback) {
	lcdLine = LL_STRING;
	this->text = text;
	init(parent, callback);
}

MenuItem::MenuItem(MenuItem * parent, const char *text) {
	lcdLine = LL_STRING;
	this->text = text;
	init(parent, NULL);
}

MenuItem::MenuItem(MenuItem * parent, lcd_line_e lcdLine) {
	this->lcdLine = lcdLine;
	this->text = NULL;
	init(parent, NULL);
}

void MenuItem::init(MenuItem * parent, VoidCallback callback) {
	this->parent = parent;
	firstChild = NULL;
	lastChild = NULL;
	next = NULL;
	this->callback = callback;

	// root element has NULL parent
	if (parent != NULL) {
		if (parent->firstChild == NULL) {
			parent->firstChild = this;
			index = 0;
			topOfTheList = this;
		}
		if (parent->lastChild != NULL) {
			index = parent->lastChild->index + 1;
			topOfTheList = parent->lastChild->topOfTheList;
			parent->lastChild->next = this;
		}
		parent->lastChild = this;
	}
}
