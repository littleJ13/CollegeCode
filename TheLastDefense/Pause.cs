using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Pause : MonoBehaviour
{
    public
    bool GamePause = false;

    public
    GameObject PauseManager;

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {

            if (GamePause == false)
            {
                Time.timeScale = 0;
                GamePause = true;
                PauseManager.SetActive(true);
            }

            else if (GamePause == true)
            {
                Time.timeScale = 1;
                GamePause = false;
                PauseManager.SetActive(false);
            }
        }
    }

public void Toggle()
    {
        GamePause = !GamePause;
        
            Time.timeScale = 1;

        PauseManager.SetActive(false);
    }
}
