#pragma once

#include <Classes/Object.h>

class UTurnManager : public UObject
{
public:
    std::vector<std::shared_ptr<APlayer>> m_Players;
    int m_CurrentTurnIndex = 0;
    bool bTurnInProgress = false;

    void Initialize(std::vector<std::shared_ptr<APlayer>> players)
    {
        m_Players = players;
        m_CurrentTurnIndex = 0;
        bTurnInProgress = true;
        m_Players[m_CurrentTurnIndex]->StartTurn();
    }

    void EndTurn()
    {
        m_Players[m_CurrentTurnIndex]->EndTurn();
        m_CurrentTurnIndex = (m_CurrentTurnIndex + 1) % m_Players.size();
        m_Players[m_CurrentTurnIndex]->StartTurn();
    }

    void Update()
    {
        if (!bTurnInProgress)
            return;

        if (m_Players[m_CurrentTurnIndex]->IsTurnFinished())
        {
            EndTurn();
        }
    }
};