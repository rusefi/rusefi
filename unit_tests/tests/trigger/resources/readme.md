This folder contains raw real-life trigger recordings and their converted CSV versions used for unit testing the trigger decoder logic.

### File Formats
* **.logicdata**: Original recordings from legacy Saleae Logic 1 software.
* **.sal**: Recordings from Saleae Logic 2 software.
* **.csv**: Converted data used by unit tests. The format is typically `Time[s], Channel X, Channel Y, ...` where channels represent crank and cam sensor signals.

### Usage in Unit Tests
The `test_real_*.cpp` files in the parent directory utilize these CSV files to simulate engine rotation. This allows developers to verify and troubleshoot trigger decoding logic against real-world data without requiring physical hardware.

Key helper classes involved:
* `RealTriggerHelper`: Orchestrates running a test against a CSV file with expected RPM and sync outcomes.
* `EngineCsvReader` / `CsvReader`: Handles parsing the CSV files.

### Contributing
When adding new trigger patterns, it is recommended to provide both the original logic analyzer capture and the exported CSV to facilitate debugging and future updates.

eBay has really cheap logic analyzers which are compatible with Saleae software!

