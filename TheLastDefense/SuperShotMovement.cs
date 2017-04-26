using UnityEngine;
using System.Collections;

    public class SuperShotMovement : MonoBehaviour
    {

        public
        int speed = 0;
        public int damage;
        float LifeTime = 3;

        void Update()
        {
            GetComponent<Rigidbody>().velocity = gameObject.transform.right * -speed;
        LifeTime -= Time.deltaTime;
        if (LifeTime <=0)
                    Destroy(gameObject);

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
                }
            }

            else if (gameObject.name.Contains("EnemyProjectile") || gameObject.name.Contains("Bomb"))
            {
                if (other.tag == "Player" || other.tag == "Wall")
                {
                    if (other.tag == "Player")
                    {
                        //Player takes damage
                        //Debug.Log("HIT");
                        other.GetComponent<Player_Controller>().takeDamage(damage);
                    }
                    Destroy(gameObject);
                }
            }
        }
    }
