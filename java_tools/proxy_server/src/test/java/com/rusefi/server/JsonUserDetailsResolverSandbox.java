package com.rusefi.server;

public class JsonUserDetailsResolverSandbox {
    public static void main(String[] args) {
        JsonUserDetailsResolver resolver = new JsonUserDetailsResolver();

        System.out.println("UserDetails result: " + resolver.apply("haba-haba"));
    }
}
