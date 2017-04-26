using UnityEngine;
using System.Collections;

public class VampireOrc : Enemy_Controller
{
    public
    int addedHealth = 10;

    protected override void Update()
    {
        base.Update();

        if (canAttack && switchPriority)
        {
            Attack();
            coolDown = 0;
        }
    }

    void Attack()
    {
        if (meleeAttack(Damage))
        {
            if (GetHealth() + addedHealth >= maxHealth)
            {
                SetHealth(maxHealth);
            }

            else if (GetHealth() > 0 && GetHealth() < maxHealth)
            {
                SetHealth(GetHealth() + addedHealth);
            }
        }
    }

    public int GetHealth()
    {
        return Health;
    }

    public void SetHealth(int _maxHealth)
    {
        Health = _maxHealth;
    }
}
