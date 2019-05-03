# min(coolant, 120) would clamp value from top
#  max(90, min(coolant, 120)) would clamp result from bottom
#   max(90, min(coolant, 120)) - 90 would be in the 0 too 30 range
#    (max(90, min(coolant, 120)) - 90) / 30 would be in 0 to 1 range
#     (max(90, min(coolant, 120)) - 90) / 30 * 0.8 would be in 0 to 0.8 range

java -jar ../java_console_binary/rusefi_console.jar compile "0.15 + (max(90, min(coolant, 120)) - 90) / 30 * 0.8"