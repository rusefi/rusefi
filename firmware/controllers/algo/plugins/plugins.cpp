//
// Created by kifir on 4/1/25.
//

#include "pch.h"

#include "plugins.h"

Plugins& Plugins::getInstance() {
    static Plugins instance;
    return instance;
}

template <>
void Plugins::registerPlugin<EtbTargetAdjuster>(EtbTargetAdjuster* const plugin) {
    if (etbAdjuster != plugin) {
        if (etbAdjuster == nullptr) {
            etbAdjuster = plugin;
        } else {
            criticalError("Attempt to register EtbTargetAdjuster plugin twice.");
        }
    }
}

template <>
EtbTargetAdjuster* Plugins::get<EtbTargetAdjuster>() const {
    return etbAdjuster;
}
