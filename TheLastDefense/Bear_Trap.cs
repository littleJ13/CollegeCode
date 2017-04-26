using UnityEngine;
using System.Collections.Generic;

public class Bear_Trap : Base_Trap
{
    public float stunTime;
    protected override void Update()
    {
        //Going through the list of enemies to deal damage
        if (InternalCooldown >= Cooldown && Targets.Count > 0)
        {
            for (int i = 0; i < Targets.Count;)
            {
                if (Targets[i] != null)
                {
                    StartCoroutine(Targets[i].GetComponent<Entity>().Stun(stunTime));
                    Targets[i].GetComponent<Entity>().takeDamage(Damage);
                    GetComponent<AudioSource>().Play();
                    i++;
                }
                else
                    Targets.RemoveAt(i);
            }
            InternalCooldown = 0.0f;
        }

        else if (InternalCooldown < Cooldown)
        {
            InternalCooldown += Time.deltaTime;
        }

        CooldownColor();
    }

    public new void Upgrade()
    {
        stunTime *= 1.5f;
    }
}
