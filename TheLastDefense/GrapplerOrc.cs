using UnityEngine;
using System.Collections;

public class GrapplerOrc : Enemy_Controller
{
    public float stunTime;
    float coolDownTime;

    protected override void Update()
    {
        base.Update();
        if (coolDown >= attackRate)
        {
            Attack();
            coolDown = 0;
        }
    }

    public void Attack()
    {
        if (meleeAttack(Damage))
        {
            if (!Player.GetComponent<Player_Controller>().stunned)
            {
                if (coolDownTime < Time.time)
                    StartCoroutine(Player.GetComponent<Player_Controller>().Stun(stunTime));
            }
            coolDownTime = (stunTime * 2) + Time.time;
        }
    }
}
