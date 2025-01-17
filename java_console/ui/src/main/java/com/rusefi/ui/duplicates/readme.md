We have a problem with classes that are used by both `rusefi_autoupdate.jar` and `rusefi_console.jar`: if we add a new method to such class and use it in `rusefi_console.jar` than we get reflection exception after update.

The reason is that `rusefi_autoupdate.jar` contains an obsolete version of the class without new method, but a new version `rusefi_console.jar` tries to call the missed method and crashes.

This is not a total catastrophe, because after crash `rusefi_console.jar` can be restarted successfully, but we want to reduce number of users experiencing this problem.

The suggested solution is to temporarily duplicate problematic classes in `com.rusefi.ui.duplicates` package and at first to use this duplicated classes in `rusefi_console.jar` instead of original classes that is used in `rusefi_autoupdate.jar`.

After the majority of users have already updated their console, we could get rid of duplicated classes and replace them with the original class used by `rusefi_autoupdate.jar`.

We will still have a few users that haven't updated their console and they will experience a crash on console update, but I do not think that it is serious problem.
