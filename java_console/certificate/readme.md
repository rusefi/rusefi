
Test certificate generated with the following command

``keytool -genkey -keyalg RSA -alias selfsigned -keystore test.jks -storepass password -validity 360 -keysize 2048``


Converted using

keytool -importkeystore -srckeystore keystore.jks -destkeystore keystore.jks -deststoretype pkcs12