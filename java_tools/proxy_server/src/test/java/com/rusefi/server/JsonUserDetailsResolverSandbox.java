package com.rusefi.server;

/**
 * Manual harness for the JSON user-details resolver against the rusEFI online API.
 */
public class JsonUserDetailsResolverSandbox {
    public static void main(String[] args) {
        JsonUserDetailsResolver resolver = new JsonUserDetailsResolver();

        System.out.println("UserDetails result: " + resolver.apply("haba-haba"));
    }
}
