#pragma once

#include "FragmentEntry.h"

template <typename TStruct>
const TStruct* getLiveDataAddr();

template <typename TStruct>
const TStruct* getLiveDataAddr(size_t index);

FragmentList getLiveDataFragments();
