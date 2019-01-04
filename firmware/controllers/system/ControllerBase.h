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
