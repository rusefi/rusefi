package com.opensr5.ini.field;

public interface IniFieldVisitor<T> {
    T visit(ScalarIniField field);
    T visit(EnumIniField field);
    T visit(ArrayIniField field);
    T visit(StringIniField field);
}
