# min(coolant, 110) would clamp value from top
#  max(90, min(coolant, 110)) would clamp result from bottom
#   max(90, min(coolant, 110)) - 90 would be in the 0 to 20 range
#    (max(90, min(coolant, 110)) - 90) / 20 would be in 0 to 1 range
#     (max(90, min(coolant, 110)) - 90) / 20 * 0.8 would be in 0 to 0.8 range

java -jar ../java_console_binary/rusefi_console.jar compile "0.15 + (max(90, min(coolant, 110)) - 90) / 20 * 0.8"