//
// Created by kifir on 4/1/25.
//

#pragma once

#include "type_list.h"
#include "etb_target_adjuster.h"

class Plugins {
public:
    static Plugins& getInstance();

    template <typename PluginType>
  	void registerPlugin(PluginType* const plugin);

    template <typename PluginType>
  	PluginType* get() const;
private:
    Plugins() {}
    Plugins(const Plugins&) = delete;
    Plugins& operator=(const Plugins&) = delete;

    EtbTargetAdjuster* etbAdjuster = nullptr;
};

template <>
void Plugins::registerPlugin<EtbTargetAdjuster>(EtbTargetAdjuster* const plugin);

template <>
EtbTargetAdjuster* Plugins::get<EtbTargetAdjuster>() const;
