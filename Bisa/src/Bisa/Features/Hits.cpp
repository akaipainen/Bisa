#include "bapch.h"

#include "Bisa/Features/Hits.h"

namespace Bisa {

    unsigned int Hit::UniqueIdCounter = 0;

    Hits::~Hits()
    {
        for (Hit* hit : m_FirstHits)
        {
            delete hit;
        }
    }

    Hits::Hits(const Hits& other)
     : m_TriggerId(other.m_TriggerId)
    {
        for (auto it = other.begin_first(); it != other.end_first(); it++)
        {
            m_FirstHits.emplace_back(new Hit(**it));
        }
        for (auto it = other.begin_add(); it != other.end_add(); it++)
        {
            m_AdditionalHits.emplace_back(new Hit(**it));
        }
    }

    Hits& Hits::operator=(const Hits& other)
    {
        Hits temp(other);
        std::swap(m_FirstHits, temp.m_FirstHits);
        std::swap(m_AdditionalHits, temp.m_AdditionalHits);
        std::swap(m_TriggerId, temp.m_TriggerId);
        return *this;
    }

    void Hits::PushHit(Hit* newHit)
    {
        BA_CORE_ASSERT(newHit->triggerId == m_TriggerId || size() == 0, "Adding new hit with different Trigger ID to Hits");

        for (auto &&hit : m_FirstHits)
        {
            if (hit->strip == newHit->strip)
            {
                m_AdditionalHits.emplace_back(newHit);
                m_TriggerId = newHit->triggerId;
                return;
            }
        }
        m_FirstHits.emplace_back(newHit);
        m_TriggerId = newHit->triggerId;
    }

    void Hits::PopHit(Hit* hit)
    {
        auto it = std::find(m_FirstHits.begin(), m_FirstHits.end(), hit);
        if (it != m_FirstHits.end())
        {
            m_FirstHits.erase(it);
            return;
        }
        it = std::find(m_AdditionalHits.begin(), m_AdditionalHits.end(), hit);
        if (it != m_AdditionalHits.end())
        {
            m_AdditionalHits.erase(it);
        }
    }

    void Hits::Merge(const Hits& other)
    {
        Hits temp(other);
        for (auto it = temp.begin_first(); it != temp.end_first(); it++)
        {
            m_FirstHits.emplace_back(*it);
        }
        for (auto it = temp.begin_add(); it != temp.end_add(); it++)
        {
            m_AdditionalHits.emplace_back(*it);
        }
        temp.m_FirstHits.clear();
        temp.m_AdditionalHits.clear();
    }

    std::string Hits::ToString()
        {
            std::stringstream ret;
            ret << "Trigger ID: " << m_TriggerId;
            ret << ", Number of hits: " << m_FirstHits.size() + m_AdditionalHits.size();
            return ret.str();
        }

}