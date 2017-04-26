using UnityEngine;
using System.Collections;

public class Arrow_Movement : MonoBehaviour
{
    public int speed = 0;
    public int damage;

    void Update()
    {
        GetComponent<Rigidbody>().velocity = gameObject.transform.right * -speed;
    }

    void OnTriggerEnter(Collider other)
    {
        if (gameObject.name.Contains("3DArrow"))
        {
            if (other.tag == "Enemy" || other.tag == "Boss" || other.tag == "Wall")
            {
                if (other.tag == "Enemy" && other.name != "RangedImmuneOrcThatIsImmuneToRangedAttacks")
                {
                    //Enemy takes damage
                    other.GetComponent<Enemy_Controller>().takeDamage(damage);
                }
            Destroy(gameObject);
            }
        }

        else if (gameObject.name.Contains("EnemyProjectile") || gameObject.name.Contains("Bomb"))
        {
            if (other.tag == "Player" || other.tag == "Wall")
            {
                if (other.tag == "Player" && other.GetComponent<Player_Controller>().absorbactivate == false)
                {
                    //Player takes damage
                    //Debug.Log("HIT");
                    other.GetComponent<Player_Controller>().takeDamage(damage);
                }
            Destroy(gameObject);
            }
        }

        else if (gameObject.name.Contains("SuperArrow"))
        {
            if (other.tag == "Enemy" || other.tag == "Boss" || other.tag == "Wall")
            {
                if (other.tag == "Enemy" && other.name != "RangedImmuneOrcThatIsImmuneToRangedAttacks")
                {
                    //Enemy takes damage
                    other.GetComponent<Enemy_Controller>().takeDamage(50);
                }
                Destroy(gameObject);
            }
        }
    }
}
