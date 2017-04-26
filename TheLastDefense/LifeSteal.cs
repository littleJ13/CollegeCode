using UnityEngine;
using System.Collections.Generic;

public class LifeSteal : Base_Special
{
    public
    List<Enemy_Controller> targets;

    public
    float count;

    public
    int damage;

    GameObject Player;
    float intern, active;

	void Start ()
    {
        targets = new List<Enemy_Controller>();
        Player = GameObject.Find("3DPlayer");
        intern = count;
        active = 0;
	}

    public override bool Activate()
    {
        if(Player == null)
            Player = GameObject.Find("3DPlayer");

            Instantiate(gameObject, Player.transform.position, Player.transform.rotation);
            return true;
    }

    void Update ()
    {
        if (active < EffectDurration)
            active += Time.deltaTime;
        else if (active >= EffectDurration)
            Destroy(gameObject);

        transform.position = Player.transform.position;
        
        if(count < intern)
        {
            count += Time.deltaTime;
        }
        else if(count >= intern && targets.Count > 0)
            Hit();
	}

    void Hit()
    {
        count = 0;

        for (int i = 0; i < targets.Count; ++i)
        {
            targets[i].GetComponent<Enemy_Controller>().takeDamage(damage);

            if (!(Player.GetComponent<Player_Controller>().Health > Player.GetComponent<Player_Controller>().maxHealth))
                Player.GetComponent<Player_Controller>().Health += damage;
        }
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Enemy")
            targets.Add(other.GetComponent<Enemy_Controller>());
    }

    void OnTriggerExit(Collider other)
    {
        if (other.tag == "Enemy" && targets.Contains(other.GetComponent<Enemy_Controller>()))
            targets.Remove(other.GetComponent<Enemy_Controller>());
    }
}
