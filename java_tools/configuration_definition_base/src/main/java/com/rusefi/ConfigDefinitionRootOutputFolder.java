package com.rusefi;

public class ConfigDefinitionRootOutputFolder {
  private static final String KEY = "META_OUTPUT_ROOT_FOLDER";
  private static final String OUTPUT_ROOT = System.getenv(KEY) == null ? "" : System.getenv(KEY);

  static {
    System.out.println(KEY + "=" + OUTPUT_ROOT);
  }

  public static String getValue() {
    return OUTPUT_ROOT;
  }
}
