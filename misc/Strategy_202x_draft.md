### We are interested to create a fundamentally better ECU DIY experience.

# Desired User Outcomes

We are looking to reduce manual data processing and to expand self-tuning capabilities. We are looking to increase the success rate for ECU amateurs.
1. Software usability
1. mass storage device
1. drivers
1. firmware update
1. tuning software
1. bench test/diagnostics
1. Community toolset - remote tuning, tune and log community involvement
1. Hardware breakage diagnostics: misfire detection
1. Hardware breakage diagnostics: broken wire detection for many wires
1. Long term fuel table autotune
1. Long term timing table tune
1. WUE Warm-Up Enrichment auto tune
1. Limp mode
1. Hardware self-detection: Injector lag?
1. Hardware self-detection: Trigger shape detection / diagnostics
1. “As good as OEM in terms of XXX” or “As good as OEM in any term”
 * Mileage
 * Cold start
 * Cranking duration
 * Trigger Decoding Quality




# SDLC efforts
1. Data workflow tools: human feedback and objective data from real users (REO)
1. HW in the loop: rusEFI hardware quality assurance
1. HW in the loop: rusEFI CI
1. HW in the loop: Competitor analysis toolset - both OEM and aftermarket
1. Universal code vs custom vehicle code
 


# Implementation Strategy
1. Gather more data about rusEFI operation. We are looking to gather more data about engine operation and process that data programmatically.

We are looking to gather:
* VSS as rotation sensor or GPS
* Direct accelerometer measurement maybe 3D
* Crankshaft acceleration data based on trigger input, on the scale of individual engine cycle
* Acoustic knock data
* Cylinder Pressure Pike data via Ion sense, maybe knock data via ion sense


# Engineering toolset development
* Faster data acquisition (at least full time composite logger/engine sniffer)
