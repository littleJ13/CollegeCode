using UnityEngine;
using System.Collections;

public class GodMode : Base_Special
{
    public GameObject player;
    void Start()
    {
        player = GameObject.Find("3DPlayer");
    }

    public override bool Activate()
    {
        if (player == !GameObject.Find("3DPlayer"))
            player = GameObject.Find("3DPlayer");
        if (!player.GetComponent<Player_Controller>().modeActive && EnemyList.CountOfList() > 0)
        {
            player.GetComponent<Player_Controller>().StartGodMode();
            base.Activate();
        }
        return true;
    }
    
}
