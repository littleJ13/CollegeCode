using UnityEngine;
using System.Collections;

public class Absorb : Base_Special
{
    GameObject player;

    void Start ()
    {
        player = GameObject.Find("3DPlayer");
    }
	

    public override bool Activate()
    {
        base.Activate();

        if (player == !GameObject.Find("3DPlayer"))
            player = GameObject.Find("3DPlayer");

        player.GetComponent<Player_Controller>().absorbactivate = true;
        player.GetComponent<Player_Controller>().superarrows = true;
        return true;
    }
}
