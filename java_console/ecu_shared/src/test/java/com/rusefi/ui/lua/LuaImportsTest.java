package com.rusefi.ui.lua;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class LuaImportsTest {
    @Test
    public void testScriptName() {
        assertEquals(null, LuaIncludeSyntax.getScriptName(""));
        assertEquals("X", LuaIncludeSyntax.getScriptName("-- scriptname X "));
        assertEquals("X2", LuaIncludeSyntax.getScriptName("-- scriptname X2\n"));
        assertEquals("X3", LuaIncludeSyntax.getScriptName("sdad\n  -- scriptname X3\n"));
        assertEquals("folder/X3", LuaIncludeSyntax.getScriptName("sdad\n  -- scriptname folder/X3\n"));
    }

    @Test
    public void testReloadTwoScripts() {

        String lua = "before\n" +
                "-- include functions.lua\n" +
                "functionOldBody\n" +
                "-- endinclude\n" +
                "between\n" +
                "--include      constants.lua    \n" +
                "oldConstants\n" +
                "-- endinclude\n" +
                "after";

        ContentSource source = name -> {
            if (name.equals("functions.lua"))
                return "newFunction";
            if (name.equals("constants.lua"))
                return "newConstant";
            throw new IllegalStateException();
        };

        String newLua = LuaIncludeSyntax.reloadScript(lua, source);

        assertEquals("before\n" +
                "-- include functions.lua\n" +
                "newFunction\n" +
                "between\n" +
                "--include      constants.lua\n" +
                "newConstant\n" +
                "after", newLua);


    }

}
