#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class CKillEffect
{
private:
    int _bloodDrop;
public:
    CKillEffect(int bloodDrop): _bloodDrop(bloodDrop)
    {}

    void operator()(int &blood)
    {
        // blood cannot drop below zero
        blood = max((int)(0), blood - _bloodDrop);
    }
};

/* 
 * A groud of players(e.g., CounterStrike terrorists). Each of them has a blood value,
 * representing their remaining life. A man is dead if his blood drops to zero.
 */
class CTeam
{
private:
    const int kFullBlood = 100;
    vector<int> _bloods;
public:
    CTeam(size_t nMembers): _bloods(nMembers, kFullBlood)
    {}

    // A range kill is a mass kill where every player drops some blood.
    void rangeKill(const CKillEffect &effect) {
        for_each(_bloods.begin(), _bloods.end(), effect);
    }

    vector<int> getBloods() const {
        return _bloods;
    }
};

int main()
{
    const int kBloodDropByBullet = 50;
    //const int kBloodDropByBomb = 80;

    CTeam team(10);
    team.rangeKill(CKillEffect(kBloodDropByBullet));
    vector<int> bloods = team.getBloods();

    for (const auto &x : bloods) {
        cout << x << " ";
    }
    cout << endl;
}

