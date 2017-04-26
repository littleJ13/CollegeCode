using UnityEngine;
using System.Collections;

public class Pit_Trap : Base_Trap
{
    protected override void Update()
    {
        if (InternalCooldown >= Cooldown && Targets.Count > 0)
        {
            for (int i = 0; i < Targets.Count; i++)
            {
                if (Targets[i] != null)
                {
                    GetComponent<AudioSource>().Play();

                    if (Targets[i].GetComponent<Orc_King>())
                        Targets[i].GetComponent<Enemy_Controller>().takeDamage(100);
                    else
                        Targets[i].GetComponent<Enemy_Controller>().takeDamage(Targets[i].GetComponent<Enemy_Controller>().Health);
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

    public override void Upgrade()
    {
        isUpgrade = true;
        Cooldown -= 5;
    }
}
