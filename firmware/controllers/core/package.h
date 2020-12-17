#pragma once

#include "engine_ptr.h"

class Logging;
struct engine_configuration_s;

// fwd decl for friend
class PackageManager;

class Package {
public:
	Package();

	// Initialize the package.  This will be called with a valid (loaded) configuration object.
	virtual void initialize(Logging* sharedLogger) = 0;
	virtual void onConfigurationChange(engine_configuration_s* previousConfiguration) = 0;

	virtual void startPins() = 0;
	virtual void stopPins() = 0;

private:
	DECLARE_ENGINE_PTR;

	// these are accessed by PackageManager
	Package* nextPackage = nullptr;
	Package* prevPackage = nullptr;

	friend class PackageManager;
};

class PackageManager {
public:
	static void registerPackage(Package* p);

	static void initializePackages(Logging* sharedLogger DECLARE_CONFIG_PARAMETER_SUFFIX);
	static void onConfigurationChange(engine_configuration_s* activeConfiguration);

	static void restartPins();

private:
	static void insertAfter(Package* insertThis, Package* afterThat);

	static Package* s_first;
	static Package* s_last;
};
