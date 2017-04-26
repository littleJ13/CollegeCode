using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Fire_Trap : Base_Trap
{
    public int burnDamage;

    ParticleSystem fire;
    protected override void Start()
    {
        base.Start();
        fire = GameObject.Find("Fire_02").GetComponent<ParticleSystem>();
        StartCoroutine(FireTrapEnum());
    }

    protected override void Update()
    {

    }

    private IEnumerator FireTrapEnum()
    {
        while (true)
        {
            for (int i = 0; i < Targets.Count;)
            {
                if (Targets[i] == null)
                {
                    Targets.RemoveAt(i);
                    continue;
                }
                if (!Targets[i].GetComponent<Entity>().burning)
                {
                    StartCoroutine(Targets[i].GetComponent<Entity>().Burn(burnDamage));
                }
                Targets[i].GetComponent<Entity>().takeDamage(Damage);
                GetComponent<AudioSource>().Play();
                i++;
            }
            yield return new WaitForSeconds(.5f);
        }
    }

    public override void Upgrade()
    {
            isUpgrade = true;
            Damage *= 2;
            burnDamage += 5;
            GetComponent<SpriteRenderer>().color = new Color(.5f, .5f, .5f);
            Instantiate(fire, transform.position, fire.transform.rotation);
            fire.Play();
    }
}
