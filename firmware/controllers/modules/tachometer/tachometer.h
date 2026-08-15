/*
 * @file tachometer.h
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once


class TachometerModule : public EngineModule {
public:
    void init();
    void onFastCallback() override;
private:
    // onFastCallback() gates everything on this flag while only init() writes it, so it has to
    // start false rather than depend on the instance being zero initialized at file scope
    bool tachHasInit = false;
};