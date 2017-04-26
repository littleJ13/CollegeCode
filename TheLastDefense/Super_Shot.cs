using UnityEngine;
using System.Collections;

public class Super_Shot : Base_Special
{
    public
    GameObject Arrow;

    GameObject Player;

    public
    GameObject ar;

    public override bool Activate()
    {
        //transform.position = Player.transform.position;
        base.Activate();

        if (Player == !GameObject.Find("3DPlayer"))
            Player = GameObject.Find("3DPlayer");

        Instantiate(Arrow,Player.transform.position, ShootAt());
        return true;
    }

    Quaternion ShootAt()
    {
        //Shooting an arrow in the direction of the mouse
        Vector3 mousePos = Input.mousePosition;
        mousePos = Camera.main.ScreenToWorldPoint(mousePos);
        Quaternion newRotation = Quaternion.AngleAxis(90, Vector3.up) * Quaternion.LookRotation(mousePos - Player.transform.position);
        newRotation *= Quaternion.AngleAxis(90, Vector3.right);
        return newRotation;
    }


}

