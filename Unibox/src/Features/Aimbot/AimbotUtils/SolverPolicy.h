#pragma once
#include <cstdint>
namespace SolverPolicy
{
    // Mutually exclusive routing: compatibility extensions always retain Unibox.
    constexpr bool UseV2(bool selected, bool extension) { return selected && !extension; }
    struct Context
    {
        uint64_t owner=0,weapon=0;
        uint32_t map=0;
        int tick=0;
        bool valid=false;
    };
    class ContextGuard
    {
        Context m_previous;
    public:
        bool Update(Context next)
        {
            const bool changed=next.valid!=m_previous.valid || (next.valid &&
                (next.owner!=m_previous.owner || next.weapon!=m_previous.weapon || next.map!=m_previous.map || next.tick<m_previous.tick));
            m_previous=next; return changed;
        }
    };
    constexpr bool RecycledTarget(int previousIndex,uint64_t previousToken,int index,uint64_t token)
    { return previousIndex==index && previousToken!=token; }
}
