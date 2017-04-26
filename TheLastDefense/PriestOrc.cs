using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class PriestOrc : Enemy_Controller
{
    public
    int Range = 5;

    ParticleSystem healing;

    protected override void Start()
    {
        base.Start();
        healing = GameObject.Find("Cyclone").GetComponent<ParticleSystem>();
    }
    protected override void Update()
    {
        base.Update();
        //if (canAttack == true)
        //    Heal();
        if (canAttack && switchPriority)
        {
            Heal();
            coolDown = 0;
        }
    }

    void Heal()
    {
        foreach (Collider c in Physics.OverlapSphere(transform.position, Range))
        {
            if (c.tag == "Enemy")
            {
                if (c.GetComponent<Enemy_Controller>().Health >= maxHealth)
                {
                    c.GetComponent<Enemy_Controller>().Health = maxHealth;
                    healing.Stop();
                }
                else
                {
                    healing.Play();
                    c.GetComponent<Enemy_Controller>().Health += 10;
                }
            }
        }
    }
}
