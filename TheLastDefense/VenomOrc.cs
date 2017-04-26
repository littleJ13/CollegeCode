using UnityEngine;
using System.Collections;

public class VenomOrc : Enemy_Controller
{
    public float posionstart = 0.0f;
    public float posioncooldown = .5f;

    void FixedUpdate()
    {
        if (Player.GetComponent<Player_Controller>().poisoned == true)
        {
            DealPosionDamage();
        }
    }
    protected override void Update()
    {
        base.Update();
        if (coolDown < attackRate)
        {
            canAttack = false;
            coolDown += Time.deltaTime;
        }
        else if (coolDown >= attackRate)
        {
            canAttack = true;
            if (distance <= 2)
            {
                ApplyPosion();
            }
            coolDown = 0;
        }
    }

    void ApplyPosion()
    {
        Player.GetComponent<Player_Controller>().poisoned = true;
    }

    void DealPosionDamage()
    {
        Player.GetComponent<Player_Controller>().Health -= 1;
        posionstart += Time.deltaTime;

        if (posionstart >= posioncooldown)
        {
            posionstart = 0.0f;
            Player.GetComponent<Player_Controller>().poisoned = false;
        }
    }
}
