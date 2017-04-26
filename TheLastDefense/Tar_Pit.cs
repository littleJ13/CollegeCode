using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class Tar_Pit : Base_Trap
{
    // variables
    public float slowtime;

    protected override void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Enemy")
        {
            StartCoroutine(other.GetComponent<Entity>().Slow(slowtime));
            //other.GetComponent<NavMeshAgent>().speed -= 1;
            GetComponent<AudioSource>().Play();

        }
    }
    protected override void OnTriggerExit(Collider other)
    {
        
    }

    protected void OnTriggerStay(Collider other)
    {
        if (!other.GetComponent<Entity>().slowed)
            StartCoroutine(other.GetComponent<Entity>().Slow(slowtime));
    }
    public override void Upgrade()
    {
        isUpgrade = true;
        GetComponent<SpriteRenderer>().color = new Color(.5f, .5f, .5f);
        slowtime += 1.5f;
    }
}
