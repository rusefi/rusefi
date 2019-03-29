/**
 *  @file controller_base.h
 * 
 *  @date Jan 5, 2019
 *  @author Matthew Kennedy, (c) 2019
 */

#pragma once

// Sparse for now...
class ControllerBase
{
private:
    const char* m_name;

public:
    ControllerBase(const char* name)
        : m_name(name)
    {
    }

    const char* GetName() const { return m_name; }
};
