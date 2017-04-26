using UnityEngine;
using System.Collections;

public class BlackBox_Trap : Base_Trap
{
    int type;
    public int fireDamage, shockDamage;
    public float freezeTime;
    ParticleSystem fire;
    ParticleSystem ice;
    ParticleSystem lightning;

    AudioSource firesound;
    AudioSource icesound;
    AudioSource lightningsound;
    protected override void Start()
    {
        base.Start();
        type = Random.Range(0, 3);
        fire = GameObject.Find("Fire_02").GetComponent<ParticleSystem>();
        firesound = GameObject.Find("Fire_02").GetComponent<AudioSource>();
        ice = GameObject.Find("Ice Floor").GetComponent<ParticleSystem>();
        icesound = GameObject.Find("Ice Floor").GetComponent<AudioSource>();
        lightning = GameObject.Find("Lightning Strike").GetComponent<ParticleSystem>();
        lightningsound = GameObject.Find("Lightning Strike").GetComponent<AudioSource>();
    }
    protected override void Update()
    {
        if (InternalCooldown >= Cooldown && Targets.Count > 0)
        {
            for (int i = 0; i < Targets.Count; i++)
            {
                if (Targets[i] != null)
                {
                    //Fire
                    if (type == 0)
                    {
                        StartCoroutine(Targets[i].GetComponent<Entity>().Burn(fireDamage));
                        fire.Play();
                        firesound.Play();
                    }
                    //Ice
                    else if (type == 1)
                    {
                        StartCoroutine(Targets[i].GetComponent<Entity>().Freeze(freezeTime));
                        ice.Play();
                        icesound.Play();
                    }
                    //Lightning 
                    else if (type == 2)
                    {
                        StartCoroutine(Targets[i].GetComponent<Entity>().Shock(shockDamage));
                        lightning.Play();
                        lightningsound.Play();
                    }
                }
                else
                {
                    Targets.RemoveAt(i);
                }
            }
            type = Random.Range(0, 3);
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
        Damage *= 2;
        fireDamage += 5;
        freezeTime *= 1.5f;
        shockDamage += 3;
        GetComponent<AudioSource>().Play();
        fire.Play();
        firesound.Play();
        ice.Play();
        icesound.Play();
        lightning.Play();
        lightningsound.Play();
    }
}
