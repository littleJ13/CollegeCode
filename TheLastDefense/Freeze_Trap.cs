using UnityEngine;
using System.Collections;

public class Freeze_Trap : Base_Trap
{
    public float freezeTime;

    protected override void Start()
    {
        base.Start();
    }
    protected override void Update()
    {
        //Going through the list of enemies to deal damage
        if (InternalCooldown >= Cooldown && Targets.Count > 0)
        {
            for (int i = 0; i < Targets.Count;)
            {
                if (Targets[i] != null)
                {
                    StartCoroutine(Targets[i].GetComponent<Entity>().Freeze(freezeTime));
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
        freezeTime *= 1.5f;
    }
}
