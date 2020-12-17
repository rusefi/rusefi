#include "package.h"
#include "error_handling.h"

Package::Package() {
	PackageManager::registerPackage(this);
}

class DummySentinelPackage : public Package {
	// nothing to do on dummy sentinels
	void initialize(Logging* DECLARE_CONFIG_PARAMETER_SUFFIX) override { }
	void onConfigurationChange(engine_configuration_s*) override { }
	void startPins() override { }
	void stopPins() override { }
};

DummySentinelPackage headSentinel;
DummySentinelPackage tailSentinel;

Package* PackageManager::s_first = nullptr;
Package* PackageManager::s_last = nullptr;

/*static*/ void PackageManager::registerPackage(Package* package) {
	if (package == &headSentinel || package == &tailSentinel) {
		// ignore sentinels, they're already added
		return;
	}

	// If needed, setup the list
	// We can't guarantee the run order of static intialization, so this check has to happen every time
	if (!s_first || !s_last) {
		s_first = &headSentinel;
		s_last = &tailSentinel;

		// link up the sentinels
		s_first->prevPackage = nullptr;
		s_first->nextPackage = s_last;
		s_last->prevPackage = s_first;
		s_last->nextPackage = nullptr;
	}

	// TODO: insert in priority order?
	insertAfter(package, s_first);
}

/*static*/ void PackageManager::insertAfter(Package* insertThis, Package* afterThat) {
	Package* right = afterThat->nextPackage;

	// Link left side
	afterThat->nextPackage = insertThis;
	insertThis->prevPackage = afterThat;

	// Link right side
	insertThis->nextPackage = right;
	right->prevPackage = insertThis;
}

/*static*/ void PackageManager::initializePackages(Logging* logger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	Package* current = s_first;

	while(current) {
		INJECT_ENGINE_REFERENCE(current);

		current->initialize(logger);
		current = current->nextPackage;

		// Bail out if there was a problem
		if (hasFirmwareError()) {
			return;
		}
	}
}

/*static*/ void PackageManager::onConfigurationChange(engine_configuration_s* activeConfig) {
	Package* current = s_first;

	while(current) {
		current->onConfigurationChange(activeConfig);
		current = current->nextPackage;
	}
}

/*static*/ void PackageManager::restartPins() {
	// Stop pins interating backwards thru the list
	Package* current = s_last;
	while(current) {
		current->stopPins();
		current = current->prevPackage;
	}

	// Re-start them interating forward
	current = s_first;
	while(current) {
		current->startPins();
		current = current->prevPackage;
	}
}
